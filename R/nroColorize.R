nroColorize <- function(
   values,
   ranges=NULL,
   amplitudes=1,
   palette="rhodo") {

    # Convert vector input to matrix.
    values <- nroRcppMatrix(values, trim=FALSE)

    # Check that ranges are usable.
    if(length(ranges) > 0) {
        if(nrow(ranges) != ncol(values)) {
            warning("Unusable ranges, reverting to defaults.")
            ranges <- NULL
        }
    }

    # Default value ranges.
    if(length(ranges) < 1) {
        mu <- apply(values, 2, stats::median, na.rm=TRUE)
        delta <- apply(values, 2, function(x){
            m <- stats::median(x, na.rm=TRUE)
	    d <- stats::quantile(abs(x - m), 0.99, na.rm=TRUE)
            return(d)
        })
        ranges <- data.frame(MIN=(mu - delta), MAX=(mu + delta),
                             stringsAsFactors=FALSE)
        rownames(ranges) <- colnames(values)
    }

    # Return ranges only.
    if(is.null(amplitudes)) return(ranges)

    # Check if amplitudes is a data frame or a matrix.
    if(is.data.frame(amplitudes) || is.matrix(amplitudes))
         amplitudes <- amplitudes[,"AMPLITUDE"]

    # Expand scalar amplitudes.
    if(length(amplitudes) < ncol(values)) {	
        if(length(amplitudes) != 1)
	     warning("Amplitudes replicated to compatible number.")
        amplitudes <- rep(amplitudes, length.out=ncol(values))
    }

    # Pre-defined palette.
    if(length(palette) < 2) {
        palette <- .Call("nro_colorize",
                         as.character(palette[[1]]),
                         PACKAGE="Numero")
    }

    # Center by the middle of the range.
    mu <- 0.5*(ranges[,1] + ranges[,2])
    z <- sweep(values, 2, mu, `-`)

    # Standardize by the width of the range.
    delta <- 0.5*(ranges[,2] - ranges[,1] + 1e-9)
    z <- sweep(z, 2, delta, `/`)

    # Amplification factors.
    z <- sweep(z, 2, amplitudes[1:ncol(z)], `*`)
    z <- 0.5*(z + 1.0) # centered at 0.5

    # Determine color indices.
    ncolors <- length(palette)
    ind <- round(z*ncolors + 0.5)
    ind <- apply(ind, 2, function(x){
        return(pmax(1, pmin(x, ncolors)))
    })

    # Collect color values.
    empty <- rep(NA, nrow(ind))
    colrs <- apply(ind, 2, function(x, p){
        mask <- which(is.finite(x))
        y <- empty; y[mask] <- p[x[mask]]
        return(y)
    }, p=palette)

    # Return a vector.
    if(ncol(values) < 2) return(as.vector(colrs))

    # Return a data frame.
    colrs <- data.frame(colrs, stringsAsFactors=FALSE)
    rownames(colrs) <- rownames(values)
    colnames(colrs) <- colnames(values)
    return(colrs)
}
