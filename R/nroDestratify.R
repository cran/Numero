nroDestratify <- function(
    data,
    labels) {

    # Convert input to numeric matrix.
    dfbit <- is.data.frame(data)
    data <- nroRcppMatrix(data, trim=FALSE)

    # Detect numeric non-binary variables.
    numerics <- c()
    for(vn in colnames(data)) {
        x <- data[,vn]
	n <- sum(is.finite(x), na.rm=TRUE)
        n0 <- sum((x == 0), na.rm=TRUE)
        n1 <- sum((x == 1), na.rm=TRUE)
	if(n == (n0 + n1)) next
	numerics <- c(numerics, vn)
    }

    # Check if anything to do.
    if(length(numerics) < 1) {
        warning("No usable columns.")
        return(NULL)
    }

    # Remove unusable columns.
    if(length(numerics) < ncol(data))
        warning("Binary or non-numeric columns excluded.")
    data <- as.matrix(data[,numerics])

    # Check that inputs are compatible.
    grp <- as.factor(as.vector(labels))  
    if(nrow(data) != length(grp)) stop("Incompatible inputs.")

    # Check batch size.
    if(length(levels(grp)) > 0.2*length(grp))
        stop("Average batch size is less than five.")

    # Remove differences in batch-specific distributions.
    res <- .Call("nro_destratify",
                 as.matrix(data),
                 as.integer(grp),
                 PACKAGE="Numero")  
    if(class(res) == "character") stop(res)

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
