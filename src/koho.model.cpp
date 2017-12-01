/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
Model::Model() {
  this->buffer = new ModelBuffer();
}

/*
 *
 */
Model::Model(const Topology& tp) {
  ModelBuffer* p = new ModelBuffer();
  p->sigma = tp.sigma();
  p->structure = tp;
  this->buffer = p;
}

/*
 *
 */
Model::Model(const Model& a) {
  this->buffer = new ModelBuffer(a.buffer);
}

/*
 *
 */
void
Model::operator=(const Model& a) {
  ModelBuffer* p = (ModelBuffer*)buffer; delete p;
  this->buffer = new ModelBuffer(a.buffer);
}

/*
 *
 */
Model::~Model() {
  ModelBuffer* p = (ModelBuffer*)buffer;
  delete p;
}
