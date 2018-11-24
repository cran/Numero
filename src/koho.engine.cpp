/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
Engine::Engine() {
  this->buffer = new EngineBuffer();
}

/*
 *
 */
Engine::Engine(const Topology& tp) {
  EngineBuffer* p = new EngineBuffer();
  p->topology = tp;
  this->buffer = p;
}

/*
 *
 */
Engine::Engine(const Engine& a) {
  this->buffer = new EngineBuffer(a.buffer);
}

/*
 *
 */
void
Engine::operator=(const Engine& a) {
  EngineBuffer* p = (EngineBuffer*)buffer; delete p;
  this->buffer = new EngineBuffer(a.buffer);
}

/*
 *
 */
Engine::~Engine() {
  EngineBuffer* p = (EngineBuffer*)buffer;
  delete p;
}
