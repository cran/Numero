/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
class BackdoorNetwork : public Network {
public:
  BackdoorNetwork() : Network() {};
  BackdoorNetwork(const Network& net) : Network(net) {};
  ~BackdoorNetwork() {};
  void fill(void* ptr) {
    NetworkBuffer* p = (NetworkBuffer*)buffer;
    EngineBuffer* e = (EngineBuffer*)ptr;    

    /* Copy annotations. */
    e->name2rank = p->name2rank;
    
    /* Set vertex values. */
    mdsize nvert = (e->name2rank).size();
    (e->values).resize(nvert, medusa::rnan());

    /* Allocate adjacency matrix. */
    (e->links).resize(nvert);
    
    /* Prepare index mask for randomization. */
    e->mask = abacus::shuffle(nvert, false);
    
    /* Copy network topology. */
    vector<Element> elem = (p->graph).elements(0);
    for(mdsize i = 0; i < elem.size(); i++) {
      mdsize a = elem[i].row;
      mdsize b = elem[i].column;
      mdreal w = elem[i].value;
      (e->links[a]).push_back(b);
      (e->graph).insert(a, b, w);
    }
  };
};

/*
 *
 */
Engine::Engine() {
  this->buffer = new EngineBuffer();
}

/*
 *
 */
Engine::Engine(const Network& net) {
  BackdoorNetwork backnet(net);
  this->buffer = new EngineBuffer();
  backnet.fill(buffer);
}

/*
 *
 */
Engine::Engine(const Engine& t) {
  this->buffer = new EngineBuffer(t.buffer);
}

/*
 *
 */
void
Engine::operator=(const Engine& t) {
  if(this == &t) return;
  EngineBuffer* p = (EngineBuffer*)buffer; delete p;
  this->buffer = new EngineBuffer(t.buffer);
}

/*
 *
 */
Engine::~Engine() {
  EngineBuffer* p = (EngineBuffer*)buffer;
  delete p;
}
