nroStatistic <- function(
    data,
    weights=NULL,
    method="center") {

    # Convert inputs to numeric matrices.
    data <- nroRcppMatrix(data, trim=FALSE)
    weights <- nroRcppMatrix(weights, trim=FALSE)    
    if(nrow(weights) < 1) weights <- (0*data + 1)

    # Check input sizes.
    if(nrow(data)*ncol(data) < 1) {
        warning("No data.")
        return(NULL)
    }
    if(nrow(data) != nrow(weights))
        stop("Incompatible inputs.")
    if(ncol(data) != ncol(weights))
        stop("Incompatible inputs.")

    # Match row and column names.
    if(nrow(data) > 1) weights <- weights[rownames(data),]
    if(ncol(data) > 1) weights <- weights[,colnames(data)]

    # Check method.
    method <- nroRcppVector(method[[1]], default="", numeric=FALSE)
    if(anyDuplicated(c(method, "center", "mean", "median", "mode", "min",
        "max", "range", "sd","iqr", "var", "number")) < 1)
        stop("Unknown method.")

    # Calculate averages.
    res <- .Call("nro_statistic",
        as.matrix(data),
	as.matrix(weights),
	as.character(method),
        PACKAGE="Numero")
    if(is.character(res)) stop(res)

    # Set output names.
    names(res) <- colnames(data)
    return(res)
}
