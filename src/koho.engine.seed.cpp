/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "koho.local.h"

/*
 *
 */
void
Engine::seed(const mdreal base) {
  EngineBuffer* p = (EngineBuffer*)buffer;
  string bytes = medusa::real2string(base);
  seed_seq value(bytes.begin(), bytes.end());
  (p->twister).seed(value);
}
