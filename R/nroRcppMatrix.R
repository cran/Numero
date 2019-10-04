nroRcppMatrix <- function(
    data,
    trim) {

    # Check if anything to do.
    if(length(data) < 1) return(matrix(nrow=0, ncol=0))
    binary <- attr(data, "binary")

    # Convert to matrix.
    if(is.vector(data)) {
        data <- as.matrix(data)
 	colnames(data) <- "data"
    }

    # Check if already fully numeric.
    formatted <- FALSE
    if(is.matrix(data)) {
        if(is.numeric(data)) formatted <- TRUE
        if(is.integer(data)) formatted <- TRUE
        if(is.logical(data)) formatted <- TRUE
    }

    # Row and column names.
    rnames <- rownames(data)
    cnames <- colnames(data)
    if(length(rnames) != nrow(data)) {
        rnames <- (1:nrow(data))
	rownames(data) <- rnames
    }
    if(length(cnames) != ncol(data)) {
        cnames <- (1:ncol(data))
        colnames(data) <- cnames
    }

    # Convert to numeric values.
    if(!formatted) {
        try(
	suppressWarnings( # data frame to matrix
        data <- apply(data, 2, function(x) {
            x <- as.numeric(x)
            x[which(!is.finite(x))] <- NA
            return(x)
        })), silent=TRUE)
        if(!is.numeric(data)) {
            warning("Unusable input.")
	    return(matrix(nrow=0, ncol=0))
        }
	
        # Restore names.
        rownames(data) <- rnames
        colnames(data) <- cnames
    }

    # Detect binary variables.
    if(is.null(binary)) {
        for(vn in colnames(data)) {
            x <- data[,vn]
            n <- sum(is.finite(x), na.rm=TRUE)
            n0 <- sum((x == 0), na.rm=TRUE)
            n1 <- sum((x == 1), na.rm=TRUE)
            if(n != (n0 + n1)) next
            binary <- c(binary, vn)
        }
    }

    # Remove unusable data points.
    nprev <- nrow(data)*ncol(data)
    if(trim && (ncol(data) > 1)) nprev <- 0
    while(nrow(data)*ncol(data) != nprev) {
        nprev <- nrow(data)*ncol(data)

        # Remove rows with no usable values.
        rows <- which(is.finite(rowMeans(data, na.rm=TRUE)))
        if(length(rows) != nrow(data)) data <- data[rows,]

        # Remove columns with no usable values.
        cols <- which(is.finite(colMeans(data, na.rm=TRUE)))
        if(length(cols) != ncol(data)) data <- data[,cols]
    }

    # Remove unusable values from a single column.
    if(trim && (ncol(data) == 1)) {
        rows <- which(is.finite(data[,1]))
        if(length(rows) < 1) return(matrix(nrow=0, ncol=0))
        rn <- rownames(data)
        cn <- colnames(data)
        data <- as.matrix(data[rows,1])
        rownames(data) <- rn[rows]
        colnames(data) <- cn
    }

    # Return results.
    attr(data, "binary") <- intersect(binary, colnames(data))
    attr(data, "excl.rows") <- setdiff(rnames, rownames(data))
    attr(data, "excl.columns") <- setdiff(cnames, colnames(data))
    return(data)
}
