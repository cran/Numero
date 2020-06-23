/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "turnip.local.h"

/*
 *
 */
vector<Edge>
Network::edges() const {
  NetworkBuffer* p = (NetworkBuffer*)buffer;
  vector<Vertex>& vert = p->vertices;
  vector<Element> elem = (p->graph).elements(0);
  vector<Edge> array(elem.size());
  for(mdsize i = 0; i < elem.size(); i++) {
    Edge& e = array[i];
    e.tail = vert[elem[i].row].name();
    e.head = vert[elem[i].column].name();
    e.weight = elem[i].value;
  }
  return array;
}

/*
 *
 */
vector<Edge>
Network::edges(const vector<string>& keysA, const vector<string>& keysB,
	       const int dirflag) const {
  NetworkBuffer* p = (NetworkBuffer*)buffer;
  vector<Edge> array;

  /* Find vertices. */
  unordered_set<mdsize> ranksA;
  unordered_set<mdsize> ranksB;
  unordered_map<string, mdsize>::iterator pos;
  unordered_map<string, mdsize>& name2rank = p->name2rank;
  for(mdsize i = 0; i < keysA.size(); i++)
    if((pos = name2rank.find(keysA[i])) != name2rank.end())
      ranksA.insert(pos->second);
  for(mdsize i = 0; i < keysB.size(); i++)
    if((pos = name2rank.find(keysB[i])) != name2rank.end())
      ranksB.insert(pos->second);

  /* Find edges between A to B. */
  Matrix& graph = p->graph;
  vector<Vertex>& vtx = p->vertices;
  for(unordered_set<mdsize>::iterator itA = ranksA.begin();
      itA != ranksA.end(); itA++) {
    Vertex& vA = vtx[*itA];
    Edge e;

    /* From B to A. */
    if(dirflag <= 0) {
      e.head = vA.name();
      for(set<mdsize>::iterator itB = vA.incoming.begin();
	  itB != vA.incoming.end(); itB++) {
	if(ranksB.count(*itB) < 1) continue;
	Vertex& vB = vtx[*itB]; 
	e.tail = vB.name();
	e.weight = graph.value(*itB, *itA);
	array.push_back(e);
      }
    }

    /* From A to B. */
    if(dirflag >= 0) {
      e.tail = vA.name();
      for(set<mdsize>::iterator itB = vA.outgoing.begin();
	  itB != vA.outgoing.end(); itB++) {
	if(ranksB.count(*itB) < 1) continue;
	Vertex& vB = vtx[*itB];
	e.head = vB.name();
	e.weight = graph.value(*itA, *itB);
	array.push_back(e);
      }
    }
  }
  return array;
}
