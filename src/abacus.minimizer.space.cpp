/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
void
Minimizer::space(const mdreal a, const mdreal b) {
  MinimizerBuffer* p = (MinimizerBuffer*)buffer;
  mdreal rlnan = medusa::rnan();
  if((a == rlnan) || (b == rlnan)) {
    (p->limits).first = rlnan;
    (p->limits).second = rlnan;
    return;
  }
  if(a > b) {
    (p->limits).first = b;
    (p->limits).second = b;
    return;
  }
  (p->limits).first = a;
  (p->limits).second = b;
}

/*
 *
 */
pair<mdreal, mdreal>
Minimizer::space() const {
  MinimizerBuffer* p = (MinimizerBuffer*)buffer;
  return p->limits;
}
