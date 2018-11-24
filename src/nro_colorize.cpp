/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "nro.h"

#define NCOLORS 512

/*
 *
 */
RcppExport SEXP
nro_colorize(SEXP name_R) {
  vector<string> array;
  string name = as<string>(name_R);
  for(mdsize i = 0; i < NCOLORS; i++) {
    Color c = scriptum::colormap(i/(NCOLORS - 1.0), name);
    array.push_back("#" + c.hex());
  }
  return wrap(array);
}
