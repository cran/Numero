nroCoalesce.split <- function(
    network,
    mtx=FALSE) {

    # Check input size.
    if(ncol(network) < 3) stop("Less than three columns.")
    if(nrow(network) < 2) stop("Less than two rows.")
    
    # Community detection.
    res <- .Call("nro_coalesce_split",
        as.character(network[,1]),
        as.character(network[,2]),
        as.double(network[,3]),
        PACKAGE="Numero")
    if(is.character(res)) stop(res)

    # Collect modules.
    modules <- split(x=res$members, f=res$modules)
    subnets <- split(x=network, f=res$subnets)
    
    # Add matrix versions.
    matrices <- NULL
    if(mtx[[1]]) {
        matrices <- list()
        for(m in names(subnets)) {
            edges <- subnets[[m]]
	    tails <- edges$TAIL
	    heads <- edges$HEAD
	    rvals <- edges$R

            # Prepare correlation matrix.
	    keys <- sort(unique(c(tails, heads)))
            r <- matrix(NA, nrow=length(keys), ncol=length(keys))
            rownames(r) <- keys
            colnames(r) <- keys

            # Copy correlation coefficients.
            for(i in 1:nrow(edges))
	        r[tails[i], heads[i]] <- rvals[i]
            for(i in 1:nrow(edges))
	        r[heads[i], tails[i]] <- rvals[i]
            for(i in 1:length(keys))
                r[i,i] <- max(is.finite(r[,i]))
	    matrices[[m]] <- r
        }
    }

    # Return results.
    output <- list()
    output$modules <- split(x=res$members, f=res$modules)
    output$subnets <- split(x=network, f=res$subnets)
    output$matrices <- matrices
    return(output)
}
