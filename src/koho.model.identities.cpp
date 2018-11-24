/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
vector<string>
Model::identities() const {
  ModelBuffer* p = (ModelBuffer*)buffer;
  vector<string> array;
  unordered_map<string, Point>& points = p->points;
  unordered_map<string, Point>::const_iterator it;
  for(it = points.begin(); it != points.end(); it++)
    array.push_back(it->first);
  return array;
}
