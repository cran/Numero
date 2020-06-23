/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
struct Item {
  string name;
  mdreal weight;
};

/*
 *
 */
class ItemCompare {
private:
  bool flag;
public:
  ItemCompare() {this->flag = false;};
  ItemCompare(const bool b) {this->flag = b;};
  ~ItemCompare() {};
  bool operator()(const Item& x, const Item& y) const {
    if(x.weight != y.weight) {
      if(flag) return (x.weight < y.weight);
      return (x.weight > y.weight);
    }
    return (x.name < y.name);
  };
};

/*
 *
 */
Network
Cohesion::expand(const Network& modnet) const {
  CohesionBuffer* p = (CohesionBuffer*)buffer;

  /* Allow self-loops for items. */
  Network output;
  output.redundant(true);

  /* Create item network. */
  vector<Edge> edges = modnet.edges();
  for(vector<Edge>::iterator e = edges.begin(); e != edges.end(); e++) {
    if((p->mod2rank).count(e->tail) < 1) continue;
    if((p->mod2rank).count(e->head) < 1) continue;
    mdsize rankT = p->mod2rank[e->tail];
    mdsize rankH = p->mod2rank[e->head];
    mdreal effect = fabs(e->weight);
    mdreal sign = (e->weight)/effect;

    /* Collect assignments. */
    vector<Item> itemsT;
    set<mdsize>& elemT = p->modules[rankT];
    for(set<mdsize>::iterator k = elemT.begin(); k != elemT.end(); k++) {
      Item item;
      item.name = p->itemkeys[*k];
      item.weight = (p->graph).value(rankT, *k);
      itemsT.push_back(item);
    }
    
    /* Collect assignments. */
    vector<Item> itemsH;
    set<mdsize>& elemH = p->modules[rankH];
    for(set<mdsize>::iterator k = elemH.begin(); k != elemH.end(); k++) {
      Item item;
      item.name = p->itemkeys[*k];
      item.weight = (p->graph).value(rankH, *k);
      itemsH.push_back(item);
    }
    
    /* Sort by weight. */
    std::sort(itemsT.begin(), itemsT.end(), ItemCompare(true));
    std::sort(itemsH.begin(), itemsH.end(), ItemCompare(sign > 0.0));
    
    /* Collect pairings. */
    for(mdsize i = 0; (i < itemsT.size()) && (i < itemsH.size()); i++) {
      Item t = itemsT[i];
      Item h = itemsH[i];
      
      /* Connection strength and direction between items
	 depends on synergy or disergy between modules. */
      mdreal flow = 0.5*(t.weight + sign*(h.weight));
	
      /* Module interaction affects flow magnitude. */
      mdreal magn = fabs(flow);
      flow *= sqrt(effect);
      flow /= (sqrt(magn) + 1e-9);
 
      /* Update pairing network. */
      output.add(t.name, h.name, flow);
    }
  }
  return output;
}
