nroFigure <- function( file, scene ) {
  if( is.list( scene ) == FALSE ) {
      stop( "'scene' must be a list." )
  }  
  results <- .Call( "nro_figure",
                   as.character(file),
                   as.character(scene$svg),
                   as.numeric(scene$bbox),
                   PACKAGE = "Numero" )  
  if( results < 1 ) {
      stop( 'Cannot save figure.' )
  }
  return( results )
}
