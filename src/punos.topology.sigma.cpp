/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "punos.local.h"

/*
 *
 */
mdreal
Topology::sigma() const {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  return p->sigma;
}
