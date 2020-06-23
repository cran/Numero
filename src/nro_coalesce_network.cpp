/* Created by Ville-Petteri Makinen 2019
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
pair<mdreal, mdsize>
fast_corr(const mdreal* x, const mdreal* y, const mdsize n) {
  mdreal rlnan = medusa::rnan();

  /* Summary statistics. */
  mdsize nsum = 0;
  double sx = 0.0;
  double sy = 0.0;
  double sxx = 0.0;
  double syy = 0.0;
  double sxy = 0.0;
  for(mdsize i = 0; i < n; i++) {
    mdreal xval = x[i];
    mdreal yval = y[i];
    if(xval == rlnan) continue;
    if(yval == rlnan) continue;
    sx += xval;
    sy += yval;
    sxx += xval*xval;
    sxy += xval*yval;
    syy += yval*yval;
    nsum++;
  }

  /* Calculate coefficient. */
  pair<mdreal, mdsize> res(rlnan, nsum);
  long double vx = sqrt(nsum*sxx - sx*sx);
  long double vy = sqrt(nsum*syy - sy*sy);
  if(vx < 1e-20) return res;
  if(vy < 1e-20) return res;
  res.first = (nsum*sxy - sx*sy)/vx/vy;
  return res;
}

/*
 *
 */
RcppExport SEXP
nro_coalesce_network(SEXP data_R, SEXP vars_R, SEXP cutoff_R) {
  mdreal rlnan = medusa::rnan();
  mdreal cutoff = as<mdreal>(cutoff_R);
  
  /* Check inputs. */
  NumericMatrix data(data_R);
  mdsize nrows = data.nrow();
  mdsize nvars = data.ncol();
  vector<string> vars = as<vector<string> >(vars_R);
  if(vars.size() != nvars) return CharacterVector("Incompatible inputs.");

  /* Allocate memory for data. */
  mdreal* buffer = (mdreal*)malloc(nrows*nvars*sizeof(mdreal));
  
  /* Collect columns. */
  vector<mdreal*> columns;
  for(mdsize j = 0; j < nvars; j++) {
    NumericVector vals = data(_, j);
    LogicalVector flags = Rcpp::is_finite(vals);
    if(flags.size() != nrows)
      medusa::panic("Bad size.", __FILE__, __LINE__);
    mdreal* ptr = (mdreal*)(buffer + j*nrows);
    for(mdsize i = 0; i < nrows; i++)
      if(flags[i]) ptr[i] = vals[i];
      else ptr[i] = rlnan;
    columns.push_back(ptr);
  }
  
  /* Calculate correlation coefficients. */
  vector<string> heads;
  vector<string> tails;
  vector<mdreal> weights;
  vector<mdsize> numbers;
  for(mdsize i = 0; i < nvars; i++) {
    for(mdsize j = (i + 1); j < nvars; j++) {
      pair<mdreal, mdsize> rho;
      rho = fast_corr(columns[i], columns[j], nrows);
      if(rho.first == rlnan) continue;
      if((rho.first)*(rho.first) < cutoff) continue;
      heads.push_back(vars[j]);
      tails.push_back(vars[i]);
      weights.push_back(rho.first);
      numbers.push_back(rho.second);
    }
  }

  /* Release data memory. */
  free(buffer); buffer = NULL;
    
  /* Construct network. */
  turnip::Network graph;
  for(mdsize i = 0; i < tails.size(); i++) {
    mdreal r2 = (weights[i])*(weights[i]);
    graph.add(tails[i], heads[i], r2);
  }
  
  /* Set topology. */
  mdsize nedges = 0;
  for(mdsize i = 0; i < tails.size(); i++) {
    medusa::mdreal w = graph.weight(tails[i], heads[i]);
    if(w == rlnan) continue;
    tails[nedges] = tails[i];
    heads[nedges] = heads[i];
    weights[nedges] = weights[i];
    numbers[nedges] = numbers[i];
    nedges++;
  }
  
  /* Trim vectors. */
  tails.resize(nedges);
  heads.resize(nedges);
  weights.resize(nedges);
  numbers.resize(nedges);
  
  /* Return results. */
  List output;
  output.push_back(heads, "HEAD");
  output.push_back(tails, "TAIL");
  output.push_back(weights, "WEIGHT");
  output.push_back(numbers, "N");
  return output;
}
