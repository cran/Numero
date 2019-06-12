numero.prepare <- function(
    data,
    variables=NULL,
    confounders=NULL,
    batch=NULL,
    method="standard",
    pipeline=NULL) {

    # Start processing.
    stamp <- date()
    cat("\n*** numero.prepare ***\n", stamp, "\n", sep="")

    # Use all data columns.
    if(length(variables) < 1) variables <- colnames(data)
    if(length(batch) > 1) batch <- batch[[1]]

    # Create a new pipeline.
    if(is.null(pipeline)) {
 	cat("\nSetting up:\n")
        pipeline <- list(batch=batch)

        # Select variables.
	convars <- intersect(confounders, colnames(data))
	batvars <- intersect(batch, colnames(data))
	datvars <- intersect(variables, colnames(data))
        datvars <- setdiff(datvars, c(convars, batch))
        convars <- setdiff(convars, batch)

        # Convert to numeric and trim empty rows and columns.
	ds <- nroRcppMatrix(data[,c(convars, datvars)], trim=TRUE)
        convars <- intersect(convars, colnames(ds))
        datvars <- intersect(datvars, colnames(ds))

        # Check that enough columns.
        cat(length(convars), " / ", length(confounders),
            " confounder column(s)\n", sep="")
        cat(length(batvars), " / ", length(batch),
	    " batch column(s)\n", sep="")
        cat(length(datvars), " / ", length(variables),
            " data column(s)\n", sep="")
	if(length(datvars) < 3) {
	    cat("too few data columns\n")
	    return(NULL)
	}

        # First round of preprocessing.
        suppressWarnings(ds <- nroPreprocess(data=ds, method=method))
        pipeline$mapping1 <- attr(ds, "mapping")

        # Regression model of confounding.
	if(length(convars) > 0) {
	    ds <- numero.prepare.regress(ds, convars)
            pipeline$adjustment <- attr(ds, "adjustment")
        }

        # Correction model for batch differences.
	if(length(batvars) > 0) {
            ds <- numero.prepare.flatten(ds, data, batvars)
            pipeline$correction <- attr(ds, "correction")
        }

        # Second round of preprocessing.
	if((length(convars) + length(batvars)) > 0) {
            suppressWarnings(ds <- nroPreprocess(data=ds, method=method))
            pipeline$mapping2 <- attr(ds, "mapping")
	}
    }

    # First round of preprocessing.
    cat("\nProcessing:\n")
    if(!is.null(pipeline$mapping1)) {
        suppressWarnings(ds <- nroPostprocess(data, pipeline$mapping1))
	cat(ncol(ds), " column(s) standardized\n", sep="")
    }

    # Adjust for confounding.
    if(!is.null(pipeline$adjustment))
        ds <- numero.prepare.adjust(ds, pipeline$adjustment)

    # Adjust for batches.
    subsets <- NULL
    if(!is.null(pipeline$correction)) {
        ds <- numero.prepare.correct(ds, data, pipeline$correction)
        subsets <- attr(ds, "subsets")
    }

    # Second round of preprocessing.
    if(!is.null(pipeline$mapping2)) {
        suppressWarnings(ds <- nroPostprocess(ds, pipeline$mapping2))
	cat(ncol(ds), " column(s) re-standardized\n", sep="")
    }

    # Final report.
    if(is.null(ds)) ds <- data.frame()
    cat("\nSummary:\n", sep="")
    cat(nrow(ds), " / ", nrow(data), " usable row(s)\n", sep="")
    cat(ncol(ds), " / ", ncol(data), " usable column(s)\n", sep="")

    # Return results.
    attr(ds, "pipeline") <- pipeline
    attr(ds, "subsets") <- subsets
    return(ds)
}

#--------------------------------------------------------------------------

numero.prepare.regress <- function(ds, convars) {
    if(length(ds) < 1) return(NULL)
    if(length(convars) < 1) return(ds)

    # Impute missing values and add constant.
    suppressWarnings(confs <- nroImpute(data=ds[,convars]))
    confs <- cbind(rep(1, nrow(ds)), confs)
    confs <- as.matrix(confs) # prevent stats::lm.fit() fail
    colnames(confs) <- c("_", convars)

    # Find target columns and usable rows.
    vars <- setdiff(colnames(ds), convars)
    rows <- which(is.finite(rowMeans(confs)))

    # Prepare coefficient matrix.
    coeff <- matrix(NA, nrow=length(vars), ncol=ncol(confs))
    rownames(coeff) <- vars
    colnames(coeff) <- colnames(confs)

    # Fit models.
    for(vn in vars) {
        y <- ds[,vn]; ds[,vn] <- NA
        mask <- intersect(which(is.finite(y)), rows)
        if(length(mask) < 10) next

        # Regress confounding variance.
        m <- stats::lm.fit(x=confs[mask,], y=y[mask])
	n <- sum(is.finite(m$residuals))
	if(n < 10) next

        # Update results.
        coeff[vn,] <- as.double(m$coefficients)
	ds[mask,vn] <- m$residuals
    }
    
    # Remove failed models.
    incl <- which(is.finite(rowMeans(coeff)))
    coeff <- coeff[incl,]

    # Collect results.
    ds <- ds[,rownames(coeff)]
    attr(ds, "adjustment") <- coeff
    return(ds)
}

#--------------------------------------------------------------------------

numero.prepare.adjust <- function(ds, coeff) {

    # Prepare confounder matrix.
    convars <- setdiff(colnames(coeff), "_")
    convars <- intersect(convars, colnames(ds))
    confs <- cbind(rep(1, nrow(ds)), ds[,convars])
    confs <- as.matrix(confs)
    colnames(confs) <- c("_", convars)

    # Check that confounders match parameters.
    if(ncol(coeff) != ncol(confs)) {
        cat("incompatible confounders\n")
        return(NULL)
    }
    if(sum(colnames(coeff) != colnames(confs)) > 0) {
        cat("incompatible confounders\n")
        return(NULL)
    }

    # Apply confounder adjustments.
    vars <- intersect(colnames(ds), rownames(coeff))
    for(vn in vars)
        ds[,vn] <- (ds[,vn] - confs %*% coeff[vn,])

    # Show report.
    ntotal <- (ncol(ds) - length(convars))
    cat(length(vars), " column(s) adjusted\n", sep="")

    # Return results.
    return(ds[,vars])
}

#--------------------------------------------------------------------------

numero.prepare.flatten <- function(ds, ds.orig, batch) {
    if(length(ds) < 1) return(NULL)
    if(length(batch) < 1) return(ds)

    # Check batch info.
    batch <- intersect(colnames(ds.orig), batch[[1]])
    if(length(batch) < 1) {
        cat("batch column missing\n")
        return(NULL)
    }

    # Assign batch labels.
    labels <- rep(NA, nrow(ds))
    pos <- match(rownames(ds), rownames(ds.orig))
    rows <- which(pos > 0)
    labels[rows] <- ds.orig[pos[rows],batch]

    # De-stratify values.
    suppressWarnings(ds.new <- nroDestratify(ds, labels))

    # Exclude incomplete corrections.
    incomp <- attr(ds.new, "incomplete")
    vars <- setdiff(colnames(ds.new), incomp)
    vars <- intersect(vars, colnames(ds))
    ds <- ds[,vars]; ds.new <- ds.new[,vars]
    cat(length(incomp), " incomplete correction(s)\n", sep="")

    # Check if anything to do.
    if(length(vars) < 2) {
        cat("batch correction failed\n")
        return(NULL)
    }

    # Create mappings by subgroup.
    mappings <- list()
    q <- seq(from=0, to=1, length.out=100)
    subsets <- split(1:length(labels), labels)
    for(sn in names(subsets)) {
        rows <- subsets[[sn]]
        model.in <- ds[rows,]
        model.out <- ds.new[rows,]
        model.in <- apply(model.in, 2, stats::quantile, probs=q, na.rm=T)
        model.out <- apply(model.out, 2, stats::quantile, probs=q, na.rm=T)	
	mappings[[sn]] <- list(input=model.in, output=model.out)
    }

    # Return results.
    results <- list()
    results$mappings <- mappings
    results$variables <- colnames(ds.new)
    results$batch <- batch
    attr(ds.new, "correction") <- results
    return(ds.new)
}

#--------------------------------------------------------------------------

numero.prepare.correct <- function(ds, data, param) {
    if(length(ds) < 1) return(NULL)
    if(nrow(ds) != nrow(data)) stop("Incompatible inputs.")
    if(is.null(param)) return(ds)

    # Check batch info.
    batch <- intersect(colnames(data), param$batch)
    if(length(batch) < 1) {
        cat("batch column missing\n")
        return(NULL)
    }

    # Separate batches.
    labels <- data[,param$batch]
    subsets <- split(1:length(labels), labels)

    # Check compatibility of mappings.
    mappings <- param$mappings
    pos <- match(names(subsets), names(mappings))
    if(sum(is.na(pos)) > 0) {
        cat("incompatible batch models\n")
        return(NULL)
    }

    # Apply correction models.
    vars <- intersect(colnames(ds), param$variables)
    for(sn in names(subsets)) {
        rows <- subsets[[sn]]
	suppressWarnings(
        ds[rows,vars] <- nroPostprocess(ds[rows,vars], mappings[[sn]]))
    }

    # Show report.
    cat(length(vars), " column(s) corrected\n", sep="")

    # Convert row indices to names.
    rnames <- rownames(ds)
    for(sn in names(subsets))
        subsets[[sn]] <- rnames[subsets[[sn]]]

    # Finish results.
    ds <- ds[,vars]
    attr(ds, "subsets") <- subsets
    return(ds)
}
