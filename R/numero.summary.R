numero.summary <- function(
    results,
    elements,
    data=NULL,
    capacity=10) {

    # Start processing.
    stamp <- date()
    cat("\n*** numero.summary ***\n", stamp, "\n", sep="")
    
    # Default dataset.
    if(is.null(data)) data <- results$data

    # Match results with original data.
    cat("\nResources:\n")
    layout <- results$layout
    pos <- match(rownames(layout), rownames(data))
    rows <- which(pos > 0)
    data <- data[pos[rows],]
    layout <- layout[rows,]
    cat(length(rows), " data point(s) matched with layout\n", sep="")
    if(nrow(data) < 10) {
        cat("less than ten usable data points\n")
        return(NULL)
    }
    cat(ncol(data), " data column(s)\n", sep="")

    # Check subgroup capacity.
    nsubs <- length(table(elements$REGION))
    if(nsubs < 2) {
        cat("less than two subgroups\n")
        return(NULL)
    }
    if(length(t) > capacity) {
        cat("subgroup capacity exceeded\n")
        return(NULL)
    }

    # Estimate subgroup statistics.
    cat("\nComparisons:\n")
    suppressWarnings(
        output <- nroSummary(data=data, districts=layout$BMC,
                             regions=elements$REGION, capacity=capacity))
    if(length(output) < 1) {
        cat("no usable columns\n")
        return(output)
    }

    # Find variables that had usable data.
    pvals <- output[, c("P.chisq", "P.t", "P.anova")]
    success <- which(rowMeans(pvals, na.rm=TRUE) >= 0)
    binary <- which(output$TYPE == "binary")
    categ <- which(output$TYPE == "categ")
    real <- which(output$TYPE == "real")
    binary <- unique(output$VARIABLE[intersect(success, binary)])
    categ <- unique(output$VARIABLE[intersect(success, categ)])
    real <- unique(output$VARIABLE[intersect(success, real)])
    cat(length(binary), " binary column(s)\n", sep="")
    cat(length(categ), " categorical column(s)\n", sep="")
    cat(length(real), " continuous column(s)\n", sep="")

    # Unusable variables.
    nskip <- (ncol(data) - length(binary) - length(categ) - length(real))
    cat(nskip, " unusable column(s)\n", sep="")

    # Clear P-values for training set.
    pos <- match(output$VARIABLE, colnames(results$som$centroids))
    output[which(pos > 0), c("P.chisq", "P.t", "P.anova")] <- NA
    output$TRAINING <- is.finite(pos)
    return(output)
}
