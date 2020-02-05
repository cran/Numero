nroDestratify <- function(
    data,
    labels) {

    # Convert input to numeric matrix.
    dfbit <- is.data.frame(data)
    data <- nroRcppMatrix(data, trim=FALSE)

    # Check if anything to do.
    binary <- attr(data, "binary")
    numerics <- setdiff(colnames(data), binary)
    if(length(numerics) < 1) {
        warning("No usable columns.")
        return(NULL)
    }

    # Remove unusable columns.
    if(length(numerics) < ncol(data))
        warning("Binary or non-numeric columns excluded.")
    data <- as.matrix(data[,numerics])

    # Check that inputs are compatible.
    labels <- nroRcppVector(labels, default=NULL, numeric=FALSE)
    grp <- as.integer(as.factor(labels))  
    if(nrow(data) != length(grp)) stop("Incompatible inputs.")

    # Check batch size.
    if(length(levels(grp)) > 0.2*length(grp))
        stop("Average batch size is less than five.")

    # Remove differences in batch-specific distributions.
    res <- .Call("nro_destratify",
                 as.matrix(data),
                 as.integer(grp),
                 PACKAGE="Numero")  
    if(is.character(res)) stop(res)

    # Convert from list to data frame or matrix.
    res <- data.frame(res, stringsAsFactors=FALSE)
    if(!dfbit) res <- as.matrix(res)

    # Set row and column names.
    rownames(res) <- rownames(data) 
    colnames(res) <- colnames(data) 

    # Determine columns that were not fully successful. 
    incomplete <- character()
    for(vn in colnames(res)) {
       xbits <- is.finite(data[,vn])
       ybits <- is.finite(res[,vn])
       if(sum(xbits) == sum(ybits)) next
       incomplete <- c(incomplete, vn)
    }
    if(length(incomplete) > 0)
        warning("Some values could not be processed.")
   
    # Update dataset.
    attr(res, "incomplete") <- incomplete
    return(res)
}
