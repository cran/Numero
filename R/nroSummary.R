nroSummary <- function(
    data,
    districts,
    regions=NULL,
    categlim=8,
    capacity=10) {

    # Check inputs.
    if(is.vector(data))
        data <- data.frame(X=data, stringsAsFactors=FALSE)
    if(is.matrix(regions) || is.data.frame(regions)) {
        regions <- regions[,"REGION"]
	names(regions) <- regions[,"REGION.label"]
    }
    rlabels <- names(regions)
    categlim <- as.integer(categlim[[1]])
    capacity <- as.integer(capacity[[1]])

    # Districts define subgroups directly.
    g <- districts

    # If available, merge districts into regions.
    if(length(regions) > 0) {
        districts <- as.integer(districts)

        # Remove unmatched data points.
        mask <- which((districts > 0) & (districts <= length(regions)))
        districts <- districts[mask]

        # Assign data points to regional subgroups.
	g <- rep(NA, nrow(data))
        g[mask] <- regions[districts]
    }

    # Set identities.
    names(g) <- rownames(data)

    # Check subgroups.
    t <- table(g)
    if(sum(is.na(g)) > 0)
        warning("Unusable district(s) or region(s) detected.")
    if(length(t) < 2) {
        warning("Less than two subgroups.")
        return(NULL)
    }
    if(length(t) > capacity) {
        warning("Subgroup capacity exceeded.")
        return(NULL)
    }

    # Process data columns.
    output <- data.frame()
    for(vn in colnames(data)) {
        x <- data[,vn]
        nlev <- nlevels(as.factor(x))

        # Unusable values.
        if(nlev < 2) {
	    warning(paste(vn, ": Unusable values.", sep=""))
            next
        }

        # Categorical data.
        if((nlev <= categlim) || (is.numeric(x) == FALSE)) {
	    stats <- nroSummary.categ(x, g)
	    if(is.character(stats)) {
	        warning(paste(vn, ": ", stats, sep=""))
            }
	    else {
	        stats <- data.frame(VARIABLE=vn, stats, stringsAsFactors=F)
	        output <- rbind(output, stats)
	    }
	    next
        }

        # Numerical data.
	stats <- nroSummary.real(x, g)
	if(is.character(stats)) {
	    warning(paste(vn, ": ", stats, sep=""))
        }
	else {
	    stats <- data.frame(VARIABLE=vn, stats, stringsAsFactors=F)
	    output <- rbind(output, stats)
	}
    }

    # Add region labels.
    if(length(rlabels) > 0) {
        pos <- match(output$SUBGROUP, regions)
        rows <- which(pos > 0)
        output$LABEL[rows] <- rlabels[pos[rows]]
    }

    # Finish results.
    attr(output, "labels") <- g
    attr(output, "subgroups") <- split(1:nrow(data), g)
    rownames(output) <- NULL
    return(output)
}

#---------------------------------------------------------------------------

nroSummary.categ <- function(x, g) {

    # Check if enough data.
    mask <- which((is.na(x) == FALSE) & (is.na(g) == FALSE))
    if(length(mask) < 10) return("Too few usable data.")
    x <- x[mask]
    g <- g[mask]

    # Split into subgroups.
    xsets <- split(x, g)
    if(length(xsets) < 2) return("Only one subgroup.")

    # Recode if binary.
    xfact <- as.factor(x)
    xlevs <- levels(xfact)
    nlevs <- length(xlevs)
    if(nlevs == 2) {
        xsets <- lapply(xsets, function(v, l) {
	    v <- (v == l)
        }, xlevs[2])
	xlevs <- c(TRUE, FALSE)
	xfact <- (as.integer(xfact) - 1)
    }

    # Subgroup sizes.
    stats <- list()
    stats$N <- lapply(xsets, function(v) {
        return(sum(is.na(v) == FALSE))
    })

    # Add extra class labels to every subgroup to ensure tests
    # work (this will slightly dilute the results).
    for(j in 1:length(xsets))
        xsets[[j]] <- c(xsets[[j]], xlevs)

    # Estimate basic subgroup stats.
    stats$MEAN <- rep(NA, length(xsets))
    stats$MEDIAN <- rep(NA, length(xsets))
    stats$SD <- rep(NA, length(xsets))
    if(nlevs == 2) stats$MEAN <- lapply(xsets, mean, na.rm=TRUE)

    # Convert to data frame.
    stats <- lapply(stats, as.double)
    stats <- as.data.frame(stats)
    stats <- data.frame(SUBGROUP=names(xsets), LABEL=NA, stats)

    # Add P-value columns.
    stats$TYPE <- "categ"
    if(nlevs == 2) stats$TYPE <- "binary"
    stats$P.chisq <- NA
    stats$P.t <- NA
    stats$P.anova <- NA

    # Find the biggest subgroup.
    ind <- which.max(stats$N)
    if(length(ind) < 1) return("Unusable subgroups.")

    # Chi-squared test by subgroup.
    suppressWarnings({
    stats$P.chisq <- lapply(xsets, function(v, v0) {
        bits <- c(rep(0, length(v)), rep(1, length(v0)))
        st <- stats::chisq.test(c(v, v0), bits)
	return(st$p.value)
    }, xsets[[ind]])})
    stats$P.chisq <- as.double(stats$P.chisq)

    # Chi-squared test for whole data.
    suppressWarnings(st <- stats::chisq.test(x, g))

    # Add extra row for full test.
    stats <- rbind(stats[1,], stats)
    stats[1,c("SUBGROUP", "MEAN","MEDIAN","SD","P.t","P.anova")] <- NA
    if(nlevs == 2) stats$MEAN[1] <- mean(xfact, na.rm=TRUE)
    stats$N[1] <- length(x)
    stats$P.chisq[1] <- st$p.value

    # Return results.
    return(stats)
}

#---------------------------------------------------------------------------

nroSummary.real <- function(x, g) {
    suppressWarnings(x <- as.numeric(x))
  
    # Check if enough data.
    mask <- which(0*x == 0)
    if(length(mask) < 10) return("Too few data.")
    sigma <- stats::sd(x[mask], na.rm=TRUE)
    if(!is.finite(sigma)) return("Unusable data.")
    if(sigma < 1e-9) return("Too low variance.")
    x <- x[mask]
    g <- g[mask]

    # Convert to tapered ranks.
    z <- (rank(x, na.last="keep") - 1)
    z <- (2*z/max(z, na.rm=TRUE) - 1)
    z <- 0.5*(z + z^3)

    # Split into subgroups.
    xsets <- split(x, g)
    zsets <- split(z, g)
    if(length(xsets) < 2) return("Only one subgroup.")

    # Subgroup sizes.
    stats <- list()
    stats$N <- lapply(xsets, function(v) {
        return(sum(0*v == 0))
    })

    # Estimate basic subgroup stats.
    stats$MEAN <- lapply(xsets, mean, na.rm=TRUE)
    stats$MEDIAN <- lapply(xsets, stats::median, na.rm=TRUE)
    stats$SD <- lapply(xsets, stats::sd, na.rm=TRUE)

    # Convert to data frame.
    stats <- lapply(stats, as.double)
    stats <- as.data.frame(stats, stringsAsFactors=FALSE)
    stats <- data.frame(SUBGROUP=names(xsets), LABEL=NA, stats)

    # Add P-value columns.
    stats$TYPE <- "real"
    stats$P.chisq <- NA
    stats$P.t <- NA
    stats$P.anova <- NA

    # Find the "most average" subgroup.
    mu <- mean(x, na.rm=TRUE)
    ind <- which.min(abs(stats$MEAN - mu))
    if(length(ind) < 1) return("Unusable subgroups.")

    # Rank-regulated T-tests against the reference group.
    stats$P.t <- lapply(zsets, function(v, v0) {
        xsigma <- stats::sd(v, na.rm=TRUE)
	ysigma <- stats::sd(v0, na.rm=TRUE)
        if(!is.finite(xsigma)) return(1.0)
	if(!is.finite(ysigma)) return(1.0)
        if(xsigma < 1e-20) return(1.0)
	if(ysigma < 1e-20) return(1.0)
        return(stats::t.test(x=v, y=v0)$p.value)
    }, zsets[[ind]])
    stats$P.t <- as.double(stats$P.t)

    # Analysis of variance with randomized design.
    p.anova <- 1.0
    try({
        tmp <- data.frame(Z=as.double(z), G=as.factor(g))
        fit <- stats::aov(formula=Z~G, data=tmp)
        p.anova <- unlist(summary(fit))["Pr(>F)1"]
        p.anova <- as.double(p.anova)
    }, silent=TRUE)

    # Add extra row for ANOVA.
    stats <- rbind(stats[1,], stats)
    stats[1,c("SUBGROUP", "MEAN","MEDIAN","SD","P.t","P.chisq")] <- NA
    stats$N[1] <- length(x)
    stats$MEAN[1] <- mean(x, na.rm=TRUE)
    stats$MEDIAN[1] <- stats::median(x, na.rm=TRUE)
    stats$SD[1] <- stats::sd(x, na.rm=TRUE)
    stats$P.anova[1] <- p.anova

    # Return results.
    return(stats)
}
