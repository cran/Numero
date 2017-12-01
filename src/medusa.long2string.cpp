/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

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
