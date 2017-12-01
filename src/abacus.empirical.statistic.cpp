/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "abacus.local.h"

/*
 *
 */
mdreal
Empirical::statistic(const std::string& name) const {
  EmpiricalBuffer* p = (EmpiricalBuffer*)buffer;
  vector<mdreal> values;
  vector<mdreal> weights;
  p->contents(values, weights);
  return abacus::statistic(values, weights, name);
}
