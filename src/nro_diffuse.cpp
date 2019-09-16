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
  punos::Topology topo = nro::reals2topology(topodata, SIGMA_nro);
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
    res.push_back(nro::reals2vector(counts), "histograms");
    return res;
  }
  
  /* Create a simulation engine. */
  Engine eng(topo);
  for(mdsize i = 0; i < vectors.size(); i++) {
    eng.insert(long2string(i), bmus[i], vectors[i]);
    vectors[i].clear(); /* reduce memory footprint */
  }
  
  /* Estimate component planes and histograms. */
  vector<vector<mdreal> > planes = eng.average();
  vector<vector<mdreal> > hgrams = eng.histograms();
  
  /* Return results. */
  res.push_back(nro::reals2matrix(planes), "planes");
  res.push_back(nro::reals2matrix(hgrams), "histograms");
  return res;
}
