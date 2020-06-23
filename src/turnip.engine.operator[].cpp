/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
mdsize
Engine::operator[](const string& key) const {
  EngineBuffer* p = (EngineBuffer*)buffer;
  unordered_map<string, mdsize>& name2rank = p->name2rank;
  if(name2rank.count(key) < 1) return medusa::snan();
  return name2rank[key];
}
