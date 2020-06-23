/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 * Join the community that is the most connected to the vertex.
 */
mdsize
Vertex::join(const Matrix& graph, const vector<Vertex>& vtx,
	     const int dirflag) {
  mdsize sznan = medusa::snan();
  mdreal rlnan = medusa::rnan();
  unordered_map<mdsize, mdreal> wsums;

  /* Check if anything to do. */
  if(community != sznan) return community;
  
  /* Calculate aggregate incoming weight magnitudes. */
  if(dirflag <= 0) {
    for(set<mdsize>::iterator it = incoming.begin();
	it != incoming.end(); it++) {
      mdsize c = vtx[*it].community;
      if(c == sznan) continue;
      mdreal w = graph.value(*it, locus);
      if(w == rlnan) continue;
      if(wsums.count(c) < 1) wsums[c] = fabs(w);
      else wsums[c] += fabs(w);
    }
  }

  /* Calculate aggregate outgoing weight magnitudes. */
  if(dirflag >= 0) {
    for(set<mdsize>::iterator it = outgoing.begin();
	it != outgoing.end(); it++) {
      mdsize c = vtx[*it].community;
      if(c == sznan) continue;
      mdreal w = graph.value(locus, *it);
      if(w == rlnan) continue;
      if(wsums.count(c) < 1) wsums[c] = fabs(w);
      else wsums[c] += fabs(w);
    }
  }

  /* Find the community with the strongest connection. */
  wsums[community] = 0.0;
  for(unordered_map<mdsize, mdreal>::iterator it = wsums.begin();
      it != wsums.end(); it++)
    if(it->second > wsums[community]) this->community = it->first;
  return community;
}
