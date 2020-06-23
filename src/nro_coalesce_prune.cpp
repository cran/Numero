/* Created by Ville-Petteri Makinen 2019
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
RcppExport SEXP
nro_coalesce_prune(SEXP tails_R, SEXP heads_R,
		   SEXP weights_R, SEXP degree_R) {
  mdreal rlnan = medusa::rnan();
  StringVector tails(tails_R);
  StringVector heads(heads_R);
  NumericVector weights(weights_R);
  mdreal degree = as<mdreal>(degree_R);

  /* Check inputs. */
  if(heads.size() != tails.size())
    return CharacterVector("Incompatible inputs.");
  if(weights.size() != tails.size())
    return CharacterVector("Incompatible inputs.");

  /* Vertex identities. */
  unordered_set<string> nodes;
  for(mdsize i = 0; i < tails.size(); i++) {
    string tail = as<string>(tails[i]);
    string head = as<string>(heads[i]);
    if(tail.size() > 0) nodes.insert(tail);
    if(head.size() > 0) nodes.insert(head);
  }

  /* Collect edges. */
  turnip::Network network;
  LogicalVector flags = Rcpp::is_finite(weights);
  for(mdsize i = 0; i < tails.size(); i++) {
    if(flags[i] == false) continue;
    string tail = as<string>(tails[i]);
    string head = as<string>(heads[i]);
    network.add(tail, head, weights[i]);
  }

  /* Remove redundant edges. */
  network.prune(degree);

  /* Update edge weights. */
  for(mdsize i = 0; i < tails.size(); i++) {
    string tail = as<string>(tails[i]);
    string head = as<string>(heads[i]);
    mdreal w = network.weight(tail, head);
    if(w == rlnan) weights[i] = 0.0;
  }
  return weights;
}
