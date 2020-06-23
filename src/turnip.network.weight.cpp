/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
mdreal
Network::weight(const string& keyA, const string& keyB) const {
  NetworkBuffer* p = (NetworkBuffer*)buffer;

  /* Find vertices. */
  unordered_map<string, mdsize>::iterator posA;
  unordered_map<string, mdsize>::iterator posB;
  unordered_map<string, mdsize>& name2rank = p->name2rank;
  if((posA = name2rank.find(keyA)) == name2rank.end())
    return medusa::rnan();
  if((posB = name2rank.find(keyB)) == name2rank.end())
    return medusa::rnan();

  /* Return connection weight. */
  return (p->graph).value(posA->second, posB->second);
}
