/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 * 
 */
mdsize
NetworkBuffer::concentrate(const set<mdsize>& members) const {
  
  /* Estimate vertex strengths. */
  map<mdreal, mdsize> priority;
  for(set<mdsize>::const_iterator it = members.begin();
      it != members.end(); it++) {
    if(*it >= vertices.size()) panic("Bad index.", __FILE__, __LINE__);
    const Vertex& v = vertices[*it];

    /* Incoming edges. */
    mdreal wsum = 0.0;
    for(set<mdsize>::const_iterator jt = v.incoming.begin();
	jt != v.incoming.end(); jt++) {
      if(members.count(*jt) < 1) continue;
      mdreal w = graph.value(*jt, *it);
      if(w == medusa::rnan()) panic("Bad value.", __FILE__, __LINE__);
      wsum -= sqrt(fabs(w));
    }

    /* Outgoing edges. */
    for(set<mdsize>::const_iterator jt = v.outgoing.begin();
	jt != v.outgoing.end(); jt++) {
      if(members.count(*jt) < 1) continue;
      mdreal w = graph.value(*it, *jt);
      if(w == medusa::rnan()) panic("Bad value.", __FILE__, __LINE__);
      wsum -= sqrt(fabs(w));
    }
    priority[wsum] = *it;
  }

  /* Check if successful. */
  if(priority.size() < 1)
    panic("Bad data.", __FILE__, __LINE__);
  
  /* Return the strongest vertex. */
  map<mdreal, mdsize>::iterator pos = priority.begin();
  return pos->second;
}
