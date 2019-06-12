/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "nro.h"

#define nro_NMAX_FALSE 20

/*
 * Synchronous permutations of multiple dimensions in parallel.
 * Only data points with no unusable values are included.
 */
static string
nro_permute_sync(abacus::Matrix& stats, vector<vector<mdreal> >& points,
		 const vector<mdsize>& bmus, const Topology& topo,
		 const mdsize ncycl) {
  stats = abacus::Matrix();
  
  /* Create a simulation engine. R-style indexing switched to C++. */
  Engine eng(topo);
  mdsize ndata = 0;
  for(mdsize i = 0; i < points.size(); i++) {
    string e = eng.insert(long2string(i), (bmus[i] - 1), points[i]);
    ndata += (e.size() < 1); /* excluded if any missing value */
    points[i].clear(); /* reduce memory footprint */
  }

  /* Observed component planes. */
  vector<vector<mdreal> > observed = eng.average();
  mdsize nvars = observed.size();
  if(nvars < 1) return "Permutation failed.";

  /* Observed statistics. */
  vector<mdreal> scores(nvars, 0.0);
  for(mdsize j = 0; j < nvars; j++)
    scores[j] = abacus::statistic(observed[j], "sd");
  
  /* Simulate null distributions. */    
  mdsize nfalse = 0;
  mdsize ntotal = 0;
  vector<abacus::Empirical> fnull(nvars);
  
  for(mdsize n = 0; n < ncycl; n++) {
    if(nfalse/nvars >= nro_NMAX_FALSE) break;
    
    /* Permute without replacement. */
    eng.shuffle(false);
    
    /* Permuted component planes. */
    vector<vector<mdreal> > permuted = eng.average();
    if(permuted.size() < 1) return "Permutation failed.";
    
    /* Permuted statistics. */
    vector<mdreal> nulls(nvars, 0.0);
    for(mdsize j = 0; j < nvars; j++)
      nulls[j] = abacus::statistic(permuted[j], "sd");
    
    /* Check if any false positives. */
    for(mdsize j = 0; j < nvars; j++)
      nfalse += (nulls[j] >= scores[j]);
    
    /* Update null distributions. */
    for(mdsize j = 0; j < nvars; j++)
      fnull[j].add(nulls[j], 1.0);
    ntotal++;
  }

  /* Estimate statistics. */
  for(mdsize j = 0; j < nvars; j++) {
    stats.insert(0, j, scores[j]);
    stats.insert(1, j, fnull[j].z(scores[j]));
    stats.insert(2, j, fnull[j].p(scores[j], 1));
    stats.insert(3, j, ndata);
    stats.insert(4, j, fnull[j].size());
  }
  return "";
}

/*
 *
 */
RcppExport SEXP
nro_permute(SEXP topo_R, SEXP bmus_R, SEXP data_R,
	    SEXP numcycl_R, SEXP lag_R) {
  vector<mdsize> numcycl = nro::vector2sizes(numcycl_R);
  mdreal lag = as<mdreal>(lag_R);
  
  /* Check inputs. */
  vector<mdsize> bmus = nro::vector2sizes(bmus_R);
  vector<vector<mdreal> > vectors = nro::matrix2reals(data_R, 0.0);
  if(vectors.size() < 1) return CharacterVector("No data.");
  if(vectors.size() != bmus.size())
    return CharacterVector("Incompatible inputs.");
  if(vectors[0].size() != numcycl.size())
    return CharacterVector("Incompatible inputs.");
  
  /* Get map topology. */
  vector<vector<mdreal> > topodata = nro::matrix2reals(topo_R, 0.0);
  punos::Topology topo = reals2topology(topodata);
  if(topo.size() < 1) return CharacterVector("Unusable topology.");

  /* Asynchronous permutations. That is, separate synchronous
     permutations for each single dimension. */
  abacus::Matrix stats;
  time_t stamp = time(NULL);
  mdsize nvars = vectors[0].size();
  for(mdsize j = 0; j < nvars; j++) {
    
    /* Extract data values. */
    vector<vector<mdreal> > column(vectors.size());
    for(mdsize i = 0; i < vectors.size(); i++)
      column[i].push_back(vectors[i][j]);
    
    /* Estimate statistics. */
    abacus::Matrix batch;
    string err = nro_permute_sync(batch, column, bmus, topo, numcycl[j]);
    if(err.size() > 0) return CharacterVector(err);
    
    /* Update results. */
    for(mdsize i = 0; i < batch.size(); i++)
      stats.insert(i, j, batch.value(i, 0));
    
    /* Progress message. */
    if(lag < 0.0) continue;
    mdreal dt = difftime(time(NULL), stamp); 
    if(dt >= lag) {
      string msg = ("completed in " + medusa::time2text(dt));
      Rprintf("%d / %d %s\n", (j + 1), nvars, msg.c_str());
      stamp = time(NULL);
    }
  }
    
  /* Return results. */
  List res;
  res.push_back(nro::reals2vector(stats.row(0)), "SCORE");
  res.push_back(nro::reals2vector(stats.row(1)), "Z");
  res.push_back(nro::reals2vector(stats.row(2)), "P.freq");
  res.push_back(nro::reals2vector(stats.row(3)), "N.data");
  res.push_back(nro::reals2vector(stats.row(4)), "N.cycles");
  return res;
}
