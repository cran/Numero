/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "nro.h"

/*
 *
 */
struct Matching {
  mdsize sample;
  mdsize prototype;
};

/*
 *
 */
RcppExport SEXP
nro_match(SEXP codebook_R, SEXP data_R, SEXP eq_R) {
  mdreal rlnan = medusa::rnan();
  mdreal eq = as<mdreal>(eq_R);

  /* Check prototypes. */
  vector<vector<mdreal> > protos = nro::matrix2reals(codebook_R, 0.0);
  if(protos.size() < 1) return CharacterVector("Empty codebook.");

  /* Check data. */
  vector<vector<mdreal> > vectors = nro::matrix2reals(data_R, 0.0);
  if(vectors.size() < 1) return CharacterVector("Too few data.");

  /* Set map topology (no smoothing). */
  punos::Topology topo(protos.size());
  
  /* Estimate coverage of valid data. */
  vector<mdreal> covers;
  mdsize ncols = vectors[0].size();
  for(mdsize i = 0; i < vectors.size(); i++) {
    mdreal nv = abacus::statistic(vectors[i], "number");
    covers.push_back(nv/ncols);
  }
  
  /* Create a self-organizing map. */
  koho::Model model(topo, vectors.size(), eq); string err;
  for(mdsize k = 0; k < protos.size(); k++) {
    err = model.configure(k, protos[k]);
    if(err.size() > 0) return CharacterVector(err);
  }

  /* Transfer data into the model. */
  for(mdsize i = 0; i < vectors.size(); i++) {
    string key = medusa::long2string(i); /* temporary identifier */
    string err = model.insert(key, vectors[i]);
    if(err.size() > 0) return CharacterVector(err);
    vectors[i].clear(); /* reduce memory footprint */
  }

  /* Apply balancing if necessary. */
  vector<mdsize> bmus;
  vector<mdreal> dist;  
  if(eq > 0.0) {
    if(eq > 1.0) eq = 1.0;

    /* Optimal subset capacities. */
    mdsize ntrain = vectors.size();
    mdsize nunits = topo.size();
    vector<mdsize> caps(nunits, 0);
    for(mdsize i = 0; i < ntrain; i++)
      caps[nunits-(i%nunits)-1] += 1;

    /* Adjust capacities. */
    mdreal w = (exp(-5.0) - exp(-5.0*eq))/(exp(-5.0) - 1.0);
    for(mdsize j = 0; j < nunits; j++)
      caps[j] += (mdsize)(w*(ntrain - caps[j] - nunits));
    
    /* Collect distances to prototypes. */
    map<mdreal, vector<Matching> > matches;
    for(mdsize i = 0; i < vectors.size(); i++) {
      string key = medusa::long2string(i);
      vector<mdreal> delta = model.distance(key);
      for(mdsize j = 0; j < delta.size(); j++) {
	Matching m; m.sample = i; m.prototype = j;
	matches[delta[j]].push_back(m);
      }
    }
    
    /* Assign map locations. */
    bmus.resize(ntrain, 0);
    dist.resize(ntrain, rlnan);  
    for(map<mdreal, vector<Matching> >::iterator it = matches.begin();
	it != matches.end(); it++) {
      for(vector<Matching>::iterator p = (it->second).begin();
	  p != (it->second).end(); p++) {
	mdsize i = p->sample;
	mdsize j = p->prototype;
	if((bmus[i] == 0) && (caps[j] > 0)) {
	  bmus[i] = (j + 1); /* R-style indexing */
	  dist[i] = it->first;
	  caps[j] -= 1;
	}
      }
    }
  } else {

    /* Find best matching units directly. */
    for(mdsize i = 0; i < vectors.size(); i++) {
      string key = medusa::long2string(i);
      vector<mdreal> delta = model.distance(key);
      mdsize bmu = abacus::extrema(delta).first;    
      if(bmu < topo.size()) {
	bmus.push_back(bmu + 1); /* R-style indexing */
	dist.push_back(delta[bmu]);
      }
      else {
	bmus.push_back(0);
	dist.push_back(rlnan);
      }
    }
  }

  
  /* Return results. */
  List res;
  res.push_back(bmus, "DISTRICT");
  res.push_back(covers, "COVERAGE");
  res.push_back(nro::reals2vector(dist), "RESIDUAL");
  return res;
}
