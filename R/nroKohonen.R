nroKohonen <- function(
  seeds,
  radius=3) {

  # Check if input is a list.
  if(!is.data.frame(seeds) && is.list(seeds))
      seeds <- seeds$centroids

  # Check input data.
  seeds <- nroRcppMatrix(seeds, trim=TRUE)
  if(nrow(seeds) < 3) stop("Less than three seeds.")
  if(ncol(seeds) < 3) stop("Less than three dimensions.")

  # Check if any rows or columns were excluded.
  if(length(attr(seeds, "excl.rows")) > 0)
      warning("Unusable row(s) excluded.")
  if(length(attr(seeds, "excl.columns")) > 0)
      warning("Unusable column(s) excluded.")

  # Check radius.
  radius <- as.integer(radius[[1]])
  if(!is.finite(radius)) stop("Unusable radius.")
  if(radius < 2) stop("Radius is less than two.")

  # Set up a self-organizing map.
  res <- .Call("nro_kohonen",
               as.matrix(seeds),
               as.integer(radius),
               PACKAGE="Numero");
  if(class(res) == "character") stop(res)

  # Convert to data frame to make it easier to add columns later.
  res$topology <- data.frame(res$topology, stringsAsFactors=FALSE)

  # Set column names.
  colnames(res$centroids) <- colnames(seeds)
  colnames(res$topology) <- c("X", "Y", "RADIUS1", "RADIUS2",
                              "ANGLE1", "ANGLE2")

  # Set row names.
  rownames(res$centroids) <- (1:nrow(res$centroids))
  rownames(res$topology) <- (1:nrow(res$topology))

  # Determine radius in logical units.
  t <- table(res$topology$RADIUS1)
  res$radius <- (length(t) - 1)
  return(res)
}
