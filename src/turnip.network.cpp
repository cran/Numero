/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
Network::Network() {
  this->buffer = new NetworkBuffer();
}

/*
 *
 */
Network::Network(const Network& t) {
  this->buffer = new NetworkBuffer(t.buffer);
}

/*
 *
 */
void
Network::operator=(const Network& t) {
  if(this == &t) return;
  NetworkBuffer* p = (NetworkBuffer*)buffer; delete p;
  this->buffer = new NetworkBuffer(t.buffer);
}

/*
 *
 */
Network::~Network() {
  NetworkBuffer* p = (NetworkBuffer*)buffer;
  delete p;
}
