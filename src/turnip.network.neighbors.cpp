/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
vector<string>
Network::neighbors(const string& key, const int dirflag) const {
  NetworkBuffer* p = (NetworkBuffer*)buffer;
  vector<string> array;

  /* Check if vertex exists. */
  unordered_map<string, mdsize>::iterator pos;
  unordered_map<string, mdsize>& name2rank = p->name2rank;
  if((pos = name2rank.find(key)) == name2rank.end()) return array;
  if(dirflag == 0) array.push_back(key);

  /* Collect neighbors. */
  vector<Vertex>& vert = p->vertices;
  Vertex& v = vert[pos->second];
  if(dirflag <= 0) {
    for(set<mdsize>::iterator it = v.incoming.begin();
	it != v.incoming.end(); it++)
      array.push_back(vert[*it].name());
  }
  if(dirflag >= 0) {
    for(set<mdsize>::iterator it = v.outgoing.begin();
	it != v.outgoing.end(); it++)
      array.push_back(vert[*it].name());
  }
  return array;
}
