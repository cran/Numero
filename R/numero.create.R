numero.create <- function(
    data,
    radius=NULL,
    subsample=NULL) {

    # Start processing.
    output <- list(stamp=date())
    cat("\n*** numero.create ***\n", output$stamp, "\n", sep="")

    # Check row names.
    if(length(data) < 1) stop("Empty input.")
    if(length(rownames(data)) != nrow(data))
        stop("Unusable row names.")
    if(anyDuplicated(rownames(data)) > 0)
        stop("Duplicated row names.")

    # Check training data.
    trdata <- nroPreprocess(data=data, method="", clip=NA)
    if(nrow(trdata) < 10) stop("Not enough usable rows.")
    if(ncol(trdata) < 3) stop("Not enough usable columns.")

    # Set map radius.
    cat("\nSelf-organizing map:\n")
    if(is.null(radius)) {
        radius <- round(log10(nrow(trdata) + 1))
        if(radius < 2) radius <- 2
        cat("automatic radius set to ", radius, "\n", sep="")
    }

    # Check radius.
    if(!is.finite(radius[[1]])) stop("Unusable radius.")
    if(radius[[1]] < 2) stop("Unusable radius.")

    # Check subsample.
    if(is.null(subsample)) subsample <- nrow(trdata)
    if(!is.finite(subsample[[1]])) stop("Unusable subsample.")
    if(subsample[[1]] < 10) stop("Unusable subsample.")

    # Print report.
    cat(nrow(trdata), " / ", nrow(data), " rows included\n", sep="")
    cat(ncol(trdata), " / ", ncol(data), " columns included\n", sep="")

    # K-means clustering.
    cat("\nK-means clustering:\n")
    km <- nroKmeans(data=trdata, subsample=subsample, message=10)
    cat(length(km$history), " training cycles\n", sep="")

    # Create a SOM.
    cat("\nSelf-organizing map:\n")
    sm <- nroKohonen(seeds=km$centroids, radius=radius)
    
    # Fit the SOM to training data.
    sm <- nroTrain(som=sm, data=trdata, subsample=subsample, message=10)
    cat(length(sm$history), " training cycles\n", sep="")

    # Evaluate map quality.
    matches <- nroMatch(centroids=sm, data=trdata)
    layout <- data.frame(BMC=matches, attr(matches, "quality"))
    rownames(layout) <- names(matches)

    # Collect results.
    output$kmeans <- km
    output$som <- sm
    output$layout <- layout
    output$data <- data
    return(output)
}