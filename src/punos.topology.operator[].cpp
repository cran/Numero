/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "punos.local.h"

/*
 *
 */
Unit
Topology::operator[](const mdsize ind) const {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  if(ind > (p->coord).size()) {
    Unit empty;
    empty.x = medusa::rnan();
    empty.y = empty.x;
    return empty;
  }
  return p->coord[ind];
}
