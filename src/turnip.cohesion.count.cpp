/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
mdsize
Cohesion::count(const string& key) const {
  CohesionBuffer* p = (CohesionBuffer*)buffer;
  unordered_map<string, mdsize>::iterator pos;
  unordered_map<string, mdsize>& i2r = p->item2rank;
  if((pos = i2r.find(key)) != i2r.end())
    return (p->items[pos->second]).size();
  return 0;
}
