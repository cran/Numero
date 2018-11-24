nroPermute <- function(
    som,
    districts,
    data,
    n=10000) {

    # Convert vector input to matrix.
    if(is.factor(data)) data <- as.numeric(data)
    if(is.vector(data)) data <- as.matrix(data)

    # Check input sizes.
    if(nrow(data) != length(districts))
        stop("Incompatible inputs.")

    # Set maximum number of cycles.
    nmax <- as.integer(n)
    if(is.na(nmax)) stop("Unusable number of permutations.")
    if(nmax < 10) stop("Too few permutations.")

    # Check if any training samples.
    trcheck <- intersect(rownames(data), rownames(som$layout))
    trcheck <- (length(trcheck) > 0)
    trflags <- rep("no", length.out=ncol(data))
    if(is.null(colnames(data)) & trcheck) {
        warning("No column names in input data.")
        trflags <- rep("unknown", length.out=ncol(data))
	trcheck <- FALSE
    }

    # Check if any training variables.
    numcycl <- rep(nmax[1], length.out=ncol(data))
    if(trcheck) {
        pos <- match(colnames(data), colnames(som$centroids))
	rows <- which(pos > 0)
	numcycl[rows] <- pmin(numcycl[rows], 1000)
	trflags[rows] <- "yes"
    }

    # Estimate statistics.
    res <- .Call("nro_permute",
                 as.matrix(som$topology),
                 as.integer(districts),
                 as.matrix(data),
                 as.integer(numcycl),
                 PACKAGE="Numero")  
    if(class(res) == "character") stop(res)

    # Convert results to data frame.
    res <- data.frame(res)
    res$TRAINING <- trflags
    rownames(res) <- colnames(data)

    # Estimate P-values for evaluation variables.
    trmask <- which(trflags == "yes")
    evmask <- which(trflags != "yes")
    res$P.z <- stats::pnorm(res$Z, lower.tail=FALSE)
    res$P.z[trmask] <- NA
    res$P.freq[trmask] <- NA

    # Calculate base scores.
    trbase <- stats::quantile(res$Z[trmask], probs=0.99, na.rm=TRUE)
    evbase <- stats::quantile(res$Z[evmask], probs=0.99, na.rm=TRUE)
    if(is.na(trbase)) trbase <- evbase
    if(is.na(evbase)) evbase <- trbase

    # Estimate color amplitudes.
    trweight <- sqrt(length(trmask) + 1)
    evweight <- sqrt(length(evmask) + 1)
    zbase <- (trweight*trbase + evweight*evbase)
    zbase <- max(zbase/(trweight + evweight), 2)
    res$AMPLITUDE <- pmax((res$Z)/zbase, 0.0)
    return(res)
}
