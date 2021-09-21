/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "scriptum.local.h"

/*
 *
 */
vector<string>
Frame::shapes() {
  vector<string> array;
  array.push_back("circle");
  array.push_back("clover");
  array.push_back("cross");
  array.push_back("pentagon");
  array.push_back("square");
  array.push_back("star");
  array.push_back("triangle");
  return array;
}
