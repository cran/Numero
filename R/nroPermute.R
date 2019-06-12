nroPermute <- function(
    som,
    districts,
    data,
    n=10000,
    clip=5.0,
    message=NA) {

    # Convert data to numeric matrix.
    data <- nroRcppMatrix(data, trim=FALSE)
    topology <- nroRcppMatrix(som$topology, trim=FALSE)

    # Remove empty data columns.
    mu <- colMeans(data, na.rm=TRUE)
    cols <- which(0*mu == 0)
    if(length(cols) < length(mu)) {
        warning("Unusable column(s) excluded.")
        data <- data[,cols]
    }

    # Check input sizes.
    if(nrow(data) != length(districts))
        stop("Incompatible inputs.")

    # Set maximum number of cycles.
    nmax <- as.integer(n[[1]])
    if(!is.finite(nmax)) stop("Unusable number of permutations.")
    if(nmax < 10) stop("Too few permutations.")

    # Check clipping parameter for extreme values.
    clip <- as.double(clip[[1]])
    if(!is.finite(clip)) {
        clip <- -1.0
    }
    else {
        if(clip < 1.0) stop("Too small clip parameter.")
    }
    
    # Check message interval.
    message <- as.double(message[[1]])
    if(!is.finite(message)) message <- -1.0

    # Truncate extreme values.
    if(clip > 0.0) {
        for(j in 1:ncol(data)) {
            dj <- data[,j]
            if(length(table(dj)) <= 2) next
            mu <- stats::median(dj, na.rm=TRUE)
            sigma <- stats::quantile(dj, c(0.3085, 0.6915), na.rm=T)
            sigma <- (sigma[2] - sigma[1])
            if(!is.finite(sigma)) next
            if(sigma < 1e-20) next
            dj <- pmin((mu + clip*sigma), dj, na.rm=TRUE)
            dj <- pmax((mu - clip*sigma), dj, na.rm=TRUE)
            data[,j] <- dj
        }
    }

    # Collect training variables.
    trvars <- attr(districts, "variables")
    if(length(trvars) < 1) trvars <- colnames(som$centroids)
    if(length(trvars) < 1)
        warning("No information on training variables.")
    if(length(colnames(data)) < 1)
        warning("No column names in input data.")
    trvars <- intersect(colnames(data), trvars)

    # Collect training samples.
    trkeys <- intersect(rownames(data), names(districts))
    if(length(rownames(data)) < 1)
        warning("No row names in input data.")
    if(length(names(districts)) < 1)
        warning("No identity information in district data.")

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
    numcycl <- rep(nmax, length.out=ncol(data))
    numcycl[trmask] <- min(1000, nmax) 

    # Estimate statistics.
    res <- .Call("nro_permute",
                 as.matrix(topology),
                 as.integer(districts),
                 as.matrix(data),
                 as.integer(numcycl),
		 as.double(message),
                 PACKAGE="Numero")  
    if(class(res) == "character") stop(res)

    # Convert results to data frame.
    output <- data.frame(res, stringsAsFactors=FALSE)
    rownames(output) <- colnames(data)
    output$TRAINING <- trflags

    # Estimate P-values.
    output$P.z <- stats::pnorm(output$Z, lower.tail=FALSE)
    output$P.z[trmask] <- NA
    output$P.freq[trmask] <- NA

    # Calculate base scores.
    z.tr <- output$Z[trmask]
    z.ev <- output$Z[evmask]
    trbase <- stats::quantile(z.tr, probs=0.95, na.rm=TRUE)
    evbase <- stats::quantile(z.ev, probs=0.95, na.rm=TRUE)
    if(is.na(trbase)) trbase <- evbase
    if(is.na(evbase)) evbase <- trbase

    # Attenuate high-scoring training variables.
    if((trbase > evbase) && (evbase > 1)) {
        mask <- which(z.tr > evbase)
	delta <- 0.2*sqrt(z.tr[mask] - evbase)
        z.tr[mask] <- (evbase + delta)
    }

    # Estimate color amplitudes.
    z <- c(z.tr, z.ev)
    rows <- c(trmask,evmask)
    zbase <- stats::quantile(z, probs=0.95, na.rm=TRUE)
    zbase <- max(1.1*zbase, 3)
    output$AMPLITUDE <- NA
    output$AMPLITUDE[rows] <- pmax(z/zbase, 0.04)
    attr(output, "zbase") <- zbase
    return(output)
}
