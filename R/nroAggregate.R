nroAggregate <- function(
    topology,
    districts,
    data=NULL) {

    # Check if input is a list.
    if(!is.data.frame(topology) && is.list(topology))
        topology <- topology$topology
    smoothness <- attr(topology, "smoothness")

    # Ensure topology is a numeric matrix.
    topology <- nroRcppMatrix(topology, trim=FALSE)

    # Make sure map info is available.
    if((length(districts) < 1) || (ncol(topology) < 1)) {
        warning("Empty inputs.")
	return(NULL)
    }

    # Check smoothness.
    if(is.null(smoothness)) stop("Map smoothness undefined.")
    smoothness <- as.double(smoothness[[1]])
    if(!is.finite(smoothness)) stop("Unusable map smoothness.")
    if(smoothness < 1) stop("Map mmoothness less than one.")

    # Estimate sample histogram.
    if(is.null(data)) {
        res <- .Call("nro_diffuse",
                     as.matrix(topology),
                     as.double(smoothness),
	             as.integer(districts),
		     matrix(nrow=0, ncol=0),
                     PACKAGE="Numero")
        if(class(res) == "character") stop(res)
	return(as.numeric(res$histograms))
    }

    # Convert data to numeric matrix.
    data <- nroRcppMatrix(data, trim=FALSE)
    binary <- attr(data, "binary")

    # Flag non-empty columns.
    mu <- colMeans(data, na.rm=TRUE)
    empty <- which(!is.finite(mu))
    if(length(empty) == ncol(data)) {
       warning("No usable data.")
       return(NULL)
    }

    # Replace empty columns with zeros.
    data[,empty] <- 0

    # Check compatibility.
    if(nrow(data) != length(districts))
        stop("Incompatible inputs.")

    # Estimate component planes.
    res <- .Call("nro_diffuse",
                 as.matrix(topology),  
                 as.double(smoothness),
                 as.integer(districts),
                 as.matrix(data),
                 PACKAGE="Numero");
    if(class(res) == "character") stop(res)

    # Transpose to column-major format.
    hgrams <- t(res$histograms)
    planes <- t(res$planes)

    # Convert to a data frame or a vector.
    if(ncol(planes) < 2) {
        hgrams <- data.frame(X=as.vector(hgrams),
	    stringsAsFactors=FALSE)
        planes <- data.frame(X=as.vector(planes),
	    stringsAsFactors=FALSE)
    }
    else {
        hgrams <- data.frame(hgrams, stringsAsFactors=FALSE)
        planes <- data.frame(planes, stringsAsFactors=FALSE)
    }

    # Set row and column names.
    colnames(planes) <- colnames(data)
    colnames(hgrams) <- colnames(data)
    rownames(planes) <- 1:nrow(planes)
    rownames(hgrams) <- 1:nrow(hgrams)

    # Clear empty variables.
    planes[,empty] <- NA
    hgrams[,empty] <- 0

    # Finish results.
    attr(planes, "histogram") <- hgrams
    attr(planes, "binary") <- intersect(binary, colnames(data))
    return(planes)
}
