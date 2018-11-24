/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "akkad.local.h"

/*
 *
 */
string
Scribe::find(vector<mdsize>& mask, const string& instr,
	     const vector<string>& words) const {
  mask.clear();

  /* Collect target words. */
  map<string, bool> targets;
  for(mdsize k = 0; k < this->size(); k++) {
    vector<string> fields = this->get(instr, k);
    if(fields[0] == "") break;
    if(fields[1] == "") continue;
    targets[fields[1]] = false;
  }

  /* Collect column indices. */
  vector<mdsize> array;
  for(mdsize j = 0; j < words.size(); j++) {
    const string& key = words[j];
    if(targets.count(key) < 1) continue;
    if(targets[key]) return ("Duplicate word '" + key + "'.");
    targets[key] = true;
    array.push_back(j);
  }

  /* Return results. */
  mask = array;
  return "";
}
