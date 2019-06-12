/* Created by Ville-Petteri Makinen 2017
   South Australian Health and Medical Research Institute */

#include "nro.h"

#define N_TOPO_COLS 6

/*
 *
 */
punos::Topology
nro::reals2topology(const vector<vector<mdreal> >& vectors) {
  mdreal rlnan = medusa::rnan();

  /* Disconnected topology. */
  if(vectors.size() == 1)
    if(vectors[0].size() == 1)
      return Topology(vectors[0][0]);
  
  /* Check unit coordinates. */
  mdsize nunits = vectors.size();
  vector<punos::Unit> units(nunits);
  for(mdsize i = 0; i < nunits; i++) {
    const vector<mdreal>& x = vectors[i];
    if(x.size() < N_TOPO_COLS) return Topology();
    for(mdsize j = 0; j < N_TOPO_COLS; j++)
      if(x[j] == rlnan) return Topology();
    units[i].x = x[0];
    units[i].y = x[1];
    units[i].radii.first = x[2];
    units[i].radii.second = x[3];
    units[i].angles.first = x[4];
    units[i].angles.second = x[5];
  }

  /* Create map topology.*/
  vector<mdreal> epochs(1, 0.0);
  punos::Topology topo(epochs, units);
  return topo;
}
