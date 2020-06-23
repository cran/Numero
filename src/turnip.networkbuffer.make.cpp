/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
mdsize
NetworkBuffer::make(const string& key) {
  unordered_map<string, mdsize>::iterator pos = name2rank.find(key);
  if(pos != name2rank.end()) return pos->second;
  mdsize rank = vertices.size();
  (this->vertices).push_back(Vertex(key, rank));
  this->name2rank[key] = rank;
  this->cleanflag = false;
  return rank;
}
