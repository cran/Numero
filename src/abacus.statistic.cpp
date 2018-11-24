/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

using namespace std;
using namespace abacus;

static mdreal stat_center(const vector<mdreal>&, const vector<mdreal>&);
static mdreal stat_extreme(const vector<mdreal>&, const int);
static mdreal stat_mean(const vector<mdreal>&, const vector<mdreal>&);
static mdreal stat_mode(const vector<mdreal>&, const vector<mdreal>&);
static mdreal stat_variance(const vector<mdreal>&, const vector<mdreal>&,
			    const mdreal);

/*
 *
 */
mdreal
abacus::statistic(const vector<mdreal>& data,
		  const vector<mdreal>& weights,
		  const string& name) {
  mdsize nelem = data.size();
  mdreal rlnan = medusa::rnan();

  /* Check weights. */
  vector<mdreal> w = weights;
  if(w.size() != nelem)
    panic("Incompatible inputs.", __FILE__, __LINE__);

  /* Remove unusable data. */
  mdsize nvalid = 0;
  vector<mdreal> x = data;
  for(mdsize i = 0; i < nelem; i++) {
    if(x[i] == rlnan) continue;
    if(w[i] == rlnan) continue;
    if(w[i] <= 0.0) continue;
    x[nvalid] = x[i];
    w[nvalid] = w[i];
    nvalid++;
  }

  /* Return the number of valid elements. */
  if(name == "number") return nvalid;

  /* Trim arrays. */
  x.resize(nvalid);
  w.resize(nvalid);

  /* Estimate statistic. */
  if(nvalid < 1) return rlnan;
  if(name == "min") return stat_extreme(x, -1);
  if(name == "max") return stat_extreme(x, 1);
  if(name == "center") return stat_center(x, w);
  if(name == "mean") return stat_mean(x, w);
  if(name == "median") return abacus::quantile(x, w, 0.5);
  if(name == "mode") return stat_mode(x, w);

  /* Estimate variance statistic. */
  if(nvalid < 2) return rlnan;
  if(name == "range") return stat_extreme(x, 0);
  if(name == "sd") return stat_variance(x, w, 0.5);
  if(name == "var") return stat_variance(x, w, 1.0);
  panic(("Unknown estimator '" + name + "'.\n"), __FILE__, __LINE__);
  return rlnan;
}

/*
 *
 */
mdreal
abacus::statistic(const vector<mdreal>& data0, const string& name0) {
  vector<mdreal> weights0(data0.size(), 1.0);
  return statistic(data0, weights0, name0);
}

/*
 *
 */
mdreal
stat_center(const vector<mdreal>& x, const vector<mdreal>& w) {
  mdsize nelem = x.size();
  if(nelem < 1) panic("No data.", __FILE__, __LINE__);

  /* Count distinct values. */
  unordered_set<mdreal> valset;
  for(mdsize i = 0; i < nelem; i++)
    valset.insert(x[i]);
  mdreal nvals = valset.size();

  /* Combine mean and median. */
  mdreal mu = stat_mean(x, w);
  mdreal med = abacus::quantile(x, w, 0.5);
  mdreal rho = 2.0/nvals/log(nvals + 1.0);
  if(rho > 1.0) rho = 1.0;
  return (rho*mu + (1.0 - rho)*med);
}

/*
 *
 */
mdreal
stat_extreme(const vector<mdreal>& x, const int alpha) {
  mdsize nelem = x.size();
  if(nelem < 1) panic("No data.", __FILE__, __LINE__);

  /* Find maximum and minimum. */
  mdreal xmin = x[0];
  mdreal xmax = x[0];
  for(mdsize i = 1; i < nelem; i++) {
    if(x[i] < xmin) xmin = x[i];
    if(x[i] > xmax) xmax = x[i];
  }

  /* Return results. */
  if(alpha < 0) return xmin;
  if(alpha > 0) return xmax;
  return (xmax - xmin);
}

/*
 *
 */
mdreal
stat_mean(const vector<mdreal>& x, const vector<mdreal>& w) {
  mdsize nelem = x.size();
  if(nelem < 1) panic("No data.", __FILE__, __LINE__);
  long double xsum = 0.0;
  long double wsum = 0.0;
  for(mdsize i = 0; i < nelem; i++) {
    xsum += (w[i])*(x[i]);
    wsum += w[i];
  }
  return (xsum/wsum);
}

/*
 *
 */
mdreal
stat_mode(const vector<mdreal>& x, const vector<mdreal>& w) {
  mdsize nelem = x.size();
  if(nelem < 1) panic("No data.", __FILE__, __LINE__);

  /* Estimate basic statistics. */
  mdreal xmu = stat_center(x, w);
  mdreal xmin = stat_extreme(x, -1);
  mdreal xrange = stat_extreme(x, 0);

  /* Determine sampling points. */
  vector<mdreal> q;
  mdsize nbins = (mdsize)(sqrt(nelem) + 0.5);
  for(mdsize i = 0; i < nbins; i++)
    q.push_back(xmin + i/(nbins - 1.0)*xrange);

  /* Estimate highest histogram bin. */
  vector<mdreal> h = abacus::histogram(x, w, q);
  mdsize pos = abacus::extrema(h).second;
  if(pos >= nbins) return xmu;

  /* Estimate mode. */
  mdreal rho = (nbins - 1.0)/nbins; rho = rho*rho;
  return (rho*(q[pos]) + (1.0 - rho)*xmu);
}

/*
 *
 */
mdreal
stat_variance(const vector<mdreal>& x, const vector<mdreal>& w,
	      const mdreal expon) {
  mdsize nelem = x.size();
  if(nelem < 2) panic("Not enough data.", __FILE__, __LINE__);

  /* Calculate first and second moments. */
  long double xsum1 = 0.0;
  long double xsum2 = 0.0;
  long double wsum = 0.0;
  for(mdsize i = 0; i < nelem; i++) {
    xsum1 += (w[i])*(x[i]);
    xsum2 += (w[i])*(x[i])*(x[i]);
    wsum += w[i];
  }

  /* Normalize moments. */
  xsum1 = (xsum1/wsum)*nelem;
  xsum2 = (xsum2/wsum)*nelem;
  xsum1 = (xsum1/nelem)*(xsum1/(nelem - 1));
  xsum2 /= (nelem - 1);

  /* Calculate variance. */
  if(xsum2 < xsum1) return 0.0;
  return pow((double)(xsum2 - xsum1), (double)expon);
}
