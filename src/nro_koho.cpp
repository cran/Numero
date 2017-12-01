/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
// [[register]]
RcppExport SEXP nro_koho( SEXP seeds_R, SEXP rho_R ) {
  vector<vector<mdreal> > seeds = nro::matrix2reals( seeds_R );
  mdreal rho = as<mdreal>( rho_R );
  
  /* Check inputs. */
  if( seeds.size() < 3 ) {
    return CharacterVector( "Too few seeds." );
  }
  if( seeds[0].size() < 3 ) {
    return CharacterVector( "Too few data columns." );
  }
  if( rho == medusa::rnan() ) {
    return CharacterVector( "Unusable map radius." );
  }
  if( rho < 2.0 ) {
    return CharacterVector( "Too small map radius." );
  }

  /* Create map topology. */
  vector<mdreal> epochs(1, nro_DEFAULT_STAMP);
  punos::Topology topo(epochs, (mdsize)(rho + 0.5));
  mdsize nunits = topo.size();
  if( nunits < 1 ) {
    return CharacterVector( "Cannot create topology." );
  }

  /* Interpolate component planes. */
  vector<vector<mdreal> > protos = topo.interpolate( seeds );
  if( protos.size() != nunits ) {
    return CharacterVector( "Prototype interpolation failed." );
  }

  /* Collect coordinate data. */
  vector<vector<mdreal> > coord(nunits);
  for(mdsize i = 0; i < nunits; i++) {
    punos::Unit u = topo[i];
    vector<mdreal>& c = coord[i];
    c.push_back(u.x);
    c.push_back(u.y);
    c.push_back(u.radii.first);
    c.push_back(u.radii.second);
    c.push_back(u.angles.first);
    c.push_back(u.angles.second);
  }

  /* Return results. */
  NumericMatrix codebook = reals2matrix( protos );
  NumericMatrix layout = reals2matrix( coord );
  return List::create( codebook, layout );
}
