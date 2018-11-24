/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "koho.local.h"

/*
 *
 */
Trainer::Trainer() {}

/*
 *
 */
Trainer::Trainer(const Matrix& codebook, const Topology& topo,
		 const mdsize ntrain, const mdreal eq) {
  
  /* Optimal subset capacities. */
  mdsize nunits = topo.size();
  vector<mdsize> subsizes(nunits, 0);
  for(mdsize i = 0; i < ntrain; i++)
    subsizes[nunits-(i%nunits)-1] += 1;
    
  /* Create subsets. */
  (this->subsets).resize(nunits);
  for(mdsize k = 0; k < nunits; k++) {
    mdsize cap = subsizes[k];
    cap += (mdsize)((1.0 - eq)*(ntrain - cap));
    (this->subsets[k]).configure(k, cap);
  }

  /* Check codebook size. */
  if(codebook.size() < 1) return;
  if(codebook.size() != nunits)
    panic("Incompatible inputs.", __FILE__, __LINE__);

  /* Set prototypes. */
  (this->prototypes).resize(nunits);
  for(mdsize i = 0; i < nunits; i++)
    this->prototypes[i] = codebook.row(i);
}

/*
 *
 */
Trainer::~Trainer() {}

/*
 *
 */
mdsize
Trainer::size() const {
  return prototypes.size();
}
