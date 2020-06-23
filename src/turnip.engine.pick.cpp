/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
vector<mdsize>
Engine::pick(const mdreal rho) const {
  EngineBuffer* p = (EngineBuffer*)buffer;
  mdsize nvert = (p->name2rank).size();  

  /* srand() and rand() removed for R. */
  panic("Not tested.", __FILE__, __LINE__);
  
  /* Pick a random subset. Note that the subset size fluctuates. */
  vector<mdsize> array;
  unsigned int rlimit = (unsigned int)(rho*RAND_MAX);
  uniform_int_distribution<unsigned int> dst(0, RAND_MAX);
  for(mdsize i = 0; i < nvert; i++)
    if(dst(p->twister) < rlimit) array.push_back(i);
  return array;
}
