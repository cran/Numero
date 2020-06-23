nroCoalesce.prune <- function(
    network,
    degree) {

    # Check input size.
    if(ncol(network) < 3) stop("Less than three columns.")
    if(nrow(network) < 2) stop("Less than two rows.")

    # Check degree.
    degree <- as.double(degree[[1]])
    if(!is.finite(degree)) stop("Unusable degree.")
    if(degree < 1.0) stop("Unusable degree.")

    # Remove redundant edges.
    weights <- .Call("nro_coalesce_prune",
        as.character(network[,1]),
        as.character(network[,2]),
        as.double(network[,3]),
        as.double(degree),
        PACKAGE="Numero")
    if(is.character(weights)) stop(weights)

    # Return results.
    mask <- which(weights > 0)
    return(network[mask,])
}
