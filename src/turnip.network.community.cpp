/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
string
Network::community(const string& key) const {
  NetworkBuffer* p = (NetworkBuffer*)buffer;

  /* Find vertex. */
  unordered_map<string, mdsize>::iterator pos;
  unordered_map<string, mdsize>& name2rank = p->name2rank;
  if((pos = name2rank.find(key)) == name2rank.end()) return "";

  /* Check community assignment. */
  vector<Vertex>& vtx = p->vertices;
  Vertex& v = vtx[pos->second];
  if(v.community >= vtx.size()) return "";
  return vtx[v.community].name();
}
