/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "punos.local.h"

/*
 *
 */
mdreal
Topology::distance(const mdsize a, const mdsize b) const {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  vector<Unit>& coord = p->coord;
  mdsize nunits = coord.size();
  if(a >= nunits) return medusa::rnan();
  if(b >= nunits) return medusa::rnan();
  mdreal dx = (coord[b].x - coord[a].x);
  mdreal dy = (coord[b].y - coord[a].y);
  return sqrt(dx*dx + dy*dy);
}
