/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
void
EngineBuffer::organize() {
  if(nloci > 0) return;
  vector<mdsize> mask;
  for(mdsize i = 0; i < samples.size(); i++) {
    samples[i].unit = samples[i].home;
    mask.push_back(i);
  }
  this->organize(mask);
}

/*
 *
 */
void
EngineBuffer::organize(const vector<mdsize>& mask) {
  this->nloci = 0;

  /* Collect data points. */
  for(mdsize k = 0; k < mask.size(); k++) {
    Sample& sampl = samples[mask[k]];
    mdsize bmu = sampl.unit;

    /* Collect member points. */
    vector<mdsize>& memb = sampl.members;
    for(vector<mdsize>::iterator it = memb.begin();
	it != memb.end(); it++, nloci++) {
      Site& lr = points[*it].layer;
      if(nloci < bmus.size()) {
	bmus[nloci] = bmu;
	loci[nloci] = *it;
	layers[nloci] = lr;
	continue;
      }
      bmus.push_back(bmu);
      loci.push_back(*it);
      layers.push_back(lr);
    }
  }

  /* Set values for counting. */
  if(values.size() < nloci) this->values.resize(nloci);
  for(mdsize k = 0; k < nloci; k++)
    values[k] = 1.0;

  /* Calculate hit frequencies. */
  this->freq = structure.diffuse(layers, bmus, values);
}
