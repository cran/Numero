nroKmeans <- function( x, k = 3 ){
  if( is.matrix( x ) == FALSE ) {
      stop( "'x' must be a matrix." )
  }
  
  # Check dataset.
  m <- nrow( x )
  n <- ncol( x )
  if( m < 10 ) { 
      stop( "Too few data." )
  }
  
  # Check number of centroids.
  if( k > m/2 ) { 
      stop( "Too many centroids." )
  }
  if( k < 2 ) { 
      stop( "Too few centroids." )
  }

  # Add small jitter to ensure convergence.
  for( j in 1:n ) {
      sigma <- sd(x[,j], na.rm=TRUE)
      jitter <- (((j/n)*m + (1:m))%%m)
      jitter <- (jitter - mean(jitter))
      jitter <- 0.001*sigma*jitter/m
      x[,j] <- (x[,j] + jitter)
  }

  # Estimate centroids.
  results <- .Call( "nro_kmeans",
                   as.matrix(x),
                   as.integer(k),
                   PACKAGE="Numero" )
  if( class( results ) == "character" ) {
      stop( results )
  }

  # Recode missing unit labels.
  bmcs <- as.integer(results[[ 2 ]])
  bmcs[which(bmcs == 0)] <- NA
  
  # Collect results.
  res <- list()
  res$centroids <- results[[ 1 ]]
  res$bmcs <- as.matrix(bmcs)
  res$history <- results[[ 3 ]]
  
  # Copy row and column names.
  rownames( res$bmcs ) <- rownames( x )
  colnames( res$bmcs ) <- c("BMC")
  colnames( res$centroids ) <- colnames( x )
  return( res )
}
