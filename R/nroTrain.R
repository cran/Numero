nroTrain <- function(
    som,
    data,
    subsample=NULL) {
    if(length(subsample) == 0) subsample <- nrow(data)

    # Check inputs.
    M <- nrow(data); N <- ncol(data)
    if(M*N < 1) stop("Unusable data matrix.")
    if(ncol(som$centroids) != N)
        stop("Incompatible centroids.")
    if(nrow(som$topology) != nrow(som$centroids))
        stop("Incompatible topology.")
    if(as.integer(subsample) < 5*nrow(som$centroids))
        if(subsample < M) stop( "Too thin subsampling." )

    # Check that column names match.
    datnames <- colnames(data)
    somnames <- colnames(som$centroids)
    if(length(datnames) != length(somnames))
        stop("Incompatible centroid variables.")
    if(sum(datnames != somnames) > 0)
        stop("Incompatible centroid variables.")
      
    # Train the SOM.
    res <- .Call("nro_train",
                 as.matrix(som$topology),
                 as.matrix(som$centroids),
                 as.matrix(data),
                 as.integer(subsample),
                 0.0,
                 PACKAGE="Numero" )
    if(class(res) == "character") stop(res)

    # Recode missing unit labels.
    res$layout[which(res$layout == 0)] <- NA
    res$layout <- data.frame(BMC=res$layout)
    rownames(res$layout) <- rownames(data)
    
    # Return results.
    som$layout <- res$layout
    som$centroids <- res$centroids
    som$history <- res$history
    colnames(som$centroids) <- somnames
    return(som)
}
