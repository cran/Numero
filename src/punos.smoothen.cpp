/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "punos.local.h"

/*
 *
 */
vector<mdreal>
punos_local::smoothen(const vector<double>& wsums,
		      const vector<LinkMap>& network) {
  mdsize nunits = network.size();
  mdreal rlnan = medusa::rnan();
  if(wsums.size() != nunits) panic("Bad input.", __FILE__, __LINE__);

  /* Temporary arrays. */
  vector<double> y(nunits, 0.0);
  vector<double> h(nunits, 0.0);
  
  /* Sum across neighboring units, including self. */
  LinkMap::const_iterator pos;
  for(mdsize i = 0; i < nunits; i++) {
    const LinkMap& neigh = network[i];
    for(pos = neigh.begin(); pos != neigh.end(); pos++) {
      double w = wsums[pos->first];
      y[i] += (pos->second)*w;
      h[i] += (pos->second);
    }
  }

  /* Estimate mean values. */
  vector<mdreal> plane(nunits, rlnan);
  for(mdsize i = 0; i < nunits; i++)
    if(h[i] > 0.0) plane[i] = (y[i])/(h[i]);
  return plane;
}
