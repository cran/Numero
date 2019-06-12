/* Created by Ville-Petteri Makinen 2019
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
vector<mdsize>
find_nearest(const vector<vector<mdreal> >& book,
	     const vector<mdreal>& x, const mdsize limit) {
  mdsize i, j;
  mdsize n_dim = x.size();
  mdsize n_sampl = book.size();
  mdreal rlnan = medusa::rnan();
  mt19937 twister;

  /* Check if missing values. */
  mdsize n_val = 0;
  for(j = 0; j < n_dim; j++) {
    if(x[j] == rlnan) continue;
    n_val++;
  }
  if(n_val == n_dim) return vector<mdsize>();
  if(n_val == 0) return vector<mdsize>();

  /* Allocate space. */
  mdsize n_elem = n_sampl;
  if(n_elem > limit) n_elem = limit;
  vector<mdreal> delta(n_elem, rlnan);
  vector<mdsize> neighbors(n_elem);

  /* Set increment for random sampling. */
  mdsize incr = n_sampl/n_elem;
  if(n_sampl > n_elem) incr++;

  /* Draw neighbor distances. */
  mdsize n = 0;
  for(i = 0; i < n_sampl; i++) {
    i += twister()%incr;
    if(i >= n_sampl) break;
    if(n >= n_elem) {
      n_elem += (limit + 1);
      delta.resize(n_elem, rlnan);
      neighbors.resize(n_elem);
    }

    double dsum = 0.0;
    mdsize counter = 0;
    const vector<mdreal>& page = book[i];
    for(j = 0; j < n_dim; j++) {
      if(x[j] == rlnan) continue;
      if(page[j] == rlnan) continue;
      double d = (x[j] - page[j]);
      dsum += d*d;
      counter++;
    }
    if(counter < 1) continue;
    delta[n] = dsum/counter;
    neighbors[n] = i; 
    n++;
  }

  /* Trim arrays. */
  delta.resize(n);
  neighbors.resize(n);

  /* Sort neighbors. */
  vector<mdsize> order = medusa::sortreal(delta, 1);
  vector<mdsize> indices(n);
  for(i = 0; i < n; i++)
    indices[i] = neighbors[order[i]];
  return indices;
}

/*
 *
 */
bool
merge(vector<mdreal>& x, const vector<mdreal>& t) {
  mdsize j;
  mdsize n = 0;
  mdsize n_dim = x.size();
  mdreal rlnan = medusa::rnan();
  for(j = 0; j < n_dim; j++) {
    if(x[j] != rlnan) continue;
    x[j] = t[j];
    n++;
  }
  return (n == 0);
}

/*
 *
 */
vector<vector<mdreal> >
impute(const vector<vector<mdreal> >& data, const mdsize n_sub) {
  mdsize i, k;
  mdsize n_sampl = data.size();
  vector<vector<mdreal> > samples = data;

  /* Fill in missing values. */
  for(i = 0; i < n_sampl; i++) {
    vector<mdreal>& x = samples[i];
    vector<mdsize> neigh = find_nearest(data, x, n_sub);
    for(k = 0; k < neigh.size(); k++) {
      mdsize ind = neigh[k];
      if(merge(x, data[ind])) break;
    }
  }
  return samples;
}

/*
 *
 */
RcppExport SEXP
nro_impute(SEXP xdata_R, SEXP nsub_R) {
  mdsize nsub = as<mdsize>(nsub_R);
  
  /* Check data. */
  vector<vector<mdreal> > xvect = nro::matrix2reals(xdata_R, 0.0);
  if(xvect.size() < 1) return CharacterVector("Empty input.");

  /* Nearest-neighbor Euclidean imputation. */
  xvect = impute(xvect, nsub);
  
  /* Return results. */
  NumericMatrix output = reals2matrix(xvect);
  return output;
}
