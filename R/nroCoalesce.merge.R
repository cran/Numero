nroCoalesce.merge <- function(data, modules) {

    # Check that all columns are available.
    for(m in modules) {
        vars <- m
        if(is.list(vars)) vars <- names(vars$weights) 
        mask <- intersect(vars, colnames(data))
        if(length(mask) == length(vars)) next
	stop("Missing or duplicated module members.")
    }

    # Calculate principal components.
    for(k in 1:length(modules)) {
        w <- modules[[k]]
        if(is.list(w)) next
        modules[[k]] <- nroCoalesce_pca(data, w)
    }

    # Apply principal components.
    vnames <- colnames(data)
    mnames <- names(modules)
    for(k in 1:length(modules)) {
        modk <- modules[[k]]
        vars <- names(modk$weights)
	cols <- match(vars, vnames)
        data[,vars[1]] <- nroCoalesce_scores(data[,vars], modk)
        vnames[cols] <- NA
	vnames[cols[1]] <- mnames[k]
    }

    # Remove empty columns.
    cols <- which(nchar(vnames) >= 0)
    data <- data[,cols]
    vnames <- vnames[cols]

    # Check if only one column left.
    if(is.vector(data) && (length(modules) == 1)) {
        data <- data.frame(X=data, stringsAsFactor=FALSE)
	colnames(data) <- names(modules)
    }
    else {
        colnames(data) <- vnames
    }

    # Return results.
    attr(data, "modules") <- modules
    return(data)
}

#----------------------------------------------------------------------------

nroCoalesce_pca <- function(data, vars) {

    # Convert input to numeric matrix.
    data <- nroRcppMatrix(data[,vars], trim=TRUE)
    if(ncol(data) < 2) stop("Less than two usable module members.")
    if(nrow(data) < 2) stop("Less than two usable module data rows.")

    # Standardize data.
    mu <- apply(data, 2, mean, na.rm=TRUE)
    sigma <- apply(data, 2, stats::sd, na.rm=TRUE)
    sigma[which(!is.finite(sigma))] <- 1
    sigma[which(sigma <= 0)] <- 1
    for(j in which(sigma > 0))
        data[,j] <- (data[,j] - mu[j])/(sigma[j])

    # Impute missing values.
    vals <- .Call("nro_impute",
        as.matrix(data),
        500,
        -1.0,
        PACKAGE="Numero")
    if(is.character(vals)) stop(vals)

    # Find usable rows.
    mask <- which(0*rowMeans(vals) == 0)
    if(length(mask) <= ncol(vals))
        stop("Not enough usable data in module.")

    # Principal component analysis.
    pca <- stats::prcomp(vals[mask,], rank=1)
    weights <- pca$rotation[,1]
    names(weights) <- colnames(data)

    # Apply rotation.
    scores <- vals %*% weights
    
    # Check sign.
    cc <- stats::cor(scores, vals, use="pairwise.complete.obs")
    cc <- mean(cc, na.rm=TRUE)
    weights <- sign(cc)*weights

    # Eigenvalues.
    lambda <- as.double(pca$sdev)^2

    # Return results.
    output <- list()
    output$weights <- weights
    output$n <- length(mask)
    output$mu <- mu
    output$sigma <- sigma
    output$explained <- lambda[1]/sum(lambda)
    return(output)
}

#----------------------------------------------------------------------------

nroCoalesce_scores <- function(data, mdat) {
    mu <- mdat$mu
    sigma <- mdat$sigma
    weights <- mdat$weights
    vars <- names(weights)

    # Convert input to numeric matrix.
    data <- nroRcppMatrix(data[,vars], trim=FALSE)
    if(ncol(data) < 2) stop("Less than two usable module members.")
    if(nrow(data) < 2) stop("Less than two usable module data rows.")

    # Standardize data.
    if(is.null(mu)) stop("Incompatible module parameters")
    if(is.null(sigma)) stop("Incompatible module parameters")
    for(j in which(sigma > 0))
        data[,j] <- (data[,j] - mu[j])/(sigma[j])

    # Impute missing values.
    vals <- .Call("nro_impute",
        as.matrix(data),
        500,
        -1.0,
        PACKAGE="Numero")
    if(is.character(vals)) stop(vals)

    # Apply rotation.
    output <- vals %*% weights
    output <- scale.default(output)

    # Set scale and location.
    mu <- mean(mu, na.rm=TRUE)
    sigma <- sqrt(mean(sigma^2, na.rm=TRUE))
    output <- (sigma*output + mu)
    return(output)
}