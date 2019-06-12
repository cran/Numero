nroPrune <- function(
    data,
    modules) {

    # Convert data to numeric matrix.
    dfbit <- is.data.frame(data)
    data <- nroRcppMatrix(data, trim=TRUE)
    if(nrow(data) < 10) stop("Less than ten usable rows.")
    if(ncol(data) < 7) stop("Less than seven usable columns.")
	 
    # Check if any rows or columns were excluded.
    if(length(attr(data, "excl.rows")) > 0)
        warning("Unusable row(s) excluded.")
    if(length(attr(data, "excl.columns")) > 0)
        warning("Unusable column(s) excluded.")

    # Create a new pruning model.
    if(!is.list(modules) && !is.matrix(modules)) {

        # Standardize training data.
	trdata <- apply(data, 2, rank, na.last="keep")
        trdata <- scale.default(trdata)

        # Transpose for detecting column clusters.
	trdata <- t(trdata)
	if(length(colnames(data)) == ncol(data))
	    rownames(trdata) <- colnames(data)
	else
	    rownames(trdata) <- (1:ncol(data))

        # Check pruning parameter.
        k <- as.integer(modules[[1]])
        if(!is.finite(k)) stop("Unusable pruning parameter.")
        if((k >= 0.499*nrow(trdata)) || (k < 3)) {
	    k <- max(floor(0.499*nrow(trdata)), 3)
	    warning(paste("Pruning parameter set to ", k, ".\n", sep=""))
        }

        # Determine cluster structure.
        res <- .Call("nro_train",
                     as.matrix(k),
                     as.matrix(NA),
                     as.matrix(trdata),
		     "pearson",
                     500,
                     0.0,
                     PACKAGE="Numero")
        if(class(res) == "character") stop(res)

        # Subsets of column names.
        modules <- split(rownames(trdata), res$layout)

        # Principal component coefficients.
	for(c in 1:length(modules))
            modules[[c]] <- nroPrune.principal(data, modules[[c]])
    }

    # Check if parameters are embedded in a data frame.
    if(is.matrix(modules) || is.data.frame(modules))
        modules <- attr(modules, "modules")

    # Check that all variables are available.
    vars <- character()
    for(c in 1:length(modules))
        vars <- c(vars, names(modules[[c]]))
    pos <- match(vars, colnames(data))
    if(sum(is.na(pos)) > 0) stop("Incompatible dataset.")

    # Calculate principal components for each module.
    output <- matrix(NA, nrow=nrow(data), ncol=length(modules))
    for(c in 1:length(modules)) {
        x <- nroPrune.merge(data, modules[[c]])
	if(!is.null(x)) output[,c] <- x
    }

    # Convert to data frame.
    if(dfbit) output <- data.frame(output, stringsAsFactors=FALSE)

    # Set column names.
    rownames(output) <- rownames(data)
    colnames(output) <- paste("MODULE", 1:ncol(output), sep=".")
    attr(output, "modules") <- modules
    return(output)
}

#---------------------------------------------------------------------------

nroPrune.principal <- function(data, vars) {
    if(length(vars) < 1) return(NULL)
    if(length(vars) == 1) {
        sigma <- stats::sd(data[,vars], na.rm=TRUE)
	if(!is.finite(sigma)) output <- 1
        else output <- 1/max(sigma, 1e-20)
	names(output) <- vars
	return(output)
    }

    # Impute missing values.
    res <- .Call("nro_impute",
                 as.matrix(data[,vars]),
		 500,
                 PACKAGE="Numero")
    if(class(res) == "character" ) stop(res)

    # Find usable rows.
    mask <- which(0*rowMeans(res) == 0)
    if(length(mask) <= ncol(res)) {
        info <- paste(vars, collapse=", ")
        warning(paste("Module failed: ", info, ".", sep=""))
        return(NULL)
    }

    # Principal component analysis.
    pca <- stats::prcomp(res[mask,])
    output <- pca$rotation[,1]
    names(output) <- vars

    # Apply rotation.
    scores <- res %*% output

    # Standardize scale.
    sigma <- stats::sd(scores, na.rm=TRUE)
    if(is.finite(sigma)) {
        output <- output/max(sigma, 1e-20)
        scores <- scores/max(sigma, 1e-20)
    }

    # Eigenvalues.
    lambda <- as.double(pca$sdev)^2

    # Set location.
    attr(output, "location") <- mean(scores, na.rm=TRUE)
    attr(output, "explained") <- lambda[1]/sum(lambda)
    return(output)
}

#---------------------------------------------------------------------------

nroPrune.merge <- function(data, coeff) {
    if(length(coeff) < 1) return(NULL)

    # Single dimension.
    vars <- names(coeff)
    if(length(coeff) == 1)
        return(coeff*(data[,vars]))

    # Impute missing values.
    res <- .Call("nro_impute",
                 as.matrix(data[,vars]),
		 500,
                 PACKAGE="Numero")
    if(class(res) == "character" ) stop(res)

    # Apply rotation.
    output <- res %*% coeff

    # Standardize location.
    mu <- attr(coeff, "location")
    if(!is.null(mu)) output <- (output - mu)
    return(output)
}