/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "medusa.local.h"

/*
 *
 */
mdreal
medusa::rlim() {
  if(sizeof(mdreal) < sizeof(double)) return FLT_MIN;
  if(sizeof(mdreal) < sizeof(long double)) return DBL_MIN;
  return LDBL_MIN;
}
