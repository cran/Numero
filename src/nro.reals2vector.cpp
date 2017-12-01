/* file: */

#include "nro.h"

/*
 *
 */
NumericVector
nro::reals2vector( const vector<mdreal>& values ) {
  mdreal rlnan = medusa::rnan();
  mdsize nelem = values.size();
  NumericVector array( nelem );
  for( mdsize i = 0; i < nelem; i++ ) {
    if( array[ i ] == rlnan )
      array[ i ] = NA_REAL;
    else
      array[ i ] = values[ i ];
  }
  return array;
}
