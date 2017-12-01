/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen */

#include "scriptum.local.h"

/*
 *
 */
bool
Artist::paint(Frame& fr) {
  ArtistBuffer* p = (ArtistBuffer*)buffer;
  if(p->output == NULL) return false;

  /* Update limits. */
  pair<mdreal, mdreal> xlim = fr.horizontal();
  pair<mdreal, mdreal> ylim = fr.vertical();
  (p->limits).first.update(xlim.first);
  (p->limits).first.update(xlim.second);
  (p->limits).second.update(ylim.first);
  (p->limits).second.update(ylim.second);

  /* Save data. */
  string data = fr.flush();
  p->filesize += fprintf(p->output, "%s", data.c_str());
  return (data.size() > 0);
}
