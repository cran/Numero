/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

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
