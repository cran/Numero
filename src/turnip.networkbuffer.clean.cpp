/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
void
NetworkBuffer::clean() {
  if(cleanflag) return;

  /* Consequtive labeling of components. */
  unordered_map<mdsize, set<mdsize> > comps;
  unordered_map<mdsize, set<mdsize> >::iterator it;
  for(it = components.begin(); it != components.end(); it++) {
    mdsize rank = comps.size();
    comps[rank] = it->second;
    (it->second).clear();
  }

  /* Re-code component attributes. */
  this->components = comps;
  for(mdsize c = 0; c < components.size(); c++) {
    set<mdsize>& memb = components[c];
    for(set<mdsize>::iterator it = memb.begin(); it != memb.end(); it++)
      vertices[*it].component = c;
  }
  this->cleanflag = true;
}
