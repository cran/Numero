/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
vector<string>
Cohesion::items() const {
  CohesionBuffer* p = (CohesionBuffer*)buffer;
  return p->itemkeys;
}

/*
 *
 */
vector<string>
Cohesion::items(const string& mkey) const {
  CohesionBuffer* p = (CohesionBuffer*)buffer;
  vector<string> array;

  /* Find module rank. */
  unordered_map<string, mdsize>& mod2rank = p->mod2rank;
  unordered_map<string, mdsize>::iterator pos = mod2rank.find(mkey);
  if(pos == mod2rank.end()) return array;

  /* Collect adjacent items. */
  vector<string>& keys = p->itemkeys;
  set<mdsize>& ranks = p->modules[pos->second];
  for(set<mdsize>::iterator it = ranks.begin();
      it != ranks.end(); it++)
    array.push_back(keys[*it]);
  return array;
}

/*
 *
 */
mdsize
Cohesion::items(vector<string>& array, vector<mdreal>& counts) const {
  CohesionBuffer* p = (CohesionBuffer*)buffer;
  return this->items(array, counts, p->modkeys);
}

/*
 *
 */
mdsize
Cohesion::items(vector<string>& array, vector<mdreal>& counts,
		const vector<string>& mkeys) const {
  CohesionBuffer* p = (CohesionBuffer*)buffer;

  /* Clear outputs. */
  array.clear();
  counts.clear();

  /* Convert module keys to ranks. */
  unordered_set<mdsize> mranks;
  unordered_map<string, mdsize> mod2rank = p->mod2rank;
  for(vector<string>::const_iterator it = mkeys.begin();
      it != mkeys.end(); it++)
    if(mod2rank.count(*it) > 0) mranks.insert(mod2rank[*it]);

  /* Collect all items within modules. */
  Matrix& graph = p->graph;
  unordered_map<mdsize, mdreal> wsums;
  for(unordered_set<mdsize>::const_iterator it = mranks.begin();
      it != mranks.end(); it++) {
    set<mdsize>& batch = p->modules[*it];
    for(set<mdsize>::iterator jt = batch.begin();
	jt != batch.end(); jt++) {
      mdsize ind = *jt;
      mdreal w = graph.value(*it, ind);
      if(wsums.count(ind) < 1) wsums[ind] = -w;
      else wsums[ind] -= w; /* descending order */
    }
  }

  /* Sort by weight sum. */
  map<mdreal, vector<mdsize> > sorted;
  for(unordered_map<mdsize, mdreal>::iterator it = wsums.begin();
      it != wsums.end(); it++)
    sorted[it->second].push_back(it->first);

  /* Collect results. */
  vector<string>& keys = p->itemkeys;
  for(map<mdreal, vector<mdsize> >::iterator it = sorted.begin();
      it != sorted.end(); it++) {
    vector<mdsize>& batch = it->second;
    for(mdsize i = 0; i < batch.size(); i++) {
      array.push_back(keys[batch[i]]);
      counts.push_back(-(it->first));
    }
  }
  return array.size();
}

/*
 *
 */
vector<string>
Cohesion::items(const vector<string>& mkeys,
		const string& seltype) const {
  vector<string> array;

  /* Collect all items. */
  if(seltype == "union") {
    unordered_set<string> incl;
    for(mdsize i = 0; i < mkeys.size(); i++) {
      vector<string> memb = this->items(mkeys[i]);
      incl.insert(memb.begin(), memb.end());
    }
    array.insert(array.end(), incl.begin(), incl.end());
    return array;
  }

  /* Module membership counts. */
  unordered_map<string, mdsize> numbers;
  for(mdsize i = 0; i < mkeys.size(); i++) {
    vector<string> memb = this->items(mkeys[i]);
    for(vector<string>::iterator it = memb.begin();
	it != memb.end(); it++) {
      if(numbers.count(*it) < 1) numbers[*it] = 1;
      else numbers[*it] += 1;
    }
  }

  /* Collect items with single memberships. */
  if(seltype == "exclusion") {
    for(unordered_map<string, mdsize>::iterator it = numbers.begin();
	it != numbers.end(); it++)
      if(it->second == 1) array.push_back(it->first);
    return array;
  }

  /* Collect items that are members of all modules. */
  if(seltype == "intersection") {
    mdsize nmods = mkeys.size();
    for(unordered_map<string, mdsize>::iterator it = numbers.begin();
	it != numbers.end(); it++)
      if(it->second == nmods) array.push_back(it->first);
    return array;
  }

  /* Bad input. */
  panic("Unknown selection type.", __FILE__, __LINE__);
  return array;
}

