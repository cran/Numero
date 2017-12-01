/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
string
ModelBuffer::update(const mdreal rho) {
  mdsize sznan = medusa::snan();
  mdreal rlnan = medusa::rnan();

  /* Adjust neighborhood radius. */
  if(structure.rewire(rho) == false)
    return "Unusable input.";

  /* Allocate local arrays. */
  mdsize npoints = points.size();
  vector<mdreal> ones(npoints, 1.0);
  vector<Site> layers(npoints);
  vector<mdsize> bmus(npoints);
  vector<mdsize> loci(npoints);

  /* Collect best-matching units and layers. */
  for(mdsize i = 0; i < npoints; i++) {
    Point& pnt = points[i];
    Sample& sampl = samples[pnt.parent];
    layers[i] = pnt.layer;
    bmus[i] = sampl.unit;
  }

  /* Update codebook. */
  vector<vector<mdreal> > wsums;
  vector<vector<mdreal> > xsums;
  vector<vector<mdreal> > planes;
  for(mdsize j = 0; j < data.order(); j++) {
    vector<mdreal> x = data.column(j);

    /* Hide unusable points. */
    for(mdsize i = 0; i < npoints; i++)
      if(x[i] == rlnan) loci[i] = sznan;
      else loci[i] = bmus[i];
    
    /* Estimate component planes. */
    wsums = structure.diffuse(layers, loci, ones);
    xsums = structure.diffuse(layers, loci, x);
    planes = koho::operate(xsums, '/', wsums);

    /* Update current dimension. */
    mdsize nunits = structure.size();
    mdsize nlevels = codebook.size();
    for(mdsize k = 0; k < planes.size(); k++) {
      if(k >= nlevels) panic("Level mismatch.", __FILE__, __LINE__);
      vector<mdreal>& plane = planes[k];
      vector<vector<mdreal> >& mtx = codebook[k];
      for(mdsize i = 0; i < nunits; i++)
	mtx[i][j] = plane[i];
    }
  }
  return "";
}
