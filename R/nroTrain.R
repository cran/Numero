nroTrain <- function(
    map,
    data,
    subsample=NULL,
    metric="euclid",
    message=NULL) {

    # Convert data to numeric matrix.
    data <- nroRcppMatrix(data, trim=TRUE)
    if(nrow(data) < 10) stop("Less than ten usable rows.")
    if(ncol(data) < 3) stop("Less than three usable columns.")
	 
    # Check if any rows or columns were excluded.
    if(length(attr(data, "excl.rows")) > 0)
        warning("Unusable row(s) excluded.")
    if(length(attr(data, "excl.columns")) > 0)
        warning("Unusable column(s) excluded.")

    # Convert centroids to numeric matrix.
    centroids <- nroRcppMatrix(map$centroids, trim=FALSE)
    topology <- nroRcppMatrix(map$topology, trim=FALSE)

    # Check map smoothness.
    smoothness <- attr(map$topology, "smoothness")
    smoothness <- nroRcppVector(smoothness[[1]], default=NA)
    if(!is.finite(smoothness)) stop("Unusable map smoothness.")
    if(smoothness < 1) stop("Map smoothness less than one.")

    # Check that column names match.
    somnames <- colnames(centroids)
    vars <- intersect(colnames(data), somnames)
    if(length(vars) < 3)
        stop("Less than three usable training columns available.")
    if(length(vars) < length(somnames))
        warning("One or more training column(s) unavailable.")

    # Check parameters.
    metric <- nroRcppVector(metric[[1]], default="", numeric=FALSE)
    message <- nroRcppVector(message[[1]], default=-1)
    subsample <- as.integer(nroRcppVector(subsample[[1]], default=NA))

    # Automatic subsample.
    if(!is.finite(subsample)) {
        subsample <- 10*sqrt(nrow(data))*sqrt(nrow(topology))
	subsample <- min(subsample, 0.95*nrow(data), na.rm=TRUE)
	subsample <- round(subsample)
    }

    # Check subsample size.
    if(subsample > nrow(data)) subsample <- nrow(data)
    if(subsample < (nrow(centroids) + 10))
        stop("Too small subsample.")

    # Check distance metric.
    if((metric != "euclid") && (metric != "pearson"))
        stop("Unknown distance metric.")
    
    # Check message interval.
    if(!is.finite(message)) message <- -1.0

    # Train the SOM.
    res <- .Call("nro_train",
        as.matrix(topology),
        as.double(smoothness),
        as.matrix(centroids[,vars]),
        as.matrix(data[,vars]),
        as.character(metric),
        as.integer(subsample),
        0.0,
        as.double(message),
        PACKAGE="Numero")
    if(is.character(res)) stop(res)

    # Recode missing unit labels.
    res$layout[which(res$layout <= 0)] <- NA
    res$layout <- data.frame(BMC=res$layout, RESIDUAL=res$residuals)
    rownames(res$layout) <- rownames(data)
    
    # Return results.
    map$layout <- res$layout
    map$centroids[,vars] <- res$centroids
    map$subsample <- subsample
    map$history <- res$history
    map$metric <- metric
    return(map)
}
