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

    # Check that column names match.
    somnames <- colnames(centroids)
    vars <- intersect(colnames(data), somnames)
    if(length(vars) < 3)
        stop("Less than three usable training columns available.")
    if(length(vars) < length(somnames))
        warning("One or more training column(s) unavailable.")

    # Automatic subsample.
    if(is.null(subsample)) {
        subsample <- 10*sqrt(nrow(data))*sqrt(nrow(topology))
	subsample <- min(subsample, 0.95*nrow(data), na.rm=TRUE)
	subsample <- round(subsample)
    }

    # Ensure inputs are safe for C++.
    else subsample <- as.integer(subsample[[1]])
    if(is.null(message)) message <- -1.0
    else message <- as.double(message[[1]])
    if(is.null(metric)) metric <- "euclid"
    else metric <- as.character(metric[[1]])

    # Check subsample size.
    if(!is.finite(subsample)) subsample <- nrow(data)
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
                 as.matrix(centroids[,vars]),
                 as.matrix(data[,vars]),
         	 as.character(metric),
                 as.integer(subsample),
                 0.0,
		 as.double(message),
                 PACKAGE="Numero" )
    if(class(res) == "character") stop(res)

    # Recode missing unit labels.
    res$layout[which(res$layout == 0)] <- NA
    res$residuals[which(res$layout == 0)] <- NA
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
