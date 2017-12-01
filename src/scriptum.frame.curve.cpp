/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen */

#include "scriptum.local.h"

/*
 *
 */
bool
Frame::curve(const vector<mdreal>& vx, const vector<mdreal>& vy) {
  mdreal rlnan = medusa::rnan();
  FrameBuffer* p = (FrameBuffer*)buffer;

  /* Check size. */
  mdsize nv = vx.size();
  if(nv < 2) return false;
  if(vy.size() != nv) return false;

  /* Check if closed path. */
  bool closeflag = ((vx[0] == vx[nv-1]) && (vy[0] == vy[nv-1]));
  if(closeflag && (nv < 4)) return false;
  if(closeflag) nv--;

  /* Check if unusable coordinates. */
  for(mdsize i = 0; i < nv; i++) {
    if(vx[i] == rlnan) return false;
    if(vy[i] == rlnan) return false;
  }

  /* Create polyline. */
  sprintf(p->f(), "\n<path d=\"\n");
  sprintf(p->f(), "M\t%.2f\t%.2f", vx[0], vy[0]);
  for(mdsize i = 1; i < nv; i++)
    sprintf(p->f(), "\nL\t%.2f\t%.2f", vx[i], vy[i]);
  
  /* Set closing flag. */
  if(closeflag) p->append("\nZ");
  p->append("\"\n");
  
  /* Apply style. */
  p->append(p->linestycode);
  p->append("/>\n");

  /* Update limits and filesize. */
  (p->limits).first.update(vx, p->style);
  (p->limits).second.update(vy, p->style);
  return true;
}
