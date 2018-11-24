nroKohonen <- function(
  seeds,
  radius=3) {
  
  # Check inputs.
  if(nrow(seeds)*ncol(seeds) < 1) stop("Empty input.")
  if(radius < 2) stop("Too small map radius.")

  # Set up a self-organizing map.
  res <- .Call("nro_kohonen",
               as.matrix(seeds),
               as.integer(radius),
               PACKAGE="Numero");
  if(class(res) == "character") stop(res)

  # Set column names.
  colnames(res$centroids) <- colnames(seeds)
  colnames(res$topology) <- c("X", "Y", "RADIUS1", "RADIUS2",
                              "ANGLE1", "ANGLE2")
  res$topology <- data.frame(res$topology, stringsAsFactors=FALSE)

  # Set row names.
  rownames(res$centroids) <- (1:nrow(res$centroids))
  rownames(res$topology) <- (1:nrow(res$topology))
  return(res)
}
