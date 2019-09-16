numero.quality <- function(
    model,
    data=NULL) {

    # Continue analyses.
    output <- list(stamp=date())
    cat("\n*** numero.quality ***\n", output$stamp, "\n", sep="")

    # Check that resources are available.
    if(is.null(model$som)) stop("Self-organizing map not available.")

    # Determine data point layout.
    layout <- numero.quality.layout(model, data)

    # Calculate component planes.
    comps <- numero.quality.planes(model, data, layout)

    # Estimate statistics.
    stats <- numero.quality.statistics(model, layout)

    # Determine district ranges.
    ranges <- nroColorize(comps, amplitudes=NULL)

    # Collect results.
    output$som <- model$som
    output$layout <- layout
    output$planes <- comps
    output$ranges <- ranges
    output$palette <- "fire"
    output$statistics <- stats
    return(output)
}

#-------------------------------------------------------------------------

numero.quality.layout <- function(model, data) {
    if(is.null(data)) return(model$layout)

    # Check dataset compatibility.
    vars <- intersect(colnames(model$data), colnames(data))
    missed <- setdiff(colnames(model$data), vars)
    if(length(vars) < 2) stop("Too few training variables in data.")
    if(length(vars) < ncol(model$data))
        warning("Dataset does not contain all training variables.")

    # Assign district locations.
    suppressWarnings(matches <- nroMatch(centroids=model$som, data=data))

    layout <- data.frame(BMC=matches, attr(matches, "quality"))
    rownames(layout) <- names(matches)
    return(layout)
}

#-------------------------------------------------------------------------

numero.quality.planes <- function(model, data, layout) {
    if(is.null(data)) data <- model$data

    # Component planes.
    comps <- nroAggregate(topology=model$som$topology,
                          data=layout, districts=layout$BMC)
    comps$HISTOGRAM <- nroAggregate(topology=model$som$topology,
                                    districts=layout$BMC)
    comps$BMC <- NULL

    # Adjust coverage for missing training variables.
    pos <- match(colnames(model$data), colnames(data))
    r <- sum(is.finite(pos))/ncol(model$data)
    comps$COVERAGE <- r*(comps$COVERAGE)
    attr(comps, "binary") <- "COVERAGE"
    return(comps)
}

#-------------------------------------------------------------------------

numero.quality.statistics <- function(model, layout) {
    cat("\nStatistics:\n")

    # Separate district labels from quality measures.
    bmc <- layout$BMC
    names(bmc) <- rownames(layout)
    layout$BMC <- NULL

    # Add jitter to coverage to prevent numerical artefacts.
    r <- stats::rnorm(nrow(layout))
    layout$COVERAGE <- (layout$COVERAGE + 0.01*r)

    # Permutation analysis.
    stats <- nroPermute(som=model$som, districts=bmc,
                        data=layout, n=1000)

    # Observed variation in sample density.
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
    stats <- rbind(stats, stats[1,])
    stats[nrow(stats),] <- NA
    stats[nrow(stats),"SCORE"] <- x
    stats[nrow(stats),"Z"] <- z
    stats[nrow(stats),"P.z"] <- stats::pnorm(z, lower.tail=FALSE)
    stats[nrow(stats),"P.freq"] <- mean((nulls >= x), na.rm=TRUE)
    stats[nrow(stats),"N.data"] <- npoints
    stats[nrow(stats),"N.cycles"] <- length(nulls)
    stats[nrow(stats),"TRAINING"] <- "no"
    rownames(stats) <- c(colnames(layout), "HISTOGRAM")

    # Update base score and apmplitudes.
    zbase <- max(c(stats$Z, 3), na.rm=TRUE)
    stats$AMPLITUDE <- pmax((stats$Z)/zbase, 0.04)
    attr(stats, "zbase") <- zbase

    # Show report.
    cat(nrow(stats), " quality measures\n", sep="")
    cat(sum(stats$N.cycles), " permutations\n", sep="")
    return(stats)
}