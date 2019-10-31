/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
mdsize
Empirical::spread() const {
  EmpiricalBuffer* p = (EmpiricalBuffer*)buffer;
  return (p->data).size();
}

/*
 *
 */
mdsize
Empirical::spread(vector<mdreal>& values, vector<mdreal>& weights) const {
  EmpiricalBuffer* p = (EmpiricalBuffer*)buffer;
  p->contents(values, weights);
  return values.size();
}
