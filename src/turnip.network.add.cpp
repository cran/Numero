/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
string
Network::add(const string& a, const string& b, const mdreal w) {
  NetworkBuffer* p = (NetworkBuffer*)buffer;
  mdsize sznan = medusa::snan();
  mdreal rlnan = medusa::rnan();

  /* Check arguments. */ 
  if(a == "") return "Unusable tail vertex name.";
  if(b == "") return "Unusable head vertex name.";
  if((a == b) && (p->redunflag == false))
    return ("End-points '" + a + "' are the same.");
  if(w == 0.0) return ("Zero weight for '" + a + "'-'" + b + "'.");
  if(w == rlnan) return ("Unusable weight for '" + a + "'-'" + b + "'.");

  /* Find or create vertices. */
  vector<Vertex>& vtx = p->vertices;
  mdsize rankA = p->make(a);
  mdsize rankB = p->make(b);
  Vertex& vA = vtx[rankA];
  Vertex& vB = vtx[rankB];

  /* Both end-points are new. */
  unordered_map<mdsize, set<mdsize> >& comp = p->components;
  if((vA.component == sznan) && (vB.component == sznan)) {
    mdsize crank = vtx.size();
    comp[crank].insert(rankA);
    comp[crank].insert(rankB);
    vA.component = crank;
    vB.component = crank;
  }
  
  /* End-points in different components. */
  while(vA.component != vB.component) {

    /* One end-point is new. */
    if((vA.component != sznan) && (vB.component == sznan)) {
      comp[vA.component].insert(rankB);
      vB.component = vA.component;
      break;
    }
    if((vA.component == sznan) && (vB.component != sznan)) {
      comp[vB.component].insert(rankA);
      vA.component = vB.component;
      break;
    }

    /* Merge two existing components. */
    set<mdsize> membcopyB = comp[vB.component];
    comp[vA.component].insert(membcopyB.begin(), membcopyB.end());
    comp.erase(vB.component);

    /* Update assignments. */
    for(set<mdsize>::iterator it = membcopyB.begin();
	it != membcopyB.end(); it++)
      vtx[*it].component = vA.component;
  }
    
  /* Add a directed weighted edge. */
  (p->graph).add(rankA, rankB, w);
  vA.outgoing.insert(rankB);
  vB.incoming.insert(rankA);
  p->cleanflag = false;
  return "";
}

/*
 *
 */
mdsize
Network::add(const Network& net) {
  NetworkBuffer* p = (NetworkBuffer*)(net.buffer);

  /* Copy edges. */
  mdsize nedges = 0;
  vector<Element> elem = (p->graph).elements(0);
  vector<Vertex>& vtx = p->vertices;
  for(mdsize i = 0; i < elem.size(); i++) {
    string a = vtx[elem[i].row].name();
    string b = vtx[elem[i].column].name();
    string err = this->add(a, b, elem[i].value);
    nedges += (err.size() == 0);
  }

  /* Clear outdated component indexing. */
  p->cleanflag = false;
  return nedges;
}
