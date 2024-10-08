/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "medusa.local.h"
#include <Rcpp.h>

/*
 *
 */
void
medusa::panic(const string& msg, const char* fname, const int lnum) {
  mdsize len = msg.size();
  if(len > 0) {
    Rcpp::Rcout << ("\nMessage: " + msg + "\n");
    Rcpp::Rcout << ("File: " + string(fname) + "\n");
    Rcpp::Rcout << ("Line: " + long2string(lnum) + "\n");
  }
  Rcpp::stop("Panic!");
}
