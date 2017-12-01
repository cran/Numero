nroLabel <- function( map, values, gap=2.3 ){

  # Check inputs.
  if( is.list(map) == FALSE ) {
      stop( "'map' must be a list." );
  }
  if( is.vector(values) == FALSE ) {
      stop( "'values' must be a vector." );
  }
  
  # Determine label positions.
  results <- .Call("nro_label",
                   as.matrix(map$topology),
                   as.numeric(values),
                   as.numeric(gap),
                   PACKAGE = "Numero" )
  if( class( results ) == "character" ) {
      stop( results );
  }
  return( results[[ 1 ]] )
}
