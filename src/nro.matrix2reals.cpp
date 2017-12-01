/* file: */

#include "nro.h"

/*
 *
 */
vector<vector<mdreal> >
nro::matrix2reals( const SEXP& data ) {  
  mdreal rlnan = medusa::rnan();

  /* Extract matrix data. */
  NumericMatrix mtx = as<NumericMatrix>( data );
  mdsize nrows = mtx.nrow();
  mdsize ncols = mtx.ncol();

  /* Convert to C++ row vectors. */
  vector<vector<mdreal> > vectors;
  for(mdsize i = 0; i < nrows; i++){
    NumericVector row = mtx( i, _ );
    vector<mdreal> x; x.reserve(ncols);
    for( mdsize c = 0; c < ncols; c++ ){
      if( NumericVector::is_na( row[ c ] ) )
        x.push_back( rlnan );
      else
        x.push_back( row[ c ] );
    }
    vectors.push_back( x );
  }
  return vectors;
}
