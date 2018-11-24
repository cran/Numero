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
#include <unordered_map>
#include <unordered_set>
#include "medusa.h"
#include "abacus.h"

using namespace std;
using namespace medusa;
using namespace abacus;

/* Encapsulate with redundant namespace in case in a collection
   of modules another module has the same class name(s) in use. */
namespace abacus_local {

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
    unordered_map<mdreal, mdreal> data;
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
      unordered_map<mdreal, mdreal>::const_iterator pos;
      for(pos = data.begin(); pos != data.end(); pos++) {
	x.push_back(pos->first);
	w.push_back(pos->second);
      }
    };
  };

  /*
   *
   */
  class Array {
  private:
    mdsize ndata;
    mdsize nelem;
    mdreal rlnan;
    vector<mdreal> full;
    map<mdsize, mdreal> sparse;


    set<mdsize> testset;
  private:
    mdsize optimize();
  public:
    Array();
    ~Array();
    mdreal operator[](const mdsize) const;
    void elements(vector<Element>&, const mdsize) const;
    mdsize length();
    mdreal remove(const mdsize);
    mdsize size() const;
    bool update(const mdsize, const mdreal, const bool);
    vector<mdreal> values() const;
  };

  /*
   *
   */
  class MatrixBuffer  {
  public:
    bool symmflag;
    mdsize nrows;
    mdsize ncols;
    mdreal rlnan;
    unordered_map<mdsize, Array> rowdata;
  public:
    MatrixBuffer() {
      this->symmflag = false;
      this->nrows = 0;
      this->ncols = 0;
      this->rlnan = medusa::rnan();
    };
    MatrixBuffer(void* ptr) {
      MatrixBuffer* p = (MatrixBuffer*)ptr;
      this->symmflag = p->symmflag;
      this->nrows = p->nrows;
      this->ncols = p->ncols;
      this->rlnan = p->rlnan;
      this->rowdata = p->rowdata;
    };
    ~MatrixBuffer() {};
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
