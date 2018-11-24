/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "koho.local.h"

/*
 *
 */
mdreal
Trainer::euclidean(const vector<mdreal>& vals,
		   const vector<mdreal>& profile) {
  mdreal rlnan = medusa::rnan();

  /* Check size. */
  mdsize ndim = profile.size();
  if(ndim > vals.size()) ndim = vals.size();
  
  /* Calculate running sums. */
  long double dsum = 0.0;
  long double wsum = 0.0;
  for(mdsize j = 0; j < ndim; j++) {
    if(vals[j] == rlnan) continue;
    if(profile[j] == rlnan) continue;
    mdreal d = (profile[j] - vals[j]);
    dsum += d*d;
    wsum += 1.0;
  }
  if(wsum <= 0.0) return rlnan;
  return sqrt(dsum)/(wsum + 1e-9);
}
