/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "medusa.local.h"

/*
 * Convert integers into human readable form.
 */
string
medusa::long2text(const long value) {
  char buffer[16];

  /* Small value. */
  if(value < 1000) {
    sprintf(buffer, "%ld", value);
    return string(buffer);
  }

  /* Collect thousand-sets. */
  vector<int> segments;
  unsigned long k = value;
  while(k > 0) {
    segments.push_back(k%1000);
    k /= 1000;
  }

  /* Finish text. */
  string s;
  for(int i = (int)(segments.size() - 1); i >= 0; i--) {
    if(s.size() < 1) sprintf(buffer, "%d", segments[i]);
    else sprintf(buffer, ",%03d", segments[i]);
    s += string(buffer);
  }
  return s;
}
