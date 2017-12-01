/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
// [[register]]
RcppExport SEXP nro_match( SEXP topo_R, SEXP codebook_R, SEXP data_R ) {
  mdreal rlnan = medusa::rnan();

  /* Determine map topology. */
  vector<vector<mdreal> > topodata = nro::matrix2reals( topo_R );
  punos::Topology topo = nro::reals2topology( topodata );
  if( topo.size() < 1 ) {
    return CharacterVector( "Unusable map topology." );
  }

  /* Check prototypes. */
  vector<vector<mdreal> > protos = nro::matrix2reals( codebook_R );
  if( topo.size() != protos.size() ) {
    return CharacterVector( "Incompatible codebook." );
  }

  /* Prepare a self-organizing map. */
  vector<string> keys;
  vector<vector<mdreal> > vectors = nro::matrix2reals( data_R );
  koho::Model som = nro::model( keys, topo, protos, vectors );
  if( som.size().first < 1 ) {
    return CharacterVector( "Unusable map model." );
  }

  /* Find best matching units. */
  vector<vector<mdreal> > results(keys.size());
  for(mdsize i = 0; i < keys.size(); i++) {
    mdsize bmu = som.location( keys[ i ] );
    if( bmu >= topo.size() ) results[i].push_back( rlnan );
    else results[i].push_back( bmu + 1.0 ); /* C++ -> R indexing */
    results[i].push_back( som.distance( keys[ i ] ) );
    results[i].push_back( abacus::statistic(vectors[ i ], "number" ) );
  }
  
  /* Return results. */
  return nro::reals2matrix( results );
}
