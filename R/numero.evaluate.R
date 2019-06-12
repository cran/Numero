numero.evaluate <- function(
    model,
    data) {

    # Continue analyses.
    output <- list(stamp=date())
    cat("\n*** numero.evaluate ***\n", output$stamp, "\n", sep="")

    # Check that resources are available.
    if(is.null(model$som)) stop("Self-organizing map not available.")
    if(is.null(model$layout)) stop("Map layout not available.")
    layout <- model$layout

    # Check that data and layout are compatible.
    cat("\nDataset:\n")
    pos <- match(rownames(data), rownames(layout))
    rows <- which(pos > 0)
    cat(length(rows), " / ", nrow(data), " rows included\n", sep="")
    if(length(rows) < 1) {
        warning("Incompatible data and layout.")
        return(NULL)
    }
    if(length(rows) != nrow(data)) data <- data[rows,]
    if(length(rows) != nrow(layout)) layout <- layout[pos[rows],]
    cat(ncol(data), " columns included\n", sep="")

    # Add identities to district assignments.
    bmc <- layout[,"BMC"]
    names(bmc) <- rownames(layout)

    # Calculate component planes.
    comps <- nroAggregate(topology=model$som$topology,
                          districts=bmc, data=data)

    # Estimate statistics in chunks.
    cat("\nStatistics:\n")
    suppressWarnings(
        stats <- nroPermute(som=model$som, districts=bmc,
                            data=data, message=20.0))
    cat(nrow(stats), " variable(s)\n", sep="")
    cat(sum(stats$N.cycles), " permutation(s)\n", sep="")

    # Determine district ranges.
    ranges <- nroColorize(comps, amplitudes=NULL)

    # Collect results.
    output$som <- model$som
    output$layout <- layout
    output$planes <- comps
    output$ranges <- ranges
    output$palette <- "rhodo"
    output$statistics <- stats
    output$data <- data
    return(output)
}