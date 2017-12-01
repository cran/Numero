/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
vector<mdreal>
Matrix::column(const mdsize c) const {
  MatrixBuffer* p = (MatrixBuffer*)buffer;
  if(c >= p->ncols) return vector<mdreal>();
  vector<mdreal> array(p->nrows, p->rlnan);
  map<Key, mdreal>& data = p->data;
  map<Key, mdreal>::const_iterator pos;
  for(Key i = 0; i < p->nrows; i++) {
    Key key = p->key(i, c);
    pos = data.find(key);
    if(pos == data.end()) continue;
    array[i] = pos->second;
  }
  return array;
}
