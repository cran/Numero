/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
Matrix::Matrix() {
  this->buffer = new MatrixBuffer();
}

/*
 *
 */
Matrix::Matrix(const Matrix& m) {
  this->buffer = new MatrixBuffer(m.buffer);
}

/*
 *
 */
void
Matrix::operator=(const Matrix& m) {
  MatrixBuffer* p = (MatrixBuffer*)buffer; delete p;
  this->buffer = new MatrixBuffer(m.buffer);
}

/*
 *
 */
Matrix::~Matrix() {
  MatrixBuffer* p = (MatrixBuffer*)(this->buffer);
  delete p;
}
