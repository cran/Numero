/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#ifndef koho_local_INCLUDED
#define koho_local_INCLUDED

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <climits>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <map>
#include "medusa.h"
#include "abacus.h"
#include "punos.h"
#include "koho.h"

using namespace std;
using namespace medusa;
using namespace abacus;
using namespace punos;
using namespace koho;


/* Encapsulate with redundant namespace in case in a collection
   of modules another module has the same class name(s) in use. */
namespace koho_local {

  /*
   *
   */
  struct Point {
    Site layer;  
    mdsize parent;
  };

  /*
   *
   */
  class Sample {
  public:
    mdsize home;
    mdsize unit;
    mdreal distance;
    vector<mdsize> members;
  public:
    Sample() {
      this->home = medusa::snan();
      this->unit = medusa::snan();
      this->distance = medusa::rnan();
    };
    ~Sample() {};
  };

  /*
   *
   */
  class Variable {
  private:
    vector<mdreal> sorted;
    vector<mdreal> data;
    vector<mdreal> ranks;
  public:
    void append(const mdreal);
    void restore(vector<mdreal>&);
    vector<mdreal> transform();
  };

  /*
   *
   */
  class Buffer {
  public:
    Topology structure;
    vector<Point> points;
    vector<Sample> samples;
    map<string, mdsize> key2rank;
  public:
    Buffer() {};
    Buffer(const void* ptr) {
      Buffer* p = (Buffer*)ptr;
      this->structure = p->structure;
      this->points = p->points;
      this->samples = p->samples;
      this->key2rank = p->key2rank;
    };
    ~Buffer() {};
    string insert(const string&, const mdreal);
  };

  /*
   *
   */
  class ModelBuffer : public Buffer {
  private:
    vector<double> dsums;
    vector<double> wsums;
  public:
    Matrix data;
    mdsize nvars;
    mdreal sigma;
    vector<vector<vector<mdreal> > > codebook;
  private:
    void calcdist(const vector<vector<mdreal> >&,
		  const vector<mdreal>&, const mdreal);
  public:
    ModelBuffer() : Buffer() {
      this->nvars = 0;
      this->sigma = 1.0;
    };
    ModelBuffer(const void* ptr) : Buffer(ptr) {
      ModelBuffer* p = (ModelBuffer*)ptr;
      this->data = p->data;
      this->nvars = p->nvars;
      this->sigma = p->sigma;
      this->codebook = p->codebook;
    };
    ~ModelBuffer() {};
    void match(Sample&);
    string update(const mdreal);
  };

  /*
   *
   */
  class EngineBuffer : public Buffer {
  public:
    mdsize nloci;
    vector<mdsize> bmus;
    vector<Site> layers;
    vector<mdsize> loci;
    vector<mdreal> values;
    vector<Variable> variables;
    vector<vector<mdreal> > freq;
  public:
    EngineBuffer() : Buffer() {this->nloci = 0;};
    EngineBuffer(const void* ptr) : Buffer(ptr) {
      EngineBuffer* p = (EngineBuffer*)ptr;
      this->nloci = p->nloci;
      this->bmus = p->bmus;
      this->layers = p->layers;
      this->loci = p->loci;
      this->variables = p->variables;
      this->freq = p->freq;
    };
    ~EngineBuffer() {};
    void organize();
    void organize(const vector<mdsize>&);
  };
};

using namespace koho_local;

#endif /* koho_local_INCLUDED */
