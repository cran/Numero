/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
class MagnitudeComparator {
public:
  bool operator()(const Element& x, const Element& y) {
    return (fabs(x.value) > fabs(y.value));
  };
};

/*
 *
 */
mdsize
Network::prune(const mdreal kappa) {
  NetworkBuffer* p = (NetworkBuffer*)buffer;
  vector<Vertex>& vtx = p->vertices;
  Matrix& graph = p->graph;

  /* Determine target number of edges. */
  mdsize ncomp = (p->components).size();
  mdsize ntree = (vtx.size() - ncomp);
  mdsize ntarget = (mdsize)(kappa*ntree + 0.5);
  if(ntarget < ntree) ntarget = ntree; 
  if(graph.count() <= ntarget) return graph.count();

  /* Clear current vertex neighbors. */
  for(mdsize i = 0; i < vtx.size(); i++) {
    vtx[i].incoming.clear();
    vtx[i].outgoing.clear();
  }

  /* Extract and sort edges by magnitude. */
  MagnitudeComparator cmp;
  vector<Element> batch = graph.remove(0);
  sort(batch.begin(), batch.end(), cmp);
  
  /* Collect spanning trees. */
  while(ntarget > 0) {
    vector<Element> tree = Matrix::trunk(batch);
    if(tree.size() > ntarget) tree.resize(ntarget);
    for(mdsize k = 0; k < tree.size(); k++) {
      Element& e = tree[k];
      graph.add(e.row, e.column, e.value);
    }
    ntarget -= tree.size();
  }
  
  /* Recreate topology. */
  vector<Element> pruned = graph.elements(0);
  for(mdsize i = 0; i < pruned.size(); i++) {
    Element e = pruned[i];
    vtx[e.column].incoming.insert(e.row);
    vtx[e.row].outgoing.insert(e.column);
  }
  return graph.count();
}
