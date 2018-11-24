/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "koho.local.h"

/*
 *
 */
vector<mdreal>
Trainer::distances(const Point& pnt) const {
  vector<mdreal> values = pnt.data();
  vector<mdreal> delta(prototypes.size());
  for(mdsize i = 0; i < prototypes.size(); i++)
    delta[i] = Trainer::euclidean(values, prototypes[i]);
  return delta;
}
