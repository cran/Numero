/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#ifndef turnip_local_INCLUDED
#define turnip_local_INCLUDED

#include <cstdlib>
#include <cstdio>
#include <climits>
#include <cmath>
#include <algorithm>
#include <random>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include "medusa.h"
#include "abacus.h"
#include "turnip.h"

using namespace std;
using namespace medusa;
using namespace abacus;
using namespace turnip;

/* Encapsulate with redundant namespace in case in a collection
   of modules another module has the same class name(s) in use. */
namespace turnip_local {

  /*
   *
   */
  class Vertex {
  private:
    mdsize locus;
    string identity;
  public:
    mdsize component;
    mdsize community;
    set<mdsize> incoming;
    set<mdsize> outgoing;
  public:
    Vertex();
    Vertex(const string&, const mdsize);
    ~Vertex();
    mdsize degree(const int) const;
    mdsize join(const Matrix&, const vector<Vertex>&, const int);    
    string name() const {return identity;}
    vector<mdsize> neighbors() const;
    mdsize rank() const {return locus;};
    mdreal strength(const Matrix&, const int) const;
  };

  /*
   *
   */
  class Triangle {
  public:
    mdsize origin;
    mdsize alpha;
    mdsize beta;
    mdreal power;
  public:
    Triangle();
    Triangle(const mdsize, const mdsize, const mdsize, const Matrix&);
    ~Triangle();
    bool operator<(const Triangle&) const;
    bool overlap(const vector<Vertex>&) const;
  public:
    static Triangle create();
  };

  /*
   *
   */
  class EngineBuffer {
  public:
    Matrix graph;
    vector<mdsize> mask;
    vector<mdreal> values;
    vector<mdreal> lottery;
    vector<vector<mdsize> > links;
    unordered_map<string, mdsize> name2rank;
    mersenne_twister_engine<uint_fast64_t, 64, 312, 156, 31,
      0xb5026f5aa96619e9, 29, 0x5555555555555555, 17,
      0x71d67fffeda60000, 37, 0xfff7eee000000000, 43,
      6364136223846793005> twister;
  public:
    EngineBuffer() {};
    EngineBuffer(const void* ptr) {
      EngineBuffer* p = (EngineBuffer*)ptr;
      this->graph = p->graph;
      this->mask = p->mask;
      this->values = p->values;
      this->lottery = p->lottery;
      this->links = p->links;
      this->name2rank = p->name2rank;
      this->twister = p->twister;
    };
    ~EngineBuffer() {};
  };

  /*
   *
   */
  class NetworkBuffer {
  public:
    bool cleanflag;
    bool redunflag;
    Matrix graph;
    vector<Vertex> vertices;
    unordered_map<string, mdsize> name2rank;
    unordered_map<mdsize, set<mdsize> > communities;
    unordered_map<mdsize, set<mdsize> > components;
  public:
    NetworkBuffer() {
      this->cleanflag = false;
      this->redunflag = false;
      (this->graph).symmetric(false);
    };
    NetworkBuffer(const void* ptr) {
      NetworkBuffer* p = (NetworkBuffer*)ptr;
      this->cleanflag = p->cleanflag;      
      this->redunflag = p->redunflag;      
      this->graph = p->graph;
      this->vertices = p->vertices;
      this->name2rank = p->name2rank;
      this->communities = p->communities;
      this->components = p->components;
    };
    ~NetworkBuffer() {};
    void clean();
    mdsize concentrate(const set<mdsize>&) const;
    mdsize make(const string&);
    set<Triangle> triangles(vector<mdsize>&) const;
  };

  /*
   *
   */
  class CohesionBuffer {
  public:
    Matrix graph;
    vector<string> modkeys;
    vector<string> itemkeys;
    vector<set<mdsize> > items;
    vector<set<mdsize> > modules;
    unordered_map<string, mdsize> mod2rank;
    unordered_map<string, mdsize> item2rank;
  public:
    CohesionBuffer() {(this->graph).symmetric(false);};
    CohesionBuffer(const void* ptr) {
      CohesionBuffer* p = (CohesionBuffer*)ptr;
      this->graph = p->graph;
      this->modkeys = p->modkeys;
      this->itemkeys = p->itemkeys;
      this->items = p->items;
      this->modules = p->modules;
      this->mod2rank = p->mod2rank;
      this->item2rank = p->item2rank;
    };
    ~CohesionBuffer() {};
  };
}

using namespace turnip_local;

#endif /* turnip_local_INCLUDED */
