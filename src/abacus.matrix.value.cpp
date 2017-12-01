/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
mdreal
Matrix::value(const mdsize r, const mdsize c) const {
  MatrixBuffer* p = (MatrixBuffer*)buffer;
  map<Key, mdreal>::const_iterator pos;
  Key key = p->key(r, c);
  pos = (p->data).find(key);
  if(pos == (p->data).end()) return p->rlnan;
  return pos->second;
}
