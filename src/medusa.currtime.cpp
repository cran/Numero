/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "medusa.local.h"

/*
 * Return current ASCII time without a newline at the end.
 */
string
medusa::currtime() {
  time_t now = time(NULL);
  string s(ctime(&now));
  return s.substr(0, (s.size() - 1));
}
