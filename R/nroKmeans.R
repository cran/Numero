nroKmeans <- function(
    data,
    k=3,
    subsample=NULL,
    balance=0){

    # Check inputs.
    M <- nrow(data); N <- ncol(data)
    if(M*N < 1) stop("Unusable data matrix.")
    if(as.integer(k) > M/2) stop( "Too many centroids." )
    if(as.integer(k) < 2) stop( "Too few centroids." )
    if(length(subsample) < 1) subsample <- nrow(data)
    if(as.integer(subsample) < 5*as.integer(k))
        if(subsample < M) stop( "Too thin subsampling." )
    if(as.double(balance) < 0.0) stop( "Unusable balancing parameter." )
    if(as.double(balance) > 1.0) stop( "Unusable balancing parameter." )

    # Estimate centroids.
    res <- .Call("nro_train",
                 as.matrix(k),
                 as.matrix(NA),
                 as.matrix(data),
                 as.integer(subsample),
                 as.double(balance),
                 PACKAGE="Numero")
    if(class(res) == "character") stop(res)
    
    # Recode missing unit labels.
    res$layout[which(res$layout == 0)] <- NA
    res$layout <- data.frame(BMC=res$layout)
    rownames(res$layout) <- rownames(data)
    
    # Copy column names.
    colnames(res$centroids) <- colnames(data)
    return(res)
}
