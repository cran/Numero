/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
static vector<mdreal>
calc_sums(const vector<mdsize>& bmus, const vector<mdreal>& values,
	  const vector<mdsize>& mask, const mdsize nunits) {
  mdsize nelem = mask.size();
  vector<mdreal> sums(nunits, 0.0);
  for(mdsize k = 0; k < nelem; k++)
    sums[bmus[k]] += values[mask[k]];
  return sums;
}
 
/*
 *
 */
vector<vector<mdreal> >
EngineBuffer::diffuse() {

  /* Check if data are ready. */
  if(cache.size() < 1) this->prepare();

  /* Process data columns. */
  mdsize nunits = topology.size();
  vector<vector<mdreal> > vsums(cache.size());
  for(mdsize j = 0; j < cache.size(); j++)
    vsums[j] = calc_sums(bmus, cache[j].values, mask, nunits);
    
  /* Smoothen component planes. */
  for(mdsize j = 0; j < cache.size(); j++)
    vsums[j] = topology.diffuse(units, vsums[j]);
  return vsums;
}
