nroMatch <- function(
    som,
    data) {

    # Check if input is a list.
    if((is.list(som) == FALSE) || is.data.frame(som))
        som <- list(centroids=som)
    
    # Check variable names.
    vars <- colnames(som$centroids)
    pos <- match(vars, colnames(data))
    mask <- which(pos > 0)
    if(length(mask) < 1) stop("Incompatible data")
    cols <- pos[mask]
    if(length(cols) < length(vars))
        warning("Incomplete coverage of variables.")  

    # Find best-matching units.
    res <- .Call("nro_match",
                 as.matrix(som$centroids),
                 as.matrix(data[,cols]),
                 PACKAGE="Numero")
    if(class(res) == "character" ) stop(res)
    
    # Convert to data frame.
    res <- data.frame(res, stringsAsFactors=FALSE)

    # Check if training history is available.
    delta <- NA
    if(is.null(som$history) == FALSE)
        delta <- som$history[length(som$history)]

    # Set mismatched labels to NA.
    bmus <- as.integer(res$DISTRICT)
    bmus[which(bmus == 0)] <- NA
    res$DISTRICT <- NULL

    # Quality compared with the average matching error after training.
    res$QUALITY <- 2*(delta + 1e-9)/(res$RESIDUAL + delta + 1e-9)

    # Separate primary output from other information.
    names(bmus) <- rownames(data)
    attr(bmus, "quality") <- res
    attr(bmus, "features") <- vars
    return(bmus)
}
