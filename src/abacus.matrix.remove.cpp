/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
mdreal
Matrix::remove(const mdsize r, const mdsize c) {
  MatrixBuffer* p = (MatrixBuffer*)buffer;
  Key key = p->key(r, c);
  map<Key, mdreal>& data = p->data;
  map<Key, mdreal>::iterator pos;
  if((pos = data.find(key)) == data.end()) return p->rlnan;
  mdreal x = pos->second;
  data.erase(key);
  return x;
}
