/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#ifndef abacus_local_INCLUDED
#define abacus_local_INCLUDED

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <climits>
#include <cmath>
#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "medusa.h"
#include "abacus.h"

using namespace std;
using namespace medusa;
using namespace abacus;

/* Encapsulate with redundant namespace in case in a collection
   of modules another module has the same class name(s) in use. */
namespace abacus_local {
  typedef unsigned long Key;

  /*
   *
   */
  class Gaussian {
  private:
    string method;
    mdreal center;
    mdreal offset;
    mdreal scale;
    mdreal factor;
    mdreal mu;
    mdreal sigma;
    vector<mdsize> qloci;
    vector<mdreal> values;
    vector<mdreal> weights;
    vector<mdreal> zscores;
  private:
  public:
    Gaussian() {
      this->center = medusa::rnan();
      this->offset = center;
      this->scale = center;
      this->factor = center;
      this->mu = center;
      this->sigma = center;
    };
    ~Gaussian() {};
    void apply(vector<mdreal>&, const mdreal) const;
    bool configure(const vector<mdreal>&, const vector<mdreal>&);
    string model() const {return method;};
    mdreal optimize(const string&);
    mdsize transform(vector<mdreal>&) const;
    mdreal quality() const;
    mdreal distance(const mdreal, const mdreal, const mdreal) const;
  };
 
  /*
   *
   */
  class Approximation {
  private:
    mdreal mode;
    Gaussian positive;
    Gaussian negative;
  public:
    Approximation() {this->mode = medusa::rnan();};
    ~Approximation() {};
    void fit(const vector<mdreal>&, const vector<mdreal>&);
    mdreal transform(const mdreal) const;
  };

  /*
   *
   */
  class EmpiricalBuffer {
  public:
    unsigned long ndata;
    Approximation approx;
    map<mdreal, mdreal> data;
  public:
    EmpiricalBuffer() {this->ndata = 0;};
    EmpiricalBuffer(void* ptr) {
      EmpiricalBuffer* p = (EmpiricalBuffer*)ptr;
      this->ndata = p->ndata;
      this->approx = p->approx;
      this->data = p->data;
    };
    ~EmpiricalBuffer() {};
    void contents(vector<mdreal>& x, vector<mdreal>& w) const {
      map<mdreal, mdreal>::const_iterator pos;
      for(pos = data.begin(); pos != data.end(); pos++) {
	x.push_back(pos->first);
	w.push_back(pos->second);
      }
    };
  };
  
  /*
   *
   */
  class MatrixBuffer  {
  public:
    bool symmflag;
    mdsize nrows;
    mdsize ncols;
    mdsize nline;
    mdreal rlnan;
    map<Key, mdreal> data;
  public:
    MatrixBuffer() {
      this->symmflag = false;
      this->nrows = 0;
      this->ncols = 0;
      this->nline = sqrt(USHRT_MAX);
      if(sizeof(mdsize) == sizeof(short)) this->nline = USHRT_MAX;
      if(sizeof(mdsize) == sizeof(int)) this->nline = UINT_MAX;
      if(sizeof(Key) == sizeof(mdsize)) this->nline = sqrt(1.0*nline);
      if(sizeof(Key) < sizeof(mdsize))
	panic("Unusable key type.", __FILE__, __LINE__);
      this->rlnan = medusa::rnan();
    };
    MatrixBuffer(void* ptr) {
      MatrixBuffer* p = (MatrixBuffer*)ptr;
      this->symmflag = p->symmflag;
      this->nrows = p->nrows;
      this->ncols = p->ncols;
      this->nline = p->nline;
      this->rlnan = p->rlnan;
      this->data = p->data;
    };
    ~MatrixBuffer() {};
    Key key(mdsize, mdsize) const;
    vector<Key> key(const vector<Element>&) const;
  };

  /*
   *
   */
  class MinimizerBuffer {
  public:
    mdsize npoints;
    mdreal epsilon;
    pair<mdreal, mdreal> limits;
  public:
    MinimizerBuffer() {
      this->npoints = 0;
      this->epsilon = 0.0;
    };
    MinimizerBuffer(void* ptr) {
      MinimizerBuffer* p = (MinimizerBuffer*)ptr;
      this->npoints = p->npoints;
      this->epsilon = p->epsilon;
    };
    ~MinimizerBuffer() {};
  };
};

using namespace abacus_local;

#endif /* abacus_local_INCLUDED */
