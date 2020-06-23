/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
mdsize
Network::component(const string& key) const {
  NetworkBuffer* p = (NetworkBuffer*)buffer;

  /* Find vertex. */
  unordered_map<string, mdsize>::iterator pos;
  unordered_map<string, mdsize>& name2rank = p->name2rank;
  if((pos = name2rank.find(key)) == name2rank.end())
    return medusa::snan();

  /* Check component labeling. */
  p->clean();
  
  /* Return component identifier. */
  return (p->vertices[pos->second]).component;
}
