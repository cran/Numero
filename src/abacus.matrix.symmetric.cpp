/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
void
Matrix::symmetric(const bool flag) {
  MatrixBuffer* p = (MatrixBuffer*)buffer;
  if((p->data).size() < 1) p->symmflag = flag;
  else panic("Matrix not empty.", __FILE__, __LINE__);
}

/*
 *
 */
bool
Matrix::symmetric() const {
  MatrixBuffer* p = (MatrixBuffer*)buffer;
  return p->symmflag;
}
