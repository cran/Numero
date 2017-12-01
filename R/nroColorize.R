nroColorize <- function( values, amplitude=1.0, palette="rhodo" ) {

  # Check inputs.
  if( is.vector(values) == FALSE ) {
      stop( "'values' must be a vector." );
  }
  if( is.vector(palette) == FALSE ) {
      stop( "'palette' must be a vector." );
  }

  # Check amplitude.
  if( amplitude < 0.0 ) amplitude = 0.0;
  if( ( amplitude >= 0.0 ) == FALSE ) {
      stop( "Unusable amplitude." )
  }
  
  # Pre-defined palette.
  if( length(palette) < 2 ) {
      palette <- .Call( "nro_colorize", 
                        as.character(palette),
                        PACKAGE = "Numero" )
      if( length(palette) < 2 ) {
          stop( palette[[ 1 ]] )
      }
  }

  # Center on average value.
  mu <- mean( values, na.rm=TRUE )
  z <- (values - mu)

  # Normalize by maximum deviation.
  zdev <- quantile( abs(z), 0.999, na.rm=TRUE )
  z <- z / max( zdev, .Machine$double.eps )

  # Apply amplitude factor.
  z <- z*amplitude
  z <- 0.5*(z + 1.0) # centered at 0.5

  # Determine color indices.
  ncolors <- length(palette)
  ind <- round( z*ncolors + 0.5 )
  ind[which(ind > ncolors)] <- ncolors
  ind[which(ind < 1)] <- 1

  # Collect color values.
  return( palette[ ind ] );
}
