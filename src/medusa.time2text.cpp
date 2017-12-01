/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

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
    sprintf(buf, "%ld d %ld h", (unsigned long)day,
	    (unsigned long)(hrs + 0.5));
    return string(buf);
  }
  
  /* Report hours and minutes. */
  if(hrs >= 1.0) {
    min -= ((unsigned long)hrs)*60;
    sprintf(buf, "%ld h %ld min", (unsigned long)hrs,
	    (unsigned long)(min + 0.5));
    return string(buf);
  }
  
  /* Report minutes and seconds. */
  if(min >= 1.0) {
    sec -= ((unsigned long)min)*60;
    sprintf(buf, "%ld min %ld s", (unsigned long)min,
	    (unsigned long)(sec + 0.5));
    return string(buf);
  }

  /* Report seconds . */
  if(sec < 1.0) sprintf(buf, "<1 s");
  else sprintf(buf, "%ld s", (unsigned long)(sec + 0.5));
  return string(buf);
}
