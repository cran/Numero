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
  (this->units).clear();
  (this->mask).clear();
  (this->bmus).clear();
  (this->freq).clear();
  (this->cache).clear();

  /* Allocate column vectors. */
  (this->cache).resize(data.order());
  
  /* Collect eligible samples. */
  unordered_map<string, Point>::iterator pt;
  for(pt = points.begin(); pt != points.end(); pt++) {
    
    /* Check location. */
    mdsize bmu = (pt->second).location();
    if(bmu >= topology.size()) continue; 
    
    /* Check for missing data. */
    mdsize rank = (pt->second).rank();
    vector<mdreal> array = data.row(rank);
    if(abacus::statistic(array, "number") != array.size())
      continue;

    /* Copy values. */
    for(mdsize j = 0; j < array.size(); j++)
      (this->cache[j]).values.push_back(array[j]);
    
    /* Copy location. */
    (this->bmus).push_back(bmu);
  }

  /* Apply rank transform. */
  for(mdsize j = 0; j < cache.size(); j++) {
    ColumnCache& cc = this->cache[j];
    cc.transf = Transformation(cc.values);
    cc.transf.transform(cc.values);
  }

  /* Prepare shuffling mask. */
  for(mdsize i = 0; i < bmus.size(); i++)
    (this->mask).push_back(i);

  /* Prepare unit indices. */
  for(mdsize i = 0; i < topology.size(); i++)
    (this->units).push_back(i);
}
