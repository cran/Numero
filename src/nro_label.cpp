/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
// [[register]]
RcppExport SEXP nro_label( SEXP topo_R, SEXP data_R, SEXP sigma_R ) {
  mdreal sigma = as<mdreal>( sigma_R );

  /* Get map topology.*/
  vector<vector<mdreal> > topodata = nro::matrix2reals( topo_R );
  punos::Topology topo = reals2topology( topodata );
  if( topo.size() < 1 ) {
    return CharacterVector( "Unusable topology." );
  }

  /* Set neighborhood radius. */
  if( topo.rewire(sigma) == false ){
    return CharacterVector( "Unusable label gap." );
  }

  /* Check data vector. */
  vector<mdreal> data = nro::vector2reals( data_R );
  if( data.size() != topo.size() ) {
    return CharacterVector( "Incompatible inputs." );
  }

  /* Calculate devaitions from the mean. */
  vector<mdreal> amps = abacus::transform(data, "z");
  for(mdsize i = 0; i < amps.size(); i++) {
    if(amps[i] == medusa::rnan()) continue;
    amps[i] = fabs(amps[i]);
  }

  /* Sort map units according to amplitude. */
  vector<mdsize> sorted = sortreal(amps, -1);

  /* Select labeled units. */
  vector<int> flags(topo.size(), 0);
  for(mdsize k = 0; k < sorted.size(); k++) {

    /* Skip units that have already been denied. */
    mdsize unit = sorted[k];
    if(flags[unit] != 0) continue;

    /* Deny label from too close neighbors. */
    vector<mdsize> neigh = topo.neighbors(unit);
    for(vector<mdsize>::iterator pos = neigh.begin();
	pos != neigh.end(); pos++) {
      mdreal r = topo.distance(unit, *pos);
      if(r > sigma) continue;
      flags[*pos] = -1;
    }

    /* Allow label for the current unit. */
    flags[unit] = 1;
  }

  /* Set labels. */
  vector<string> labels(data.size());
  for(mdsize k = 0; k < data.size(); k++) {
    if(flags[k] != 1) continue;
    labels[k] = medusa::real2text(data[k]);
  }
  return List::create( labels );
}
