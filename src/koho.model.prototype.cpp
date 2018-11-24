/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
vector<mdreal>
Model::prototype(const mdsize unit) const {
  ModelBuffer* p = (ModelBuffer*)buffer;
  return (p->codebook).row(unit);
}
