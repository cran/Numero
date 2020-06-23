/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 * 
 */
set<Triangle>
NetworkBuffer::triangles(vector<mdsize>& trifree) const {
  set<Triangle> output;
  trifree.clear();
  
  /* Find triangles. */
  for(mdsize orig = 0; orig < vertices.size(); orig++) {
    const Vertex& v = vertices[orig];

    /* Unvisited neighbors -> their unvisited neighbors -> origin.
       This will visit all triangles that include the origin vertex. */
    vector<mdsize> neigh1 = v.neighbors();
    for(vector<mdsize>::const_iterator it = neigh1.begin();
	it != neigh1.end(); it++) {
      if(*it <= orig) continue; /* triangles already covered */

      /* Neighbors of neighbors. */
      vector<mdsize> neigh2 = vertices[*it].neighbors();
      for(vector<mdsize>::const_iterator jt = neigh2.begin();
	  jt != neigh2.end(); jt++) {
	if(*jt <= orig) continue; /* triangles already covered */
	Triangle t(orig, *it, *jt, graph);
	if(v.incoming.count(*jt) > 0) output.insert(t);
	if(v.outgoing.count(*jt) > 0) output.insert(t);
      }
    }
  }

  /* Find components with triangles. */
  unordered_set<mdsize> cyclic;
  for(set<Triangle>::iterator it = output.begin();
      it != output.end(); it++)
    cyclic.insert(vertices[it->origin].component);
  
  /* Collect components with no triangles. */
  unordered_map<mdsize, set<mdsize> >::const_iterator pos;
  for(pos = components.begin(); pos != components.end(); pos++)
    if(cyclic.count(pos->first) < 1) trifree.push_back(pos->first);
  return output;
}
