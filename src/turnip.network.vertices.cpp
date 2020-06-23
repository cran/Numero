/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
vector<string>
Network::vertices() const {
  NetworkBuffer* p = (NetworkBuffer*)buffer;
  vector<Vertex>& vert = p->vertices;
  vector<string> array(vert.size());
  for(mdsize i = 0; i < vert.size(); i++)
    array[i] = vert[i].name();
  return array;
}

/*
 *
 */
vector<string>
Network::vertices(const string& key) const {
  NetworkBuffer* p = (NetworkBuffer*)buffer;
  vector<string> array;

  /* Find vertex. */
  unordered_map<string, mdsize>::iterator pos;
  unordered_map<string, mdsize>& name2rank = p->name2rank;
  if((pos = name2rank.find(key)) == name2rank.end()) return array;

  /* Find hub. */
  vector<Vertex>& vtx = p->vertices;
  Vertex& v = vtx[pos->second];
  if(v.community >= vtx.size()) return array;

  /* Find community members. */
  set<mdsize>& memb = p->communities[v.community];
  if(memb.size() < 1) panic("Bad community.", __FILE__, __LINE__);
  
  /* Collect community members. */
  for(set<mdsize>::iterator it = memb.begin(); it != memb.end(); it++)
    array.push_back(vtx[*it].name());
  return array;
}
