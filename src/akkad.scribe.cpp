/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "akkad.local.h"

/*
 *
 */
Scribe::Scribe() {
  this->buffer = new InstrMap();
}

/*
 *
 */
Scribe::Scribe(const string& fname) {
  this->buffer = new InstrMap();

  /* Open file. */
  File f;
  if(f.open(fname, "r") == false) return;

  /* Read instructions. */
  while(f.error().size() < 1) {
    vector<string> array = f.read('\t', 2);
    if(array[0] == "") continue;
    if(array[1] == "") continue;
    this->add(array);
  }
}

/*
 *
 */
Scribe::Scribe(const Scribe& t) {
  InstrMap* p = (InstrMap*)(t.buffer);
  this->buffer = new InstrMap(*p);
}

/*
 *
 */
void
Scribe::operator=(const Scribe& t) {
  InstrMap* p = (InstrMap*)(this->buffer);
  delete p; p = (InstrMap*)(t.buffer);
  this->buffer = new InstrMap(*p);
}

/*
 *
 */
Scribe::~Scribe() {
  InstrMap* p = (InstrMap*)(this->buffer);
  delete p;
}
