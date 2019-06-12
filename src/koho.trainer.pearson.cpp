/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "koho.local.h"

/*
 *
 */
mdreal
Trainer::pearson(const vector<mdreal>& x, const vector<mdreal>& y) {
  mdreal rlnan = medusa::rnan();

  /* Check size. */
  mdsize ndim = x.size();
  if(ndim > y.size()) ndim = y.size();

  /* Summary statistics. */
  mdsize nsum = 0;
  long double sx = 0.0;
  long double sy = 0.0;
  long double sxx = 0.0;
  long double syy = 0.0;
  long double sxy = 0.0;
  for(mdsize i = 0; i < ndim; i++) {
    if(x[i] == rlnan) continue;
    if(y[i] == rlnan) continue;
    long double xval = x[i];
    long double yval = y[i];
    sx += xval;
    sy += yval;
    sxx += xval*xval;
    sxy += xval*yval;
    syy += yval*yval;
    nsum++;
  }

  /* Calculate coefficient. */
  long double vx = sqrt(nsum*sxx - sx*sx);
  long double vy = sqrt(nsum*syy - sy*sy);
  if(vx < 1e-20) return rlnan;
  if(vy < 1e-20) return rlnan;
  mdreal rho = (nsum*sxy - sx*sy)/vx/vy;

  /* Convert to distance. */
  return (1.0 - fabs(rho));
}
