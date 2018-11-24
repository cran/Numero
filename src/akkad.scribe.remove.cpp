/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "akkad.local.h"

/*
 *
 */
bool
Scribe::remove(const string& s) {
  if(s == "") return false;
  InstrMap* p = (InstrMap*)(this->buffer);
  return (p->erase(s) > 0);
}
