/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
string
Engine::put(const string& key, const mdsize u) {
  EngineBuffer* p = (EngineBuffer*)buffer;

  /* Check map size. */
  mdsize nunits = (p->structure).size();
  if(u >= nunits) return "Unusable unit index.";

  /* Find the sample. */
  map<string, mdsize>::iterator pos;
  map<string, mdsize>& key2rank = p->key2rank;
  if((pos = key2rank.find(key)) == key2rank.end())
    return ("Unknown sample '" + key + "'.");

  /* Update sample. */
  Sample& sampl = p->samples[pos->second];
  sampl.home = u;
  sampl.unit = u;
  return "";
}
