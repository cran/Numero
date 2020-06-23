/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
void
Engine::release(const mdsize rank) {
  EngineBuffer* p = (EngineBuffer*)buffer;
  vector<mdreal>& vals = p->values;
  if(rank >= vals.size()) return;
  vals[rank] = medusa::rnan();
}
