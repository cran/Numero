/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
void
ModelBuffer::calcdist(const vector<vector<mdreal> >& mtx,
		      const vector<mdreal>& x, const mdreal w) {
  mdreal rlnan = medusa::rnan();
  if(w == 0.0) return;

  /* Check inputs. */
  mdsize ndim = x.size();
  mdsize nrows = mtx.size();
  if(w < 0.0) panic("Bad input.", __FILE__, __LINE__);
  if(w == rlnan) panic("Bad input.", __FILE__, __LINE__);
  if(dsums.size() != nrows) panic("Bad input.", __FILE__, __LINE__);
  if(wsums.size() != nrows) panic("Bad input.", __FILE__, __LINE__);

  /* Update running sums. */
  for(mdsize i = 0; i < nrows; i++) {
    const vector<mdreal>& y = mtx[i];
    for(mdsize j = 0; j < ndim; j++) {
      if(x[j] == rlnan) continue;
      if(y[j] == rlnan) continue;
      double d = (y[j] - x[j]);
      dsums[i] += w*d*d;
      wsums[i] += w;
    }
  }
}
