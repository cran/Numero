/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
vector<vector<mdreal> >
Engine::count() const {
  EngineBuffer* p = (EngineBuffer*)buffer;
  if((p->freq).size() < 1) p->organize();
  return p->freq;
}
