/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
mdsize
Engine::size() const {
  EngineBuffer* p = (EngineBuffer*)buffer;
  return (p->graph).count();
}
