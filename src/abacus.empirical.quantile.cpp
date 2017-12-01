/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
mdreal
Empirical::quantile(const mdreal q) const {
  EmpiricalBuffer* p = (EmpiricalBuffer*)buffer;
  vector<mdreal> values;
  vector<mdreal> weights;
  p->contents(values, weights);
  return abacus::quantile(values, weights, q);
}
