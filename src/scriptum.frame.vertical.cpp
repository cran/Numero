/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen */

#include "scriptum.local.h"

/*
 *
 */
pair<mdreal, mdreal>
Frame::vertical() const {
  FrameBuffer* p = (FrameBuffer*)buffer;
  pair<mdreal, mdreal> res;
  res.first = (p->limits).second.alpha;
  res.second = (p->limits).second.omega;
  return res;
}
