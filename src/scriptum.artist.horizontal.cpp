/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "scriptum.local.h"

/*
 *
 */
pair<mdreal, mdreal>
Artist::horizontal() const {
  ArtistBuffer* p = (ArtistBuffer*)buffer;
  mdreal rlnan = medusa::rnan();
  pair<mdreal, mdreal> res;
  res.first = (p->limits).first.alpha;
  res.second = (p->limits).first.omega;
  if((res.first == rlnan) || (res.second == rlnan)) {
    res.first = 0.0;
    res.second = 0.0;
  }
  return res;
}
