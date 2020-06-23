/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
mdsize
Network::degree(const string& key, const int dirflag) const {
  NetworkBuffer* p = (NetworkBuffer*)buffer;
  unordered_map<string, mdsize>& name2rank = p->name2rank;
  unordered_map<string, mdsize>::iterator pos = name2rank.find(key);
  if(pos != name2rank.end())
    return (p->vertices[pos->second]).degree(dirflag);
  return 0;
}
