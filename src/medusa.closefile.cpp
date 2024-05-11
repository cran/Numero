/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "medusa.local.h"

/*
 *
 */
bool
medusa::closefile(FILE* fid) {
  if((FileBuffer::handles).count(fid) < 1)
    panic("Invalid stream.\n", __FILE__, __LINE__);
  char* buffer = FileBuffer::handles[fid];
  (FileBuffer::handles).erase(fid);
  fclose(fid); free(buffer);
  return true;
}
