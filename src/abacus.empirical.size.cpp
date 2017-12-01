/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
mdsize
Empirical::size() const {
  EmpiricalBuffer* p = (EmpiricalBuffer*)buffer;
  return p->ndata;
}
