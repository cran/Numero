/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
mdsize
Network::size() const {
  NetworkBuffer* p = (NetworkBuffer*)buffer;
  return (p->graph).count();
}
