/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
vector<Element>
Matrix::elements(const int flag) const {
  MatrixBuffer* p = (MatrixBuffer*)buffer;
  return p->elements(flag, false);
}
