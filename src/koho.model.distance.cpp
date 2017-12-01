/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
mdreal
Model::distance(const string& key) const { 
  ModelBuffer* p = (ModelBuffer*)buffer;

  /* Find the sample. */
  map<string, mdsize>::const_iterator pos;
  const map<string, mdsize>& key2rank = p->key2rank;
  if((pos = key2rank.find(key)) == key2rank.end())
    return medusa::rnan();
  
  /* Find the best match. */
  mdsize rank = pos->second;
  mdsize nunits = (p->structure).size();
  Sample& smp = p->samples[rank];
  if(smp.unit >= nunits) p->match(smp);
  return smp.distance;
}
