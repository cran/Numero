/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "punos.local.h"

/*
 *
 */
vector<mdreal>
Topology::levels() const {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  return p->levels;
}
