/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
string
Model::configure(const mdsize level, const mdsize unit,
		 const vector<mdreal>& x) {
  ModelBuffer* p = (ModelBuffer*)buffer;
  mdreal rlnan = medusa::rnan();

  /* Check inputs. */
  Topology& topo = p->structure;
  mdsize nlevs = topo.depth();
  mdsize nunits = topo.size();
  if(level >= nlevs) return "Unusable level index.";
  if(unit >= nunits) return "Unusable unit index.";

  /* Check dataset dimensions. */
  mdsize ncols = x.size();
  mdsize ndim = (p->data).order();
  if((ndim > 0) && (ndim != ncols))
    return "Incompatible input.";

  /* Allocate codebook. */
  if((p->codebook).size() < 1) {
    vector<mdreal> empty(ncols, rlnan);
    vector<vector<mdreal> > page(nunits, empty);
    (p->codebook).resize(nlevs, page);
  }
  
  /* Replace codebook pages. */
  vector<vector<mdreal> >& mtx = p->codebook[level];
  mtx[unit] = x;
  return "";
}
