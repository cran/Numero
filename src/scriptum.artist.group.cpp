/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen */

#include "scriptum.local.h"

/*
 *
 */
mdsize
Artist::group(const int flag) {
  ArtistBuffer* p = (ArtistBuffer*)buffer;
  if(p->output == NULL)
    panic("Null pointer.\n", __FILE__, __LINE__);
  if(flag > 0) {
    p->filesize += fprintf(p->output, "\n<g>\n");
    p->ngroups += 1;
  }
  if((flag < 0) && (p->ngroups > 0)) { 
    p->filesize += fprintf(p->output, "\n</g>\n");
    p->ngroups -= 1;
  }
  return p->ngroups;
}
