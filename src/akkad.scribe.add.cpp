/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "akkad.local.h"

/*
 *
 */
bool
Scribe::add(const string& a, const string& b) {
  vector<string> fields(2);
  fields[0] = a;
  fields[1] = b;
  return this->add(fields);
}

/*
 *
 */
bool
Scribe::add(const vector<string>& array) {
  if(array.size() < 2) return false;

  /* Check that instruction is usable. */
  const string& s1 = array[0];
  const string& s2 = array[1];
  if(s1 == "") return false;
  if(s2 == "") return false;
  if(s1[0] == '#') return false;
  if(s2[0] == '#') return false;

  /* Create database entry. */
  InstrMap* p = (InstrMap*)(this->buffer);
  FieldSet& entries = (*p)[s1];

  /* Insert values. */
  return entries.insert(array);
}
