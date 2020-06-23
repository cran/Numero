/* Created by Ville-Petteri Makinen 2019
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
RcppExport SEXP
nro_coalesce_split(SEXP tails_R, SEXP heads_R, SEXP weights_R) {
  StringVector tails(tails_R);
  StringVector heads(heads_R);
  NumericVector weights(weights_R);

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

    /* Check for reserved words. */
    if(tail.size() >= 6) {
      if(tail.substr(0, 6) == "module")
	return CharacterVector("Reserved vertex name.");
    }
    if(head.size() >= 6) {
      if(head.substr(0, 6) == "module")
	return CharacterVector("Reserved vertex name.");
    }
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
  
  /* Identify communities. */
  mdreal nhubs = network.agglomerate();
  vector<string> hubs = network.hubs();
  std::sort(hubs.begin(), hubs.end());
  
  /* Naming template. */
  mdsize ndigits = (mdsize)(log10(nhubs) + 1.0);
  
  /* Collect module members. */
  vector<string> modules;
  vector<string> members;
  map<string, string> hub2name;
  for(mdsize k = 0; k < nhubs; k++) {

    /* Set module name. */
    string digits = long2string(k + 1);
    long rem = (ndigits - digits.size());
    string name = ("module" + string(rem, '0') + digits);
    hub2name[hubs[k]] = name;

    /* Sort member names. */
    vector<string> array = network.vertices(hubs[k]);
    std::sort(array.begin(), array.end());
    
    /* Copy member names. */
    for(vector<string>::iterator it = array.begin();
	it != array.end(); it++) {
      modules.push_back(name);
      members.push_back(*it);
      nodes.erase(*it);
    }
  }

  /* Add singleton hubs. */
  for(unordered_set<string>::iterator it = nodes.begin();
      it != nodes.end(); it++) {
    hub2name[*it] = *it;
    modules.push_back(*it);
    members.push_back(*it);
  }
  
  /* Classify edges. */
  vector<string> subnets(tails.size(), "unassigned");
  for(mdsize i = 0; i < tails.size(); i++) {
    string tail = as<string>(tails[i]);
    string head = as<string>(heads[i]);
    string t = network.community(tail);
    string h = network.community(head);
    if(t != h) continue;
    subnets[i] = hub2name[t];
  }

  /* Return results. */
  List output;
  output.push_back(modules, "modules");
  output.push_back(members, "members");
  output.push_back(subnets, "subnets");
  return output;
}
