/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "koho.local.h"

/*
 *
 */
Transformation::Transformation() {}

/*
 *
 */
Transformation::Transformation(const vector<mdreal>& values) {
  
  /* Find distinct values and sort them. */
  this->lookup = values;
  uniqreal(this->lookup);
  sortreal(this->lookup, 1);
  
  /* Convert to ranks. */
  for(mdsize i = 0; i < lookup.size(); i++)
    (this->output).push_back(i);
}

/*
 *
 */
Transformation::~Transformation() {}

/*
 *
 */
void
Transformation::process(vector<mdreal>& array,
		  const vector<mdreal>& valsFrom,
		  const vector<mdreal>& valsTo) const {
  mdreal rlnan = medusa::rnan();

  /* Check if transform possible. */
  mdsize nelem = valsFrom.size();
  if(nelem < 1) {
    array = vector<mdreal>(array.size(), medusa::rnan());
    return;
  }
  if(nelem < 2) {
    array = vector<mdreal>(array.size(), valsTo[0]);
    return;
  }
  
  /* Set pivot points. */
  pair<mdreal, mdreal> vmin(valsFrom[0], valsFrom[1]);
  pair<mdreal, mdreal> vmax(valsFrom[nelem-2], valsFrom[nelem-1]);
  pair<mdreal, mdreal> rmin(valsTo[0], valsTo[1]);
  pair<mdreal, mdreal> rmax(valsTo[nelem-2], valsTo[nelem-1]);

  /* Transform values. */
  for(mdsize i = 0; i < array.size(); i++) {
    mdreal value = array[i];
    if(value == rlnan) continue;

    /* Exact matches to edges. */
    if(value == vmin.first) {
      array[i] = rmin.first;
      continue;
    }
    if(value == vmax.second) {
      array[i] = rmax.second;
      continue;
    }
    
    /* Extrapolate small value. */
    if(value < vmin.first) {
      mdreal ratio = (value - vmin.second)/(vmin.first - vmin.second);
      mdreal delta = ratio*(rmin.first - rmin.second);
      array[i] = (rmin.second + delta);
      continue;
    }
    
    /* Extrapolate large value. */
    if(value > vmax.second) {
      mdreal ratio = (value - vmax.first)/(vmax.second - vmax.first);
      mdreal delta = ratio*(rmax.second - rmax.first);
      array[i] = (rmax.first + delta);
      continue;
    }

    /* Find the segment in the lookup table. */
    Site locus = medusa::binsearch(valsFrom, value);
    mdsize a = locus.bounds.first;
    mdsize b = locus.bounds.second;
    
    if((a >= nelem) || (b >= nelem)) {
      array[i] = rlnan;
      continue;
    }

    /* Interpolate an intermediate value. */
    mdreal wA = locus.weights.first;
    mdreal wB = locus.weights.second;
    array[i] = (wA*(valsTo[a]) + wB*(valsTo[b]));
  }
}

/*
 *
 */
void
Transformation::restore(vector<mdreal>& array) const {
  this->process(array, output, lookup);
}

/*
 *
 */
void
Transformation::transform(vector<mdreal>& array) const {
  this->process(array, lookup, output);
}
