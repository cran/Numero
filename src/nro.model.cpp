/* file: */

#include "nro.h"

/*
 *
 */
koho::Model
nro::model( vector<string>& keys,
	    const punos::Topology& topo,
	    const vector<vector<mdreal> >& protos,
	    const vector<vector<mdreal> >& data) {
  keys.clear();
  
  /* Check codebook size. */
  mdsize nunits = protos.size();
  if(nunits < 1) return koho::Model();
  if(topo.size() != nunits) return koho::Model();
  mdsize ncols = protos[0].size();

  /* Check data matrix. */
  mdsize nrows = data.size();
  if(nrows > 0) {
    if(data[0].size() != ncols) return koho::Model();
  }

  /* Create a dataset model. */
  koho::Model res(topo);
 
  /* Copy codebook. */
  for(mdsize i = 0; i < nunits; i++) {
    string err = res.configure( nro_DEFAULT_LEVEL, i, protos[ i ] );
    if( err.size() > 0 ) return koho::Model();
  }
  
  /* Insert data into the model. */
  for(mdsize i = 0; i < nrows; i++) {
    string key = medusa::long2string(i); /* temporary identifier */
    string err = res.insert( key, nro_DEFAULT_STAMP, data[ i ] );
    if( err.size() > 0 ) return koho::Model();
    keys.push_back(key);
  }
  return res;
}
