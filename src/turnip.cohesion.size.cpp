/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
mdsize
Cohesion::size() const {
  CohesionBuffer* p = (CohesionBuffer*)buffer;
  return (p->items).size();
}

/*
 *
 */
mdsize
Cohesion::size(const string& mkey) const {
  CohesionBuffer* p = (CohesionBuffer*)buffer;
  unordered_map<string, mdsize>& mod2rank = p->mod2rank;
  unordered_map<string, mdsize>::iterator pos = mod2rank.find(mkey);
  if(pos == mod2rank.end()) return 0;
  return (p->modules[pos->second]).size();
}
