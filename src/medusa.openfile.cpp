/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "medusa.local.h"

unordered_map<FILE*, char*> FileBuffer::handles;

/*
 *
 */
FILE*
medusa::openfile(const string& fname, const string& prm) {
  if(fname.size() < 1) return NULL;
  FILE* fid = fopen(fname.c_str(), prm.c_str());
  if(fid == NULL) return NULL;
  char* buffer = (char*)malloc(IOBUFCAP_medusa);
  setvbuf(fid, buffer, _IOFBF, IOBUFCAP_medusa);
  FileBuffer::handles[fid] = buffer;
  return fid;
}
