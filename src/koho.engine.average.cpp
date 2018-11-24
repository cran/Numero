/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
vector<vector<mdreal> >
Engine::average() const {
  EngineBuffer* p = (EngineBuffer*)buffer;
  mdreal rlnan = medusa::rnan();

  /* Estimate smoothed point frequencies. */
  vector<mdreal> hgram = this->histogram();
  
  /* Estimate smoothed sums. */
  vector<vector<mdreal> > planes = p->diffuse();

  /* Normalize values. */
  for(mdsize j = 0; j < planes.size(); j++) {
    vector<mdreal>& sums = planes[j];
    for(mdsize i = 0; i < sums.size(); i++)
      if(hgram[i] > 0.0) sums[i] /= (hgram[i] + 1e-9);
      else sums[i] = rlnan;
  }
  
  /* Restore original data space. */
  vector<ColumnCache>& cache = p->cache;
  for(mdsize j = 0; j < planes.size(); j++)
    cache[j].transf.restore(planes[j]);
  return planes;
}
