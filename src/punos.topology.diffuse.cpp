/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "punos.local.h"

/*
 *
 */
vector<vector<mdreal> >
Topology::diffuse(const vector<Site>& layers, const vector<mdsize>& bmus,
		  const vector<mdreal>& vals) const {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  mdsize nelem = bmus.size();
  mdsize nunits = (p->coord).size();
  mdsize nlevels = (p->levels).size();
  mdreal rlnan = medusa::rnan();
  if(layers.size() != nelem)
    panic("Incompatible inputs.", __FILE__, __LINE__);
  if(vals.size() != nelem)
    panic("Incompatible inputs.", __FILE__, __LINE__);

  /* Allocate temporary arrays. */
  vector<double> empty(nunits, 0.0);
  vector<vector<double> > xsums(nlevels, empty);

  /* Calculate running sums. */
  for(mdsize i = 0; i < nelem; i++) {
    const Site& layer = layers[i];
    mdsize bmu = bmus[i];
    if(bmu >= nunits) continue;
    mdreal x = vals[i];
    if(x == rlnan) panic("Unusable value.", __FILE__, __LINE__);
    mdsize a = layer.bounds.first;
    mdsize b = layer.bounds.second;
    mdreal wa = layer.weights.first;
    mdreal wb = layer.weights.second;
    if(a >= nlevels) continue;
    if(b >= nlevels) continue;
    if(wa == rlnan) continue;
    if(wb == rlnan) continue;
    xsums[a][bmu] += wa*x;
    xsums[b][bmu] += wb*x;
  }

  /* Return smoothed sums. */
  vector<vector<mdreal> > planes(nlevels);
  for(mdsize k = 0; k < nlevels; k++)
    planes[k] = punos_local::smoothen(xsums[k], p->network);
  return planes;
}
