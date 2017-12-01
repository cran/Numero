/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "punos.local.h"

/*
 *
 */
vector<mdreal>
Topology::levels() const {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  return p->levels;
}
