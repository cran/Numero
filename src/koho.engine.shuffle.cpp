/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
bool
Engine::shuffle(const bool flag) {
  EngineBuffer* p = (EngineBuffer*)buffer;
  vector<mdsize>& mask = p->mask;
  mt19937& twister = p->twister;

  /* Check engine state. */
  mdsize nelem = mask.size();
  if(nelem < 1) return false;
  
  /* Permutations without replacement. */
  if(flag == false) {
    for(mdsize i = 0; i < nelem; i++) {
      mdsize ind = twister()%nelem;
      mdsize rank = mask[ind];
      mask[ind] = mask[i];
      mask[i] = rank;
    }
    return true;
  }

  /* Permutations with replacement. */
  for(mdsize i = 0; i < nelem; i++)
    mask[i] = twister()%nelem;

  /* Reset point frequencies due to replacements. */
  (p->freq).clear();
  return true;
}
