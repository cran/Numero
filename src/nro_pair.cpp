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
static void
make_pairs(vector<abacus::Element>& pairs,
	   const vector<vector<mdreal> >& samples,
	   const vector<vector<mdreal> >& pool,
	   const mdsize nsub, const bool flip) {
  mdreal rlnan = medusa::rnan();
  mt19937 twister;

  /* Full sampling mask. */
  vector<mdsize> full;
  mdsize npool = pool.size();
  for(mdsize k = 0; k < npool; k++)
    full.push_back(k);

  /* Find a peer for each sample. */
  vector<mdsize> mask = full;
  for(mdsize i = 0; i < samples.size(); i++) {

    /* Randomized subsampling. */
    if(nsub < npool) {
      for(mdsize k = 0; k < nsub; k++) {
	mdsize pos = twister()%npool;
	mdsize swap = full[k];
	full[k] = full[pos];
	full[pos] = swap;
      }

      /* Re-populate and sort working mask. */
      mask.resize(nsub);
      for(mdsize k = 0; k < nsub; k++)
	mask[k] = full[k];
      std::sort(mask.begin(), mask.end());
    }

    /* Calculate pair-wise distances. */
    Element e;
    for(vector<mdsize>::iterator jt = mask.begin();
	jt != mask.end(); jt++) {
      if(flip) {
	e.row = *jt;
	e.column = i;
      }
      else {
	e.row = i;
	e.column = *jt;
      }
      e.value = calc_euclidean(samples[i], pool[*jt]);
      if(e.value != rlnan) pairs.push_back(e);
    }
  }
}

/*
 *
 */
RcppExport SEXP
nro_pair(SEXP xdata_R, SEXP ydata_R, SEXP nsub_R) {
  mdsize nsub = as<mdsize>(nsub_R);
  
  /* Check data. */
  vector<vector<mdreal> > xvect = nro::matrix2reals(xdata_R, 0.0);
  vector<vector<mdreal> > yvect = nro::matrix2reals(ydata_R, 0.0);
  mdsize nx = xvect.size();
  mdsize ny = yvect.size();
  if(nx < 1) return CharacterVector("Empty input.");
  if(ny < 1) return CharacterVector("Empty input.");

  /* Calculate pair-wise distances. */
  vector<abacus::Element> pairs;
  if((nx <= nsub) && (ny <= nsub))
    make_pairs(pairs, xvect, yvect, nsub, false);
  else {
    if(ny > nsub) make_pairs(pairs, xvect, yvect, nsub, false);
    if(nx > nsub) make_pairs(pairs, yvect, xvect, nsub, true);
  }

  /* Sort by distance. */
  std::sort(pairs.begin(), pairs.end(), ElementCompare());
  
  /* Collect best matches. */
  vector<mdsize> xmask;
  vector<mdsize> ymask;
  vector<mdreal> delta;
  vector<bool> xflags(nx, false);
  vector<bool> yflags(ny, false);
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
