/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
vector<string>
Model::identities() const {
  ModelBuffer* p = (ModelBuffer*)buffer;
  vector<string> array;
  map<string, mdsize>::const_iterator pos;
  const map<string, mdsize>& key2rank = p->key2rank;
  for(pos = key2rank.begin(); pos != key2rank.end(); pos++)
    array.push_back(pos->first);
  return array;
}
