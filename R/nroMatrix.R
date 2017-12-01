nroMatrix <- function( file, vars = c(), keyvars = TRUE ) {  

  # Expand to full path.
  fileName <- path.expand(file)

  # Check file.
  if( file.exists( file ) == FALSE ) {
      stop( "Cannot access \"", file, "\"." )
  }

  # Check if row names are included without a heading.
  keyflag <- -1
  if( is.logical(keyvars) ) {
      keyflag <- as.integer(keyvars)
      keyvars <- c()
  }

  # Rcpp does not handle empty vectors well.
  if( length(vars) == 0 ) vars <- c( "" )
  if( length(keyvars) == 0 ) keyvars <- c( "" )
  
  # Check that inputs are distinct.
  if( length( vars ) != length( unique( vars ) ) ){
      stop( "Duplicate entries in 'vars'." )
  }
  if( length( keyvars) != length( unique( keyvars ) ) ){
      stop( "Duplicate entries in 'keyvars'." )
  }

  
  # Read data from disk.
  results <- .Call('nro_matrix',
                   as.character(fileName),
                   as.character(vars),
                   as.character(keyvars),
                   as.integer(keyflag),
                   PACKAGE = 'Numero')
  if( class( results ) == "character" ) {
      stop( results )
  }

  # Set row and column names.
  x <- results[[ 1 ]]
  xkeys <- results[[ 2 ]]
  xvars <- results[[ 3 ]]
  xkeyvars <- results[[ 4 ]]
  colnames( x ) <- xvars 
  rownames( x ) <- xkeys

  # Check identities.
  if( length( xkeys) != length( unique( xkeys ) ) ) {
      warning( "Duplicate identities." )
  }
  if( length(xvars) < length(vars) ) {
      warning( "Inconsistent variables in \"", fileName, "\"")
  }

  # Clear identity data from matrix.
  if( length(xkeyvars) > 0 ) {
      mask <- which( is.element( xvars, keyvars ) == FALSE )
      x <- x[,mask]
  }
  return( x )
}
