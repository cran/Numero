/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
string
Engine::insert(const string& key, const mdsize unit,
	       const vector<mdreal>& values) {
  EngineBuffer* p = (EngineBuffer*)buffer;
  mdreal rlnan = medusa::rnan();
  
  /* Check inputs. */
  if(key.size() < 1) return "Empty identity.";
  if(unit >= (p->topology).size()) return "Unusable map unit.";

  /* Check if any unusable values. */
  for(mdsize j = 0; j < values.size(); j++)
    if(values[j] == rlnan) return "Unusable data value.";
  if(values.size() < 1) return "No usable data.";

  /* Check that dimensions match. */
  mdsize ncols = (p->data).order();
  if(ncols < 1) ncols = values.size();
  if(values.size() != ncols) return "Incompatible input.";
			       
  /* Insert a new data point. */
  mdsize rank = (p->points).size();
  p->points[key] = Point(rank, unit);

  /* Update data matrix. */
  for(mdsize j = 0; j < values.size(); j++)
    (p->data).insert(rank, j, values[j]);
  
  /* Reset engine state. */
  (p->cache).clear();
  return "";
}

