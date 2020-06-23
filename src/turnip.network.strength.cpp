/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
mdreal
Network::strength(const string& key, const int dirflag) const {
  NetworkBuffer* p = (NetworkBuffer*)buffer;
  unordered_map<string, mdsize>& name2rank = p->name2rank;
  unordered_map<string, mdsize>::iterator pos = name2rank.find(key);
  if(pos == name2rank.end()) return medusa::rnan();
  return (p->vertices[pos->second]).strength(p->graph, dirflag);
}
