/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "punos.local.h"

static string load_levels(TopologyBuffer*, File&);
static string load_coord(TopologyBuffer*, File&);
static string load_sigma(TopologyBuffer*, File&);

/*
 *
 */
string
Topology::import(const string& fname) {

  /* Discard previous contents. */
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  delete p; p = new TopologyBuffer();
  this->buffer = p;

  /* Open file. */
  File f; f.open(fname, "r");
  if(f.error().size() > 0) return f.error();

  /* Import vertical levels. */
  string err = load_levels(p, f);
  if(err.size() > 0) return err;

  /* Import node coordinates. */
  err = load_coord(p, f);
  if(err.size() > 0) return err;

  /* Import neighborhood radius. */
  err = load_sigma(p, f);
  if(err.size() > 0) return err;

  /* Set neighborhood radius. */
  if(this->rewire(p->sigma)) return "";
  return "Unusable neighborhood radius.";
}

/*
 *
 */
string
load_levels(TopologyBuffer* p, File& f) {
  mdreal rlnan = medusa::rnan();

  /* Find first row. */
  while(f.error().size() < 1) {
    vector<string> fields = f.read('\t', 1);
    if(fields[0] == "LEVEL") break;
  }

  /* Import data rows. */
  vector<mdreal> levels;
  while(f.error().size() < 1) {
    vector<string> fields = f.read('\t', 0);
    if(fields.size() != 1) break;
    mdreal z = string2real(fields[0]);
    if(z == rlnan) return "Unusable level.";
    levels.push_back(z);
  }

  /* Check if enough data. */
  mdsize nlevels = levels.size();
  if(nlevels < 1) return "No levels.";
  if(nlevels != uniqreal(levels).size())
    return "Duplicate levels.";

  /* Make sure levels are sorted. */
  sort(levels.begin(), levels.end());
  p->levels = levels;
  return "";
}

/*
 *
 */
string
load_coord(TopologyBuffer* p, File& f) {
  mdreal rlnan = medusa::rnan();

  /* Find first row. */
  while(f.error().size() < 1) {
    vector<string> fields = f.read('\t', 1);
    if(fields[0] == "UNIT") break;
  }

  /* Import data rows. */
  vector<Unit> units; Unit u;
  while(f.error().size() < 1) {
    vector<string> fields = f.read('\t', 0);
    if(fields.size() != 7) break;

    /* Convert to real values. */
    vector<mdreal> values(fields.size());
    for(mdsize j = 0; j < fields.size(); j++) {
      values[j] = string2real(fields[j]);
      if(values[j] == rlnan) return "Unusable value.";
    }

    /* Check unit identity. */
    mdsize key = (mdsize)(values[0] + 0.5);
    if(key != units.size()) return "Inconsistent unit data.";
    
    /* Store unit. */
    u.x = values[1];
    u.y = values[2];
    u.radii.first = values[3];
    u.radii.second = values[4];
    u.angles.first = values[5];
    u.angles.second = values[6];
    units.push_back(u);
  }

  /* Check if any data. */
  if(units.size() < 1) return "No units.";

  /* Determine map radius. */
  mdreal rmax = rlnan;
  for(mdsize i = 0; i < units.size(); i++) {
    if(rmax == rlnan) rmax = units[i].radii.second;
    if(units[i].radii.second < rmax) continue;
    rmax = units[i].radii.second;
  }
  
  /* Update object. */
  p->maxradius = rmax;
  p->coord = units;
  return "";
}

/*
 *
 */
string
load_sigma(TopologyBuffer* p, File& f) {
  mdreal rlnan = medusa::rnan();
  while(f.error().size() < 1) {
    vector<string> fields = f.read('\t', 1);
    if(fields[0] != "SIGMA") continue;
    fields = f.read('\t', 0);
    if(fields.size() != 1) return "Incompatible file.";
    mdreal sigma = string2real(fields[0]);
    if(sigma == rlnan) return "Unusable neighborhood radius.";
    if(sigma <= 0.0) return "Non-positive neighborhood radius.";
    p->sigma = sigma;
    return "";
  }
  return "No neighborhood radius.";
}
