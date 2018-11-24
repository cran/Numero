nroAggregate <- function(
    topology,
    districts,
    data=NULL) {
    info <- attr(data, "numero")

    # Convert vector input to matrix.
    if(is.factor(data)) data <- as.numeric(data)
    if(is.vector(data)) data <- as.matrix(data)

    # Estimate sample histogram.
    if(is.null(data)) {
        res <- .Call("nro_diffuse",
                     as.matrix(topology),
	             as.integer(districts),
		     matrix(nrow=0, ncol=0),
                     PACKAGE="Numero")
        if(class(res) == "character") stop(res)
	res$planes <- data.frame(res$planes, stringsAsFactors=FALSE)
        return(res)
    }

    # Check input sizes.
    if(nrow(data) != length(districts))
        stop("Incompatible inputs.")

    # Estimate component planes.
    res <- .Call("nro_diffuse",
                 as.matrix(topology),  
                 as.integer(districts),
                 as.matrix(data),
                 PACKAGE="Numero");
    if(class(res) == "character") stop(res)

    # Transpose to column-major format.
    planes <- t(res$planes)
    
    # Set column names.
    if(length(colnames(data)) > 0)
        colnames(planes) <- colnames(data)
    else
        colnames(planes) <- (1:ncol(data))

    # Convert to data frame.
    planes <- data.frame(planes, stringsAsFactors=FALSE)

    # Finish results.
    attr(planes, "histogram") <- as.numeric(res$histogram)
    attr(planes, "numero") <- info
    return(planes)
}
