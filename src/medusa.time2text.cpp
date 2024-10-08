/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "medusa.local.h"

/*
 * Input is time in seconds.
 */
string
medusa::time2text(const mdreal t) {
  double sec = t;
  double min = t/60;
  double hrs = t/60/60;
  double day = t/60/60/24;
  char buf[1024];

  /* Report days and hours. */
  if(day >= 1.0) {
    hrs -= ((unsigned long)day)*24;
    snprintf(buf, sizeof(buf), "%ldd %ldh", (unsigned long)day,
	    (unsigned long)(hrs + 0.5));
    return string(buf);
  }
  
  /* Report hours and minutes. */
  if(hrs >= 1.0) {
    min -= ((unsigned long)hrs)*60;
    snprintf(buf, sizeof(buf), "%ldh %ldm", (unsigned long)hrs,
	    (unsigned long)(min + 0.5));
    return string(buf);
  }
  
  /* Report minutes and seconds. */
  if(min >= 1.0) {
    sec -= ((unsigned long)min)*60;
    snprintf(buf, sizeof(buf), "%ldm %lds", (unsigned long)min,
	    (unsigned long)(sec + 0.5));
    return string(buf);
  }

  /* Report seconds . */
  if(sec < 1.0) snprintf(buf, sizeof(buf), "<1s");
  else snprintf(buf, sizeof(buf), "%lds", (unsigned long)(sec + 0.5));
  return string(buf);
}
