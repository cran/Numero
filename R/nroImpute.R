nroImpute <- function(
    data,
    subsample=500,
    standard=TRUE,
    message=NULL) {

    # Convert input to numeric matrix.
    dfbit <- is.data.frame(data)
    data <- nroRcppMatrix(data, trim=FALSE)
    binary <- attr(data, "binary")
    binary <- nroRcppVector(binary, default=NULL,
        numeric=is.numeric(binary))

    # Check input size.
    if(ncol(data) < 2) {
        warning("Less than two columns.")
        return(data)
    }
    if(nrow(data) < 2) {
        warning("Less than two rows.")
        return(data)
    }

    # Ensure inputs are safe for C++.
    subsample <- nroRcppVector(subsample[[1]], default=nrow(data))
    standard <- (nroRcppVector(standard[[1]], default=1) == 1)
    message <- nroRcppVector(message[[1]], default=-1)

    # Check subsample size.
    if(!is.finite(subsample)) stop("Unusable subsample size.")
    if(subsample > nrow(data)) subsample <- nrow(data)
    if(subsample < 10) stop("Too small subsample.")

    # Check message interval.
    if(!is.finite(message)) stop("Unusable message interval.")

    # Copy names.
    rnames <- rownames(data)
    cnames <- colnames(data)
    
    # Detect numeric variables.
    numerics <- c()
    for(j in 1:ncol(data)) {
	flags <- is.finite(data[,j])
        if(sum(flags) > 0) numerics <- c(numerics, j)
    }
    if(length(numerics) < 2) {
        warning("Less than two numeric columns.")
        return(data)
    }
    if(length(numerics) < ncol(data))
        warning("Non-numeric column(s) excluded.")

    # Standardize data.
    sigma <- rep(1, ncol(data))
    if(standard) {
        for(j in numerics) {
	    x <- data[,j]
            s <- stats::sd(x, na.rm=TRUE)
            if(!is.finite(s)) next
            if(s <= 0.0) next
	    sigma[j] <- s
            data[,j] <- x/s
	}
    }

    # Impute missing values.
    res <- .Call("nro_impute",
        as.matrix(data[,numerics]),
        as.integer(subsample),
        as.double(message),
        PACKAGE="Numero")
    if(is.character(res)) stop(res)
    data[,numerics] <- res

    # Restore original scale.
    for(j in numerics)
        data[,j] <- (sigma[j])*(data[,j])

    # Convert to data frame.
    if(dfbit) data <- as.data.frame(data, stringsAsFactors=FALSE)

    # Restore names and attributes.
    rownames(data) <- rnames
    colnames(data) <- cnames
    attr(data, "binary") <- intersect(binary, cnames)
    return(data)
}
