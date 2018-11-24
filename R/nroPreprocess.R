nroPreprocess <- function(
    data,
    training=NULL,
    strata=NULL,
    key=NULL) {

    # Set row names.
    orig <- data
    if(is.null(key) == FALSE) {
        rownames(data) <- data[,key]
        data[,key] <- NA
    }
    
    # Collect usable columns and detect binary columns.
    varinfo <- nroPreprocess.variables(data)
    data <- data[,varinfo$numerics]
    for(vn in varinfo$logicals)
        data[,vn] <- (data[,vn] == 1)

    # Make sure all unusable values are set to NA.
    data <- apply(data, 2, function(x) {
        x[which(is.na(0*x == 0))] <- NA
    	return(x)
    })

    # Check training variables.
    if(is.null(training)) training <- colnames(data)
    training <- intersect(training, colnames(data))
    if(length(training) < 1) {
        warning("No usable feature variables.")
        return(list())
    }

    # Set standardization subsets.
    subsets <- list()
    subsets[[1]] <- (1:nrow(data))
    if(is.null(strata) == FALSE)
        subsets <- split((1:nrow(data)), data[,strata])

    # Standardize subgroups.
    trdata <- data[,training]
    for(k in 1:length(subsets)) {
        mask <- subsets[[k]]
        trdata[mask,] <- scale.default(trdata[mask,])
    }

    # Collect usable rows.   
    rowmeans <- apply(trdata, 1, mean, na.rm=TRUE)
    rows <- which(0*rowmeans == 0)
    nskip <- (nrow(trdata) - length(rows))
    if(nskip > 0) warning(sprintf("%d row(s) excluded.", nskip))

    # Binary variables.
    orig.logicals <- intersect(varinfo$logicals, colnames(orig))
    data.logicals <- intersect(varinfo$logicals, colnames(data))
    trdata.logicals <- intersect(varinfo$logicals, colnames(trdata))

    # Return results.
    res <- list()
    res$original <- orig[rows,]
    res$values <- data[rows,]
    res$features <- trdata[rows,]
    attr(res$original, "numero") <- list(binary=orig.logicals)
    attr(res$values, "numero") <- list(binary=data.logicals)
    attr(res$features, "numero") <- list(binary=trdata.logicals)
    return(res)
}

#---------------------------------------------------------------------------

nroPreprocess.variables <- function(x) {
    numerics <- character()
    logicals <- character()
    for(vn in colnames(x)) {
	vals <- as.numeric(x[,vn])
	sigma <- stats::sd(vals, na.rm=TRUE)
	if(is.na(sigma)) next
	if(sigma <= .Machine$double.eps) next
	n <- sum((0*vals == 0), na.rm=TRUE)
        n0 <- sum((vals == 0), na.rm=TRUE)
        n1 <- sum((vals == 1), na.rm=TRUE)
        if((n0 + n1) >= n) logicals <- c(logicals, vn)
	numerics <- c(numerics, vn)
    }
    output <- list()
    output$numerics <- numerics
    output$logicals <- logicals
    return(output)
}