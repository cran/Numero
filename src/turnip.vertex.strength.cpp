/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
mdreal
Vertex::strength(const Matrix& graph, const int flag) const {
  long double wsum = 0.0;
  mdreal rlnan = medusa::rnan();
  if(flag <= 0) {
    for(set<mdsize>::const_iterator it = incoming.begin();
	it != incoming.end(); it++) {
      mdreal w = graph.value(*it, locus);
      if(w != rlnan) wsum += w;
    }
  }
  if(flag >= 0) {
    for(set<mdsize>::const_iterator it = outgoing.begin();
	it != outgoing.end(); it++) {
      mdreal w = graph.value(locus, *it);
      if(w != rlnan) wsum += w;
    }
  }
  return wsum;
}
