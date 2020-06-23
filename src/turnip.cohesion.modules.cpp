/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

vector<string>
Cohesion::modules() const {
  CohesionBuffer* p = (CohesionBuffer*)buffer;
  return p->modkeys;
}

/*
 *
 */
vector<string>
Cohesion::modules(const string& key) const {
  CohesionBuffer* p = (CohesionBuffer*)buffer;
  vector<string> array;

  /* Find item rank. */
  unordered_map<string, mdsize>& item2rank = p->item2rank;
  unordered_map<string, mdsize>::iterator pos = item2rank.find(key);
  if(pos == item2rank.end()) return array;

  /* Collect adjacent modules. */
  vector<string>& mkeys = p->modkeys;
  set<mdsize>& mranks = p->items[pos->second];
  for(set<mdsize>::iterator it = mranks.begin();
      it != mranks.end(); it++)
    array.push_back(mkeys[*it]);
  return array;
}
