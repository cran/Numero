/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
mdsize
Network::agglomerate() {
  NetworkBuffer* p = (NetworkBuffer*)buffer;
  vector<Vertex>& vtx = p->vertices;
  mdsize sznan = medusa::snan();

  /* Reset community assignments. */
  (p->communities).clear();
  for(mdsize i = 0; i < vtx.size(); i++)
    vtx[i].community = sznan;

  /* Find and sort triangles according to power. */
  vector<mdsize> trifree;
  set<Triangle> tri = p->triangles(trifree);

  /* Set seed triangles (no overlap via neighbors). */
  for(set<Triangle>::iterator it = tri.begin();
      it != tri.end(); it++) {
    if(it->overlap(vtx)) continue;
    vtx[it->origin].community = it->origin;
    vtx[it->alpha].community = it->origin;
    vtx[it->beta].community = it->origin;
  }

  /* Add triangles to seeds. */
  for(set<Triangle>::iterator it = tri.begin();
      it != tri.end(); it++) {
    mdsize c0 = vtx[it->origin].community;
    mdsize cA = vtx[it->alpha].community;
    mdsize cB = vtx[it->beta].community;
    if(c0 == sznan) c0 = cA;
    if(c0 == sznan) c0 = cB;
    if(cA == sznan) cA = c0;
    if(cB == sznan) cB = c0;
    if(cA != c0) continue;
    if(cB != c0) continue;
    vtx[it->origin].community = c0;
    vtx[it->alpha].community = cA;
    vtx[it->beta].community = cB;
  }

  /* Reduce memory footprint. */
  tri.clear();
  
  /* A triangle-free graph comprises a single "community". */
  for(mdsize i = 0; i < trifree.size(); i++) {
    mdsize seed = sznan; /* strongest vertex */
    set<mdsize>& members = p->components[trifree[i]];
    for(set<mdsize>::iterator it = members.begin();
	it != members.end(); it++) {
      if(seed == sznan) seed = *it;
      mdreal x = vtx[*it].strength(p->graph, 0);
      mdreal y = vtx[seed].strength(p->graph, 0);
      if(x > y) seed = *it;
    }
    for(set<mdsize>::iterator it = members.begin();
	it != members.end(); it++)
      vtx[*it].community = seed;
  }

  /* Collect unassigned vertices. */
  vector<string> unassigned;
  for(mdsize i = 0; i < vtx.size(); i++) {
    if(vtx[i].community != sznan) continue;
    unassigned.push_back(vtx[i].name());
  }

  /* Create a sorted pool of vertices. */
  vector<mdsize> pool;
  sort(unassigned.begin(), unassigned.end());
  for(vector<string>::iterator it = unassigned.begin();
      it != unassigned.end(); it++)
    pool.push_back(p->name2rank[*it]);
  
  /* Assign vertices to communities. */
  while(pool.size() > 0) {
    vector<mdsize> subset;
    for(vector<mdsize>::iterator it = pool.begin();
	it != pool.end(); it++) {
      mdsize c = vtx[*it].join(p->graph, vtx, 0);
      if(c >= vtx.size()) subset.push_back(*it);
    }
    if(subset.size() == pool.size()) break;
    pool = subset;
  }

  /* Collect communities. */
  unordered_map<mdsize, set<mdsize> >& comts = p->communities;
  for(mdsize i = 0; i < vtx.size(); i++) {
    mdsize c = vtx[i].community;
    if(c == sznan) panic("Bad community.", __FILE__, __LINE__);
    comts[c].insert(i);
  }

  /* Determine community hubs. */
  for(unordered_map<mdsize, set<mdsize> >::iterator it = comts.begin();
      it != comts.end(); it++) {
    set<mdsize>& members = it->second;
    mdsize hub = p->concentrate(it->second);
    for(set<mdsize>::iterator jt = members.begin();
	jt != members.end(); jt++)
      vtx[*jt].community = hub;
  }

  /* Update community labels. */
  comts.clear();
  for(mdsize i = 0; i < vtx.size(); i++) {
    mdsize c = vtx[i].community;
    if(c == sznan) panic("Bad community.", __FILE__, __LINE__);
    comts[c].insert(i);
  }
  return comts.size();
}
