/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
RcppExport SEXP
nro_label(SEXP topo_R, SEXP data_R, SEXP binflags_R, SEXP sigma_R) {
  mdreal rlnan = medusa::rnan();
  mdreal sigma = as<mdreal>(sigma_R);

  /* Get map topology.*/
  vector<vector<mdreal> > topodata = nro::matrix2reals(topo_R, 0.0);
  punos::Topology topo = nro::reals2topology(topodata);
  if(topo.size() < 1)
    return CharacterVector("Unusable topology.");

  /* Set neighborhood radius. */
  if(topo.rewire(sigma) == false)
    return CharacterVector("Unusable label gap.");

  /* Check data vectors. */
  vector<vector<mdreal> > vectors = nro::matrix2reals(data_R, 0.0);
  if(vectors.size() < 0) return CharacterVector("No data.");
  if(vectors.size() != topo.size())
    return CharacterVector("Incompatible inputs.");

  /* Transpose to column-major. */
  vector<vector<mdreal> > columns(vectors[0].size());
  for(mdsize i = 0; i < vectors.size(); i++) {
    for(mdsize j = 0; j < vectors[i].size(); j++)
      columns[j].push_back(vectors[i][j]);
    vectors[i].clear();
  }

  /* Process columns. */
  List res;
  IntegerVector binflags(binflags_R);
  for(mdsize j = 0; j < columns.size(); j++) {
    vector<mdreal>& values = columns[j];

    /* Calculate deviations from mean. */
    vector<mdreal> amps = abacus::transform(values, "z");
    for(mdsize i = 0; i < amps.size(); i++)
      if(amps[i] != rlnan) amps[i] = fabs(amps[i]);
    
    /* Sort map units according to amplitude. */
    vector<mdreal> tmp = amps;
    vector<mdsize> sorted = sortreal(tmp, -1);
    
    /* Select labeled units. */
    vector<int> flags(amps.size(), 0);
    for(mdsize k = 0; k < amps.size(); k++) {

      /* Skip units that have already been denied. */
      mdsize unit = sorted[k];
      if(flags[unit] != 0) continue;

      /* Deny label from too close neighbors. */
      vector<mdsize> neigh = topo.neighbors(unit);
      for(vector<mdsize>::iterator pos = neigh.begin();
	  pos != neigh.end(); pos++) {
	mdreal r = topo.distance(unit, *pos);
	if(r > sigma) continue;
	flags[*pos] = -1;
      }
      
      /* Allow label for the current unit. */
      flags[unit] = 1;
    }

    /* Check if only positive values. */
    mdreal xmin = abacus::statistic(values, "min");
    mdreal xmax = abacus::statistic(values, "max");

    /* Check if binary. */
    bool binary = (binflags[j] > 0);
    if(xmin < 0.0) binary = false;
    if(xmax > 1.0) binary = false;

    /* Set labels. */
    vector<string> labels(amps.size());
    for(mdsize k = 0; k < amps.size(); k++) {
      if(flags[k] != 1) continue;
      string txt = "Err";
      if(binary) {
	mdreal percent = 100*(values[k]);
	if(percent < 0.1) txt = "<0.1%";
	else txt = (medusa::real2text(percent) + "%");
      }
      else {
	txt = medusa::real2text(values[k]);
      }
      if((xmin >= 0) && (txt[0] == '+')) txt = txt.substr(1);
      labels[k] = txt;
    }

    /* Update results. */
    string name = ("Col" + long2string(j));
    res.push_back(labels, name);
  }
  return res;
}
