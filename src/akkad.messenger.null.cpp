/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "akkad.local.h"

static NullMessenger NullMsg;

/*
 *
 */
Messenger*
Messenger::null() {
  return &NullMsg;
}

/*
 *
 */
unsigned long
NullMessenger::counter(const mdsize k) {
  nticks.resize(1, 0);
  nticks[0] += k;
  return nticks[0];
}

/*
 *
 */
mdreal
NullMessenger::seconds() {
  if(stamp.size() < 1) stamp.resize(2, time(NULL));
  stamp[1] = stamp[0];
  stamp[0] = time(NULL);
  double delta = difftime(stamp[0], stamp[1]);
  return delta;
}

