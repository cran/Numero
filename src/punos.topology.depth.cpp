/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "punos.local.h"

/*
 *
 */
mdsize
Topology::depth() const {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  return (p->levels).size();
}
