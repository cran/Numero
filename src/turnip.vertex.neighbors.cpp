/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
vector<mdsize>
Vertex::neighbors() const {
  vector<mdsize> output;
  output.reserve(incoming.size() + outgoing.size());
  output.insert(output.end(), incoming.begin(), incoming.end());
  for(set<mdsize>::const_iterator it = outgoing.begin();
      it != outgoing.end(); it++)
    if(incoming.count(*it) < 1) output.push_back(*it);
  return output;
}
