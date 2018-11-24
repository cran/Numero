/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "koho.local.h"

/*
 *
 */
void
Model::connect(Messenger* ptr) {
  ModelBuffer* p = (ModelBuffer*)(this->buffer);
  if(ptr != NULL) p->msg = ptr;
  else p->msg = Messenger::null();
}
