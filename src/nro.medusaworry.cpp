/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "medusa.local.h"
#include <Rcpp.h>

/*
 * Print a warning message.
 */
void
medusa::worry(const string& msg, const char* fname) {
  mdsize len = msg.size();
  if(len < 1) return;
  Rcpp::Rcout << ("\nMessage: " + msg + "\n");
  Rcpp::Rcout << ("File: " + string(fname) + "\n");
}
