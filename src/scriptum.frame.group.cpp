/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen */

#include "scriptum.local.h"

/*
 *
 */
mdsize
Frame::group(const int flag) {
  FrameBuffer* p = (FrameBuffer*)buffer;
  if(flag > 0) {
    p->append("\n<g>");
    p->ngroups += 1;
  }
  if((flag < 0) && (p->ngroups > 0)) { 
    p->append("\n</g>");
    p->ngroups -= 1;
  }
  return p->ngroups;
}
