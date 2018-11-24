/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
RcppExport SEXP
nro_diffuse(SEXP topo_R, SEXP bmus_R, SEXP data_R) {
  
  /* Check inputs. */
  vector<mdsize> bmus = nro::vector2sizes(bmus_R);
  vector<vector<mdreal> > vectors = nro::matrix2reals(data_R, 0.0);
  if((vectors.size() > 0) && (vectors.size() != bmus.size()))
    return CharacterVector("Incompatible inputs.");

  /* Get map topology. */
  vector<vector<mdreal> > topodata = nro::matrix2reals(topo_R, 0.0);
  punos::Topology topo = nro::reals2topology(topodata);
  if(topo.size() < 1) return CharacterVector("Unusable topology.");

  /* Switch to C++ indexing. */
  for(mdsize i = 0; i < bmus.size(); i++) {
    if(bmus[i] > 0) bmus[i] -= 1;
    else bmus[i] = topo.size();
  }
  
  /* Estimate histogram only. */
  List res;
  if(vectors.size() < 1) {
    vector<mdreal> ones(bmus.size(), 1.0);
    vector<mdreal> counts = topo.diffuse(bmus, ones);
    res.push_back(NumericMatrix(), "planes");
    res.push_back(nro::reals2vector(counts), "histogram");
    return res;
  }

  /* Create a simulation engine. R-style indexing switched to C++. */
  Engine eng(topo);
  for(mdsize i = 0; i < vectors.size(); i++) {
    eng.insert(long2string(i), bmus[i], vectors[i]);
    vectors[i].clear(); /* reduce memory footprint */
  }
  
  /* Estimate component planes. */
  vector<vector<mdreal> > planes = eng.average();
  vector<mdreal> hgram = eng.histogram();
  
  /* Return results. */
  res.push_back(nro::reals2matrix(planes), "planes");
  res.push_back(nro::reals2vector(hgram), "histogram");
  return res;
}
