numero.quality <- function(
    model,
    data=NULL) {

    # Continue analyses.
    output <- list(stamp=date())
    cat("\n*** numero.quality ***\n", output$stamp, "\n", sep="")

    # Check that resources are available.
    if(is.null(model$map)) stop("Self-organizing map not available.")

    # Determine data point layout.
    layout <- numero.quality.layout(model, data)

    # Calculate component planes.
    comps <- numero.quality.planes(model, data, layout)
    
    # Determine district ranges.
    colrs <- nroColorize(comps)
    ranges <- attr(colrs, "ranges")

    # Estimate quality statistics.
    stats.qc <- numero.quality.control(model, layout, ranges)

    # Collect results.
    output$map <- model$map
    output$layout <- layout
    output$planes <- comps
    output$ranges <- ranges
    output$palette <- attr(colrs, "palette")
    output$statistics <- stats.qc
    output$zbase <- model$zbase
    return(output)
}

#-------------------------------------------------------------------------

numero.quality.layout <- function(model, data) {
    if(length(data) < 1) return(model$layout)

    # Check dataset compatibility.
    vars <- intersect(colnames(model$data), colnames(data))
    missed <- setdiff(colnames(model$data), vars)
    if(length(vars) < 2) stop("Too few training variables in data.")
    if(length(vars) < ncol(model$data))
        warning("Dataset does not contain all training variables.")

    # Check for missing data points.
    mu <- rowMeans(data[,vars], na.rm=TRUE)
    valid <- which(is.finite(mu))
    if(length(valid) < 1) stop("No usable values for training variables.")
    if(length(valid) < nrow(data)) warning("Unusable rows excluded.")

    # Assign district locations.
    suppressWarnings(matches <- nroMatch(centroids=model$map,
        data=data[valid,]))

    layout <- data.frame(BMC=matches, attr(matches, "quality"))
    rownames(layout) <- names(matches)
    return(layout)
}

#-------------------------------------------------------------------------

numero.quality.planes <- function(model, data, layout) {
    if(length(data) < 1) data <- model$data

    # Component planes.
    h <- nroAggregate(topology=model$map, districts=layout[,"BMC"])
    comps <- nroAggregate(topology=model$map,
        data=layout[,setdiff(colnames(layout),"BMC")],
	districts=layout[,"BMC"])
    comps <- cbind(comps, HISTOGRAM=h)

    # Adjust coverage for missing training variables.
    pos <- match(colnames(model$data), colnames(data))
    r <- sum(is.finite(pos))/ncol(model$data)
    comps[,"COVERAGE"] <- r*(comps[,"COVERAGE"])
    attr(comps, "binary") <- "COVERAGE"
    return(comps)
}

#-------------------------------------------------------------------------

numero.quality.control <- function(model, data, ranges, refranges) {
    cat("\nQuality statistics:\n")

    # Separate district labels from quality measures.
    bmc <- data[,"BMC"]
    names(bmc) <- rownames(data)
    data[,"BMC"] <- NULL

    # Add jitter to coverage to prevent numerical artefacts.
    r <- ((13*(1:nrow(data)) + 127)%%177)/177
    data[,"COVERAGE"] <- (data[,"COVERAGE"] + 0.001*r)

    # Permutation analysis.
    stats <- nroPermute(map=model$map, districts=bmc,
                        data=data, n=1000)
    attr(stats, "zbase") <- NULL

    # Observed variation in sample density.
    dens <- nroAggregate(topology=model$map, districts=bmc)
    h <- table(bmc)
    levs <- as.integer(names(h))
    x <- stats::sd(h)

    # Simulate null distribution of density variation.
    npoints <- sum(h)
    nulls <- rep(NA, 1000)
    for(i in 1:length(nulls)) {
        bmc <- sample(levs, npoints, replace=TRUE)
        nulls[i] <- stats::sd(table(bmc))
    }

    # Statistics for density variation.
    mu <- mean(nulls, na.rm=TRUE)
    sigma <- stats::sd(nulls, na.rm=TRUE)
    z <- (x - mu)/(sigma + 1e-9)

    # Append to the statistics data frame.
    ind <- nrow(stats)
    stats <- rbind(stats, stats[1,])
    stats[ind,] <- NA
    stats[ind,"SCORE"] <- x
    stats[ind,"Z"] <- z
    stats[ind,"P.z"] <- stats::pnorm(z, lower.tail=FALSE)
    stats[ind,"P.freq"] <- mean((nulls >= x), na.rm=TRUE)
    stats[ind,"N.data"] <- npoints
    stats[ind,"N.cycles"] <- length(nulls)
    stats[ind,"TRAINING"] <- "no"
    stats[ind,"AMPLITUDE"] <- NA
    rownames(stats) <- c(colnames(data), "HISTOGRAM")

    # Set amplitudes.
    stats["COVERAGE","AMPLITUDE"] <- (ranges["COVERAGE","MAX"] -
        ranges["COVERAGE","MIN"])/(1.0 - 0.0)
    stats["RESIDUAL.z","AMPLITUDE"] <- (ranges["RESIDUAL.z","MAX"] -
        ranges["RESIDUAL.z","MIN"])/(3.5 + 3.5)
    stats["RESIDUAL","AMPLITUDE"] <- stats["RESIDUAL.z","AMPLITUDE"]
    stats["HISTOGRAM","AMPLITUDE"] <- (max(dens) - min(dens))/max(dens)

    # Show report.
    cat(nrow(stats), " quality measures\n", sep="")
    cat(sum(stats[,"N.cycles"]), " permutations\n", sep="")
    return(stats)
}
