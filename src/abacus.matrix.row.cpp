/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
vector<mdreal>
Matrix::row(const mdsize r) const {
  MatrixBuffer* p = (MatrixBuffer*)buffer;
  if(r >= p->nrows) return vector<mdreal>();
  vector<mdreal> array(p->ncols, p->rlnan);
  map<Key, mdreal>& data = p->data;
  map<Key, mdreal>::const_iterator pos;
  for(mdsize j = 0; j < p->ncols; j++) {
    Key key = p->key(r, j);
    pos = data.find(key);
    if(pos == data.end()) continue;
    array[j] = pos->second;
  }
  return array;
}
