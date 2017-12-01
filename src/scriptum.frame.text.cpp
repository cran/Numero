/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen */

#include "scriptum.local.h"

/*
 *
 */
bool
Frame::text(const mdreal x, const mdreal y, const string& s) {
  FrameBuffer* p = (FrameBuffer*)buffer;
  Style& sty = p->style;
  
  /* Check if unusable coordinates. */
  if(x == medusa::rnan()) return false;
  if(y == medusa::rnan()) return false;
  if(s.size() < 1) return false;

  /* Create element. */
  mdreal fs = sty.fontsize;
  sprintf(p->f(), "\n<text x=\"%.2f\" ", x);
  sprintf(p->f(), "y=\"%.2f\"\n", (y + 0.3*fs));
  p->append(p->textstycode);
  p->append(">\n"); p->append(s); 
  p->append("\n</text>\n");
 
  /* Estimate element width and height. */
  mdsize len = s.size();
  double width = 0.58*len*fs;
  vector<mdreal> vx(2, x);
  if(sty.anchor == "middle") {
    vx[0] -= 0.5*width;
    vx[1] += 0.5*width;
  }
  if(sty.anchor == "end") vx[0] -= width;
  if(vx[0] == vx[1]) vx[1] += width;

  /* Estimate element height. */
  vector<mdreal> vy(2);
  vy[0] = (y - 0.5*fs);
  vy[1] = (y + 0.6*fs);

  /* Take rotation angle into account. */
  double rx = cos(3.14159265*(sty.angle)/180.0);
  double ry = sin(3.14159265*(sty.angle)/180.0);
  mdreal vxA = vx[0]; mdreal vxB = vx[1];
  mdreal vyA = vy[0]; mdreal vyB = vy[1];
  vx[0] = (rx*vxA - ry*vyA);
  vx[1] = (rx*vxB - ry*vyB);
  vy[0] = (ry*vxA + rx*vyA);
  vy[1] = (ry*vxB + rx*vyB);

  /* Update limits and filesize. */
  (p->limits).first.update(vx, sty);
  (p->limits).second.update(vy, sty);
  return true;
}
