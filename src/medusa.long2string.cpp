/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "medusa.local.h"

/*
 *
 */
string
medusa::long2string(const long value) {
  char buf[128];
  sprintf(buf, "%ld", value);
  return string(buf);
}
