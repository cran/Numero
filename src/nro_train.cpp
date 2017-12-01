/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
// [[register]]
RcppExport SEXP nro_train( SEXP topo_R, SEXP codebook_R, SEXP data_R ) {

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

  /* Train the map with the data. */
  vector<mdreal> history;
  string err = som.train( history );
  if( err.size() > 0 ) {
    return CharacterVector( err );
  }

  /* Copy updated prototypes. */
  for(mdsize i = 0; i < protos.size(); i++) {
    protos[i] = som.prototype(nro_DEFAULT_LEVEL, i);
  }
  
  /* Return results. */
  NumericMatrix protomtx = nro::reals2matrix( protos );
  return List::create( protomtx, history );
}
