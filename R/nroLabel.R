nroLabel <- function(
    topology,
    values,
    gap=2.3) {

    # Check for binary data.
    info <- attr(values, "numero")
    if(is.null(info)) info <- list(binary=character()) 
    bincols <- match(colnames(values), info$binary)
    bincols <- as.integer(bincols > 0)

    # Determine label positions.
    res <- .Call("nro_label",
                 as.matrix(topology),
                 as.matrix(values),
		 as.integer(bincols),
                 as.numeric(gap),
                 PACKAGE = "Numero" )
    if(class(res) == "character") stop(res)

    # Convert to matrix.
    res <- data.frame(res, stringsAsFactors=FALSE)
    colnames(res) <- colnames(values)
    return(res)
}
