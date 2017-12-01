/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
Empirical::Empirical() {
  EmpiricalBuffer* p = new EmpiricalBuffer();
  this->buffer = p;
}

/*
 *
 */
Empirical::Empirical(const Empirical& t) {
  this->buffer = new EmpiricalBuffer(t.buffer);
}

/*
 *
 */
void
Empirical::operator=(const Empirical& t) {
  EmpiricalBuffer* p = (EmpiricalBuffer*)buffer; delete p;
  this->buffer = new EmpiricalBuffer(t.buffer);
}

/*
 *
 */
Empirical::~Empirical() {
  EmpiricalBuffer* p = (EmpiricalBuffer*)buffer;
  delete p;
}
