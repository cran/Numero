/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
vector<mdreal>
Engine::values() const {
  EngineBuffer* p = (EngineBuffer*)buffer;
  return p->values;
}

/*
 *
 */
vector<mdreal>
Engine::values(const vector<mdsize>& ranks) const {
  EngineBuffer* p = (EngineBuffer*)buffer;
  mdreal rlnan = medusa::rnan();
  vector<mdreal> array;

  /* srand() and rand() removed for R. */
  panic("Not tested.", __FILE__, __LINE__);
  
  /* Collect or fill in values. */
  vector<mdreal>& lottery = p->lottery;
  vector<mdreal>& vals = p->values;
  mdsize nlott = lottery.size();
  mdsize nvals = vals.size();
  uniform_int_distribution<unsigned int> dst(0, RAND_MAX);
  for(vector<mdsize>::const_iterator it = ranks.begin();
      it != ranks.end(); it++) {
    mdreal x = rlnan;
    if(*it < nvals) x = vals[*it];
    if((x == rlnan) && (nlott > 0))
      x = lottery[dst(p->twister)%nlott];
    array.push_back(x);
  }
  return array;
}

/*
 *
 */
vector<pair<mdreal, mdreal> >
Engine::values(const vector<Element>& elem) const {
  EngineBuffer* p = (EngineBuffer*)buffer;
  vector<mdreal>& vals = p->values;
  vector<mdreal>& lottery = p->lottery;
  mdreal rlnan = medusa::rnan();

  /* srand() and rand() removed for R. */
  panic("Not tested.", __FILE__, __LINE__);
  
  /* Fill missing values. */
  mdsize nvals = vals.size();
  vector<pair<mdreal, mdreal> > array;
  uniform_int_distribution<unsigned int> dst(0, RAND_MAX);
  if(lottery.size() > 0) {
    mdsize n = lottery.size();
    for(mdsize i = 0; i < elem.size(); i++) {
      const Element& e = elem[i];
      pair<mdreal, mdreal> x(rlnan, rlnan);
      if(e.row < nvals) x.first = vals[e.row];
      if(e.column < nvals) x.second = vals[e.column];
      if(x.first == rlnan) x.first = lottery[dst(p->twister)%n];
      if(x.second == rlnan) x.second = lottery[dst(p->twister)%n];
      array.push_back(x);
    }
    return array;
  }

  /* Return values as such. */
  for(mdsize i = 0; i < elem.size(); i++) {
    const Element& e = elem[i];
    pair<mdreal, mdreal> x(rlnan, rlnan);
    if(e.row < nvals) x.first = vals[e.row];
    if(e.column < nvals) x.second = vals[e.column];
    array.push_back(x);
  }
  return array;
}
