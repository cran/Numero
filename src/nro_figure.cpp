/* Created by Ville-Petteri Makinen 2017
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
class SVGFrame : public Frame {
private:
  string code;
  vector<mdreal> bbox;
public:
  SVGFrame() : Frame() {};
  SVGFrame(const string& c, const vector<mdreal>& b) : Frame() {
    this->code = c;
    this->bbox = b;
  };
  string flush() {return code;}
  pair<mdreal, mdreal> horizontal() const {
    pair<mdreal, mdreal> res(0.0, 0.0);
    if(bbox.size() != 4) return res;
    res.first = bbox[0];
    res.second = bbox[2];
    return res;
  }
  pair<mdreal, mdreal> vertical() const {
    pair<mdreal, mdreal> res(0.0, 0.0);
    if(bbox.size() != 4) return res;
    res.first = bbox[1];
    res.second = bbox[3];
    return res;
  }
};

/*
 *
 */
// [[register]]
RcppExport SEXP nro_figure( SEXP fname_R, SEXP data_R, SEXP bbox_R ) {
  string fname = as<string>( fname_R );
  string data = as<string>( data_R );
  vector<mdreal> bbox = nro::vector2reals( bbox_R );

  /* Open output file. */
  Artist art(fname);

  /* Use the derived class to pass data to renderer. */
  SVGFrame frame(data, bbox);

  /* Create the figure. */
  art.paint(frame);

  /* Return file size. */
  return wrap( art.close() );
}
