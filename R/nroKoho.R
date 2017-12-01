nroKoho <- function( seeds, radius=3 ) {
  if( is.matrix( seeds ) == FALSE ) {
      stop( "'seeds' must be a matrix." );
  }
  
  # Check inputs.
  m <- nrow( seeds )
  n <- ncol( seeds )
  if( m * n < 1 ) {
    stop( "'seeds' is empty." );
  }
  if( radius < 2 ) {
    stop( "Too small map radius." );
  }

  # Set up a self-organizing map.
  results <- .Call("nro_koho",
                   as.matrix(seeds),
                   as.integer(radius),
                   PACKAGE="Numero" );
  if( class( results ) == "character" ){
    stop( results )
  }

  # Collect results.
  res <- list()
  res$centroids <- results[[ 1 ]]
  res$topology <- results[[ 2 ]]

  # Set column names.
  colnames(res$centroids) <- colnames(seeds)
  colnames(res$topology) <- c("X", "Y", "RADIUS1", "RADIUS2",
                              "ANGLE1", "ANGLE2")
  return( res )
}
