/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
koho::Engine
nro::engine( const punos::Topology& topo, const vector<mdreal>& data,
	     const vector<mdsize>& bmus) {
  if( data.size() != bmus.size() ) return Engine();

  /* Create simulation engine. */
  Engine eng(topo);

  /* Fill map engine with data. */
  vector<mdreal> value(1);
  for(mdsize i = 0; i < data.size(); i++) {
    value[ 0 ] = data[ i ];

    /* Insert new sample. */
    string key = long2string( i );
    string err = eng.insert( key, nro_DEFAULT_STAMP, value );
    if( err.size() > 0 ) return Engine();

    /* Set sample position. */
    err = eng.put( key, (bmus[ i ] - 1)  ); /* R -> C++ indexing */
    if( err.size() > 0 ) return Engine();
  }
  return eng;
}
