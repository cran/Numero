/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
pair<mdsize, mdsize>
abacus::extrema(const vector<mdreal>& x) {
  mdsize sznan = medusa::snan();
  mdsize bottom = sznan;
  mdsize top = sznan;
  mdreal rlnan = medusa::rnan();
  for(mdsize i = 0; i < x.size(); i++) {
    if(x[i] == rlnan) continue;
    if(bottom == sznan) bottom = i;
    if(top == sznan) top = i;
    if(x[bottom] > x[i]) bottom = i;
    if(x[top] < x[i]) top = i;
  }
  return pair<mdsize, mdsize>(bottom, top);
}
