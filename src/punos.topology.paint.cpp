/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "punos.local.h"

/*
 *
 */
Frame
Topology::paint(const mdreal x0, const mdreal y0,
		const vector<Color>& colors) const {
  TopologyBuffer* p = (TopologyBuffer*)buffer;

  /* Check input. */
  const vector<Unit>& units = p->coord;
  if(colors.size() != units.size())
    return Frame();

  /* Check if anything to do. */
  if(p->maxradius <= 0.0) return Frame();
  
  /* Set origin. */
  mdreal rho = TopologyBuffer::scale();
  mdreal xorig = rho*x0;
  mdreal yorig = rho*y0;

  /* Set base attributes. */
  scriptum::Style sty;
  sty.strokewidth = 0.0;

  /* Open group. */
  Frame fr;
  fr.stylize(sty);
  fr.group(1);

  /* Open subgroup. */
  fr.group(1);

  /* Draw slices. */
  mdreal rmax = 0.0;
  for(mdsize i = 0; i < units.size(); i++) {
    if(colors[i].opacity <= 0.0) continue;

    /* Scale positions to canvas coordinates. */
    const Unit& u = units[i];
    mdreal r1 = rho*(u.radii.first);
    mdreal r2 = rho*(u.radii.second);
    mdreal a1 = u.angles.first;
    mdreal a2 = u.angles.second;
    
    /* Apply a small inflation to fill gaps. */
    if(r1 > 1e-9) {
      r1 -= 0.07;
      r2 += 0.07;
      a1 -= 0.07;
      a2 += 0.07;
    }
    
    /* Update circle radius. */
    if(r2 > rmax) rmax = r2;

    /* Set slice attributes. */
    sty.fillcolor = colors[i];
    sty.strokecolor = colors[i];
    sty.identity = i;
    fr.stylize(sty);

    /* Draw slice. */
    bool flag = fr.slice(xorig, yorig, r1, r2, a1, a2);
    if(!flag) return Frame();
  }
 
  /* Close subgroup. */
  fr.group(-1);

  /* Set line style. */
  sty.strokewidth = 0.5;
  sty.strokecolor = scriptum::colormap(0.7, "gray");
  sty.fillcolor.opacity = 0.0;
  fr.stylize(sty);

  /* Enclose in a circle. */
  if(fr.shape(xorig, yorig, rmax, "circle") == false)
    return Frame();

  /* Close group. */
  fr.group(-1);
  return fr;
}

#ifdef DUMMY  
  /* Switch subgroup. */
  fr.group(-1);
  fr.group(1);

  /* Set text style. */
  sty.strokewidth = 0.0;
  sty.fontsize = 0.8*(sty.fontsize);
  sty.anchor = "middle";

  /* Add unit indices. */
  Color gray("A0A0A0");
  for(mdsize i = 0; i < units.size(); i++) {
    if(colors[i].opacity <= 0.0) continue;

    /* Scale positions to canvas coordinates. */
    const Unit& u = units[i];
    mdreal x = rho*(x0 + u.x);
    mdreal y = rho*(y0 + u.y);

    /* Adjust color. */
    Color c = colors[i];
    mdreal kappa = 1.0;
    mdreal delta = gray.contrast(c);
    if(delta >= 0.0) {
      if(c.red > 0.5) c.red -= 0.03;
      if(c.green > 0.5) c.green -= 0.04;
      if(c.blue > 0.5) c.blue -= 0.02;
      kappa = 1.3;
    }
    else {
      if(c.red < 0.5) c.red += 0.03;
      if(c.green < 0.5) c.green += 0.02;
      if(c.blue < 0.5) c.blue += 0.04;
      kappa = 0.7;
    }
    c.red = pow(c.red, kappa);
    c.blue = pow(c.blue, kappa);
    c.green = pow(c.green, kappa);

    /* Set color. */
    sty.fillcolor = c;
    fr.stylize(sty);

    /* Write text. */
    string txt = long2string(i + 1);
    bool flag = fr.text(x, y, txt);
    if(!flag) return Frame();
  }
#endif
