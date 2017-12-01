/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "medusa.local.h"
#include <Rcpp.h>

/*
 *
 */
void
medusa::panic(const string& msg, const char* fname, const int lnum) {
  mdsize len = msg.size();
  if(len > 0) {

    /* Remove path from file name. */
    char* ptr = strrchr((char*)fname, '/');
    if(ptr == NULL) ptr = strrchr((char*)fname, '\\');
    if(ptr == NULL) ptr = (char*)fname;
    else ptr++;

    /* Show message. */
    char buf[64];
    sprintf(buf, "\r%s at line %d: ", ptr, lnum);
    Rcpp::Rcerr << (string(buf) + msg);
    if(msg[len-1] != '\n') Rcpp::Rcerr << "\n";
  }
  Rcpp::stop("Rcpp::stop() from medusa::panic().");
}
