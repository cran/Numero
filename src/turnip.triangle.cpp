/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
Triangle::Triangle() {
  this->origin = medusa::snan();
  this->alpha = origin;
  this->beta = origin;
  this->power = 0.0;
}

/*
 *
 */
Triangle::Triangle(const mdsize orig, const mdsize a,
		   const mdsize b, const Matrix& graph) {
  
  /* Set corner vertices. */
  this->origin = orig;
  this->alpha = a;
  this->beta = b;

  /* Collect edge weights. */
  vector<mdreal> weights(6);
  weights[0] = graph.value(orig, a);
  weights[1] = graph.value(orig, b);
  weights[2] = graph.value(a, orig);
  weights[3] = graph.value(a, b);
  weights[4] = graph.value(b, orig);
  weights[5] = graph.value(b, a);
  
  /* Estimate power. */
  this->power = abacus::statistic(weights, "center");
}

/*
 *
 */
Triangle::~Triangle() {}

/*
 *
 */
bool
Triangle::operator<(const Triangle& t) const {
  if(this->power == t.power) {
    if(this->origin < t.origin) return true;
    if(this->alpha < t.alpha) return true;
    if(this->beta < t.beta) return true;
  }
  return (this->power > t.power);
}
