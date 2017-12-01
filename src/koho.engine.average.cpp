/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
vector<vector<mdreal> >
Engine::average(const mdsize ind) const {
  EngineBuffer* p = (EngineBuffer*)buffer;
  Topology& topo = p->structure;
  p->organize();
  
  /* Check input. */
  vector<vector<mdreal> > planes;
  if(ind >= (p->variables).size()) return planes;

  /* Collect data positions. */
  vector<mdsize>& mask = p->loci;
  vector<mdreal>& vals = p->values;
  vals.resize(p->nloci);

  /* Set data values. */
  Variable& v = p->variables[ind];
  vector<mdreal> array = v.transform();
  for(mdsize k = 0; k < p->nloci; k++)
    vals[k] = array[mask[k]];

  /* Estimate component planes. */
  planes = topo.diffuse(p->layers, p->bmus, vals);

  /* Calculate unit averages. */
  planes = koho::operate(planes, '/', p->freq);

  /* Restore original data space. */
  for(mdsize k = 0; k < planes.size(); k++)
    v.restore(planes[k]);
  return planes;
}
