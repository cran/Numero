/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
void
Matrix::values(vector<Element>& elem) const {
  MatrixBuffer* p = (MatrixBuffer*)buffer;
  const map<Key, mdreal>& data = p->data;
  vector<Key> keys = p->key(elem);
  for(mdsize i = 0; i < keys.size(); i++) {
    map<Key, mdreal>::const_iterator pos;
    pos = data.find(keys[i]);
    if(pos == data.end()) elem[i].value = p->rlnan;
    else elem[i].value = pos->second;
  }
}
