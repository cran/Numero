/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "medusa.local.h"

/*
 *
 */
mdsize
medusa::snan() {
  if(sizeof(mdsize) == sizeof(short)) return (mdsize)USHRT_MAX;
  if(sizeof(mdsize) == sizeof(int)) return (mdsize)UINT_MAX;
  if(sizeof(mdsize) == sizeof(long)) return (mdsize)ULONG_MAX;
  panic("Unusable precision.", __FILE__, __LINE__);
  return 0;
}
