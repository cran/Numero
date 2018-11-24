nroColorize <- function(
   values,
   amplitudes=1,
   palette="rhodo") {

    # Convert vector inputs to matrices.
    if(is.vector(values)) values <- as.matrix(values)

    # Expand scalar amplitudes.
    if(length(amplitudes) < ncol(values)) {	
        if(length(amplitudes) != 1) warning("Amplitudes replicated.")
        amplitudes <- rep(amplitudes, length.out=ncol(values))
    }
  
    # Pre-defined palette.
    if(length(palette) < 2) {
        palette <- .Call("nro_colorize",
                         as.character(palette),
                         PACKAGE="Numero")
    }

    # Prepare output matrix.
    colrs <- matrix("", nrow=nrow(values), ncol=ncol(values))
    rownames(colrs) <- rownames(values)
    colnames(colrs) <- colnames(values)

    # Process columns.
    ncolors <- length(palette)
    for(j in 1:ncol(values)) {

        # Center on average value.
        mu <- stats::median(values[,j], na.rm=TRUE)
        z <- (values[,j] - mu)

        # Normalize by maximum deviation.
        zdev <- stats::quantile(abs(z), 0.999, na.rm=TRUE)
        z <- z/max(zdev, .Machine$double.eps)

        # Apply amplitude factor.
        z <- z*(amplitudes[j])
        z <- 0.5*(z + 1.0) # centered at 0.5

        # Determine color indices.
        ind <- round(z*ncolors + 0.5)
	ind <- pmax(1, pmin(ind, ncolors))

        # Collect color values.
        colrs[,j] <- palette[ind]
    }
    return(colrs)
}
