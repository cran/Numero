/* file: */

#include "nro.h"

/*
 *
 */
punos::Topology
nro::reals2topology(const vector<vector<mdreal> >& vectors ) {
  mdreal rlnan = medusa::rnan();

  /* Check unit coordinates. */
  mdsize nunits = vectors.size();
  vector<punos::Unit> units(nunits);
  for(mdsize i = 0; i < nunits; i++) {
    const vector<mdreal>& x = vectors[ i ];
    if( x.size() != 6 ) {
      return Topology();
    }
    for(mdsize j = 0; j < x.size(); j++) {
      if( x[j] == rlnan ) {
        return Topology();
      }
    }
    units[i].x = x[0];
    units[i].y = x[1];
    units[i].radii.first = x[2];
    units[i].radii.second = x[3];
    units[i].angles.first = x[4];
    units[i].angles.second = x[5];
  }

  /* Create map topology.*/
  vector<mdreal> epochs(1, nro_DEFAULT_STAMP);
  punos::Topology topo(epochs, units);
  return topo;
}
