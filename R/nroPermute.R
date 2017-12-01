nroPermute <- function( map, bmus, x, n=10000 ){

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
  
  # Estimate statistics.
  nmax <- n
  if( is.na(nmax) ) nmax <- 1000
  results <- .Call("nro_permute",
                   as.matrix(map$topology),
                   as.numeric(bmus),
                   as.numeric(vals),
                   as.integer(nmax),
                   PACKAGE="Numero" )  
  if( class( results ) == "character" ) {
      stop( results );
  }
  
  # Estimate P-value.
  stats <- data.frame(Z=results[2], P.z=NA, P.freq=NA,
                      N.data=results[3], N.cycles=results[4])
  if( is.na(n) == FALSE ) {
    stats$P.z <- pnorm(results[2], lower.tail=FALSE)
    stats$P.freq <- results[1]
  }
  return( stats );
}
