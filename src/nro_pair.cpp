/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
class ElementCompare {
public:
  bool operator()(const abacus::Element& x,
		  const abacus::Element& y) const {
    return (x.value < y.value);
  };
};

/*
 *
 */
static mdreal
calc_euclidean(const vector<mdreal>& vA, const vector<mdreal>& vB) {
  mdreal rlnan = medusa::rnan();

  /* Check size. */
  mdsize ndim = vB.size();
  if(ndim > vA.size()) ndim = vA.size();
  
  /* Calculate running sums. */
  long double dsum = 0.0;
  long double wsum = 0.0;
  for(mdsize j = 0; j < ndim; j++) {
    if(vA[j] == rlnan) continue;
    if(vB[j] == rlnan) continue;
    mdreal d = (vB[j] - vA[j]);
    dsum += d*d;
    wsum += 1.0;
  }
  if(wsum <= 0.0) return rlnan;
  return sqrt(dsum)/(wsum + 1e-9);
}

/*
 *
 */
RcppExport SEXP
nro_pair(SEXP xdata_R, SEXP ydata_R) {
  mdreal rlnan = medusa::rnan();
  
  /* Check data. */
  vector<vector<mdreal> > xvect = nro::matrix2reals(xdata_R, 0.0);
  vector<vector<mdreal> > yvect = nro::matrix2reals(ydata_R, 0.0);
  if(xvect.size() < 1) return CharacterVector("Empty input.");
  if(yvect.size() < 1) return CharacterVector("Empty input.");

  /* Evaluate distances between vectors. */
  vector<abacus::Element> pairs;
  for(mdsize i = 0; i < xvect.size(); i++) {
    for(mdsize j = 0; j < yvect.size(); j++) {
      mdreal d = calc_euclidean(xvect[i], yvect[j]);
      if(d == rlnan) continue;
      Element e; e.row = i; e.column = j; e.value = d;
      pairs.push_back(e);
    }
  }
  
  /* Sort by distance. */
  std::sort(pairs.begin(), pairs.end(), ElementCompare());
  
  /* Collect best matches. */
  vector<mdsize> xmask;
  vector<mdsize> ymask;
  vector<mdreal> delta;
  vector<bool> xflags(xvect.size(), false);
  vector<bool> yflags(yvect.size(), false);
  for(mdsize k = 0; k < pairs.size(); k++) {
    mdsize i = pairs[k].row;
    mdsize j = pairs[k].column;
    if(xflags[i]) continue;
    if(yflags[j]) continue;
    xflags[i] = true;
    yflags[j] = true;
    xmask.push_back(i);
    ymask.push_back(j);
    delta.push_back(pairs[k].value);
  }
  
  /* Convert to R indexing. */
  for(mdsize i = 0; i < xmask.size(); i++) {
    xmask[i] += 1;
    ymask[i] += 1;
  }

  /* Return results. */
  List res;
  res.push_back(xmask, "ROW.x");
  res.push_back(ymask, "ROW.y");
  res.push_back(delta, "DISTANCE");
  return res;
}
