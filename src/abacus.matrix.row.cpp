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
  if(p->symmflag) panic("Symmetric matrix.", __FILE__, __LINE__);
  if(r >= p->nrows) return vector<mdreal>();
  if((p->rowdata).count(r) < 1) return vector<mdreal>();
  return (p->rowdata[r]).values();
}
