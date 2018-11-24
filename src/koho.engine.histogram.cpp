/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
vector<mdreal>
Engine::histogram() const {
  EngineBuffer* p = (EngineBuffer*)buffer;

  /* Check if data are ready. */
  if((p->cache).size() < 1) p->prepare();
  if((p->freq).size() > 0) return p->freq;

  /* Estimate point histogram on the map. */
  vector<mdreal> ones((p->bmus).size(), 1.0);
  p->freq = (p->topology).diffuse(p->bmus, ones);
  return p->freq;
}
