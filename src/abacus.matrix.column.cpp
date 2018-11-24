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
  if(p->symmflag) panic("Symmetric matrix.", __FILE__, __LINE__);
  if(c >= p->ncols) return vector<mdreal>();
  vector<mdreal> values(p->nrows, p->rlnan);
  unordered_map<mdsize, Array>& rowdata = p->rowdata;
  for(unordered_map<mdsize, Array>::iterator it = rowdata.begin();
      it != rowdata.end(); it++)
    values[it->first] = it->second[c];
  return values;
}
