nroCoalesce <- function(
    data,
    threshold=0.25,
    degree=4.0) {
    
    # Convert data to numeric matrix.
    dfbit <- is.data.frame(data)
    data <- nroRcppMatrix(data, trim=TRUE)
    if(nrow(data) < 10) stop("Less than ten usable rows.")
    if(ncol(data) < 5) stop("Less than five usable columns.")

    # Check if any rows or columns were excluded.
    if(length(attr(data, "excl.rows")) > 0)
        warning("Unusable row(s) excluded.")
    if(length(attr(data, "excl.columns")) > 0)
        warning("Unusable column(s) excluded.")

    # Check degree.
    degree <- as.double(degree[[1]])
    if(!is.finite(degree)) stop("Unusable degree.")
    if(degree < 1.0) stop("Unusable degree.")
    
    # Check threshold.
    threshold <- as.double(threshold[[1]])
    if(!is.finite(threshold)) stop("Unusable threshold.")
    if(threshold >= 1) stop("Unusable threshold.")
    if(threshold < 0) stop("Unusable threshold.")

    # Calculate correlations.
    net <- .Call("nro_coalesce_network",
         as.matrix(data),
         as.character(colnames(data)),
         as.double(threshold),
	 PACKAGE="Numero")
    if(is.character(net)) stop(net)

    # Convert to data frame.
    net <- data.frame(net, stringsAsFactors=FALSE)
    net$R <- net$WEIGHT
    net$WEIGHT <- (net$R)^2

    # Check if any edges to be merged.
    if(nrow(net) < 2) return(data)

    # Prune redundant edges.
    net <- nroCoalesce.prune(network=net, degree=degree)

    # Community detection.
    res <- nroCoalesce.split(network=net)

    # Merge redundant columns.
    output <- nroCoalesce.merge(data=data, modules=res$modules)
    mods <- attr(output, "modules")

    # Return results.
    if(dfbit) output <- data.frame(output, stringsAsFactors=FALSE)
    attr(output, "network") <- net
    attr(output, "modules") <- mods
    attr(output, "subnets") <- res$subnets
    return(output)
}
