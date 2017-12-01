/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "punos.local.h"

/*
 *
 */
bool
Topology::rewire(const mdreal s) {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  mdbyte btmin = medusa::bmin();
  mdbyte btmax = medusa::bmax();

  /* Check input. */
  if((s <= 0.0) || (s >= p->maxradius)) {
    worry("Unusable input.", __FILE__);
    return false;
  }

  /* Check if changes needed. */
  vector<LinkMap>& network = p->network;
  if(network.size() < 1) p->sigma = 0.0;
  if(s == p->sigma) return true;
  network.clear();

  /* Calculate (non-normalized) Gaussian link weights. */
  const vector<Unit>& coord = p->coord;
  mdsize nunits = coord.size();
  for(mdsize i = 0; i < nunits; i++) {
    LinkMap neigh;
    Unit unitA = coord[i];
    for(mdsize j = 0; j < nunits; j++) {
      Unit unitB = coord[j];
      double dx = (unitB.x - unitA.x)/(s + 1e-6);
      double dy = (unitB.y - unitA.y)/(s + 1e-6);
      double r = sqrt(dx*dx + dy*dy);
      mdbyte w = (mdbyte)(exp(-0.5*r*r)*btmax);
      if(w < 0.05*btmax) continue;
      if(w < btmin) continue;
      neigh[j] = w;
    }
    network.push_back(neigh);
  }

  /* Save new scale factor. */
  p->sigma = s;
  return (network.size() > 0);
}
