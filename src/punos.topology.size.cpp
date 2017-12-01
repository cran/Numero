/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "punos.local.h"

/*
 *
 */
mdsize
Topology::size() const {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  return (p->coord).size();
}
