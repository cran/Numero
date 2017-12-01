/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"
#include <Rcpp.h>

/*
 *
 */
vector<mdsize>
abacus::shuffle(const mdsize n, const bool flag) {

  /* Allocate output. */
  vector<mdsize> x; x.reserve(n);
  for(mdsize i = 0; i < n; i++)
    x.push_back(i);

  /* Generate random numbers. */
  Rcpp::NumericVector r = Rcpp::runif(n);

  /* Bootstrap sample. */
  if(flag) {
    for(mdsize i = 0; i < n; i++) {
      mdsize ind = (mdsize)(r[i]*n);
      x[i] = ind%n;
    }
    return x;
  }

  /* Permutation. */
  for(mdsize i = 0; i < n; i++) {
    mdsize ind = (mdsize)(r[i]*n); ind = ind%n;
    mdsize tmp = x[i];
    x[i] = x[ind];
    x[ind] = tmp;
  }
  return x;
}
