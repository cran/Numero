nroSummary <- function(
    data,
    districts,
    regions,
    categlim=8) {
    if(is.null(regions)) {
        warning("No regions defined.")
        return(data.frame())
    }

    # Training variables.
    trvars <- attr(districts, "features")
    trvars <- as.character(trvars)
    
    # Exclude unmatched data points.
    districts <- as.integer(districts)
    mask <- which((districts > 0) & (districts <= length(regions)))
    districts <- districts[mask]
    data <- data[mask,]

    # Assign data points to subgroups.
    g <- regions[districts]
    if(sum(is.na(g)) > 0) {
        warning("Unusable regions.")
        return(nroSummary.empty())
    }
    if(length(unique(g)) < 2) {
        warning("Less than two subgroups.")
        return(nroSummary.empty())
    }

    # Process data columns.
    results <- data.frame()
    for(vn in colnames(data)) {
        nlev <- nlevels(as.factor(data[,vn]))
	stats <- "Unusable values."
        if(nlev > categlim) stats <- nroSummary.real(data[,vn], g)
	else stats <- nroSummary.categ(data[,vn], g)
	if(is.character(stats)) {
	    warning(paste(vn, ": ", stats, sep=""))
            next
        }
	stats <- data.frame(VARIABLE=vn, stats)
	results <- rbind(results, stats)
    }

    # Erase P-values from training variables.
    pos <- match(results$VARIABLE, trvars)
    rows <- which(pos > 0)
    results[rows,c("P.chisq","P.t","P.anova")] <- NA
    return(results)
}

#---------------------------------------------------------------------------

nroSummary.empty <- function() {
    stats <- data.frame(VARIABLE=NA)
    stats$SUBGROUP <- NA
    stats$N <- NA
    stats$MEAN <- NA
    stats$MEDIAN <- NA
    stats$SD <- NA
    stats$TYPE <- NA
    stats$P.chisq <- NA
    stats$P.t <- NA
    stats$P.anova <- NA
    return(stats)
}

#---------------------------------------------------------------------------

nroSummary.uniform <- function(x, name) {
    stats <- data.frame(SUBGROUP=name)
    stats$N <- sum(0*x == 0)
    stats$MEAN <- mean(x, na.rm=TRUE)
    stats$MEDIAN <- stats::median(x, na.rm=TRUE)
    stats$SD <- stats::sd(x, na.rm=TRUE)
    stats$TYPE <- "singular"
    stats$P.chisq <- NA
    stats$P.t <- NA
    stats$P.anova <- NA
    return(stats)
}

#---------------------------------------------------------------------------

nroSummary.categ <- function(x, g) {

    # Check if enough data.
    mask <- which(is.na(x) == FALSE)
    if(length(mask) < 10) return("Too few data.")

    # Split into subgroups.
    xsets <- split(x[mask], g[mask])
    if(length(xsets) < 2) return("Only one subgroup.")

    # Recode if binary.
    levs <- levels(as.factor(x[mask]))
    nlevs <- length(levs)
    if(nlevs == 2) {
        xsets <- lapply(xsets, function(v, l) {
	    v <- (v == l)
        }, levs[2])
	levs <- c(TRUE, FALSE)
    }

    # Subgroup sizes.
    stats <- list()
    stats$N <- lapply(xsets, function(v) {
        return(sum(0*v == 0))
    })

    # Estimate basic subgroup stats.
    stats$MEAN <- rep(NA, length(xsets))
    stats$MEDIAN <- rep(NA, length(xsets))
    stats$SD <- rep(NA, length(xsets))
    if(nlevs == 2) stats$MEAN <- lapply(xsets, mean, na.rm=TRUE)

    # Convert to data frame.
    stats <- lapply(stats, as.double)
    stats <- as.data.frame(stats)
    stats <- data.frame(SUBGROUP=names(xsets), stats)

    # Add P-value columns.
    stats$TYPE <- "categ"
    if(nlevs == 2) stats$TYPE <- "binary"
    stats$P.chisq <- NA
    stats$P.t <- NA
    stats$P.anova <- NA

    # Find the biggest subgroup.
    mu <- mean(x, na.rm=TRUE)
    ind <- which.max(stats$N)
    if(length(ind) < 1) return("Unusable subgroups.")

    # Add extra class labels to every subgroup to ensure Chi2-test
    # works (this will slightly dilute the results).
    for(k in 1:length(xsets))
        xsets[[k]] <- c(xsets[[k]], levs)

    # Chi-squared test.
    suppressWarnings(
    stats$P.chisq <- lapply(xsets, function(v, v0) {
        bits <- c(rep(0, length(v)), rep(1, length(v0)))
        st <- stats::chisq.test(c(v, v0), bits)
	return(st$p.value)
    }, xsets[[ind]]))
    stats$P.chisq <- as.double(stats$P.chisq)
    
    # Analysis of variance with randomized design.
    tmp <- data.frame(X=x, G=as.factor(g))
    fit <- stats::aov(formula=X~G, data=tmp)
    stats$P.anova <- unlist(summary(fit))["Pr(>F)1"]
    stats$P.anova <- as.double(stats$P.anova)

    # Return results.
    return(stats)
}

#---------------------------------------------------------------------------

nroSummary.real <- function(x, g) {

    # Check if enough data.
    mask <- which(0*x == 0)
    sigma <- stats::sd(x, na.rm=TRUE)
    if(length(mask) < 10) return("Too few data.")
    if(sigma < 1e-9) return("Too low variance.")

    # Convert to tapered ranks.
    z <- (rank(x, na.last="keep") - 1)
    z <- (2*z/max(z, na.rm=TRUE) - 1)
    z <- 0.5*(z + z^3)

    # Split into subgroups.
    xsets <- split(x[mask], g[mask])
    zsets <- split(z[mask], g[mask])
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
    stats <- as.data.frame(stats)
    stats <- data.frame(SUBGROUP=names(xsets), stats)

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
        return(stats::t.test(x=v, y=v0)$p.value)
    }, zsets[[ind]])
    stats$P.t <- as.double(stats$P.t)

    # Analysis of variance with randomized design.
    tmp <- data.frame(Z=as.double(z), G=as.factor(g))
    fit <- stats::aov(formula=Z~G, data=tmp)
    stats$P.anova <- unlist(summary(fit))["Pr(>F)1"]
    stats$P.anova <- as.double(stats$P.anova)

    # Return results.
    return(stats)
}
