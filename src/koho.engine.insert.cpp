/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
string
Engine::insert(const string& key, const mdreal z,
	       const vector<mdreal>& vals) {
  EngineBuffer* p = (EngineBuffer*)buffer;
  mdreal rlnan = medusa::rnan();

  /* Reset buffer state. */
  p->nloci = 0;

  /* Check data values. */
  mdsize ndim = vals.size();
  for(mdsize j = 0; j < ndim; j++)
    if(vals[j] == rlnan) return "Unusable value.";

  /* Allocate data matrix. */
  vector<Variable>& vars = p->variables;
  if(vars.size() < 1) vars.resize(ndim);
  if(vars.size() != ndim) return "Incompatible data point.";

  /* Create a new data point. */
  string err = p->insert(key, z);
  if(err.size() > 0) return err;

  /* Add data values. */
  for(mdsize j = 0; j < ndim; j++)
    vars[j].append(vals[j]);
  return "";
}
