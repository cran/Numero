/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen */

#include "scriptum.local.h"
#include <Rcpp.h>

/*
 *
 */
unsigned long
Artist::close() {
  ArtistBuffer* p = (ArtistBuffer*)buffer;
  if(p->output == NULL) return p->filesize;

  /* Close all open groups. */
  while(p->ngroups > 0) this->group(-1);

  /* Close umbrella group. */
  p->filesize += fprintf(p->output, "\n</g>\n</svg>\n");

  /* Update prolog. */
  string protext = p->prolog(Color());
  if(protext.size() != p->prosize) {
    Rcpp::Rcout << protext << "\n";
    panic("Prolog failed.", __FILE__, __LINE__);
  }

  /* Overwrite prolog. */
  rewind(p->output);
  fprintf(p->output, "%s", protext.c_str());

  /* Close file. */
  closefile(p->output);
  p->output = NULL;
  return p->filesize;
}
