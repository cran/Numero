/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "abacus.local.h"

/*
 *
 */
mdsize
Matrix::order() const {
  MatrixBuffer* p = (MatrixBuffer*)buffer;
  return p->ncols;
}
