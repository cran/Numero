/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen */

#include "scriptum.local.h"

/*
 *
 */
Frame::Frame() {
  this->buffer = new FrameBuffer();
}

/*
 *
 */
Frame::Frame(const Frame& t) {
  this->buffer = new FrameBuffer(t.buffer);
}

/*
 *
 */
void
Frame::operator=(const Frame& t) {
  if(this == &t) return;
  FrameBuffer* p = (FrameBuffer*)buffer; delete p;
  this->buffer = new FrameBuffer(t.buffer);
}

/*
 *
 */
Frame::~Frame() {
  FrameBuffer* p = (FrameBuffer*)buffer;
  delete p;
}

