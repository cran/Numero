/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
Cohesion
Cohesion::subset(const vector<string>& mkeys) const {
  CohesionBuffer* p = (CohesionBuffer*)buffer;
  
  /* Find modules. */
  unordered_set<mdsize> selected;
  unordered_map<string, mdsize>& mod2rank = p->mod2rank;
  for(mdsize i = 0; i < mkeys.size(); i++) {
    const string& mkey = mkeys[i];
    if(mod2rank.count(mkey) < 1) continue;
    selected.insert(mod2rank[mkey]);
  }
  
  /* Create the subset. */
  Cohesion output;
  for(unordered_set<mdsize>::iterator it = selected.begin();
      it != selected.end(); it++) {
    string& parent = p->modkeys[*it];
    set<mdsize>& members = p->modules[*it];
    for(set<mdsize>::iterator jt = members.begin();
	jt != members.end(); jt++) {
      string& child = p->itemkeys[*jt];
      mdreal x = (p->graph).value(*it, *jt);
      output.insert(parent, child, x);
    }
  }
  return output;
}
