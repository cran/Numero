/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
static bool
test_overlap(const Vertex& v, const vector<Vertex>& vtx) {
  mdsize nvert = vtx.size();
  for(set<mdsize>::const_iterator it = v.incoming.begin();
      it != v.incoming.end(); it++)
    if(vtx[*it].community < nvert) return true;
  for(set<mdsize>::const_iterator it = v.outgoing.begin();
      it != v.outgoing.end(); it++)
    if(vtx[*it].community < nvert) return true;
  return false;
}

/*
 *
 */
bool
Triangle::overlap(const vector<Vertex>& vtx) const {
  mdsize nvert = vtx.size();

  /* Check integrity. */
  if(origin >= nvert) panic("Bad triangle.", __FILE__, __LINE__);
  if(alpha >= nvert) panic("Bad triangle.", __FILE__, __LINE__);
  if(beta >= nvert) panic("Bad triangle.", __FILE__, __LINE__);

  /* Check if direct overlap. */
  if(vtx[origin].community < nvert) return true;
  if(vtx[alpha].community < nvert) return true;
  if(vtx[beta].community < nvert) return true;

  /* Check neighborhood. */
  if(test_overlap(vtx[origin], vtx)) return true;
  if(test_overlap(vtx[alpha], vtx)) return true;
  if(test_overlap(vtx[beta], vtx)) return true;
  return false;
}
