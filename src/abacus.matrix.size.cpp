/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "abacus.local.h"

/*
 *
 */
mdsize
Matrix::size() const {
  MatrixBuffer* p = (MatrixBuffer*)buffer;
  return p->nrows;
}
