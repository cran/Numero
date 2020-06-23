/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
mdsize
Vertex::degree(const int flag) const {
  if(flag < 0) return incoming.size();
  if(flag > 0) return outgoing.size();
  return (incoming.size() + outgoing.size());
}
