/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "koho.local.h"

/*
 *
 */
static mdreal
calc_euclid(const vector<mdreal>& vals,
	    const vector<mdreal>& profile) {
  mdreal rlnan = medusa::rnan();
  
  /* Check size. */
  mdsize nelem = profile.size();
  if(nelem > vals.size()) nelem = vals.size();
  
  /* Calculate running sums. */
  long double dsum = 0.0;
  long double wsum = 0.0;
  for(mdsize j = 0; j < nelem; j++) {
    if(vals[j] == rlnan) continue;
    if(profile[j] == rlnan) continue;
    mdreal d = (profile[j] - vals[j]);
    dsum += d*d;
    wsum += 1.0;
  }

   /* Use difference in extremes if comparison failed. */
  if(wsum < 1.0) {
    vector<mdreal> xtrem;
    xtrem.push_back(abacus::statistic(vals, "min")); 
    xtrem.push_back(abacus::statistic(vals, "max"));   
    xtrem.push_back(abacus::statistic(profile, "min"));    
    xtrem.push_back(abacus::statistic(profile, "max"));
    mdreal xmin = abacus::statistic(xtrem, "min");
    mdreal xmax = abacus::statistic(xtrem, "max");
    if(xmin != rlnan) return (xmax - xmin);
    return rlnan;
  }
  return sqrt(dsum/(wsum - 0.5));
}

/*
 *
 */
vector<mdreal>
Trainer::distance(const Point& pnt) const {
  vector<mdreal> vals = pnt.data();
  vector<mdreal> delta(prototypes.size());
  for(mdsize i = 0; i < prototypes.size(); i++)
    delta[i] = calc_euclid(vals, prototypes[i]);
  return delta;
}

/*
 *
 */
mdreal
Trainer::distance(const Point& pnt, const mdsize bmu) const {
  if(bmu >= prototypes.size()) return medusa::rnan();
  return calc_euclid(pnt.data(), prototypes[bmu]);
}
