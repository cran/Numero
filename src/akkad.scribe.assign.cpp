/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "akkad.local.h"

/*
 *
 */
mdsize
Scribe::assign(const Scribe& cfg) {
  mdsize n = 0;
  InstrMap* p = (InstrMap*)(this->buffer);
  InstrMap* q = (InstrMap*)(cfg.buffer);

  /* Overwrite matching instructions. */
  InstrMap::iterator pos;
  for(pos = q->begin(); pos != q->end(); pos++) {
    if(p->count(pos->first) < 1) continue;
    (*p)[pos->first] = pos->second;
    n += (pos->second).size();
  }
  return n;
}
