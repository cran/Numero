/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
vector<vector<mdreal> >
koho::operate(const vector<vector<mdreal> >& xdata, const char op,
	      const vector<vector<mdreal> >& ydata) {
  mdreal rlnan = medusa::rnan();
  vector<vector<mdreal> > zdata;

  /* Check inputs. */
  if(xdata.size() != ydata.size()) return zdata;
  for(mdsize i = 0; i < xdata.size(); i++)
    if(xdata[i].size() != ydata[i].size()) return zdata;
  
  /* Operate on values. */
  zdata.resize(xdata.size());
  for(mdsize i = 0; i < xdata.size(); i++) {
    const vector<mdreal>& x = xdata[i];
    const vector<mdreal>& y = ydata[i];

    /* Allocate memory. */
    vector<mdreal>& z = zdata[i];
    mdsize nelem = x.size();
    z.resize(nelem, rlnan);

    /* Process array. */
    for(mdsize j = 0; j < nelem; j++) {
      mdreal xj = x[j];
      mdreal yj = y[j];
      if(xj == rlnan) continue;
      if(yj == rlnan) continue;
      switch(op) {
      case '+':
	z[j] = (xj + yj);
	break;
      case '-':
	z[j] = (xj - yj);
	break;
      case '*':
	z[j] = xj*yj;
	break;
      case '/':
	if(yj != 0.0) z[j] = xj/yj;
	break;
      default:
	panic("Unknown operator.", __FILE__, __LINE__);
      }
    }
  }
  return zdata;
}
