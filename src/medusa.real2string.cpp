/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "medusa.local.h"

/*
 *
 */
string
medusa::real2string(const mdreal x) {
  char buf[32];
  if(x == medusa::rnan()) return "nan";
  if(x == 0.0) return "0";

  /* Integer. */
  double integer = 0.0;
  double fraction = modf(x, &integer);
  if((fraction == 0.0) && (fabs(integer) < 1e24)) {
    snprintf(buf, sizeof(buf), "%.0f", x);
    return string(buf);
  }

  /* Exponential form. */
  if(sizeof(mdreal) < sizeof(double)) snprintf(buf, sizeof(buf), "%.6e", x);
  else snprintf(buf, sizeof(buf), "%.14e", x);
  
  /* Find excess zeros. */
  char* pos = strchr(buf, 'e');
  char* stop = strchr(buf, '.');
  if(pos == NULL) return "nan";
  if(stop != NULL) stop++;
  for(pos--; (*pos == '0') && (pos != stop); pos--)
    *pos = '\t';
  
  /* Remove excess zeros. */
  mdsize len = strlen(buf); pos = buf;
  for(mdsize i = 0; i <= len; i++) {
    *pos = buf[i];
    if(*pos != '\t') pos++;
  }
  return string(buf);
}
