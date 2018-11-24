/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
bool
Matrix::insert(const mdsize r0, const mdsize c0, const mdreal x) {
  MatrixBuffer* p = (MatrixBuffer*)buffer;
  if(x == p->rlnan) return false;

  /* Check if symmetric. */
  mdsize r = r0;
  mdsize c = c0;
  if(p->symmflag && (r > c)) {
    r = c0;
    c = r0;
  }
  
  /* Update contents. */
  (p->rowdata[r]).update(c, x, true);
  if(r >= p->nrows) p->nrows = (r + 1);
  if(c >= p->ncols) p->ncols = (c + 1);
  return true;
}
