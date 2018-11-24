/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "koho.local.h"

/*
 *
 */
mdreal
Trainer::distance(const Point& pnt, const mdsize bmu) const {
  if(bmu >= prototypes.size()) return medusa::rnan();
  vector<mdreal> vals = pnt.data();
  return Trainer::euclidean(vals, prototypes[bmu]);
}
