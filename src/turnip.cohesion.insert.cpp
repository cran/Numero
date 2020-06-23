/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
string
Cohesion::insert(const string& mkey, const string& key,
		 const mdreal weight) {
  CohesionBuffer* p = (CohesionBuffer*)buffer;
  mdreal rlnan = medusa::rnan();

  /* Check inputs. */
  if(mkey.size() < 1) return "Empty parent.";
  if(key.size() < 1) return "Empty child.";
  if(weight == rlnan)
    return ("Unusable weight in '" + mkey + "'-'" + key + "'.");
  if(weight <= 0.0)
    return ("Non-positive weight in '" + mkey + "'-'" + key + "'.");

  /* Determine the parent's identity. */
  unordered_map<string, mdsize>& mod2rank = p->mod2rank;
  mdsize parent = mod2rank.size();
  if(mod2rank.count(mkey) < 1) mod2rank[mkey] = parent;
  else parent = mod2rank[mkey];

  /* Determine the child's identity. */
  unordered_map<string, mdsize>& item2rank = p->item2rank;
  mdsize child = item2rank.size();
  if(item2rank.count(key) < 1) item2rank[key] = child;
  else child = item2rank[key];

  /* Check if duplicate. */
  Matrix& graph = p->graph;
  if(graph.value(parent, child) != rlnan)
    return ("Duplicated edge '" + mkey + "'-'" + key + "'.");

  /* Allocate name arrays. */
  vector<string>& keys = p->itemkeys;
  vector<string>& mkeys = p->modkeys;
  mkeys.resize(mod2rank.size());
  keys.resize(item2rank.size());

  /* Allocate adjacency structures. */
  vector<set<mdsize> >& items = p->items;
  vector<set<mdsize> >& modules = p->modules;
  items.resize(item2rank.size());
  modules.resize(mod2rank.size());

  /* Update data structures. */
  keys[child] = key;
  mkeys[parent] = mkey;
  items[child].insert(parent);
  modules[parent].insert(child);
  graph.add(parent, child, weight);
  return "";
}
