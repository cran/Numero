/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
mdsize
Matrix::count() const {
  MatrixBuffer* p = (MatrixBuffer*)buffer;
  unordered_map<mdsize, Array>& rowdata = p->rowdata;
  mdsize ndata = 0;
  for(unordered_map<mdsize, Array>::iterator it = rowdata.begin();
      it != rowdata.end(); it++)
    ndata += (it->second).size();
  return ndata;
}
