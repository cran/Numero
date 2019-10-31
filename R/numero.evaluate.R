numero.evaluate <- function(
    model,
    data,
    logarithm=NULL,
    n=1000) {

    # Continue analyses.
    output <- list(stamp=date())
    cat("\n*** numero.evaluate ***\n", output$stamp, "\n", sep="")

    # Check that resources are available.
    if(is.null(model$map)) stop("Self-organizing map not available.")
    if(is.null(model$layout)) stop("Map layout not available.")
    layout <- model$layout

    # Check if input is a vector.
    if(is.vector(data)) stop("Data must be a matrix or a data frame.")

    # Check if dataset was pruned.
    #if(!is.null(attr(data, "modules")))
    #    stop("Pruned dataset (non-identifiable variables).")

    # Check that data and layout are compatible.
    cat("\nDataset:\n")
    pos <- match(rownames(data), rownames(layout))
    rows <- which(pos > 0)
    if(length(rows) < 1) {
        warning("Incompatible data and layout.")
        return(NULL)
    }

    # Harmonize data and layout.
    nprev <- nrow(data)
    data <- data[rows,]
    layout <- layout[pos[rows],]
    cat(nrow(data), " / ", nprev, " rows included\n", sep="")
    cat(ncol(data), " columns included\n", sep="")

    # Add identities to district assignments.
    bmc <- layout[,"BMC"]
    names(bmc) <- rownames(layout)

    # Check if any logarithms needed.    
    logarithm <- intersect(logarithm, colnames(data))
    alpha <- rep(NA, length(logarithm))
    names(alpha) <- logarithm

    # Apply logarithm to selected variables.
    data.orig <- data
    for(vn in names(alpha)) {
        x <- data[,vn]
	mask <- which(x > 0)
	if(length(mask) < 1) next
	if(min(x, na.rm=TRUE) < 0) next
	alpha[vn] <- min(x[mask])
	data[,vn] <- log(x + alpha[vn])
    }

    # Check how many logarithms succeeded.
    nsuccess <- sum(is.finite(alpha))
    cat(nsuccess, " / ", length(alpha), " logarithm(s) applied\n", sep="")

    # Calculate component planes.
    comps <- nroAggregate(topology=model$map, districts=bmc, data=data)

    # Estimate statistics in chunks.
    cat("\nStatistics:\n")
    suppressWarnings(
        stats <- nroPermute(map=model$map, districts=bmc,
                            data=data, n=n, message=10))
    cat(nrow(stats), " variable(s)\n", sep="")
    cat(sum(stats$N.cycles), " permutation(s)\n", sep="")

    # Revert logarithms.
    for(vn in names(alpha))
        comps[,vn] <- (exp(comps[,vn]) - alpha[vn])

    # Determine district ranges.
    colrs <- nroColorize(comps)

    # Collect results.
    output$map <- model$map
    output$layout <- layout
    output$planes <- comps
    output$ranges <- attr(colrs, "ranges")
    output$palette <- "rhodo"
    output$statistics <- stats
    output$logarithm <- logarithm
    output$data <- data.orig
    return(output)
}
