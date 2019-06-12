nroKmeans <- function(
    data,
    k=3,
    subsample=NULL,
    balance=0,
    metric="euclid"){

    # Convert data to numeric matrix.
    data <- nroRcppMatrix(data, trim=TRUE)
    if(nrow(data) < 10) stop("Less than ten usable rows.")
    if(ncol(data) < 3) stop("Less than three usable columns.")
	 
    # Check if any rows or columns were excluded.
    if(length(attr(data, "excl.rows")) > 0)
        warning("Unusable row(s) excluded.")
    if(length(attr(data, "excl.columns")) > 0)
        warning("Unusable column(s) excluded.")

    # Ensure parameter inputs are safe for C++.
    k <- as.integer(k[[1]])
    if(is.null(subsample)) subsample <- nrow(data)
    else subsample <- as.integer(subsample[[1]])
    balance <- as.double(balance[[1]])
    metric <- as.character(metric[[1]])

    # Check the number of centroids.
    if(!is.finite(k)) stop("Unusable number of centroids.")
    if(k >= 0.5*nrow(data)) stop("Less than two rows per centroid.")
    if(k < 2) stop("Less than two centroids.")
    
    # Check subsample size.
    if(!is.finite(subsample)) stop("Unusable subsample size.")
    if(k > subsample/2)
       stop("Less than two subsampled rows per centroid.")
    if(subsample > nrow(data)) subsample <- nrow(data)

    # Check balance parameter.
    if(!is.finite(balance)) stop("Unusable balancing parameter.")
    if(balance < 0.0) stop("Negative balancing parameter.")
    if(balance > 1.0) stop("Unusable balancing parameter.")

    # Check distance metric.
    if((metric != "euclid") && (metric != "pearson"))
        stop("Unknown distance metric.")
 
    # Estimate centroids.
    res <- .Call("nro_train",
                 as.matrix(k),
                 as.matrix(NA),
                 as.matrix(data),
		 as.character(metric),
                 as.integer(subsample),
                 as.double(balance),
                 PACKAGE="Numero")
    if(class(res) == "character") stop(res)
    
    # Recode missing unit labels.
    res$layout[which(res$layout == 0)] <- NA
    res$layout <- data.frame(BMC=res$layout, RESIDUAL=res$residuals)
    rownames(res$layout) <- rownames(data)
    res$residuals <- NULL

    # Copy column names.
    colnames(res$centroids) <- colnames(data)
    return(res)
}
