/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "medusa.local.h"

/*
 *
 */
bool
medusa::closefile(FILE* fid) {
  if((FileBuffer::handles).count(fid) < 1)
    panic("Invalid stream.\n", __FILE__, __LINE__);
  fclose(fid);
  free(FileBuffer::handles[fid]);
  (FileBuffer::handles).erase(fid);
  return true;
}
