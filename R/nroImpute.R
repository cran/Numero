nroImpute <- function(
    data,
    subsample=500,
    standard=TRUE) {

    # Convert input to numeric matrix.
    dfbit <- is.data.frame(data)
    data <- nroRcppMatrix(data, trim=FALSE)
    binary <- attr(data, "binary")

    # Make sure parameters are safe for C++.
    subsample <- as.integer(subsample[[1]])
    standard <- as.logical(standard[[1]])
    if(!is.finite(subsample)) stop("Unusable subsample size.")

    # Detect numeric variables.
    numerics <- c()
    for(vn in colnames(data)) {
	flags <- is.finite(data[,vn])
        if(sum(flags) > 0) numerics <- c(numerics, vn)
    }
    if(length(numerics) < 2) {
        warning("Less than two numeric columns.")
        return(data)
    }

    # Select variables.
    if(length(numerics) < ncol(data)) {
        data <- data[,numerics]
        warning("Non-numeric column(s) excluded.")
    }

    # Standardize data.
    sigma <- rep(1, ncol(data))
    if(standard) {
        for(j in 1:ncol(data)) {
	    x <- data[,j]
            sigma[j] <- stats::sd(x, na.rm=TRUE)
            if(!is.finite(sigma[j])) sigma[j] <- 1
            data[,j] <- x/(sigma[j])
	}
    }

    # Copy names.
    rnames <- rownames(data)
    cnames <- colnames(data)

    # Impute missing values.
    data <- .Call("nro_impute",
                 as.matrix(data),
		 as.integer(subsample),
                 PACKAGE="Numero")
    if(class(data) == "character" ) stop(data)

    # Restore original scale.
    for(j in 1:ncol(data))
       data[,j] <- (sigma[j])*(data[,j])

    # Convert to data frame.
    if(dfbit) data <- data.frame(data, stringsAsFactors=FALSE)

    # Restore names and attributes.
    rownames(data) <- rnames
    colnames(data) <- cnames
    attr(data, "binary") <- intersect(binary, cnames)
    return(data)
}
