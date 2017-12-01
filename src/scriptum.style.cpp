/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen */

#include "scriptum.local.h"

/*
 *
 */
Style::Style() {
  anchor = "start";
  angle = 0.0;
  fillcolor.red = 0.7;
  fillcolor.green = 0.7;
  fillcolor.blue = 0.7;
  fillcolor.opacity = 1.0;
  fontfamily = "Helvetica";
  fontsize = 10.0;
  fontweight = 500;
  origin.resize(2, 0.0);
  padding = 0.0;
  strokecolor.red = 0.0;
  strokecolor.green = 0.0;
  strokecolor.blue = 0.0;
  strokecolor.opacity = 1.0;
  strokewidth = 1.0;
}

/*
 *
 */
Style::~Style() {}
