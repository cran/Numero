/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
bool
Empirical::add(const mdreal x, const mdreal w) {
  EmpiricalBuffer* p = (EmpiricalBuffer*)buffer;
  mdreal rlnan = medusa::rnan();
  if(x == rlnan) return false;
  if(w == rlnan) return false;
  if(w <= 0.0) return false;
  map<mdreal, mdreal>& data = p->data;
  if(data.count(x) < 1) data[x] = w;
  else data[x] += w;
  p->approx = Approximation();
  p->ndata += 1;
  return true;
}
