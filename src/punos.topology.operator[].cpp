/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "punos.local.h"

/*
 *
 */
District
Topology::operator[](const mdsize ind) const {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  if(ind > (p->coord).size()) {
    District empty;
    empty.x = medusa::rnan();
    empty.y = empty.x;
    return empty;
  }
  return p->coord[ind];
}
