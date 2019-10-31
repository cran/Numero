/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
void
Matrix::rename(const mdsize pos, const string& name, const string& flag) {
  MatrixBuffer* p = (MatrixBuffer*)buffer;
  if(flag == "row") {
    if(name.size() < 1) (p->rownames).erase(pos);
    else (p->rownames).insert(pos, name);
  }
  if(flag == "column") {
    if(name.size() < 1) (p->colnames).erase(pos);
    else (p->colnames).insert(pos, name);
  }
}
