/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
Minimizer::Minimizer() {
  this->buffer = new MinimizerBuffer();
}

/*
 *
 */
Minimizer::Minimizer(const Minimizer& m) {
  this->buffer = new MinimizerBuffer(m.buffer);
}

/*
 *
 */
void
Minimizer::operator=(const Minimizer& m) {
  MinimizerBuffer* p = (MinimizerBuffer*)buffer; delete p;
  this->buffer = new MinimizerBuffer(m.buffer);
}

/*
 *
 */
Minimizer::~Minimizer() {
  MinimizerBuffer* p = (MinimizerBuffer*)(this->buffer);
  delete p;
}
