/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "punos.local.h"

/*
 *
 */
Topology::Topology() {
  buffer = new TopologyBuffer();
}

/*
 *
 */
Topology::Topology(const vector<mdreal>& zpos,
		   const mdsize ncircles) {
  TopologyBuffer* p = new TopologyBuffer();
  mdreal rlnan = medusa::rnan();

  /* Check level positions. */
  if(zpos.size() < 1) return;
  if(zpos[0] == rlnan) return;
  for(mdsize k = 1; k < zpos.size(); k++) {
    if(zpos[k] <= zpos[k-1]) return;
    if(zpos[k] == rlnan) return;
  }

  /* First unit. */
  Unit unit;
  double rad = 1.05/sqrt(M_PI);
  unit.x = 0.0;
  unit.y = 0.0;
  unit.radii.first = 0.0;
  unit.radii.second = rad;
  unit.angles.first = 0.0;
  unit.angles.second = 360;
  vector<Unit> units(1, unit);
  
  /* Create a concentric circular lattice. */
  while(rad < ncircles) {
    double rA = rad;
    double rB = (rA + 1.0);
    mdsize n_div = (mdsize)(M_PI*(rB*rB - rA*rA) + 0.5);
    unit.radii.first = rA;
    unit.radii.second = rB;
    for(mdsize i = 0; i < n_div; i++) {
      double phiA = (360.0*i/n_div - (11*n_div)%180);
      double phiB = (360.0*(i + 1)/n_div - (11*n_div)%180);
      unit.x = 0.5*(rA + rB)*cos(M_PI*(phiA + phiB)/360.0);
      unit.y = 0.5*(rA + rB)*sin(M_PI*(phiA + phiB)/360.0);
      unit.angles.first = phiA;
      unit.angles.second = phiB;
      units.push_back(unit);
    }
    rad = rB;
  }

  /* Fine-tune coordinates to ensure correct surface area. */
  mdsize nunits = units.size();
  double scale = sqrt(nunits/M_PI)/rad;
  for(mdsize i = 0; i < nunits; i++) {
    Unit& u = units[i];
    u.x *= scale;
    u.y *= scale;
    u.radii.first *= scale;
    u.radii.second *= scale;
  }

  /* Determine maximum radius. */
  mdreal rmax = rlnan;
  for(mdsize i = 0; i < nunits; i++) {
    mdreal r = units[i].radii.second;
    if(rmax == rlnan) rmax = r;
    if(rmax < r) rmax = r;
  }

  /* Update object. */
  p->maxradius = rmax;
  p->levels = zpos;
  p->coord = units;
  this->buffer = p;
  this->rewire(1.0);
}

/*
 *
 */
Topology::Topology(const vector<mdreal>& zpos,
		   const vector<Unit>& units) {
  TopologyBuffer* p = new TopologyBuffer();
  mdreal rlnan = medusa::rnan();

  /* Check level positions. */
  if(zpos.size() < 1) return;
  if(zpos[0] == rlnan) return;
  for(mdsize k = 1; k < zpos.size(); k++) {
    if(zpos[k] <= zpos[k-1]) return;
    if(zpos[k] == rlnan) return;
  }

  /* Determine maximum radius. */
  mdreal rmax = rlnan;
  for(mdsize i = 0; i < units.size(); i++) {
    mdreal r = units[i].radii.second;
    if(rmax == rlnan) rmax = r;
    if(rmax < r) rmax = r;
  }

  /* Update object. */
  p->maxradius = rmax;
  p->levels = zpos;
  p->coord = units;
  this->buffer = p;
  this->rewire(1.0);
}

/*
 *
 */
Topology::Topology(const Topology& a) {
  this->buffer = new TopologyBuffer(a.buffer);
}

/*
 *
 */
void
Topology::operator=(const Topology& a) {
  TopologyBuffer* p = (TopologyBuffer*)buffer; delete p;
  this->buffer = new TopologyBuffer(a.buffer);
}

/*
 *
 */
Topology::~Topology() {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  delete p;
}
