nroTrain <- function( som, x ) {

  # Check input types.
  if( is.list( som ) == FALSE ) {
      stop( "'som' must be a list." )
  }
  if( is.matrix( x ) == FALSE ) {
      stop( "'x' must be a matrix." )
  }  
  
  # Check map and dataset compatibility.
  M <- nrow( x )
  N <- ncol( x )
  Mc <- nrow( som$centroids )
  Nc <- ncol( som$centroids )
  Mt <- nrow( som$topology )
  Nt <- ncol( som$topology )
  if( N != Nc ){ 
    stop( "Incompatible columns." )
  }
  if( Mc != Mt ){ 
    stop( "Incompatible topology." )
  }
  if( Nt != 6 ){ 
    stop( "Unusable topology." ) 
  }
  if( M * N < 1 ){ 
    stop( "'x' is empty." )
  }

  # Check that column names match.
  datnames <- colnames(x)
  somnames <- colnames(som$centroids)
  if( length( datnames ) != length( somnames) )
      stop( "Incompatible SOM and data columns." )
  if( sum( datnames != somnames ) > 0 )
      stop( "Incompatible SOM and data columns." )

  # Check if row names are distinct.
  keys <- rownames(x)
  if( length( unique( keys) ) != length( keys ) )
      warning( "Duplicate row names in data matrix." );
  
  # Train the SOM.
  results <- .Call("nro_train",
                   as.matrix(som$topology),
                   as.matrix(som$centroids),
                   as.matrix(x),
                   PACKAGE="Numero" )
  if( class( results ) == "character" ){
      stop( results )
  }
  
  # Copy column names.
  som$centroids <- results[[ 1 ]]
  som$history <- results[[ 2 ]]
  colnames( som$centroids ) <- somnames
  return( som );
}
