/* file: */

#include "nro.h"

static string nro_matrix_scan(map<string, mdsize>&, File&, const int);
static string nro_matrix_identify(vector<mdsize>&, map<string, mdsize>&,
				  const vector<string>&, const int);
static string nro_matrix_select(vector<mdsize>&, vector<string>&,
				const map<string, mdsize>&);
static string nro_matrix_read(vector<string>&, vector<vector<mdreal> >&,
			      File&, const vector<mdsize>&,
			      const vector<mdsize>&);

/*
 *
 */
// [[register]]
RcppExport SEXP nro_matrix( SEXP fileName_R, SEXP vars_R, SEXP kvars_R, SEXP kflag_R ) {
  string fname = as<string>( fileName_R );
  int keyflag = as<int>( kflag_R );

  /* Check if any data or key variables. */
  vector<string> vars = as<vector<string> >( vars_R );
  vector<string> keyvars = as<vector<string> >( kvars_R );
  if((vars.size() == 1) && (vars[0] == "")) vars.clear();
  if((keyvars.size() == 1) && (keyvars[0] == "")) keyvars.clear();

  /* Open file. */
  File f; f.open(fname, "r");
  if(f.error().size() > 0) return CharacterVector( f.error() );

  /* Assign column headings. */
  map<string, mdsize> header;
  string err = nro_matrix_scan( header, f, keyflag );
  if(err.size() > 0) return CharacterVector( err );

  /* Find identity columns. */
  vector<mdsize> keycols;
  err = nro_matrix_identify( keycols, header, keyvars, keyflag );
  if(err.size() > 0) return CharacterVector( err );

  /* Find data columns. */
  vector<mdsize> cols;
  err = nro_matrix_select( cols, vars, header );
  if(err.size() > 0) return CharacterVector( err );

  /* Read data from the file. */
  vector<string> keys;
  vector<vector<mdreal> > data;
  err = nro_matrix_read( keys, data, f, keycols, cols );
  if(err.size() > 0) return CharacterVector( err );

  /* Return results. */
  NumericMatrix mtx = nro::reals2matrix( data );
  return List::create( mtx, keys, vars, keyvars );
}

/*
 *
 */
string
nro_matrix_scan( map<string, mdsize>& header, File& f,
		   const int keyflag ) {
  header.clear();

  /* Import headings. Increment indexing if R-style
     row names are present. */
  mdsize incr = (mdsize)(keyflag == 1);
  vector<string> headings = f.read('\t', 0);
  for(mdsize j = 0; j < headings.size(); j++) {
    string& name = headings[j];
    for(mdsize k = 0; k < name.size(); k++) {
      if( !isspace(name[k]) ) continue;
      return ( "White space in header, is file tab delimited?" );
    }
    header[name] = (incr + j);
  }
  
  /* Check if duplicate headings. */
  if(header.size() != headings.size())
    return ( "Duplicate headings." );
  return "";
}

/*
 *
 */
string
nro_matrix_identify( vector<mdsize>& keycols, map<string, mdsize>& header,
		     const vector<string>& keyvars,
		     const int keyflag ) {
  keycols.clear();

  /* Check if R-style row names are present. */
  if(keyflag == 1) {
    if(keyvars.size() > 0) return "Bad inputs.";
    keycols.push_back(0);
    return "";
  }

  /* Set identity mask. */
  for(mdsize j = 0; j < keyvars.size(); j++) {
    const string& name = keyvars[j];
    if(header.count(name) > 0) {
      keycols.push_back(header[name]);
      header.erase(name); /* exclude from data */
      continue;
    }

    /* All key columns must be available. */
    return ( "Unmatched identity heading '" + name + "'." );
  }
  return "";
}

/*
 *
 */
string
nro_matrix_select( vector<mdsize>& cols, vector<string>& vars,
		   const map<string, mdsize>& header) {
  vector<string> array;
  cols.clear();

  /* Select all variables. */
  if(vars.size() < 1) {
    for(map<string, mdsize>::const_iterator it = header.begin();
	it != header.end(); it++) {
      array.push_back(it->first);
      cols.push_back(it->second);
    } 

    /* Restore original order of columns. */
    vector<mdsize> sorted = sortsize(cols, 1);
    for(vector<mdsize>::iterator it = sorted.begin();
	it != sorted.end(); it++)
      vars.push_back(array[*it]);
  }
  else {

    /* Set selection mask. */
    array = vars; vars.clear();
    for(mdsize j = 0; j < array.size(); j++) {
      string& name = array[j];
      if(header.count(name) < 1) continue;
      cols.push_back(header.at(name));
      vars.push_back(name);
    }
  }

  /* Check if any usable variables. */
  if(cols.size() < 1) return ( "No usable columns." );
  return "";
}

/*
 *
 */
string
nro_matrix_read( vector<string>& keys, vector<vector<mdreal> >& data,
		 File& f, const vector<mdsize>& keycols,
		 const vector<mdsize>& cols) {
  mdreal rlnan = medusa::rnan();
  keys.clear();
  data.clear();

  /* Collect data values. */
  for(unsigned long nlines = 1; f.error().size() < 1; nlines++) {
    vector<mdreal> values(cols.size(), rlnan);
    vector<string> fields = f.read('\t', 0);
    mdsize nfields = fields.size();
    if(nfields < 1) continue;
    
    /* Create identity. */
    string key;
    for(mdsize k = 0; k < keycols.size(); k++) {
      mdsize ind = keycols[k];
      if(ind < nfields) {
	if(fields[ind].size() > 0) {
	  key += (fields[ind] + "\t");
	  continue;
	}
      }

      /* All key segments must be non-empty. */
      return ( "Unusable identity '" + key + "'." );
    }   
    
    /* Default identity. */
    mdsize len = key.size();
    if(len < 1) key = long2string(nlines);
    else key.resize(len - 1); /* strip trailing delimiter */
    
    /* Copy row vector. */
    for(mdsize k = 0; k < cols.size(); k++) {
      mdsize ind = cols[k];
      if(ind >= nfields) continue;
      values[k] = string2real(fields[ind]);
    }

    /* Update dataset. */
    data.push_back(values);
    keys.push_back(key);
  }
  return "";
}
