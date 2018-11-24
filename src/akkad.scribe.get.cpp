/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "akkad.local.h"

/*
 *
 */
vector<string>
Scribe::get(const string& s, const mdsize ind) const {
  FieldSet empty;
  InstrMap* p = (InstrMap*)(this->buffer);
  InstrMap::iterator pos = p->find(s);
  if(pos == p->end()) return empty[0];
  FieldSet& fset = pos->second;
  return fset[ind];
}
