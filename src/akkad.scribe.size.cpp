/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "akkad.local.h"

/*
 *
 */
mdsize
Scribe::size() const {
  mdsize n = 0;
  InstrMap* p = (InstrMap*)(this->buffer);
  InstrMap::const_iterator pos;
  for(pos = p->begin(); pos != p->end(); pos++)
    n += (pos->second).size();
  return n;
}
