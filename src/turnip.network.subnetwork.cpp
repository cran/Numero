/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
Network
Network::subnetwork(const vector<string>& subset) const {
  NetworkBuffer* p = (NetworkBuffer*)buffer;
  Matrix& graph = p->graph;

  /* Prepare a subnetwork. */
  Network sub; NetworkBuffer* psub = (NetworkBuffer*)(sub.buffer);
  Matrix& subgraph = psub->graph;
  subgraph.symmetric(graph.symmetric());

  /* Find vertices in the network. */
  unordered_set<mdsize> shared;
  vector<Vertex>& vtx = p->vertices;
  unordered_map<string, mdsize>& name2rank = p->name2rank;
  for(vector<string>::const_iterator it = subset.begin();
      it != subset.end(); it++) {
    unordered_map<string, mdsize>::iterator pos;
    if((pos = name2rank.find(*it)) == name2rank.end()) continue;
    shared.insert(pos->second);
  }

  /* Collect adjacent connections. */
  for(unordered_set<mdsize>::iterator it = shared.begin();
      it != shared.end(); it++) {
    mdsize a = *it;
    string keyA = vtx[a].name();

    /* From B to A. */
    set<mdsize>& incoming = vtx[a].incoming;
    for(set<mdsize>::iterator jt = incoming.begin();
	jt != incoming.end(); jt++) {
      mdsize b = *jt;
      if(shared.count(b) < 1) continue;
      string keyB = vtx[b].name();
      mdreal w = graph.value(b, a);
      sub.add(keyB, keyA, w);
    }
  }    
  return sub;
}
