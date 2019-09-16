/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen */

#include "scriptum.local.h"

/*
 *
 */
string
Frame::flush() {
  FrameBuffer* p = (FrameBuffer*)buffer;
  while(this->group() > 0) {}
  return p->flush();
}
