/* Created by Ville-Petteri Makinen 2017
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
vector<mdsize>
nro::vector2sizes( const SEXP& data ) {  
  mdsize sznan = medusa::snan();
  vector<mdsize> array;
  NumericVector values = as<NumericVector>( data );
  mdsize nelem = values.size();
  for(mdsize i = 0; i < nelem; i++) {
    if( NumericVector::is_na( values[ i ] ) )
      array.push_back( sznan );
    else
      array.push_back( (mdsize)(values[ i ] + 0.5) );
  }
  return array;
}
