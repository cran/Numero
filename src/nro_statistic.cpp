/* Created by Ville-Petteri Makinen 2019
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
RcppExport SEXP
nro_statistic(SEXP data_R, SEXP weights_R, SEXP method_R) {
  mdreal rlnan = medusa::rnan();
  string method = as<string>(method_R);
  vector<mdreal> output;

  /* Check inputs. */
  NumericMatrix data(data_R);
  NumericMatrix weights(weights_R);
  if(data.nrow() != weights.nrow())
    return CharacterVector("Incompatible inputs.");
  if(data.ncol() != weights.ncol())
    return CharacterVector("Incompatible inputs.");
  
  /* Process columns. */
  for(int j = 0; j < data.ncol(); j++) {
    
    /* Check for non-finite values. */
    NumericVector dataj = data(_, j);
    NumericVector weightsj = weights(_, j);
    LogicalVector dflags = Rcpp::is_finite(dataj);
    LogicalVector wflags = Rcpp::is_finite(weightsj);
	  
    /* Copy column and weight vectors. */
    vector<mdreal> x(dataj.size(), rlnan);
    vector<mdreal> w(dataj.size(), rlnan);
    for(mdsize i = 0; i < x.size(); i++) {
      if(!dflags[i]) continue;
      if(!wflags[i]) continue;
      x[i] = dataj[i];
      w[i] = weightsj[i];
    }

    /* Estimate center. */
    mdreal mu = abacus::statistic(x, w, method);
    output.push_back(mu);
  }
  
  /* Return results. */
  return nro::reals2vector(output);
}
