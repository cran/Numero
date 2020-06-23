/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
void
Engine::pool(const vector<mdreal>& vals) {
  EngineBuffer* p = (EngineBuffer*)buffer;
  mdreal rlnan = medusa::rnan();
  for(mdsize i = 0; i < vals.size(); i++)
    if(vals[i] == rlnan) panic("Bad value.", __FILE__, __LINE__);
  p->lottery = vals;
}
