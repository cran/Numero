/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "akkad.local.h"

/*
 *
 */
string
Scribe::contents() const {
  InstrMap* p = (InstrMap*)(this->buffer);
  string array;
  InstrMap::iterator pos;
  for(pos = p->begin(); pos != p->end(); pos++) {
    FieldSet& entries = pos->second;
    for(mdsize k = 0; k < entries.size(); k++) {
      vector<string> fields = entries[k];
      for(mdsize j = 0; j < fields.size(); j++) {
	if(j < 1) array.append(fields[j]);
	else array.append("\t" + fields[j]);
      }
      array.push_back('\n');
    }
  }
  return array;
}
