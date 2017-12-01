/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
// [[register]]
RcppExport SEXP nro_kmeans( SEXP data_R, SEXP ncent_R ) {
  mdsize ncent = as<mdsize>( ncent_R );
  vector<vector<mdreal> > data = nro::matrix2reals( data_R );
  
  /* Check inputs. */
  mdsize nrows = data.size();
  if( nrows < 1 ) { 
    return CharacterVector( "Too few rows in data matrix." );
  }
  mdsize ncols = data[0].size();
  if( ncols < 1 ) { 
    return CharacterVector( "Too few columns in data matrix." );
  }

  /* K-means clustering. */
  vector<mdsize> labels;
  vector<mdreal> history;
  vector<vector<mdreal> > centroids
    = koho::kmeans( labels, history, data, ncent );

  /* Recode best matching unit labels. */
  for(mdsize i = 0; i < labels.size(); i++) {
    if( labels[ i ] >= centroids.size() ) labels[ i ] = 0;
    else labels[ i ] += 1;
  }

  /* Return results. */
  NumericMatrix cmatrix = nro::reals2matrix( centroids );
  return List::create( cmatrix, labels, history );
}
