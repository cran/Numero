/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

string
Model::insert(const string& key, const vector<mdreal>& values) {
  ModelBuffer* p = (ModelBuffer*)buffer;
  mdreal rlnan = medusa::rnan();
  if(key.size() < 1) return "Empty identity.";
  
  /* Check if any usable values. */
  mdsize nvalid = 0;
  for(mdsize j = 0; j < values.size(); j++)
    if(values[j] != rlnan) nvalid++;
  if(nvalid < 1) return "Empty data point.";

  /* Insert a new data point. */
  mdsize rank = (p->points).size();
  p->points[key] = Point(rank, values);
  return "";
}
