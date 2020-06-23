/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
Vertex::Vertex() {
  this->locus = medusa::snan();
  this->component = medusa::snan();
  this->community = medusa::snan();
}

/*
 *
 */
Vertex::Vertex(const string& key, const mdsize rank) {
  this->identity = key;
  this->locus = rank;
  this->component = medusa::snan();
  this->community = medusa::snan();
}

/*
 *
 */
Vertex::~Vertex() {}
