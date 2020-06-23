/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
mdsize
Cohesion::order() const {
  CohesionBuffer* p = (CohesionBuffer*)buffer;
  return (p->modules).size();
}
