/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#ifndef punos_local_INCLUDED
#define punos_local_INCLUDED

#include <cstdlib>
#include <cstdio>
#include <climits>
#include <cctype>
#include <ctime>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "medusa.h"
#include "abacus.h"
#include "scriptum.h"
#include "punos.h"

using namespace std;
using namespace medusa;
using namespace abacus;
using namespace scriptum;
using namespace punos;

/* Encapsulate with redundant namespace in case in a collection
   of modules another module has the same class name(s) in use. */
namespace punos_local {
  typedef unordered_map<unsigned short, mdbyte> LinkMap;

  /*
   *
   */
  class TopologyBuffer {
  public:
    mdreal sigma;
    mdreal maxradius;
    vector<Unit> coord;
    vector<mdreal> levels;
    vector<LinkMap> network;
  public:
    TopologyBuffer() {
      this->sigma = medusa::rnan();
      this->maxradius = medusa::rnan();
    };
    TopologyBuffer(const void* ptr) {
      TopologyBuffer* t = (TopologyBuffer*)ptr;
      this->sigma = t->sigma;
      this->maxradius = t->maxradius;
      this->coord = t->coord;
      this->levels = t->levels;
      this->network = t->network;
    };
    ~TopologyBuffer() {};
    static mdreal scale() {
      Style sty;
      return 2*(sty.fontsize);
    }
  };

  /* Utility functions. */
  extern vector<mdreal> smoothen(const vector<mdreal>&,
				 const vector<LinkMap>&);
};

using namespace punos_local;

#endif /* punos_local_INCLUDED */
