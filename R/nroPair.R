nroPair <- function(
    data.x,
    data.y,
    subsample=500,
    standard=TRUE) {

    # Check variable names.
    vars <- colnames(data.x)
    if(length(vars) < 1) stop("No column names.")
    vars <- intersect(vars, colnames(data.y))
    if(length(vars) < 1) stop("Incompatible data")
    if(length(vars) < ncol(data.x))
        warning("Incomplete coverage of variables.")  
    if(length(vars) < ncol(data.y))
        warning("Incomplete coverage of variables.")  

    # Make sure inputs are numeric.
    data.x <- nroRcppMatrix(data.x[,vars], trim=FALSE)
    data.y <- nroRcppMatrix(data.y[,vars], trim=FALSE)
    subsample <- as.integer(subsample[[1]])
    standard <- as.logical(standard[[1]])

    # Standardize data.
    if(standard) {
        for(vn in vars) {
	    vals <- c(data.x[,vn], data.y[,vn])
            sigma <- stats::sd(vals, na.rm=TRUE)
            if(!is.finite(sigma)) sigma <- 1
            data.x[,vn] <- data.x[,vn]/(sigma + 1e-20)
            data.y[,vn] <- data.y[,vn]/(sigma + 1e-20)
	}
    }

    # Find best-matching pairs.
    res <- .Call("nro_pair",
                 as.matrix(data.x),
                 as.matrix(data.y),
		 as.integer(subsample),
                 PACKAGE="Numero")
    if(is.character(res)) stop(res)

    # Convert to data frame.
    res <- data.frame(res, stringsAsFactors=FALSE)
    
    # Sort by distance.
    sorted <- order(res$DISTANCE)
    res <- res[sorted,]

    # Convert row indices to row names.
    rnames.x <- rownames(data.x)
    rnames.y <- rownames(data.y)
    if(length(rnames.x) > 0) res$ROWNAME.x <- rnames.x[res$ROW.x]
    if(length(rnames.y) > 0) res$ROWNAME.y <- rnames.y[res$ROW.y]

    # Move distance info to the last column.
    d <- res$DISTANCE
    res$DISTANCE <- NULL
    res$DISTANCE <- d
    return(res)
}
