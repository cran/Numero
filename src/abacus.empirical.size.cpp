/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "abacus.local.h"

/*
 *
 */
mdsize
Empirical::size() const {
  EmpiricalBuffer* p = (EmpiricalBuffer*)buffer;
  return p->ndata;
}
