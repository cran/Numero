nroPermute <- function(
    map,
    districts,
    data,
    n=1000,
    message=NULL,
    zbase=NULL,
    seed=0.0) {

    # Convert data to numeric matrix.
    data <- nroRcppMatrix(data, trim=FALSE)
    topology <- nroRcppMatrix(map$topology, trim=FALSE)

    # Check districts.
    districts <- nroRcppVector(districts,
        default=NULL, numeric=is.numeric(districts))

    # Check smoothness.
    smoothness <- attr(map$topology, "smoothness")
    smoothness <- nroRcppVector(smoothness[[1]], default=1)
    if(!is.finite(smoothness)) stop("Unusable map smoothness.")
    if(smoothness < 1) stop("Map smoothness less than one.")

    # Check parameters.
    n <- as.integer(nroRcppVector(n[[1]], default=1000))
    message <- nroRcppVector(message[[1]], default=-1)

    # Check if any training variables.
    trvars <- colnames(map$centroids)
    if(length(trvars) < 1)
        warning("No information on training variables.")
    if(length(colnames(data)) < 1)
        warning("No column names in input data.")
    trvars <- intersect(colnames(data), trvars)

    # Check if any training samples.
    trkeys <- rownames(map$layout)
    if(length(trkeys) < 1)
        warning("No information on training samples.")
    if(length(rownames(data)) < 1)
        warning("No row names in input data.")
    trkeys <- intersect(rownames(data), trkeys)

    # Remove empty data columns.
    mu <- colMeans(data, na.rm=TRUE)
    cols <- which(0*mu == 0)
    if(length(cols) < length(mu)) {
        warning("Unusable columns excluded.")
        data <- data[,cols]
    }

    # Check input sizes.
    if(nrow(data) != length(districts))
        stop("Incompatible inputs.")

    # Check maximum number of cycles.
    if(!is.finite(n)) stop("Unusable number of permutations.")
    if(n < 10) stop("Too few permutations.")

    # Check message interval.
    if(!is.finite(message)) message <- -1.0

    # Check seed for randomization.
    seed <- as.double(seed[[1]])
    if(!is.finite(seed)) seed <- 0

    # Set flags for training variables.
    trflags <- rep("no", length.out=ncol(data))
    cols <- which(match(colnames(data), trvars) > 0)
    if(length(trkeys) > 0) trflags[cols] <- "yes"
    if(length(rownames(data)) < 1) trflags[cols] <- "unknown"
    if(length(names(districts)) < 1) trflags[cols] <- "unknown"
    if(ncol(data) == 1) trflags <- "unknown"

    # Set maximum number of permutations.
    evmask <- which(trflags != "yes")
    trmask <- which(trflags == "yes")
    numcycl <- rep(n, length.out=ncol(data))
    numcycl[trmask] <- min(1000, n) 

    # Estimate statistics.
    res <- .Call("nro_permute",
        as.matrix(topology),
        as.double(smoothness),
        as.integer(districts),
        as.matrix(data),
        as.integer(numcycl),
        as.double(c(message, seed)),
        PACKAGE="Numero") 
    if(is.character(res)) stop(res)

    # Convert results to data frame.
    output <- data.frame(res, stringsAsFactors=FALSE)
    output$TRAINING <- trflags

    # Set rownames.
    rnames <- colnames(data)
    mask <- which(duplicated(rnames))
    if(length(mask) > 0) {
        rnames[mask] <- paste(rnames[mask], 1:length(mask), sep=".")
        warning("Duplicated column names.")
    }
    rownames(output) <- rnames

    # Estimate P-values.
    output$P.z <- stats::pnorm(output$Z, lower.tail=FALSE)
    output$P.z[trmask] <- NA
    output$P.freq[trmask] <- NA
    output$AMPLITUDE <- NA

    # Calculate base scores.
    z.tr <- output$Z[trmask]
    z.ev <- output$Z[evmask]
    trbase <- stats::quantile(z.tr, probs=0.95, na.rm=TRUE)
    evbase <- stats::quantile(z.ev, probs=0.95, na.rm=TRUE)
    if(!is.finite(trbase)) trbase <- evbase
    if(!is.finite(evbase)) evbase <- trbase
    if(!is.finite(trbase)) {
        warning("Permutations failed.")
        return(output)
    }

    # Attenuate high-scoring training variables.
    if((trbase > evbase) && (evbase > 1)) {
        mask <- which(z.tr > evbase)
	delta <- 0.2*sqrt(z.tr[mask] - evbase)
        z.tr[mask] <- (evbase + delta)
    }

    # Mean Z-magnitude for color reference.
    z <- c(z.tr, z.ev)
    if(length(zbase) < 1) {
        zbase <- stats::quantile(z, probs=0.95, na.rm=TRUE)
        zbase <- max(zbase, 3.0)
    }
    
    # Set color amplitudes.
    rows <- c(trmask, evmask)
    output$AMPLITUDE[rows] <- pmax(z/zbase, 0.04)
    attr(output, "zbase") <- as.double(zbase[[1]])
    return(output)
}
