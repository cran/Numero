/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 * Check the stability for a sequence of numbers.
 */
bool
abacus::convergence(const vector<mdreal>& diary, const mdreal limit) {
  mdsize ndiary = diary.size();
  mdreal rlnan = medusa::rnan();
  if(ndiary < 10) return false;

  /* Check the three last points. */
  mdreal tA = diary[ndiary-3];
  mdreal tB = diary[ndiary-2];
  mdreal tC = diary[ndiary-1];
  if((tA == tB) && (tA == tC) && (tA != rlnan))
    return true;

  /* Determine pivot points. */
  if(ndiary < 20) return false;
  mdsize alpha = (ndiary - sqrt(20*ndiary) + 0.5);
  mdsize beta = 0.5*(ndiary + alpha); 

  /* Second last batch of samples. */
  long double xsumA = 0.0;
  for(mdsize i = alpha; i < beta; i++) {
    mdreal x = diary[i];
    if(x == rlnan) panic("Unusable value.", __FILE__, __LINE__);
    xsumA += x;
  }

  /* Last batch of samples. */
  long double xsumB = 0.0;
  for(mdsize i = beta; i < ndiary; i++) {
    mdreal x = diary[i];
    if(x == rlnan) panic("Unusable value.", __FILE__, __LINE__);
    xsumB += x;
  }

  /* Estimate mean value. */
  double xmean = 0.5*(xsumA + xsumB);
  xmean /= (ndiary - alpha);

  /* Calculate deviation. */
  long double dsum = 0.0;
  for(mdsize i = alpha; i < ndiary; i++)
    dsum += fabs(diary[i] - xmean);

  /* Check proportional difference. */
  double delta = fabs(xsumB - xsumA)/(dsum + 1e-10);
  return (delta < limit);
}
