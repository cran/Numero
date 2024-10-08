numero.create <- function(
    data,
    radius=NULL,
    smoothness=NULL,
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
    trdata <- nroPreprocess(data=data, method="", clip=NA, trim=TRUE)
    if(nrow(trdata) < 10) stop("Not enough usable rows.")
    if(ncol(trdata) < 3) stop("Not enough usable columns.")

    # Set map radius.
    cat("\nModeling setup:\n")
    if(is.null(radius)) {
        radius <- floor(0.5*log(nrow(trdata) + 2000))
        cat("automatic radius set to ", radius, "\n", sep="")
    }
    
    # Set map smoothness.
    if(is.null(smoothness)) {
        smoothness <- (0.3*log(radius - 1) + 1)
        cat("automatic smoothness set to ", smoothness, "\n", sep="")
    }

    # Check radius.
    if(!is.finite(radius[[1]])) stop("Unusable radius.")
    if(radius[[1]] < 2) stop("Unusable radius.")

    # Check smoothness.
    smoothness <- as.double(smoothness[[1]])
    if(!is.finite(smoothness)) stop("Unusable smoothness.")
    if(smoothness < 1) stop("Smoothness too low.")
    if(smoothness > 0.49*radius) stop("Smoothness too high.")

    # Check subsample.
    if(!is.null(subsample)) {
        if(!is.finite(subsample[[1]])) stop("Unusable subsample.")
        if(subsample[[1]] < 10) stop("Unusable subsample.")
	subsample <- as.integer(min(subsample[[1]], nrow(trdata)))
    }

    # Print report.
    cat(nrow(trdata), " / ", nrow(data), " rows included\n", sep="")
    cat(ncol(trdata), " / ", ncol(data), " columns included\n", sep="")

    # K-means clustering.
    cat("\nK-means clustering:\n")
    km <- nroKmeans(data=trdata, subsample=subsample, balance=1, message=10)
    cat(km$subsample, " subsamples\n", sep="")
    cat(length(km$history), " training cycles\n", sep="")

    # Create a SOM.
    cat("\nSelf-organizing map:\n")
    sm <- nroKohonen(seeds=km$centroids,
        radius=radius, smoothness=smoothness)
    
    # Fit the SOM to training data.
    sm <- nroTrain(map=sm, data=trdata, subsample=subsample, message=10)
    cat(sm$subsample, " subsamples\n", sep="")
    cat(length(sm$history), " training cycles\n", sep="")

    # Evaluate fit quality.
    matches <- nroMatch(centroids=sm, data=trdata)
    layout <- data.frame(BMC=matches, attr(matches, "quality"))
    rownames(layout) <- names(matches)

    # Set permutation design.
    r <- ((13*(1:ncol(trdata)) + 127)%%177)/177
    cols <- unique(rep(order(r), length.out=100))
    nperm <- round(20000/length(cols))

    # Set base amplitude for colors.
    cat("\nColor amplitude:\n")
    suppressWarnings(
        stats <- nroPermute(map=sm, districts=layout$BMC,
            data=trdata[,cols], n=nperm, message=10))
    if(nrow(stats) < ncol(trdata))
        cat(nrow(stats), " / ", ncol(trdata), " columns sampled\n", sep="")
    cat(sum(stats$N.cycles), " permutations\n", sep="")
    cat("reference Z-score ", attr(stats, "zbase"), " \n", sep="")

    # Collect results.
    output$kmeans <- km
    output$map <- sm
    output$layout <- layout
    output$data <- data
    output$zbase <- attr(stats, "zbase")
    return(output)
}
