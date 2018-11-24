/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "akkad.local.h"

/*
 *
 */
string
Scribe::accessible(const string& key, const string& prm) const {
  string fname = (*this)[key][1];
  string err = File::check(fname, prm);
  if(err.size() > 0) err += (" (" + key + ")");
  return err;
}
