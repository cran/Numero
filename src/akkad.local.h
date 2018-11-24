/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#ifndef akkad_local_INCLUDED
#define akkad_local_INCLUDED

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "medusa.h"
#include "akkad.h"

using namespace std;
using namespace medusa;
using namespace akkad;

/* Encapsulate with redundant namespace in case in a collection
   of modules another module has the same class name(s) in use. */
namespace akkad_local {
 
  /*
   *
   */
  class FieldSet {
  private:
    vector<vector<string> > data;
    map<string, mdsize> string2rank;
  public:
    vector<string> operator[](const mdsize) const;
    bool insert(const vector<string>&);
    mdsize size() const {return data.size();};
  };

  /*
   *
   */
  class NullMessenger : public Messenger {
  private:
    vector<unsigned long> nticks;
    vector<time_t> stamp;
  public:
    mdsize alert(const string& s) {return 0;};
    unsigned long counter(const mdsize);
    mdsize print(const string& s) {return 0;};
    mdsize progress(const mdreal r) {return 0;};
    mdsize report(const string& s) {return 0;};
    mdsize warn(const string& s) {return 0;};
    bool stopping() const {return false;};
    unsigned long size() const {return 0;};
    mdreal seconds();
  };

  /*
   *
   */
  typedef map<string, FieldSet> InstrMap;
};

using namespace akkad_local;

#endif /* akkad_local_INCLUDED */
