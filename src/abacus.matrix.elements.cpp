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
  const unordered_map<mdsize, Array>& rowdata = p->rowdata;
  
  /* Collect elements. */
  vector<Element> elem;
  for(unordered_map<mdsize, Array>::const_iterator it = rowdata.begin();
      it != rowdata.end(); it++)
    (it->second).elements(elem, it->first);

  /* Sort according to value. */
  if(flag == 0) return elem;
  ElementComparator cmp(flag);
  sort(elem.begin(), elem.end(), cmp);
  return elem;
}
