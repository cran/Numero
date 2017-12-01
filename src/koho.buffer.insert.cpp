/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
string
Buffer::insert(const string& key, const mdreal z) {
  mdsize sznan = medusa::snan();
  mdreal rlnan = medusa::rnan();
  
  /* Check inputs. */
  if(key.size() < 1) return "Empty identity.";
  if(z == rlnan) return "Unusable level.";

  /* Find the sample or create a new one. */
  map<string, mdsize>::iterator pos;
  if((pos = key2rank.find(key)) == key2rank.end()) {
    key2rank[key] = samples.size();
    pos = key2rank.find(key);
    (this->samples).push_back(Sample());
  }

  /* Update sample. */
  mdsize npoints = points.size();
  Sample& sampl = samples[pos->second];
  sampl.members.push_back(npoints);

  /* Create a new data point. */
  Point pnt; pnt.parent = pos->second;
  pnt.layer = structure.stratify(z);
  if(pnt.layer.bounds.first == sznan) return "Unusable level.";
  if(pnt.layer.bounds.second == sznan) return "Unusable level.";
  (this->points).push_back(pnt);
  return "";
}
