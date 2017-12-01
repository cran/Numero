/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"
#include <Rcpp.h>

/*
 *
 */
void
Engine::shuffle(const bool flag) {
  EngineBuffer* p = (EngineBuffer*)buffer;
  vector<Sample>& samples = p->samples;
  mdsize nsampl = samples.size();

  /* Allocate index mask. */
  vector<mdsize> mask;
  mask.reserve(nsampl);

  /* Generate random numbers. */
  Rcpp::NumericVector r = Rcpp::runif(nsampl);

  /* Bootstrapping. */
  if(flag) {
    for(mdsize i = 0; i < nsampl; i++) {
      mdsize ind = (mdsize)(r[i]*nsampl);
      mask.push_back(ind%nsampl);
    }
    for(mdsize i = 0; i < nsampl; i++)
      samples[i].unit = samples[i].home;    
  }

  /* Permutation. */
  if(!flag) {
    for(mdsize i = 0; i < nsampl; i++)
      mask.push_back(i);
    for(mdsize i = 0; i < nsampl; i++) {
      mdsize ind = (mdsize)(r[i]*nsampl);
      Sample& a = samples[ind%nsampl];
      Sample& b = samples[i];
      mdsize u = a.unit;
      a.unit = b.unit;
      b.unit = u;
    }
  }
  
  /* Update layout. */
  p->organize(mask);
}
