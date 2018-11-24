/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "akkad.local.h"

/*
 *
 */
vector<string>
Scribe::keys() const {
  InstrMap* p = (InstrMap*)buffer;
  vector<string> array;
  InstrMap::const_iterator pos;
  for(pos = p->begin(); pos != p->end(); pos++)
    array.push_back(pos->first);
  return array;
}
