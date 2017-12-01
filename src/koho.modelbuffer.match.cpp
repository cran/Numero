/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
void
ModelBuffer::match(Sample& smp) {
  mdsize sznan = medusa::snan();
  mdreal rlnan = medusa::rnan();
  mdsize nunits = structure.size();

  /* Reset local buffers. */
  (this->dsums).resize(nunits);
  (this->wsums).resize(nunits);
  for(mdsize i = 0; i < nunits; i++) {
    dsums[i] = 0.0;
    wsums[i] = 0.0;
  }
  
  /* Calculate mean distances. */
  vector<mdsize>::const_iterator pos;
  for(pos = smp.members.begin(); pos != smp.members.end(); pos++) {
    Site& lr = points[*pos].layer;
    vector<mdreal> x = data.row(*pos);
    this->calcdist(codebook[lr.bounds.first], x, lr.weights.first);
    this->calcdist(codebook[lr.bounds.second], x, lr.weights.second);
  }

  /* Find smallest distance. */
  smp.unit = sznan;
  smp.distance = rlnan;
  for(mdsize i = 0; i < nunits; i++) {
    if(wsums[i] <= 0.0) continue;
    mdreal d = sqrt(dsums[i])/sqrt(wsums[i]);
    if(smp.distance == rlnan) smp.distance = d;
    if(smp.distance < d) continue;
    smp.distance = d;
    smp.unit = i;
  }
}
