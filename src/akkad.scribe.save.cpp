/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "akkad.local.h"

/*
 *
 */
unsigned long
Scribe::save(const string& fname) const {
  unsigned long n = 0;
  InstrMap* p = (InstrMap*)(this->buffer);

  /* Open file. */
  File f;
  if(f.open(fname, "w") == false) {
    worry(("Cannot open '" + fname + "'."), "");
    return 0;
  }

  /* Save instructions. */
  InstrMap::iterator pos;
  for(pos = p->begin(); pos != p->end(); pos++) {
    FieldSet& entries = pos->second;
    for(mdsize k = 0; k < entries.size(); k++) {
      vector<string> array = entries[k];
      n += f.write(array, '\t');
    }
  }
  return n;
}
