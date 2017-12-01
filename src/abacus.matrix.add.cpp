/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
bool
Matrix::add(const mdsize r, const mdsize c, const mdreal x) {
  MatrixBuffer* p = (MatrixBuffer*)buffer;
  if(x == p->rlnan) return false;

  /* Determine element index. */
  Key key = p->key(r, c);

  /* Check if element already exists. */
  map<Key, mdreal>::iterator pos;
  pos = (p->data).find(key);
  if(pos != (p->data).end()) {
    pos->second += x;
    return true;
  }

  /* Add new element. */
  if(r >= p->nrows) p->nrows = (r + 1);
  if(c >= p->ncols) p->ncols = (c + 1);
  p->data[key] = x;
  return true;
}
