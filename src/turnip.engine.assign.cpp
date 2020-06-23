/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
bool
Engine::assign(const mdsize rank, const mdreal x) {
  EngineBuffer* p = (EngineBuffer*)buffer;
  if(x == medusa::rnan()) return false;
  vector<mdreal>& vals = p->values;
  if(rank >= vals.size()) return false;
  vals[rank] = x;
  return true;
}
