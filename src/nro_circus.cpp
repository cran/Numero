/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#include "nro.h"

/*
 *
 */
RcppExport SEXP
nro_circus(SEXP offsets_R, SEXP topo_R,
	   SEXP subcoldata_R, SEXP sublabdata_R,
	   SEXP colordata_R, SEXP labels_R,
	   SEXP txt_R, SEXP stamp_R) {
  string txt = as<string>(txt_R);
  string stamp = as<string>(stamp_R);
  scriptum::Color black = scriptum::colormap(0.0, "gray");
  scriptum::Color gray = scriptum::colormap(0.7, "gray");
  scriptum::Color white = scriptum::colormap(1.0, "gray");
  scriptum::Style sty;

  /* Position offsets. */
  vector<mdreal> offsets = nro::vector2reals(offsets_R);
  offsets.resize(2, 0.0);
  mdreal dx = offsets[0];
  mdreal dy = offsets[1];
  
  /* Check inputs. */
  vector<string> colordata = as<vector<string> >(colordata_R);
  vector<string> labels = as<vector<string> >(labels_R);
  if(colordata.size() != labels.size())
    return CharacterVector("Incompatible inputs.");

  /* Get map topology. */
  vector<vector<mdreal> > topodata = nro::matrix2reals(topo_R, 0.0);
  punos::Topology topo = reals2topology(topodata);
  if(topo.size() < 1)
    return CharacterVector("Unusable topology.");

  /* Get highlights. */
  vector<string> subcoldata = as<vector<string> >(subcoldata_R);
  vector<string> sublabdata = as<vector<string> >(sublabdata_R);
  
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
  for(mdsize i = 0; i < labels.size(); i++)
    labels[i] = medusa::string2safe(labels[i], 9);
  for(mdsize i = 0; i < sublabdata.size(); i++)
    sublabdata[i] = medusa::string2safe(sublabdata[i], 1);

  /* Prepare subgroup color data. */
  vector<Color> subcolors;
  for(mdsize i = 0; i < subcoldata.size(); i++) {
    Color c(subcoldata[i]);
    subcolors.push_back(c);
  }

  /* Prepare subgroup label data. */
  vector<char> sublabels;
  for(mdsize i = 0; i < sublabdata.size(); i++) {
    if(sublabdata[i].size() < 1) sublabels.push_back('\0');
    else sublabels.push_back(sublabdata[i][0]);
  }

  /* Remove conflicting labels. */
  for(mdsize i = 0; i < labels.size(); i++)
    if(sublabels[i] != '\0') labels[i].clear();
  
  /* Paint component plane. */
  scriptum::Frame wheel = topo.paint(dx, dy, colors);

  /* Write labels. */
  sty = Style();
  sty.strokewidth = 0.0;
  scriptum::Frame words = topo.write(dx, dy, labels, colorsL, sty);

  /* Write label shadows. */
  sty = Style();
  sty.strokewidth = 0.1*(sty.fontsize);
  scriptum::Frame shadows = topo.write(dx, dy, labels, colorsS, sty);
  
  /* Write subgroup labels. */
  sty = Style();
  sty.strokewidth = 0.0;
  sty.fillcolor = white;
  sty.fontweight = 900;
  sty.fontsize *= 0.95;
  scriptum::Frame subs = topo.highlight(dx, dy, sublabels, subcolors, sty);

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
  sty = Style();
  sty.fillcolor = black;
  sty.strokecolor = Color();
  sty.anchor = "middle";
  words.stylize(sty);

  /* Determine title location. */
  mdreal x = 0.5*(inner[0] + inner[2]);
  mdreal y = (inner[1] - 0.95*(sty.fontsize));

  /* Write title. */
  if((txt.size() > 0) && (words.text(x, y, txt) == false))
    return CharacterVector("Title failed.");

  /* Reduce font size. */
  sty.fontsize *= 0.9;
  sty.fillcolor = gray;
  words.stylize(sty);

  /* Add time and date. */
  y = (inner[3] + 1.15*(sty.fontsize));
  if(words.text(x, y, stamp) == false)
    return CharacterVector("Time stamp failed.");

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

  /* Finish SVG code. */
  string code = shadows.flush();
  code.append(wheel.flush());
  code.append(words.flush());
  code.append(subs.flush());
  
  /* Return results. */
  List res;
  res.push_back(txt, "title");
  res.push_back(code, "code");
  res.push_back(outer, "bbox");
  res.push_back(inner, "tight");
  return res;
}
