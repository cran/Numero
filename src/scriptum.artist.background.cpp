/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "scriptum.local.h"

/*
 *
 */
void
Artist::background(const Color& c) {
  ArtistBuffer* p = (ArtistBuffer*)buffer;
  p->bgcolor = c;
}
