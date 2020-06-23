/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
void
Network::redundant(const bool flag) {
  NetworkBuffer* p = (NetworkBuffer*)buffer;
  if((p->graph).size() < 1) p->redunflag = flag;
  else panic("Network not empty.", __FILE__, __LINE__);
}

/*
 *
 */
bool
Network::redundant() const {
  NetworkBuffer* p = (NetworkBuffer*)buffer;
  return p->redunflag;
}
