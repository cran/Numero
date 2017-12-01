/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
class ElementComparator {
private:
  int flag;
public:
  ElementComparator() {this->flag = 0;};
  ElementComparator(const int k) {this->flag = k;};
  ~ElementComparator() {};
  bool operator()(const Element& x, const Element& y) {
    if(flag > 0) return (x.value < y.value);
    if(flag < 0) return (x.value > y.value);
    panic("Bad parameter.", __FILE__, __LINE__);
    return false;
  };
};

/*
 *
 */
vector<Element>
Matrix::elements(const int flag) const {
  MatrixBuffer* p = (MatrixBuffer*)buffer;

  /* Collect elements. */
  map<Key, mdreal>::const_iterator pos;
  const map<Key, mdreal>& dat = p->data;
  vector<Element> array; array.reserve(dat.size());
  for(pos = dat.begin(); pos != dat.end(); pos++) {
    mdsize r = (pos->first)/(p->nline);
    mdsize c = (pos->first)%(p->nline);
    Element e = {r, c, pos->second};
    array.push_back(e);
  }

  /* Sort according to value. */
  if(flag == 0) return array;
  ElementComparator cmp(flag);
  sort(array.begin(), array.end(), cmp);
  return array;
}
