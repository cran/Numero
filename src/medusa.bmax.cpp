/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "medusa.local.h"

/*
 *
 */
mdbyte
medusa::bmax() {
  if(sizeof(mdbyte) == sizeof(char)) return (mdbyte)UCHAR_MAX;
  if(sizeof(mdbyte) == sizeof(short)) return (mdbyte)USHRT_MAX;
  if(sizeof(mdbyte) == sizeof(int)) return (mdbyte)UINT_MAX;
  panic("Unusable precision.", __FILE__, __LINE__);
  return 0;
}
