nroAggregate <- function( map, bmus, x=double() ) {

  # Check input types.
  if( is.list( map ) == FALSE ) {
      stop( "'map' must be a list." )
  }
  if( is.vector( bmus ) == FALSE ) {
      stop( "'bmus' must be a vector." )
  }
  if( is.vector( x ) == FALSE ) {
      stop( "'x' must be a vector." )
  }

  # Check if histogram is needed.
  if( length(x) < 1 ) {
      counts <- as.data.frame(table(bmus))
      results <- .Call( "nro_aggregate", 
                        as.matrix(map$topology),  
                        as.numeric(counts[,1]),
                        as.numeric(counts[,2]), 
                        PACKAGE = "Numero" );
      if( class( results ) == "character" ) {
          stop( results )
      }
      return( results )
  }

  # Check input sizes.
  if( length(x) != length(bmus) ) {
      stop( "'x' and 'bmus' size mismatch." )
  }

  # Remove missing values.
  mask <- which( 0*x*bmus == 0 )
  if( length( mask ) < 10 ) {
      stop( "Too few usable samples." )
  }
  bmus <- bmus[ mask ]
  vals <- x[ mask ]
  
  # Estimate component planes.
  results <- .Call("nro_aggregate",
                   as.matrix(map$topology),  
                   as.numeric(bmus),
                   as.numeric(vals),
                   PACKAGE="Numero" );
  if( class( results ) == "character" ) {
      stop( results )
  }
  return( results )
}
