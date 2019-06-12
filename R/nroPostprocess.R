nroPostprocess <- function(data, mapping) {

    # Nothing to do.
    if(length(mapping) < 1) return(data)

    # Check input.
    if(is.data.frame(mapping)) mapping <- attr(mapping, "mapping")
    if(is.matrix(mapping)) mapping <- attr(mapping, "mapping")
    model.in <- mapping$input
    model.out <- mapping$output

    # Check model data.
    if(nrow(model.in) != nrow(model.out))
        stop("Incompatible model, size mismatch.")
    if(ncol(model.in) != ncol(model.out))
        stop("Incompatible model, size mismatch.")
    if(sum(rownames(model.in) != rownames(model.out)) > 0)
        stop("Incompatible model, row name conflict.")
    if(sum(colnames(model.in) != colnames(model.out)) > 0)
        stop("Incompatible model, column name conflict.")

    # Find variables.
    vars <- intersect(colnames(model.in), colnames(data))
    if(length(vars) < 1) {
        warning("No matching column names.")
        return(NULL)
    }

    # Preprocess new data.
    output <- data[,vars]
    for(vn in colnames(output)) {
      output[,vn] <- NA
      x <- model.in[,vn]
      y <- model.out[,vn]
      xout <- as.double(data[,vn])
      mask <- which(is.finite(x*y))
      if(length(mask) < 3) next
      output[,vn] <- stats::approx(x=x[mask], y=y[mask], rule=2, xout=xout)$y
    }

    # Remove empty rows.
    mu <- rowMeans(output, na.rm=TRUE)
    output <- output[which(is.finite(mu)),]
    if(nrow(output) < 1) {
        warning("No usable rows.")
        return(NULL)
    }
    if(nrow(output) < nrow(data))
        warning("Unusable row(s) excluded.")

    # Remove empty columns.
    mu <- colMeans(output, na.rm=TRUE)
    output <- output[,which(is.finite(mu))]
    if(ncol(output) < 1) {
        warning("No usable columns.")
        return(NULL)
    }
    if(ncol(output) < ncol(data))
        warning("Unusable column(s) excluded.")

    # Return results.
    return(output)
}
