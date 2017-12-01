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
Frame::Frame(const Frame& a) {
  this->buffer = new FrameBuffer(a.buffer);
}

/*
 *
 */
void
Frame::operator=(const Frame& a) {
  FrameBuffer* p = (FrameBuffer*)buffer; delete p;
  this->buffer = new FrameBuffer(a.buffer);
}

/*
 *
 */
Frame::~Frame() {
  FrameBuffer* p = (FrameBuffer*)buffer;
  delete p;
}

