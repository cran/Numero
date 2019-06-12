nroPair <- function(
    data.x,
    data.y) {

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
    data.x <- nroRcppMatrix(data.x, trim=FALSE)
    data.y <- nroRcppMatrix(data.y, trim=FALSE)

    # Find best-matching units.
    res <- .Call("nro_pair",
                 as.matrix(data.x),
                 as.matrix(data.y),
                 PACKAGE="Numero")
    if(class(res) == "character" ) stop(res)

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
