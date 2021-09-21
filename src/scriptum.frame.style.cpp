/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "scriptum.local.h"

/*
 *
 */
Style
Frame::style() const {
  FrameBuffer* p = (FrameBuffer*)buffer;
  return p->style;
}
