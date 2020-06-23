/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
vector<Element>
Engine::edges() const {
  EngineBuffer* p = (EngineBuffer*)buffer;
  return (p->graph).elements(0);
}

/*
 *
 */
vector<Element>
Engine::edges(const vector<mdsize>& ranks) const {
  EngineBuffer* p = (EngineBuffer*)buffer;
  vector<Element> array;

  /* Create a look-up table. */
  unordered_set<mdsize> subset;
  subset.insert(ranks.begin(), ranks.end());

  /* Collect edges. */  
  const vector<vector<mdsize> >& links = p->links;
  for(unordered_set<mdsize>::const_iterator it = subset.begin();
      it != subset.end(); it++) {
    if(*it > links.size()) continue;
    const vector<mdsize>& neighbors = links[*it];
    for(mdsize k = 0; k < neighbors.size(); k++) {
      if(subset.count(neighbors[k]) < 1) continue;
      Element e = {*it, neighbors[k], 0.0};
      array.push_back(e);
    } 
  }

  /* Set edge weights. */
  (p->graph).values(array);
  return array;
}
