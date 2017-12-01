/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "koho.local.h"

/*
 *
 */
void
Variable::append(const mdreal x) {
  sorted.clear();
  ranks.clear();
  data.push_back(x);
}

/*
 *
 */
void
Variable::restore(vector<mdreal>& vals) {
  mdreal rlnan = medusa::rnan();
  mdsize nelem = vals.size();

  /* Check object. */
  if(sorted.size() < 1) this->transform();
  if(sorted.size() < 1) {
   for(mdsize i = 0; i < nelem; i++)
     vals[i] = rlnan;
    return;
  }

  /* Interpolate to original data space. */
  mdsize rmax = (sorted.size() - 1);
  for(mdsize i = 0; i < nelem; i++) {
    mdreal x = vals[i];
    if(x == rlnan) continue;
    if(x <= 0.0) {
      vals[i] = sorted[0];
      continue;
    }
    if(x >= rmax) {
      vals[i] = sorted[rmax];
      continue;
    }
    mdsize a = (mdsize)x;
    mdsize b = (a + 1);
    mdreal yA = sorted[a];
    mdreal yB = sorted[b];
    mdreal wA = (b - x);
    mdreal wB = (x - a);
    x = (wA*yA + wB*yB);
    vals[i] = x/(wA + wB);
  }
}

/*
 *
 */
vector<mdreal>
Variable::transform() {
  mdreal sznan = medusa::snan();
  mdreal rlnan = medusa::rnan();
  if(ranks.size() > 0) return ranks;
  if(data.size() < 1) return ranks;

  /* Find and sort distinct values. */
  map<mdreal, mdsize> value2rank;
  for(mdsize i = 0; i < data.size(); i++) {
    if(data[i] == rlnan) continue;
    value2rank[data[i]] = sznan;
  }

  /* Assign ranks. */
  (this->sorted).clear();
  for(map<mdreal, mdsize>::iterator pos = value2rank.begin();
      pos != value2rank.end(); pos++) {
    pos->second = sorted.size();
    sorted.push_back(pos->first);
  }

  /* Convert values to ranks. */
  this->ranks = data;
  for(mdsize i = 0; i < data.size(); i++) {
    if(data[i] == rlnan) continue;
    ranks[i] = value2rank[data[i]];
  }
  return ranks;
}
