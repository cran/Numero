/* file: */

#include "nro.h"

/*
 *
 */
vector<mdreal>
nro::vector2reals( const SEXP& data ) {  
  mdreal rlnan = medusa::rnan();
  vector<mdreal> array;
  NumericVector values = as<NumericVector>( data );
  mdsize nelem = values.size();
  for(mdsize i = 0; i < nelem; i++) {
    if( NumericVector::is_na( values[ i ] ) )
      array.push_back( rlnan );
    else
      array.push_back( values[ i ] );
  }
  return array;
}
