/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
vector<mdsize>
Engine::shuffle(const mdsize nmax) const {
  EngineBuffer* p = (EngineBuffer*)buffer;
  mdsize nvert = (p->name2rank).size();  

  /* srand() and rand() removed for R. */
  panic("Not tested.", __FILE__, __LINE__);

  /* Shuffle index mask. */
  vector<mdsize>& mask = p->mask;
  uniform_int_distribution<unsigned int> dst(0, RAND_MAX);
  for(mdsize i = 0; (i < nvert) && (i < nmax); i++) {
    mdsize ind = dst(p->twister)%nvert;
    mdsize tmp = mask[i];
    mask[i] = mask[ind];
    mask[ind] = tmp;  
  }

  /* Create subset. */
  vector<mdsize> array;
  for(mdsize i = 0; (i < nvert) && (i < nmax); i++)
    array.push_back(mask[i]);
  return array;
}
