/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
unordered_set<mdsize>
Engine::neighbors(const mdsize rank) const {
  EngineBuffer* p = (EngineBuffer*)buffer;
  vector<vector<mdsize> >& links = p->links;
  unordered_set<mdsize> array;
  if(rank >= links.size()) return array;
  array.insert(links[rank].begin(), links[rank].end());
  array.insert(rank);
  return array;
}
