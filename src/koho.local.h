/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

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
  class Point {
  private:
    mdsize key;
    mdsize home;
    vector<mdreal> contents;
  public:
    Point();
    Point(const mdsize, const vector<mdreal>&, const mdsize);
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
    map<string, Point> points; /* data as rows */
  public:
    Buffer() {};
    Buffer(const void* ptr) {
      Buffer* p = (Buffer*)ptr;
      this->topology = p->topology;
      this->points = p->points;
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
    vector<Subset> subsets;
    vector<vector<mdreal> > prototypes;
  private:
    void allocate(const mdsize, const mdsize);
  public:
    Trainer();
    Trainer(const Matrix&, const Topology&, const mdsize, const mdreal);
    ~Trainer();
    Matrix codebook() const;
    vector<mdreal> distance(const Point&) const;
    mdreal distance(const Point&, const mdsize) const;
    mdreal match(vector<Point*>&, const Topology&);
    mdsize size() const;
    void update(Topology&, const mdreal);
  };

  /*
   *
   */
  class ModelBuffer {
  public:
    mdsize ntrain;
    mdreal equality;
    mt19937 twister;
    Matrix codebook;
    Trainer trainer;
    Topology topology;
    vector<mdreal> trace;
    map<string, mdreal> state;
    map<string, Point> points; /* data as rows */
  public:
    ModelBuffer() {
      this->ntrain = medusa::snan();
      this->equality = 0.0;
    };
    ModelBuffer(const void* ptr) {
      ModelBuffer* p = (ModelBuffer*)ptr;
      this->ntrain = p->ntrain;
      this->equality = p->equality;
      this->twister = p->twister;
      this->codebook = p->codebook;
      this->trainer = p->trainer;
      this->topology = p->topology;
      this->trace = p->trace;
      this->state = p->state;
      this->points = p->points;
    };
    ~ModelBuffer() {};
  };

  /*
   *
   */
  class EngineBuffer : public Buffer {
  public:
    bool complete;
    mdsize order;
    mt19937 twister;
    pair<vector<mdsize>, vector<mdsize> > bmus;
    vector<vector<mdreal> > freqs;
    vector<vector<mdreal> > cache; /* data as columns */
  public:
    EngineBuffer() : Buffer() {
      this->order = 0;
      this->complete = true;
    };
    EngineBuffer(const void* ptr) : Buffer(ptr) {
      EngineBuffer* p = (EngineBuffer*)ptr;
      this->complete = p->complete;
      this->order = p->order;
      this->twister = p->twister;
      this->bmus = p->bmus;
      this->freqs = p->freqs;
      this->cache = p->cache;
    };
    ~EngineBuffer() {};
    void prepare();
  };
}

using namespace koho_local;

#endif /* koho_local_INCLUDED */
