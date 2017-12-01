/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
Topology
Model::topology() const {
  ModelBuffer* p = (ModelBuffer*)buffer;
  return p->structure;
}
