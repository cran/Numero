/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen */

#include "scriptum.local.h"

/*
 *
 */
Artist::Artist() {
  this->buffer = new ArtistBuffer();
}

/*
 *
 */
Artist::Artist(const string& fname) {
  ArtistBuffer* p = new ArtistBuffer();
  this->buffer = p;

  /* Open output file. */
  p->output = openfile(fname, "w");
  if(p->output == NULL) {
    worry(("Cannot open '" + fname + "'."), "");
    return;
  }

  /* Print prolog. */
  string protext = p->prolog(Color());
  p->filesize += fprintf(p->output, "%s", protext.c_str());
  p->prosize = protext.size();
}

/*
 *
 */
Artist::Artist(const Artist& a) {
  this->buffer = new ArtistBuffer(a.buffer);
}

/*
 *
 */
void
Artist::operator=(const Artist& a) {
  ArtistBuffer* p = (ArtistBuffer*)buffer; delete p;
  this->buffer = new ArtistBuffer(a.buffer);
}

/*
 *
 */
Artist::~Artist() {
  ArtistBuffer* p = (ArtistBuffer*)buffer;
  delete p;
}

