/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
// [[register]]
RcppExport SEXP nro_aggregate( SEXP topo_R, SEXP bmus_R, SEXP data_R ) {

  /* Check inputs. */
  vector<mdsize> bmus = nro::vector2sizes( bmus_R );
  vector<mdreal> data = nro::vector2reals( data_R );
  if( data.size() != bmus.size() ) {
    return CharacterVector( "Incompatible inputs." );
  }

  /* Get map topology. */
  vector<vector<mdreal> > topodata = nro::matrix2reals( topo_R );
  punos::Topology topo = nro::reals2topology( topodata );
  if( topo.size() < 1 ) {
    return CharacterVector( "Unusable topology." );
  }

  /* Create simulation engine. */
  Engine eng = nro::engine(topo, data, bmus);

  /* Estimate component plane. */
  vector<vector<mdreal> > planes = eng.average(0);
  if(planes.size() != 1) return CharacterVector( "Engine failed." );
  return nro::reals2vector( planes[ 0 ] );
}
