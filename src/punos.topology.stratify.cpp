/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "punos.local.h"

/*
 *
 */
Site
Topology::stratify(const mdreal z) const {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  return medusa::binsearch(p->levels, z);
}
