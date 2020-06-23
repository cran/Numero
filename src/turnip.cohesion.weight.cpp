/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
mdreal
Cohesion::weight(const string& mkey, const string& key) const {
  CohesionBuffer* p = (CohesionBuffer*)buffer;

  /* Determine the parent's identity. */
  unordered_map<string, mdsize>& mod2rank = p->mod2rank;
  if(mod2rank.count(mkey) < 1) return medusa::rnan();
  mdsize parent = mod2rank[mkey];

  /* Determine the child's identity. */
  unordered_map<string, mdsize>& item2rank = p->item2rank;
  if(item2rank.count(key) < 1) return medusa::rnan();
  mdsize child = item2rank[key];

  /* Return results. */
  return (p->graph).value(parent, child);
}
