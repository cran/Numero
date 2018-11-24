/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen */

#include "scriptum.local.h"

/*
 *
 */
Style
Frame::style() const {
  FrameBuffer* p = (FrameBuffer*)buffer;
  return p->style;
}
