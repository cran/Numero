/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
mdreal
Empirical::z(const mdreal x) const {
  EmpiricalBuffer* p = (EmpiricalBuffer*)buffer;
  mdreal rlnan = medusa::rnan();
  if(x == rlnan) return rlnan;

  /* Check if Gaussian model already available. */
  mdreal z = (p->approx).transform(x);
  if(z != rlnan) return z;

  /* Collect data values. */
  vector<mdreal> values;
  vector<mdreal> weights;
  p->contents(values, weights);

  /* Create Gaussian approximation. */
  (p->approx).fit(values, weights);
  return (p->approx).transform(x);
}
