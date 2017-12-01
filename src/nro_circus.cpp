/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
// [[register]]
RcppExport SEXP nro_circus( SEXP topo_R, SEXP colordata_R, SEXP labels_R, SEXP txt_R, SEXP stamp_R ) {
  string txt = as<string>( txt_R );
  string stamp = as<string>( stamp_R );
  scriptum::Color black = scriptum::colormap(0.0, "gray");
  scriptum::Color gray = scriptum::colormap(0.7, "gray");
  scriptum::Color white = scriptum::colormap(1.0, "gray");
  scriptum::Style sty;

  /* Check inputs. */
  vector<string> colordata = as<vector<string> >( colordata_R );
  vector<string> labels = as<vector<string> >( labels_R );
  if( colordata.size() != labels.size() ) {
    return CharacterVector( "Incompatible inputs." );
  }

  /* Get map topology. */
  vector<vector<mdreal> > topodata = nro::matrix2reals( topo_R );
  punos::Topology topo = reals2topology( topodata );
  if( topo.size() < 1 ) {
    return CharacterVector( "Unusable topology." );
  }

  /* Prepare color data. */
  vector<Color> colors;
  vector<Color> colorsL;
  vector<Color> colorsS;
  for(mdsize i = 0; i < colordata.size(); i++) {
    Color c(colordata[i]);
    colors.push_back(c);

    /* Determine label color. */
    mdreal cB = black.contrast(c);
    mdreal cW = white.contrast(c);
    if(fabs(cB) > fabs(cW)) {
      colorsL.push_back(black);
      colorsS.push_back(Color());
    }
    else {
      colorsL.push_back(white);
      colorsS.push_back(gray);
    }
  }

  /* Exclude problematic characters. */
  txt = medusa::string2safe(txt, 32);
  for(mdsize i = 0; i < labels.size(); i++) {
    labels[i] = medusa::string2safe(labels[i], 9);
  }

  /* Paint component plane. */
  scriptum::Frame wheel = topo.paint(0, 0, colors);

  /* Write labels. */
  sty.strokewidth = 0.0;
  scriptum::Frame words = topo.write(0, 0, labels, colorsL, sty);

  /* Write label shadows. */
  sty.strokewidth = 0.1*(sty.fontsize);
  scriptum::Frame shadows = topo.write(0, 0, labels, colorsS, sty);

  /* Get circle bounding box. */
  vector<mdreal> inner(4);
  inner[0] = wheel.horizontal().first;
  inner[1] = wheel.vertical().first;
  inner[2] = wheel.horizontal().second;
  inner[3] = wheel.vertical().second;

  /* Initial full bounding box. */
  vector<mdreal> outer = inner;
  outer[0] -= 2*(sty.fontsize);
  outer[1] -= 2*(sty.fontsize);
  outer[2] += 2*(sty.fontsize);
  outer[3] += 2*(sty.fontsize);

  /* Set base attributes for title text. */
  sty.fillcolor = black;
  sty.strokecolor = Color();
  sty.anchor = "middle";
  words.stylize(sty);

  /* Determine title location. */
  mdreal x = 0.5*(inner[0] + inner[2]);
  mdreal y = (inner[1] - 0.95*(sty.fontsize));

  /* Write title. */
  if((txt.size() > 0) && (words.text(x, y, txt) == false)) {
    return CharacterVector("Title failed.");
  }

  /* Reduce font size. */
  sty.fontsize *= 0.9;
  words.stylize(sty);

  /* Add time and date. */
  y = (inner[3] + 1.15*(sty.fontsize));
  if(words.text(x, y, stamp) == false) {
    return CharacterVector("Time stamp failed.");
  }

  /* Check if more space needed for labels. */
  vector<mdreal> middle(4);
  middle[0] = words.horizontal().first;
  middle[1] = words.vertical().first;
  middle[2] = words.horizontal().second;
  middle[3] = words.vertical().second;
  if(middle[0] < outer[0]) outer[0] = middle[0];
  if(middle[1] < outer[1]) outer[1] = middle[1];
  if(middle[2] > outer[2]) outer[2] = middle[2];
  if(middle[3] > outer[3]) outer[3] = middle[3];

  /* Collect SVG code. */
  string code = (shadows.flush());
  code.append(wheel.flush());
  code.append(words.flush());
  return( List::create( code, outer, inner ) );
}
