/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen */

#include "scriptum.local.h"

/*
 *
 */
void
scriptum_local::style2code(string& linecode, string& textcode,
			   const Style& sty) {
  linecode.clear();
  textcode.clear();

  /* Reserve memory for code. */
  char buf[2048]; buf[0] = '\0';
  char* p = buf;

  /* Rotation. */
  if(sty.angle != 0.0) {
    vector<mdreal> origin = sty.origin;
    if(origin.size() < 2) origin = vector<mdreal>(2, 0.0);
    p += sprintf(p, "rotate(%.2f, ", sty.angle);
    p += sprintf(p, "%.2f, %.2f)", origin[0], origin[1]);
  }

  /* Collect transforms. */
  string tform(buf); p = buf; buf[0] = '\0';
  if(tform.size() > 0) {
    linecode.append("\ntransform=\"" + tform + "\"");
    textcode.append("\ntransform=\"" + tform + "\"");
  }

  /* Font. */
  if(sty.fontfamily.size() > 0)
    p += sprintf(p, "\nfont-family: %s;", sty.fontfamily.c_str());
  if(sty.fontsize > 0.0)
    p += sprintf(p, "\nfont-size: %.2fpx;", sty.fontsize);
  if((sty.fontweight >= 100) && (sty.fontweight <= 900)) {
    int fw = (int)((sty.fontweight)/100 + 0.5);
    p += sprintf(p, "\nfont-weight: %d;", 100*fw);
  }

  /* Text-anchor. */
  if(sty.anchor != "")
    p += sprintf(p, "\ntext-anchor: %s;", sty.anchor.c_str());

  /* Collect text properties. */
  string textprop(buf); p = buf; buf[0] = '\0';

  /* Fill color. */
  if(sty.fillcolor.opacity > 0.0) {
    string tmp = sty.fillcolor.hex();
    p += sprintf(p, "\nfill: #%s;", tmp.substr(0, 6).c_str());
    if(sty.fillcolor.opacity < 1.0)
      p += sprintf(p, "\nfill-opacity: %.4f;", sty.fillcolor.opacity);
  }
  else
    p += sprintf(p, "\nfill: none;");
  
  /* Stroke color and width. */
  mdreal opacity = sty.strokecolor.opacity;
  if((opacity > 0.0) && (sty.strokewidth > 0.0)) {
    p += sprintf(p, "\nstroke: #%s;", sty.strokecolor.hex().c_str());
    p += sprintf(p, "\nstroke-linecap: round;");
    p += sprintf(p, "\nstroke-width: %.2fpx;", sty.strokewidth);
    if(opacity < 1.0)
      p += sprintf(p, "\nstroke-opacity: %.4f;", opacity);
  }
  else
    p += sprintf(p, "\nstroke: none;");

  /* Collect line properties. */
  string lineprop(buf); p = buf; buf[0] = '\0';

  /* Finish style results. */
  linecode.append("\nstyle=\"" + lineprop + "\"");
  textcode.append("\nstyle=\"" + textprop + lineprop + "\"");

  /* Add identity. */
  if(sty.identity != medusa::snan()) {
    string key = long2string(sty.identity);
    linecode.append("\nid=\"" + key + "\"");
    textcode.append("\nid=\"" + key + "\"");
  }
}
