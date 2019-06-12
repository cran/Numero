numero.plot <- function( 
    results,
    variables=NULL,
    elements=NULL,
    folder=NULL,
    reference=NULL,
    subplot=c(3,3),
    gain=1.0,
    detach=FALSE,
    capacity=90) {

    # Start processing.
    cat("\n*** numero.plot ***\n", date(), "\n", sep="")

    # Default inputs.
    if(is.null(variables)) variables <- colnames(results$planes)
    if(is.null(elements)) elements <- results$som$topology
    if(is.null(reference)) reference <- results
    detach <- as.character(detach[[1]])

    # Check capacity.
    cat("\nResources:\n")
    if(capacity < 2) {
        cat("capacity less than two\n")
        return(0)
    }

    # Select variables.
    comps <- results$planes
    stats <- results$statistics
    variables <- intersect(variables, colnames(comps))
    cat(length(variables), " column(s) included\n", sep="")
    if(length(variables) < 2) {
        cat("less than two usable variables\n")
        return(0)
    }

    # Check if too many variables.
    comps <- comps[,variables]
    stats <- stats[variables,]
    if(nrow(stats) > capacity) {
        cat("capacity exceeded, showing", capacity, "plots.\n")
        comps <- comps[,1:capacity]
	stats <- stats[1:capacity,]
    }

    # Check if folder is accessible.
    if(length(folder) > 0) {
        if(!dir.exists(folder)) dir.create(folder)
	if(!dir.exists(folder)) {
	    cat("destination '", folder, "' not available\n")
	    folder <- NULL
	}
	if(!is.null(folder)) cat("destination folder '", folder, "'\n")
    }
    else {
        cat("destination folder not defined\n")
    }

    # Check if reference is usable.
    rvars <- rownames(reference$statistics)
    if(sum(is.na(match(variables, rvars))) > 0) {
	cat("incompatible reference\n")
        return(0)
    }

    # Check if gain is usable.
    gain <- as.double(gain[[1]])
    if(!is.finite(gain)) {
        gain <- 1
        cat("unusable gain, reverted to 1\n")
    }
    if(gain <= 0.0) {
        gain <- 1
        cat("non-positive gain, reverted to 1\n")
    }

    # Check if subplot is usable.
    if(length(subplot) < 2) {
        cat("unusable subplot, reverted to c(3,3)\n")
        subplot <- c(3,3)
    }
    subplot <- as.integer(subplot[c(1,2)])
    if((subplot[1] < 1) || (subplot[2] < 1)) {
        cat("unusable subplot, reverted to c(3,3)\n")
        subplot <- c(3,3)
    }

    # Get coloring parameters.
    amplitudes <- reference$statistics[variables,"AMPLITUDE"]
    amplitudes <- gain*amplitudes
    ranges <- reference$ranges[variables,]
    palette <- reference$palette

    # Restore attribute for binary variables.
    binary <- attr(results$planes, "binary")
    attr(comps, "binary") <- intersect(binary, variables)
 
    # Set colors and labels.
    colrs <- nroColorize(values=comps, amplitudes=amplitudes,
                         ranges=ranges, palette=palette)
    labls <- nroLabel(topology=elements, values=comps)

    # Split into several figures.
    nfigs <- 0
    nsubs <- (subplot[1])*(subplot[2])
    nstats <- nrow(stats)
    while(TRUE) {

        # Select colorings.
        mask <- (nfigs*nsubs + 1:nsubs)
        mask <- mask[which(mask <= nstats)]
        nfigs <- (nfigs + 1)
        cat("\nFigure ", nfigs, "\n", sep="")
        cat(length(mask), " subplot(s)\n", sep="")

        # Set file name.
        fname <- NULL
        if(length(folder) > 0) {
	    fname <- sprintf("figure%02d.svg", nfigs)
	    fname <- file.path(folder, fname)
	    cat("file name '", fname, "'\n", sep="")
        }

        # Make sure column names are preserved.
        colrs.masked <- as.matrix(colrs[,mask])
        labls.masked <- as.matrix(labls[,mask])
        comps.masked <- as.matrix(comps[,mask])
        if(length(mask) == 1) {
            cname <- colnames(colrs)
	    if(length(cname) < 1) cname <- mask
	    else cname <- cname[mask]
	    colnames(colrs.masked) <- cname
	    colnames(labls.masked) <- cname
	    colnames(comps.masked) <- cname
        }

        # Create figure.
	if(length(mask) > 0) {

           # Launch a detached window.
           if((length(fname) < 1) && (detach != "FALSE")) {
               if(detach == "TRUE") grDevices::dev.new()
               if(detach == "aqua") {
	           if(capabilities("aqua")) grDevices::quartz()
	           else warning("Quartz display server not available.")
	       }
               if(detach == "X11") {
	           if(capabilities("X11")) grDevices::x11()
	           else warning("X11 display server not available.")
	       }
           }

           # Plot colorings.
           nroPlot(elements=elements,
                    colors=colrs.masked,
	            labels=labls.masked,
                    values=comps.masked,
		    subplot=subplot, file=fname)
        }
	if(max(mask) >= nstats) break
    }

    # Final report.
    cat("\nSummary:\n")
    if(length(folder) < 1) cat(nfigs, " figure(s)\n", sep="")
    else cat(nfigs, " figure(s) -> '", folder, "'\n", sep="")
    return(nfigs)
}
