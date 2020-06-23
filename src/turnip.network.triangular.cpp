/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
bool
Network::triangular(const string& key) const {
  NetworkBuffer* p = (NetworkBuffer*)buffer;

  /* Check if vertex exists. */
  unordered_map<string, mdsize>::iterator pos;
  unordered_map<string, mdsize>& name2rank = p->name2rank;
  if((pos = name2rank.find(key)) == name2rank.end()) return false;

  /* Collect primary neighbors. */
  vector<Vertex>& vtx = p->vertices;
  Vertex& v = vtx[pos->second];
  mdsize rank = v.rank();
  
  /* Check neighbors via incoming edges. */
  for(set<mdsize>::iterator it = v.incoming.begin();
      it != v.incoming.end(); it++) {
    Vertex& x = vtx[*it];
    for(set<mdsize>::iterator jt = x.incoming.begin();
	jt != x.incoming.end(); jt++) {
      if(vtx[*jt].incoming.count(rank) > 0) return true;
      if(vtx[*jt].outgoing.count(rank) > 0) return true;
    }
    for(set<mdsize>::iterator jt = x.outgoing.begin();
	jt != x.outgoing.end(); jt++) {
      if(vtx[*jt].incoming.count(rank) > 0) return true;
      if(vtx[*jt].outgoing.count(rank) > 0) return true;
    }
  }
  
  /* Check neighbors via outgoing edges. */
  for(set<mdsize>::iterator it = v.outgoing.begin();
      it != v.outgoing.end(); it++) {
    Vertex& x = vtx[*it];
    for(set<mdsize>::iterator jt = x.incoming.begin();
	jt != x.incoming.end(); jt++) {
      if(vtx[*jt].incoming.count(rank) > 0) return true;
      if(vtx[*jt].outgoing.count(rank) > 0) return true;
    }
    for(set<mdsize>::iterator jt = x.outgoing.begin();
	jt != x.outgoing.end(); jt++) {
      if(vtx[*jt].incoming.count(rank) > 0) return true;
      if(vtx[*jt].outgoing.count(rank) > 0) return true;
    }
  }
  return false;
}
