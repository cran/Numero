/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
vector<Network>
Network::split() {
  mdsize sznan = medusa::snan();
  NetworkBuffer* p = (NetworkBuffer*)buffer;

  /* Consecutive component labels. */
  p->clean();

  /* Copy vertices without links. */
  vector<Vertex> vtx;
  for(mdsize i = 0; i < (p->vertices).size(); i++) {
    Vertex& v = p->vertices[i];
    v.incoming.clear();
    v.outgoing.clear();
    vtx.push_back(v);
  }

  /* Copy edges. */
  vector<Element> elem = (p->graph).elements(0);

  /* Clear source object to reduce memory footprint. */
  delete p; this->buffer = new NetworkBuffer();
  p = (NetworkBuffer*)buffer;
  
  /* Re-create network components. */
  vector<Network> output;
  for(mdsize i = 0; i < elem.size(); i++) {
    Vertex& vA = vtx[elem[i].row];
    Vertex& vB = vtx[elem[i].column];
    if(vA.component != vB.component) {
      panic("Bad components.", __FILE__, __LINE__);
    }
    if(vA.component >= output.size())
      output.resize(vA.component + 1);
    output[vA.component].add(vA.name(), vB.name(), elem[i].value);
  }

  /* Copy vertex attributes. */
  for(mdsize i = 0; i < vtx.size(); i++) {
    Vertex& v = vtx[i];
    string key = v.name();

    /* Find target vertex in new subnetwork. */
    NetworkBuffer* nb = (NetworkBuffer*)(output[v.component].buffer);
    if((nb->name2rank).count(key) < 1)
      panic("Bad vertex.", __FILE__, __LINE__);
    mdsize rank = nb->name2rank[key];

    /* Identify community hub. */
    string hubkey;
    if(v.community != sznan)
      hubkey = vtx[v.community].name();
    
    /* Find hub vertex in new subnetwork. */
    mdsize hub = sznan;
    if((nb->name2rank).count(hubkey) > 0)
      hub = nb->name2rank[hubkey];
    
    /* Update community assignment. */
    Vertex& x = nb->vertices[rank];
    if(hub != sznan) nb->communities[hub].insert(rank);
    x.community = hub;
  }
  return output;
}
