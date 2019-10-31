nroImpute <- function(
    data,
    subsample=500,
    standard=TRUE,
    message=NULL) {

    # Convert input to numeric matrix.
    dfbit <- is.data.frame(data)
    data <- nroRcppMatrix(data, trim=FALSE)    
    binary <- attr(data, "binary")

    # Check input size.
    if(length(data) < 1) return(data)
    if(ncol(data) < 2) {
        warning("Less than two columns.")
        return(data)
    }
    if(nrow(data) < 2) {
        warning("Less than two rows.")
        return(data)
    }

    # Ensure inputs are safe for C++.
    if(is.null(subsample)) subsample <- nrow(data)
    else subsample <- as.integer(subsample[[1]])
    if(is.null(standard)) standard <- TRUE
    else standard <- as.logical(standard[[1]])
    if(is.null(message)) message <- -1.0
    else message <- as.double(message[[1]])

    # Check subsample size.
    if(!is.finite(subsample)) stop("Unusable subsample size.")
    if(subsample > nrow(data)) subsample <- nrow(data)
    if(subsample < 10) stop("Too small subsample.")

    # Check message interval.
    if(!is.finite(message)) message <- -1.0

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
    sigma <- rep(NA, ncol(data))
    if(standard) {
        for(j in numerics) {
	    x <- data[,j]
            sigma[j] <- stats::sd(x, na.rm=TRUE)
            if(!is.finite(sigma[j])) sigma[j] <- 1
            data[,j] <- x/(sigma[j])
	}
    }

    # Impute missing values.
    data[,numerics] <- .Call("nro_impute",
        as.matrix(data[,numerics]),
        as.integer(subsample),
        as.double(message),
        PACKAGE="Numero")
    if(class(data) == "character" ) stop(data)

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
