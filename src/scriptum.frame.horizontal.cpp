/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen */

#include "scriptum.local.h"

/*
 *
 */
pair<mdreal, mdreal>
Frame::horizontal() const {
  FrameBuffer* p = (FrameBuffer*)buffer;
  pair<mdreal, mdreal> res;
  res.first = (p->limits).first.alpha;
  res.second = (p->limits).first.omega;
  return res;
}
