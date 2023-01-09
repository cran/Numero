/* Created by Ville-Petteri Makinen
   email: vpmakine@gmail.com */

#include "scriptum.local.h"

/*
 * This function must always return exactly
 * the same number of bytes or zero.
 */
string
ArtistBuffer::prolog() const {
  mdreal rlnan = medusa::rnan();
  string output;
  
  /* Document declaration. */
  output += "<?xml version=\"1.0\" standalone=\"yes\"?>\n";
  output += "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\"\n";
  output += "\"http://www.w3.org/Graphics/";
  output += "SVG/1.1/DTD/svg11.dtd\">\n";
  
  /* Check limits. */
  mdreal xmin = limits.first.alpha;
  mdreal xmax = limits.first.omega;
  mdreal ymin = limits.second.alpha;
  mdreal ymax = limits.second.omega;
  if(xmin == rlnan) xmin = 0.0;
  if(xmax == rlnan) xmax = 0.0;
  if(ymin == rlnan) ymin = 0.0;
  if(ymax == rlnan) ymax = 0.0;

  /* Extract canvas size. */
  long width = (long)(xmax - xmin + 0.5);
  long height = (long)(ymax - ymin + 0.5);
  
  /* Determine offset. */
  long dx = (long)(fabs(xmin) + 0.5);
  long dy = (long)(fabs(ymin) + 0.5);

  /* Standard length strings to make sure prolog size is stable. */
  char buf[128];
  snprintf(buf, sizeof(buf), "%.4f", bgcolor.opacity); string opac_s(buf);
  snprintf(buf, sizeof(buf), "%06ld", width); string width_s(buf);
  snprintf(buf, sizeof(buf), "%06ld", height); string height_s(buf);
  snprintf(buf, sizeof(buf), "+%06ld", dx); string dx_s(buf);
  snprintf(buf, sizeof(buf), "+%06ld", dy); string dy_s(buf);
  if(xmin > 0.0) dx_s[0] = '-';
  if(ymin > 0.0) dy_s[0] = '-';
  
  /* Set main element. */
  output += "\n<svg id=\"plot\"";
  output += "\ndraggable=\"false\"";
  output += "\nonload=\"initPage('plot', false)\"";
  output += "\nxmlns=\"http://www.w3.org/2000/svg\"";
  output += "\nstyle=\"user-select: none;\"";
  output += "\nx=\"0\" y=\"0\" ";
  output += ("width=\"" + width_s + "\" ");
  output += ("height=\"" + height_s + "\">\n");
    
  /* Add background. */
  output += ("\n<polygon points=\"");
  output += ("\n\t0,0\n\t" + width_s + ",0");
  output += ("\n\t" + width_s + "," + height_s);
  output += ("\n\t0," + height_s + "\"");
  output += ("\nstyle=\"");
  output += ("\nfill: #" + bgcolor.hex().substr(0,6) + ";");
  output += ("\nfill-opacity: " + opac_s + ";");
  output += ("\npointer-events: none;");
  output += ("\"\nid=\"plot_background\"/>\n");
  
  /* Set origin to zero. */
  output += ("\n<g transform=\"translate(");
  output += (dx_s + "," + dy_s + "\"");
  output += ("\ntfx=\"" + dx_s + "\"");
  output += ("\ntfy=\"" + dy_s + "\"");
  output += ("\nid=\"plot_contents\">\n");
  return output;
}
