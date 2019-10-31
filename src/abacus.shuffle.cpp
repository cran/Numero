/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
vector<mdsize>
abacus::shuffle(const mdsize n, const bool flag) {
  static mt19937 twister;
  
  /* Allocate output. */
  vector<mdsize> x; x.reserve(n);
  for(mdsize i = 0; i < n; i++)
    x.push_back(i);

  /* Bootstrap sample. */
  if(flag) {
    for(mdsize i = 0; i < n; i++)
      x[i] = twister()%n;
    return x;
  }

  /* Permutation. */
  for(mdsize i = 0; i < n; i++) {
    mdsize ind = twister()%n;
    mdsize tmp = x[i];
    x[i] = x[ind];
    x[ind] = tmp;
  }
  return x;
}
