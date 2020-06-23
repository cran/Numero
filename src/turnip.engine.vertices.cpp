/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
vector<string>
Engine::vertices() const {
  EngineBuffer* p = (EngineBuffer*)buffer;
  unordered_map<string, mdsize>& name2rank = p->name2rank;
  vector<string> array(name2rank.size());
  for(unordered_map<string, mdsize>::iterator it = name2rank.begin();
      it != name2rank.end(); it++)
    array[it->second] = it->first;
  return array;
}
