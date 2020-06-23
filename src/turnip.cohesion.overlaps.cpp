/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
Network
Cohesion::overlaps(const string& method, const mdreal rlim) const {
  CohesionBuffer* p = (CohesionBuffer*)buffer;
  unordered_map<string, mdsize>& mod2rank = p->mod2rank;
  mdreal rlnan = medusa::rnan();

  /* Check method. */
  char mflag = '\0';
  if(method == "Jaccard") mflag = 'J';
  if(method == "S-S") mflag = 'S';
  if(mflag == '\0') return Network();
  
  /* Local matrix. */
  vector<long double> masses(mod2rank.size(), 0.0);
  Matrix freq; freq.symmetric(true);

  /* Count overlapping items between modules. */
  vector<set<mdsize> >& items = p->items;
  for(mdsize k = 0; k < items.size(); k++) {
    set<mdsize>& memb = items[k];

    /* Copy connected modules. */
    vector<mdsize> mods;
    mods.insert(mods.end(), memb.begin(), memb.end());

    /* Collect edge weights. */
    vector<mdreal> weights;
    Matrix& graph = p->graph;
    mdsize nmods = mods.size();
    for(mdsize i = 0; i < nmods; i++) {
      mdreal w = graph.value(mods[i], k);
      if(w == rlnan) panic("Bad weight.", __FILE__, __LINE__);
      weights.push_back(w);
    }

    /* Update module masses. */
    for(mdsize i = 0; i < nmods; i++)
      masses[mods[i]] += weights[i];

    /* Update overlap frequencies. */
    for(mdsize i = 0; i < nmods; i++) {
      mdsize a = mods[i];
      mdreal wA = weights[i];
      for(mdsize j = (i + 1); j < nmods; j++) {
	mdreal w = 0.5*(wA + weights[j]);
	freq.add(a, mods[j], w);
      }
    }
  }

  /* Collect non-zero edges. */
  vector<Element> elem = freq.elements(0);
  freq = Matrix(); /* conserve memory */

  /* Calculate final edge weights. */
  mdsize nelem = 0;
  for(mdsize i = 0; i < elem.size(); i++) {
    Element& e = elem[i];
    long double nA = masses[e.row];
    long double nB = masses[e.column];
    long double nAB = e.value;
    switch(mflag) {
    case 'J':
      e.value = (nAB/(nA + nB - nAB + 1e-9));
      break;
    case 'S':
      if(nA < nB) e.value = nAB/(nA + 1e-9);
      else e.value = nAB/(nB + 1e-9);
      break;
    default:
      e.value = 0.0;
    }
    if(e.value < rlim) continue;
    elem[nelem] = e; nelem++;    
  }
  elem.resize(nelem);

  /* Collect module identities. */
  vector<string> mkeys(mod2rank.size());
  for(unordered_map<string, mdsize>::iterator it = mod2rank.begin();
      it != mod2rank.end(); it++)
    mkeys[it->second] = it->first;

  /* Create a network. */
  Network net;
  for(mdsize i = 0; i < elem.size(); i++) {
    Element& e = elem[i];
    net.add(mkeys[e.row], mkeys[e.column], e.value);
  }
  return net;
}
