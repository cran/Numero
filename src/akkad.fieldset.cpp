/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "akkad.local.h"

/*
 *
 */
vector<string>
FieldSet::operator[](const mdsize ind) const {
  if(ind >= data.size()) return vector<string>(2);
  return data[ind];
}

/*
 *
 */
bool
FieldSet::insert(const vector<string>& array) {

  /* Convert to single string. */
  string entry;
  for(mdsize j = 1; j < array.size(); j++)
    entry += (array[j] + "\t");

  /* Check if already inserted. */
  map<string, mdsize>::iterator pos;
  pos = string2rank.find(entry);
  if(pos != string2rank.end()) return false;

  /* Update database. */
  string2rank[entry] = data.size();
  data.push_back(array);
  return true;
}
