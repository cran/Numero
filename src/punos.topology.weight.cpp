/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "punos.local.h"

/*
 *
 */
mdreal
Topology::weight(const mdsize a, const mdsize b) const {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  mdsize ncoord = (p->coord).size();
  if(a >= ncoord) return medusa::rnan();
  LinkMap& neigh = p->network[a];
  LinkMap::iterator pos = neigh.find(b);
  if(pos == neigh.end()) return medusa::rnan();
  return pos->second;
}
