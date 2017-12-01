/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
// [[register]]
RcppExport SEXP nro_permute( SEXP topo_R, SEXP bmus_R, SEXP data_R, SEXP nperm_R ) {
  mdsize nperm = as<mdsize>( nperm_R );

  /* Check inputs. */
  vector<mdsize> bmus = nro::vector2sizes( bmus_R );
  vector<mdreal> data = nro::vector2reals( data_R );
  if( data.size() != bmus.size() ) {
    return CharacterVector( "Incompatible inputs." );
  }

  /* Get map topology. */
  vector<vector<mdreal> > topodata = nro::matrix2reals( topo_R );
  punos::Topology topo = reals2topology( topodata );
  if( topo.size() < 1 ) {
    return CharacterVector( "Unusable topology." );
  }

  /* Create simulation engine. */
  Engine eng = nro::engine(topo, data, bmus);

  /* Observed component plane. */
  vector<vector<mdreal> > observed = eng.average(0);
  if(observed.size() < 1) return CharacterVector( "Permutation failed." );
  mdreal score = abacus::statistic(observed[0], "sd");

  /* Permuted planes. */
  mdsize nfalse = 0;
  mdsize ntotal = 0;
  abacus::Empirical fnull;
  for(mdsize n = 0; (n < nperm) && (nfalse < nro_NMAX_FALSE); n++) {
    eng.shuffle(false);
    vector<vector<mdreal> > permuted = eng.average(0);
    if(permuted.size() < 1) return CharacterVector( "Permutation failed." );
    mdreal sc = abacus::statistic(permuted[0], "sd");
    fnull.add(sc, 1.0);
    nfalse += (sc >= score);
    ntotal++;
  }

  /* Check that permutations worked. */
  mdreal zscore = fnull.z( score );
  if(zscore == medusa::rnan()) {
    return CharacterVector("Permutations failed");
  }

  /* Estimate statistics. */
  NumericVector stats( 4 );
  stats( 0 ) = fnull.p( score, 1 );
  stats( 1 ) = zscore;
  stats( 2 ) = data.size();
  stats( 3 ) = ntotal;
  return stats;
}
