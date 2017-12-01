/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
class Group {
public:
  vector<mdsize> keys;
  vector<mdreal> values;
public:
  void add(const mdsize key, const mdreal value) {
    keys.push_back(key);
    values.push_back(value);
  };
};

static vector<mdreal> calc_quantiles(const vector<mdreal>&,
				     const vector<mdreal>&);

/*
 *
 */
vector<mdreal>
abacus::destratify(const vector<mdreal>& x, const vector<mdsize>& g) {
  mdreal rl_nan = medusa::rnan();

  /* Check inputs. */
  if(x.size() != g.size())
    panic("Incompatible arguments.", __FILE__, __LINE__);

  /* Divide values into groups. */
  map<mdsize, Group> groups;
  for(mdsize i = 0; i < x.size(); i++) {
    if(x[i] == rl_nan) continue;
    groups[g[i]].add(i, x[i]);
  }
  if(groups.size() < 2) return x;

  /* Collect background. */
  vector<mdreal> y;
  for(mdsize i = 0; i < x.size(); i++) {
    if(x[i] == rl_nan) continue;
    y.push_back(x[i]);
  }
  if(y.size() < 2) return x;

  /* Sort background. */
  sort(y.begin(), y.end());

  /* Convert group values to global values. */
  map<mdsize, Group>::iterator pos;
  for(pos = groups.begin(); pos != groups.end(); pos++) {
    vector<mdreal> tmp = transform((pos->second).values, "uniform");
    (pos->second).values = calc_quantiles(y, tmp);
  }

  /* Collect results. */
  y = x;
  for(pos = groups.begin(); pos != groups.end(); pos++) {
    vector<mdreal>& values  = (pos->second).values;
    vector<mdsize>& keys = (pos->second).keys;
    for(mdsize k = 0; k < keys.size(); k++)
      y[keys[k]] = values[k];
  }
  return y;
}

/*
 * X must be sorted and must contain only valid values.
 */
vector<mdreal>
calc_quantiles(const vector<mdreal>& x, const vector<mdreal>& q) {
  mdsize n = x.size();
  vector<mdreal> y = q;
  mdreal rl_nan = medusa::rnan();
  if(n < 2) panic("Invalid input.", __FILE__, __LINE__);
  for(mdsize i = 0; i < q.size(); i++) {
    double qi = q[i];
    if(qi == rl_nan) continue;
    if(qi < 0.0) panic("Invalid quantile.", __FILE__, __LINE__);
    if(qi > 1.0) panic("Invalid quantile.", __FILE__, __LINE__);
    
    /* Determine quantile indices. */
    mdsize a = (mdsize)(qi*(n - 1));
    mdsize b = (a + 1);
    if(b >= n) b = (n - 1);
    if(a == b) {
      y[i] = x[a];
      continue;
    }
    
    /* Interpolate between indices. */
    double d = (qi*(n - 1) - a);
    y[i] = ((1.0 - d)*x[a] + d*x[b]);
  }
  return y;
}
