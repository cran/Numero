/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "punos.local.h"

/*
 *
 */
Site
Topology::stratify(const mdreal z) const {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  return binsearch(p->levels, z);
}
