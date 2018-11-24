/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
string
Model::train(vector<pair<string, mdsize> >& layout,
	     vector<mdreal>& history) {
  ModelBuffer* p = (ModelBuffer*)buffer;
  unordered_map<string, Point>& points = p->points;

  /* Clear output containers. */
  layout.clear();
  history.clear();

  /* Check resources. */
  Topology topocopy = p->topology;
  mdsize npoints = (p->points).size();
  if(topocopy.size() < 1) return "No map units.";
  if(npoints < 10) return "Too few points.";
  if(p->ntrain < 10) return "Too few training points.";

  /* Initial neighborhood radius. */
  mdreal sigma = topocopy.sigma();
  mdreal rho = 0.5*(topocopy.radius());
  if(rho < sigma) rho = sigma;
  topocopy.rewire(rho);
  
  /* Create the training engine. */
  Trainer trainer(p->codebook, topocopy, p->ntrain, p->equality);
  
  /* Make pointers to points. */
  vector<Point*> pointers;
  for(unordered_map<string, Point>::iterator it = points.begin();
      it != points.end(); it++)
    pointers.push_back(&(it->second));

  /* Prepare sampling mask. */
  vector<Point*> mask = pointers;
  if(p->ntrain < npoints) mask.resize(p->ntrain);
  
  /* Fit codebook to training data. */
  unsigned long rvalue = p->ntrain;
  while(true) {
    vector<mdreal> batch;
    while(convergence(batch, 0.01) == false) {
      
      /* Shuffle sampling mask. */
      if(mask.size() < npoints) {
	rvalue += (batch.size())*8121;
	for(mdsize i = 0; i < mask.size(); i++) {
	  rvalue = (69069*rvalue + i)%npoints;
	  Point* pnt = pointers[rvalue];
	  pointers[rvalue] = pointers[i];
	  pointers[i] = pnt;
	  mask[i] = pnt;
	}
      }
 
      /* Perform a training cycle. */
      mdreal delta = trainer.cycle(mask, topocopy);
      batch.push_back(delta);
    }

    /* Update history. */
    history.insert(history.end(), batch.begin(), batch.end());

    /* Update neighborhood radius. */
    if(rho <= sigma) break;
    if((rho *= 0.67) < sigma) rho = sigma;
    topocopy.rewire(rho);
  }

  /* Update codebook. */
  p->codebook = trainer.codebook();
  
  /* Return final layout. */
  for(unordered_map<string, Point>::iterator it = points.begin();
      it != points.end(); it++) {
    pair<string, mdsize> entry;
    entry.first = it->first;
    entry.second = (it->second).location();
    layout.push_back(entry);
  }
  return "";
}
