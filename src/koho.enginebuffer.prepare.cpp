/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
void
EngineBuffer::prepare() {
  
  /* Discard previous contents. */
  (this->bmus).first.clear();
  (this->bmus).second.clear();
  (this->freqs).clear();
  (this->cache).clear();

  /* Allocate column vectors. */
  (this->cache).resize(data.order());
   
  /* Collect data. */
  vector<mdsize> loci;
  unordered_map<string, Point>::iterator pt;
  for(pt = points.begin(); pt != points.end(); pt++) {
    
    /* Copy location. */
    mdsize bmu = (pt->second).location();
    if(bmu >= topology.size()) continue; 
    loci.push_back(bmu);

    /* Copy values. */
    mdsize rank = (pt->second).rank();
    vector<mdreal> array = data.row(rank);
    for(mdsize j = 0; j < array.size(); j++)
      (this->cache[j]).values.push_back(array[j]);  
  }
  
  /* Apply rank transform. */
  for(mdsize j = 0; j < cache.size(); j++) {
    ColumnCache& cc = this->cache[j];
    cc.transf = Transformation(cc.values);
    cc.transf.transform(cc.values);
  }

  /* Prepare shuffling mask. */
  (this->bmus).first = loci;
  (this->bmus).second = loci;
}
