/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
mdsize
Engine::order() const {
  EngineBuffer* p = (EngineBuffer*)buffer;
  return (p->name2rank).size();
}
