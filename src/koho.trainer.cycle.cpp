/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
mdreal
Trainer::cycle(vector<Point*>& points, const Topology& topo) {
  mdreal delta = this->match(points, topo);
  this->update(topo);
  return delta;
}
