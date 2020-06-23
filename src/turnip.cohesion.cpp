/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
Cohesion::Cohesion() {
  this->buffer = new CohesionBuffer();
}

/*
 *
 */
Cohesion::Cohesion(const Cohesion& t) {
  this->buffer = new CohesionBuffer(t.buffer);
}

/*
 *
 */
void
Cohesion::operator=(const Cohesion& t) {
  if(this == &t) return;
  CohesionBuffer* p = (CohesionBuffer*)buffer; delete p;
  this->buffer = new CohesionBuffer(t.buffer);
}

/*
 *
 */
Cohesion::~Cohesion() {
  CohesionBuffer* p = (CohesionBuffer*)buffer;
  delete p;
}
