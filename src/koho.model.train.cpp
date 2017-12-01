/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
string
Model::train(vector<mdreal>& history) {
  ModelBuffer* p = (ModelBuffer*)buffer;
  mdreal rlnan = medusa::rnan();

  /* Reset history. */
  history.clear();

  /* Check resources. */
  Topology& topo = p->structure;
  vector<Sample>& samples = p->samples;
  if(topo.size() < 1) return "No map units.";
  if(samples.size() < 10) return "Too few samples.";

  /* Initial neighborhood radius. */
  mdreal sigma = p->sigma;
  mdreal rho = 0.33*(topo.radius());
  if(rho < sigma) rho = sigma;

  /* Fit codebook to training data. */
  while(true) {
    vector<mdreal> batch;
    while(convergence(batch, 0.01) == false) {
      long double dsum = 0.0;
      long double wsum = 0.0;
      
      /* Find best-matching units. */
      for(mdsize i = 0; i < samples.size(); i++) {
	p->match(samples[i]);

	/* Global error measure. */
	mdreal delta = samples[i].distance;
	if(delta == rlnan) continue;
	dsum += delta;
	wsum += 1.0;
      }

      /* Check if any hits. */
      if(wsum <= 0.0) return "Unit matching failed.";
 
      /* Update map with new layout. */
      string err = p->update(rho);
      if(err.size() > 0) return err;    
      
      /* Estimate average training error. */
      batch.push_back(dsum/wsum);
    }

    /* Update history. */
    history.insert(history.end(), batch.begin(), batch.end());

    /* Update neighborhood radius. */
    if(rho <= sigma) break;
    if((rho *= 0.5) < sigma) rho = sigma;
  }
  return "";
}
