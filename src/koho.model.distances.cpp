/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
vector<mdreal>
Model::distances(const string& key) const { 
  ModelBuffer* p = (ModelBuffer*)buffer;

  /* Find the data point. */
  unordered_map<string, Point>::const_iterator pos;
  const unordered_map<string, Point>& points = p->points;
  if((pos = points.find(key)) == points.end())
    return vector<mdreal>();

  /* Check if a trainer exists. */
  if((p->trainer).size() < 1)
    p->trainer = Trainer(p->codebook, p->topology, 0, 0.0);
  
  /* Estimate distances. */
  return (p->trainer).distances(pos->second);
}
