/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
vector<mdreal>
Model::prototype(const mdsize level, const mdsize unit) const {
  ModelBuffer* p = (ModelBuffer*)buffer;
  if((p->codebook).size() <= level) return vector<mdreal>();
  if((p->codebook[level]).size() <= unit) return vector<mdreal>();
  return p->codebook[level][unit];
}
