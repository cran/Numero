/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "punos.local.h"

/*
 *
 */
mdsize
Topology::size() const {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  return (p->coord).size();
}
