/* Created by Ville-Petteri Makinen 2017
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
Network
Network::combine(const Network& net, const int flag) const {

  /* Network topologies. */
  vector<Edge> graphA = this->edges();
  vector<Edge> graphB = net.edges();
  
  /* Redundancy flag. */
  bool redn = (this->redundant() || net.redundant());
  
  /* Count edge frequencies. */
  Network freqs; freqs.redundant(redn);
  for(vector<Edge>::iterator it = graphA.begin();
      it != graphA.end(); it++)
    freqs.add(it->tail, it->head, 1.0);
  for(vector<Edge>::iterator it = graphB.begin();
      it != graphB.end(); it++)
    freqs.add(it->tail, it->head, 1.0);
  
  /* Construct combination. */
  Network output; output.redundant(redn);
  for(vector<Edge>::iterator it = graphA.begin();
      it != graphA.end(); it++) {
    mdreal f = freqs.weight(it->tail, it->head);
    if((flag > 0) && (f != 2.0)) continue;
    if((flag < 0) && (f != 1.0)) continue;
    output.add(it->tail, it->head, it->weight);
  }
  for(vector<Edge>::iterator it = graphB.begin();
      it != graphB.end(); it++) {
    mdreal f = freqs.weight(it->tail, it->head);
    if((flag > 0) && (f != 2.0)) continue;
    if((flag < 0) && (f != 1.0)) continue;
    output.add(it->tail, it->head, it->weight);
  }
  return output;
}
