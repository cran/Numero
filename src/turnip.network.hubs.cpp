/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
vector<string>
Network::hubs() const {
  NetworkBuffer* p = (NetworkBuffer*)buffer;
  vector<string> array;
  vector<Vertex>& vtx = p->vertices;
  unordered_map<mdsize, set<mdsize> >& comts = p->communities;
  for(unordered_map<mdsize, set<mdsize> >::iterator it = comts.begin();
      it != comts.end(); it++)
    array.push_back(vtx[it->first].name());
  return array;
}
