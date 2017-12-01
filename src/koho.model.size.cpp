/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
pair<mdsize, mdsize>
Model::size() const {
  ModelBuffer* p = (ModelBuffer*)(this->buffer);
  pair<mdsize, mdsize> res;
  res.first = (p->samples).size();
  res.second = (p->points).size();
  return res;
}
