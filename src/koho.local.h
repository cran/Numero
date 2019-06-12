/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#ifndef koho_local_INCLUDED
#define koho_local_INCLUDED

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstring>
#include <climits>
#include <random>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "medusa.h"
#include "akkad.h"
#include "abacus.h"
#include "punos.h"
#include "koho.h"

using namespace std;
using namespace medusa;
using namespace akkad;
using namespace abacus;
using namespace punos;
using namespace koho;


/* Encapsulate with redundant namespace in case in a collection
   of modules another module has the same class name(s) in use. */
namespace koho_local {

  /*
   *
   */
  class Point {
  private:
    mdsize key;
    mdsize home;
    vector<mdreal> contents;
  public:
    Point();
    Point(const mdsize, const mdsize);
    Point(const mdsize, const vector<mdreal>&);
    ~Point();
    vector<mdreal> data() const;
    mdsize location() const;
    void move(const mdsize);
    mdsize rank() const;
  };

  /*
   *
   */
  class Buffer {
  public:
    Topology topology;
    unordered_map<string, Point> points;
    Messenger* msg;
  public:
    Buffer() {this->msg = Messenger::null();};
    Buffer(const void* ptr) {
      Buffer* p = (Buffer*)ptr;
      this->topology = p->topology;
      this->points = p->points;
      this->msg = p->msg;
    };
    ~Buffer() {};
  }; 
  
  /*
   *
   */
  typedef map<mdreal, vector<Point*> > ContentMap;
  class Subset {
  private:
    mdsize label;
    mdsize capacity;
    mdsize occupancy;
    ContentMap contents;
  public:
    Subset();
    ~Subset();
    void configure(const mdsize, const mdsize);
    void clear();
    Point* join(Point*, const mdreal);
    vector<mdsize> moments(vector<mdreal>&, vector<mdreal>&) const;
    mdsize size() const;
    static Point* match(vector<Subset>&, const vector<mdreal>&, Point*);
  };
  
  /*
   *
   */
  class Trainer {
  private:
    char formula;
    vector<Subset> subsets;
    vector<vector<mdreal> > prototypes;
  private:
    mdreal match(vector<Point*>&, const Topology&);
    void update(const Topology&);
  public:
    Trainer();
    Trainer(const Matrix&, const Topology&, const mdsize,
	    const mdreal, const string&);
    ~Trainer();
    mdreal cycle(vector<Point*>&, const Topology&);
    Matrix codebook() const;
    mdreal distance(const Point&, const mdsize) const;
    vector<mdreal> distances(const Point&) const;
    mdsize size() const;
    static mdreal euclid(const vector<mdreal>&, const vector<mdreal>&);
    static mdreal pearson(const vector<mdreal>&, const vector<mdreal>&);
  };
  
  /*
   *
   */
  class ModelBuffer : public Buffer {
  public:
    mdsize ntrain;
    mdreal equality;
    string metric;
    Matrix codebook;
    Trainer trainer;
  public:
    ModelBuffer() : Buffer() {
      this->ntrain = medusa::snan();
      this->equality = 0.0;
      this->metric = "euclid";
    };
    ModelBuffer(const void* ptr) : Buffer(ptr) {
      ModelBuffer* p = (ModelBuffer*)ptr;
      this->ntrain = p->ntrain;
      this->equality = p->equality;
      this->metric = p->metric;
      this->codebook = p->codebook;
      this->trainer = p->trainer;
    };
    ~ModelBuffer() {};
  };

  
  /*
   *
   */
  class Transformation {
  private:
    vector<mdreal> lookup;
    vector<mdreal> output;
  private:
    void process(vector<mdreal>&, const vector<mdreal>&,
		 const vector<mdreal>&) const;
  public:
    Transformation();
    Transformation(const vector<mdreal>&);
    ~Transformation();
    void restore(vector<mdreal>&) const;
    void transform(vector<mdreal>&) const;
    mdsize size() {return lookup.size();};
  };

  /*
   *
   */
  struct ColumnCache {
    Transformation transf;
    vector<mdreal> values;
  };

  /*
   *
   */
  class EngineBuffer : public Buffer {
  public:
    bool complete;
    mt19937 twister;
    pair<vector<mdsize>, vector<mdsize> > bmus;
    vector<vector<mdreal> > freqs;
    vector<ColumnCache> cache;
    Matrix data;
  public:
    EngineBuffer() : Buffer() {
      this->complete = true;
    };
    EngineBuffer(const void* ptr) : Buffer(ptr) {
      EngineBuffer* p = (EngineBuffer*)ptr;
      this->complete = p->complete;
      this->twister = p->twister;
      this->bmus = p->bmus;
      this->freqs = p->freqs;
      this->cache = p->cache;
      this->data = p->data;
    };
    ~EngineBuffer() {};
    void prepare();
  };
};

using namespace koho_local;

#endif /* koho_local_INCLUDED */
