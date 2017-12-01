/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
Key
MatrixBuffer::key(mdsize r, mdsize c) const {
  if(symmflag && (r > c)) {
    mdsize t = r;
    r = c;
    c = t;
  }
  Key n = nline;
  Key locus = (r*n + c);
  if(locus/nline != r) panic("Indexing failed.", __FILE__, __LINE__);
  if(locus%nline != c) panic("Indexing failed.", __FILE__, __LINE__);
  return locus;
}

/*
 *
 */
vector<Key>
MatrixBuffer::key(const vector<Element>& elem) const {
  Key n = nline;
  vector<Element>::const_iterator it;
  vector<Key> array; array.reserve(elem.size());
  for(it = elem.begin(); it != elem.end(); it++) {
    mdsize r = it->row;
    mdsize c = it->column;
    if(symmflag && (r > c)) {
      mdsize t = r;
      r = c;
      c = t;
    }
    array.push_back(r*n + c);
  }
  return array;
}
