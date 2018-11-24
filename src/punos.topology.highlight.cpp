/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "punos.local.h"

/*
 *
 */
Frame
Topology::highlight(const mdreal x0, const mdreal y0,
		    const vector<char>& labels,
		    const vector<Color>& colors,
		    const Style& base) const {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  mdreal rho = TopologyBuffer::scale();
  
  /* Check input. */
  const vector<Unit>& units = p->coord;
  if(labels.size() != units.size()) return Frame();
  if(colors.size() != units.size()) return Frame();
  
  /* Set style parameters. */
  Style fgsty = base;
  Style bgsty = base;
  fgsty.anchor = "middle";
  bgsty.strokewidth = 0.07*(base.fontsize);
  bgsty.strokecolor = base.fillcolor;

  /* Open group. */
  Frame fr;
  fr.group(1);
  
  /* Write highlighted labels. */
  for(mdsize k = 0; k < labels.size(); k++) {
    char label = labels[k];
    if(label == '\0') continue;
    
    /* Scale positions to canvas coordinates. */
    const Unit& u = units[k];
    mdreal x = rho*(x0 + u.x);
    mdreal y = rho*(y0 + u.y);

    /* Set background color. */
    bgsty.fillcolor = colors[k];
    fr.stylize(bgsty);
    
    /* Draw background circle. */
    if(fr.shape(x, y, 0.7*(fgsty.fontsize), "circle") == false)
      return Frame();

    /* Fine-tune label position. */
    y += 0.05*(fgsty.fontsize);
    
    /* Set label style. */
    fr.stylize(fgsty);
    
    /* Write label. */
    string txt(1, label);
    bool flag = fr.text(x, y, txt);
    if(!flag) return Frame();
  }

  /* Close group. */
  fr.group(-1);
  return fr;
}
