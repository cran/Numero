/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "koho.local.h"

/*
 *
 */
vector<mdreal>
Model::prototype(const mdsize unit) const {
  ModelBuffer* p = (ModelBuffer*)buffer;
  return (p->codebook).row(unit);
}
