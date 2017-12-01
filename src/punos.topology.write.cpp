/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#include "punos.local.h"

/*
 *
 */
Frame
Topology::write(const mdreal x0, const mdreal y0,
		const vector<string>& labels,
		const vector<Color>& colors,
		const Style& base) const {
  TopologyBuffer* p = (TopologyBuffer*)buffer;
  mdreal rho = TopologyBuffer::scale();

  /* Check input. */
  const vector<Unit>& units = p->coord;
  if(labels.size() != units.size()) return Frame();
  if(colors.size() != units.size()) return Frame();

  /* Force labels to the center of unit areas. */
  Style sty = base;
  sty.anchor = "middle";

  /* Open group. */
  Frame fr;
  fr.group(1);

  /* Write labels. */
  for(mdsize k = 0; k < labels.size(); k++) {
    const string& label = labels[k];
    if(label.size() < 1) continue;

    /* Scale positions to canvas coordinates. */
    const Unit& u = units[k];
    mdreal x = rho*(x0 + u.x);
    mdreal y = rho*(y0 + u.y);

    /* Fine tune label position. */
    if(label[0] == '+') x -= 0.33*(sty.fontsize);
    if(label[0] == '-') x -= 0.33*(sty.fontsize);

    /* Set label color. */
    sty.fillcolor = colors[k];
    sty.strokecolor = colors[k];
    fr.stylize(sty);

    /* Write label. */
    bool flag = fr.text(x, y, label);
    if(!flag) return Frame();
  }

  /* Close group. */
  fr.group(-1);
  return fr;
}
