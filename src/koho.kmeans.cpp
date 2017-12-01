
/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

static vector<mdreal> calc_mean(const vector<vector<mdreal> >&,
				const vector<mdsize>&);
static vector<mdreal> calc_distances(const vector<vector<mdreal> >&,
				     const vector<mdreal>&);

/*
 *
 */
vector<vector<mdreal> >
koho::kmeans(vector<mdsize>& labels, vector<mdreal>& history,
	     const vector<vector<mdreal> >& data,
	     const mdsize kmax) {
  mdsize sznan = medusa::snan();
  mdreal rlnan = medusa::rnan();

  /* Reset outputs. */
  vector<vector<mdreal> > centroids;
  labels.clear();
  history.clear();

  /* Check inputs. */
  mdsize nrows = data.size();
  if(nrows < 1) return centroids;
  if(kmax == sznan) return centroids;

  /* Set initial cluster memberships. */
  map<mdsize, vector<mdsize> >::iterator pos;
  map<mdsize, vector<mdsize> > subsets;
  for(mdsize i = 0; i < nrows; i++)
    subsets[i%kmax].push_back(i);

  /* Iteration loop. */
  centroids.resize(subsets.size());
  while(abacus::convergence(history, 0.01) == false) {

    /* Estimate centroids. */
    for(pos = subsets.begin(); pos != subsets.end(); pos++)
      centroids[pos->first] = calc_mean(data, pos->second);
    subsets.clear();

    /* Find best-matching centroids. */
    double dsum = 0.0; mdsize nsum = 0;
    for(mdsize i = 0; i < nrows; i++) {
      const vector<mdreal>& x = data[i];
      vector<mdreal> delta = calc_distances(centroids, x);
      mdsize bmu = abacus::extrema(delta).first;
      if(bmu == sznan) continue;
      subsets[bmu].push_back(i);
      dsum += delta[bmu];
      nsum += 1;
    }

    /* Global error. */
    dsum /= (nsum + 1e-6);
    history.push_back(dsum);
  }
  
  /* Collect cluster labels. */
  labels.resize(nrows, rlnan);
  for(pos = subsets.begin(); pos != subsets.end(); pos++) {
    vector<mdsize>& mask = pos->second;
    for(mdsize k = 0; k < mask.size(); k++)
      labels[mask[k]] = pos->first;
  }
  return centroids;
}

/*
 *
 */
vector<mdreal>
calc_mean(const vector<vector<mdreal> >& vectors,
	  const vector<mdsize>& mask) {
  mdreal rlnan = medusa::rnan();
  mdsize nvect = vectors.size();
  if(nvect < 1) panic("No data.", __FILE__, __LINE__);

  /* Calculate base statistics. */
  vector<mdreal> sums;
  vector<mdsize> nums;
  vector<mdsize>::const_iterator pos;
  for(pos = mask.begin(); pos != mask.end(); pos++) {
    if(*pos >= nvect) panic("Bad index.", __FILE__, __LINE__);

    /* Check capacity. */
    const vector<mdreal>& x = vectors[*pos];
    mdsize ndim = x.size();
    if(sums.size() < ndim) {
      sums.resize(ndim, 0.0);
      nums.resize(ndim, 0);
    }

    /* Update statistics. */
    for(mdsize j = 0; j < ndim; j++) {
      if(x[j] == rlnan) continue;
      sums[j] += x[j];
      nums[j] += 1;
    }
  }

  /* Estimate mean values. */
  vector<mdreal> mu(sums.size(), rlnan);
  for(mdsize j = 0; j < sums.size(); j++)
    if(nums[j] > 0) mu[j] = sums[j]/nums[j];
  return mu;
}

/*
 *
 */
vector<mdreal>
calc_distances(const vector<vector<mdreal> >& vectors,
	       const vector<mdreal>& values) {
  mdreal rlnan = medusa::rnan();
  mdsize nvect = vectors.size();
  mdsize ndim = values.size();
  vector<mdreal> delta(nvect, rlnan);
  for(mdsize i = 0; i < nvect; i++) {
    const vector<mdreal>& x = vectors[i];
    mdsize n = x.size();
    if(n > ndim) n = ndim;
    long double dsum = 0.0;
    long double wsum = 0.0;
    for(mdsize j = 0; j < ndim; j++) {
      if(values[j] == rlnan) continue;
      if(x[j] == rlnan) continue;
      double d = (x[j] - values[j]);
      dsum += d*d;
      wsum += 1.0;
    }
    if(wsum <= 0.0) continue;
    delta[i] = sqrt(dsum)/sqrt(wsum);
  }
  return delta;
}
