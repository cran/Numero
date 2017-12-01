/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
string
Model::insert(const string& key, const mdreal z,
	      const vector<mdreal>& values) {
  ModelBuffer* p = (ModelBuffer*)buffer;
  mdsize sznan = medusa::snan();
  mdreal rlnan = medusa::rnan();
  
  /* Check if any usable values. */
  mdsize nvalid = 0;
  mdsize ndim = values.size();
  for(mdsize j = 0; j < ndim; j++)
    if(values[j] != rlnan) nvalid++;
  if(nvalid < 1) return "No usable data.";

  /* Check number of dimensions. */
  if(p->nvars < 1) p->nvars = ndim;
  if(p->nvars != ndim) return "Incompatible input.";

  /* Create a data point. */
  string err = p->insert(key, z);
  if(err.size() > 0) return err;

  /* Update data matrix. */
  mdsize ndata = (p->data).size();
  for(mdsize j = 0; j < ndim; j++)
    (p->data).insert(ndata, j, values[j]);

  /* Allocate codebook by triggering a configuration. */
  if((p->codebook).size() > 0) return "";
  this->configure(sznan, sznan, values);
  return "";
}
