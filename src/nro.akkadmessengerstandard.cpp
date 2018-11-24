/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "akkad.local.h"
#include <Rcpp.h>

#define PRINTGAP 3.0
#define MAX_ALERT_COUNT 500

/*
 *
 */
class StdMessenger : public NullMessenger {
private:
  char eol;
  mdsize wheel;
  mdsize alertcount;
  mdreal previous;
  unsigned long nbytes;
  time_t stamp;
private:
  mdsize alertExec(FILE*, const string&, const string&);
public:
  void init();
  mdsize alert(const string&);
  mdsize print(const string&);
  mdsize progress(const mdreal);
  mdsize report(const string&);
  mdsize warn(const string&);
  unsigned long size() const;
};

static StdMessenger GlobalMsg;

/*
 *
 */
Messenger*
Messenger::standard() {
  GlobalMsg.init();
  return &GlobalMsg;
}

/*
 *
 */
mdsize
StdMessenger::alert(const string& s) {
  Rcpp::stop("StdMessenger::alert() not implemented.");
  return 0;
}

/*
 *
 */
mdsize
StdMessenger::alertExec(FILE* fid, const string& txt,
			const string& prefix) {
  Rcpp::stop("StdMessenger::alertExec() not implemented.");
  return 0;
}

/*
 *
 */
void
StdMessenger::init() {
  this->eol = '\n';
  this->wheel = 0;
  this->alertcount = 0;
  this->nbytes = 0;
  this->previous = 0.0;
  this->stamp = time(NULL);
}

/*
 *
 */
mdsize
StdMessenger::print(const string& s) {
  Rcpp::stop("StdMessenger::print() not implemented.");
  return 0;
}

/*
 *
 */
mdsize
StdMessenger::progress(const mdreal r0) {
  Rcpp::stop("StdMessenger::progress() not implemented.");
  return 0;
}

/*
 *
 */
mdsize
StdMessenger::report(const string& s) {
  Rcpp::stop("StdMessenger::report() not implemented.");
  return 0;
}

/*
 *
 */
unsigned long
StdMessenger::size() const {
  return nbytes;
}

/*
 *
 */
mdsize
StdMessenger::warn(const string& s) {
  Rcpp::stop("StdMessenger::warn() not implemented.");
  return 0;
}
