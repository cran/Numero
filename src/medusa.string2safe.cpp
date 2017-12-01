/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "medusa.local.h"

/*
 * Replaces most non-alphanumeric characters.
 * Truncates the string to the given maximum length. 
 */
string
medusa::string2safe(const std::string& orig, const mdsize maxlen) {

  /* Replace characters. */  
  string s = orig;
  for(mdsize i = 0; i < orig.size(); i++) {
    char c = orig[i];
    if(isalnum(c)) continue;
    switch(c) {
    case '-': continue;
    case '+': continue;
    case '=': continue;
    case '%': continue;
    case '.': continue;
    case ':': continue;
    case ';': continue;
    case ',': continue;
    case '/': continue;
    case '\\': continue;
    case '@': continue;
    case '(': continue;
    case ')': continue;
    case ' ': continue;
    default: s[i] = '_';
    }
  }
  
  /* Adjust length. */
  if(s.length() > maxlen) {
    s = s.substr(0, maxlen);
    if(maxlen > 1) {
      s[maxlen-1] = '.';
      s[maxlen-2] = '.';
    }
  }
  return s;
}
