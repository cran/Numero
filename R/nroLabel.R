nroLabel <- function(
    topology,
    values,
    gap=2.3) {

    # Check if input is a list.
    if(!is.data.frame(topology) && is.list(topology))
        topology <- topology$topology

    # Convert inputs to numeric matrices.
    topology <- nroRcppMatrix(topology, trim=FALSE)
    values <- nroRcppMatrix(values, trim=FALSE)

    # Check topology and values.
    if(is.vector(topology)) stop("Unusable topology.")
    if(nrow(topology) < 2) stop("Unusable topology.")
    if(nrow(topology) != nrow(values)) stop("Incompatible inputs.")
	
    # Check gap.
    gap <- as.double(gap[[1]])
    if(!is.finite(gap)) stop("Unusable gap.")
    if(gap < 1.0) stop("Gap is less than one.")

    # Set flags for binary data.
    binflags <- match(colnames(values), attr(values, "binary"))
    binflags <- is.finite(binflags)

    # Determine label positions.
    res <- .Call("nro_label",
                 as.matrix(topology),
                 as.matrix(values),
		 as.integer(binflags),
                 as.numeric(gap),
                 PACKAGE = "Numero" )
    if(is.character(res)) stop(res)

    # Convert to data frame.
    if(ncol(values) < 2) {
        res$labels <- data.frame(as.vector(res$labels[[1]]),
	    stringsAsFactors=FALSE)
        res$visible <- data.frame(as.vector(res$visible[[1]]),
	    stringsAsFactors=FALSE)
    }
    else {
        res$labels <- data.frame(res$labels, stringsAsFactors=FALSE)
        res$visible <- data.frame(res$visible, stringsAsFactors=FALSE)
    }

    # Set row and columns names.
    rownames(res$labels) <- rownames(values)
    rownames(res$visible) <- rownames(values)
    colnames(res$labels) <- colnames(values)
    colnames(res$visible) <- colnames(values)

    # Return results.
    output <- res$labels
    attr(output, "visible") <- res$visible
    return(output)
}
