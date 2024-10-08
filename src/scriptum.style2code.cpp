/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

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
  char buf[256]; buf[0] = '\0';
  char* p = buf;

  /* Rotation. */
  if(sty.angle != 0.0) {
    vector<mdreal> origin = sty.origin;
    if(origin.size() < 2) origin = vector<mdreal>(2, 0.0);
    p += snprintf(p, 64, "\nrotate(%.2f, ", sty.angle);
    p += snprintf(p, 64, "%.2f, %.2f)", origin[0], origin[1]);
  }

  /* Collect transforms. */
  string tform(buf); p = buf; buf[0] = '\0';
  if(tform.size() > 0) {
    linecode.append("transform=\"" + tform + "\"");
    textcode.append("transform=\"" + tform + "\"");
  }

  /* Font. */
  if(sty.fontfamily.size() > 0)
    p += snprintf(p, 64, "\nfont-family: %s;",
		  sty.fontfamily.substr(0, 63).c_str());
  if(sty.fontsize > 0.0)
    p += snprintf(p, 64, "\nfont-size: %.2fpx;", sty.fontsize);
  if((sty.fontweight >= 100) && (sty.fontweight <= 900)) {
    int fw = (int)((sty.fontweight)/100 + 0.5);
    p += snprintf(p, 64, "\nfont-weight: %d;", 100*fw);
  }

  /* Text-anchor. */
  if(sty.anchor != "")
    p += snprintf(p, 64, "\ntext-anchor: %s;",
		  sty.anchor.substr(0, 63).c_str());

  /* Collect text properties. */
  string textprop(buf); p = buf; buf[0] = '\0';

  /* Check if pointable. */
  if(!sty.pointable)
    p += snprintf(p, 64, "\npointer-events: none;");
  
  /* Fill color. */
  if(sty.fillcolor.opacity > 0.0) {
    string tmp = sty.fillcolor.hex();
    p += snprintf(p, 64, "\nfill: #%s;", tmp.substr(0, 6).c_str());
    if(sty.fillcolor.opacity < 1.0)
      p += snprintf(p, 64, "\nfill-opacity: %.4f;", sty.fillcolor.opacity);
  }
  else
    p += snprintf(p, 64, "\nfill: none;");
  
  /* Stroke color and width. */
  mdreal opacity = sty.strokecolor.opacity;
  if((opacity > 0.0) && (sty.strokewidth > 0.0)) {
    string tmp = sty.strokecolor.hex();
    p += snprintf(p, 64, "\nstroke: #%s;", tmp.substr(0, 6).c_str());
    p += snprintf(p, 64, "\nstroke-linecap: round;");
    p += snprintf(p, 64, "\nstroke-width: %.2fpx;", sty.strokewidth);
    if(opacity < 1.0)
      p += snprintf(p, 64, "\nstroke-opacity: %.4f;", opacity);
  }
  else
    p += snprintf(p, 64, "\nstroke: none;");
  
  /* Collect line properties. */
  string lineprop(buf); p = buf; buf[0] = '\0';
  
  /* Finish style results. */
  if(tform.size() > 0) {
    linecode.append("\nstyle=\"" + lineprop + "\"");
    textcode.append("\nstyle=\"" + textprop + lineprop + "\"");
  }
  else {
    linecode.append("style=\"" + lineprop + "\"");
    textcode.append("style=\"" + textprop + lineprop + "\"");
  }
  
  /* Add values. */
  for(mdsize i = 0; i < sty.values.size(); i++) {
    string vname = ("\nv" + long2string(i) + "=");
    linecode.append(vname + "\"" + sty.values[i] + "\"");
    textcode.append(vname + "\"" + sty.values[i] + "\"");
  }

  /* Add identity. */
  if(sty.identity.size() > 0) {
    linecode.append("\nid=\"" + sty.identity + "\"");
    textcode.append("\nid=\"" + sty.identity + "\"");
  }
}
