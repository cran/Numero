nroTrain <- function(
    som,
    data,
    subsample=NULL,
    metric="euclid") {

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
    centroids <- nroRcppMatrix(som$centroids, trim=FALSE)
    topology <- nroRcppMatrix(som$topology, trim=FALSE)

    # Check that column names match.
    somnames <- colnames(centroids)
    vars <- intersect(colnames(data), somnames)
    if(length(vars) < 3)
        stop("Less than three usable training columns available.")
    if(length(vars) < length(somnames))
        warning("One or more training column(s) unavailable.")

    # Ensure inputs are safe for C++.
    if(is.null(subsample)) subsample <- nrow(data)
    else subsample <- as.integer(subsample[[1]])
    metric <- as.character(metric[[1]])

    # Check subsample size.
    if(!is.finite(subsample)) subsample <- nrow(data)
    if(subsample > nrow(data)) subsample <- nrow(data)
    if(subsample < 10) stop("Subsample smaller than ten.")

    # Check distance metric.
    if((metric != "euclid") && (metric != "pearson"))
        stop("Unknown distance metric.")
    
    # Train the SOM.
    res <- .Call("nro_train",
                 as.matrix(topology),
                 as.matrix(centroids[,vars]),
                 as.matrix(data[,vars]),
         	 as.character(metric),
                 as.integer(subsample),
                 0.0,
                 PACKAGE="Numero" )
    if(class(res) == "character") stop(res)

    # Recode missing unit labels.
    res$layout[which(res$layout == 0)] <- NA
    res$residuals[which(res$layout == 0)] <- NA
    res$layout <- data.frame(BMC=res$layout, RESIDUAL=res$residuals)
    rownames(res$layout) <- rownames(data)
    
    # Return results.
    som$layout <- res$layout
    som$centroids[,vars] <- res$centroids
    som$history <- res$history
    som$metric <- metric
    return(som)
}
