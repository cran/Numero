/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "koho.local.h"

/*
 *
 */
Topology
Model::topology() const {
  ModelBuffer* p = (ModelBuffer*)buffer;
  return p->topology;
}
