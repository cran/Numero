/* Created by Ville-Petteri Makinen 2017
   South Australian Health and Medical Research Institute */

#ifndef nro_INCLUDED
#define nro_INCLUDED

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cfloat>
#include <cctype>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <Rcpp.h>
#include <map>
#include <set>
#include "medusa.h"
#include "abacus.h"
#include "scriptum.h"
#include "punos.h"
#include "koho.h"

#define nro_DEFAULT_LEVEL 0
#define nro_DEFAULT_STAMP 1.0
#define nro_NMAX_FALSE 20

using namespace std;
using namespace Rcpp;
using namespace medusa;
using namespace abacus;
using namespace scriptum;
using namespace punos;
using namespace koho;

/*
 *
 */
namespace nro {

  /* Format conversions. */
  extern vector<vector<mdreal> > matrix2reals( const SEXP& );
  extern NumericMatrix reals2matrix(const vector<vector<mdreal> >& );
  extern punos::Topology reals2topology( const vector<vector<mdreal> >& );
  extern NumericVector reals2vector(const vector<mdreal>& );
  extern vector<mdreal> vector2reals( const SEXP& );
  extern vector<mdsize> vector2sizes( const SEXP& );
 
  /* Utility functions. */
  extern koho::Model model(vector<string>&, const punos::Topology&,
			   const vector<vector<mdreal> >&,
			   const vector<vector<mdreal> >& );
  extern koho::Engine engine(const punos::Topology&,
			     const vector<mdreal>&,
			     const vector<mdsize>& );
};

using namespace nro;

#endif /* nro_INCLUDED */

