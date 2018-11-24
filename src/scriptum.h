/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen */

#ifndef scriptum_INCLUDED
#define scriptum_INCLUDED

#include <string>
#include <vector>
#include "medusa.h"

namespace scriptum {

  /* Color definition. */
  class Color {
  public:
    medusa::mdreal red;
    medusa::mdreal green;
    medusa::mdreal blue;
    medusa::mdreal opacity;
  public:
    Color();
    ~Color();

    /* Create a color from an XML string (with or without the '#'). */
    Color(const std::string&);

    /* Estimate perceived contrast between two colors.
       If the input is darker, returns a negative value. 
       If the input is lighter, returns a positive value. */
    medusa::mdreal contrast(const Color&) const;

    /* Return RGB color as a six-character hexadecimal code. */
    std::string hex() const;
  };

  /* Color functions. */
  extern Color colormap(const medusa::mdreal, const std::string&);
  extern std::vector<Color> colorize(const std::vector<medusa::mdreal>&,
				     const medusa::mdreal,
				     const std::string&);

  /*
   * Parameters for graphics primitives.
   */
  class Style {
  public:

    /* Text-anchor: 'start', 'middle', 'end'. */
    std::string anchor;

    /* Rotation angle about the element origin in degrees[0, 360]. */
    medusa::mdreal angle;

    /* Fill color in RGB[0, 1]. */
    Color fillcolor;

    /* Font family. */
    std::string fontfamily;

    /* Font size (pixels). */
    medusa::mdreal fontsize;

    /* Font weight: 100 | 200 | 300 | 400 | 500 | 600 | 700 | 800 | 900. */
    medusa::mdsize fontweight;

    /* Identity number. */
    medusa::mdsize identity;

    /* Origin of the element coordinate system (pixels). */
    std::vector<medusa::mdreal> origin;

    /* Minimum distance to canvas margin. */
    medusa::mdreal padding;

    /* Stroke color in RGB[0, 1]. */
    Color strokecolor;

    /* Stroke width (pixels). */
    medusa::mdreal strokewidth;

  public:
    Style();
    ~Style();
  };

  /*
   * Create Scalable Vector Graphics code.
   */
  class Frame {
  private:
    void* buffer;
  public:
    Frame();

    /* Copy the contents from the argument.  */
    Frame(const Frame&);
    void operator=(const Frame&);

    /* Free resources. */
    ~Frame();

    /* Draw curve or shape. The 1st argument sets the horizontal
       and the 2nd the vertical coordinates. If the first and last
       points are equal, the curve is drawn closed. */
    bool curve(const std::vector<medusa::mdreal>&,
	       const std::vector<medusa::mdreal>&);

    /* Return current graphics code and remove it from the object. */
    virtual std::string flush();

    /* Start/stop an element group. If argument is positive, a new
       group is started, if negative the current active group is closed. 
       Returns the number of open groups. */
    medusa::mdsize group(const int);

    /* Coordinate range that contains rendered elements. */
    virtual std::pair<medusa::mdreal, medusa::mdreal> horizontal() const;

    /* Draw a predefined shape. The first two arguments set the position
       and the third sets the radius. The fourth argument defines the shape
       itself and the optional rotation angle (e.g. 'circle', 'cross/50',
       'square/120', 'star/130' or 'triangle/200'). */
    bool shape(const medusa::mdreal, const medusa::mdreal,
	       const medusa::mdreal, const std::string&);

    /* Draw an elliptical slice. The first two arguments set the ellipse
       center (X and Y). The 3rd and 4th arguments set the smaller and larger
       radii for the slice. The 5th and 6th arguments set the smaller and
       larger angle (in degrees) for the slice. */
    bool slice(const medusa::mdreal, const medusa::mdreal,
	       const medusa::mdreal, const medusa::mdreal,
	       const medusa::mdreal, const medusa::mdreal);

    /* Return current style. */
    Style style() const;

    /* Set current style. Empty style data is reverted to previous values
       or system defaults if history is not available. */
    void stylize(const Style&);

    /* Write a string of text. */
    bool text(const medusa::mdreal, const medusa::mdreal,
	      const std::string&);

    /* Coordinate range that contains rendered elements. */
    virtual std::pair<medusa::mdreal, medusa::mdreal> vertical() const;

    /* List of shape names available. */
    static std::vector<std::string> shapes();
  };

  /*
   *
   */
  class Artist {
  private:
    void* buffer;
  public:
    Artist();

    /* Create a new object with the specified output file. */
    Artist(const std::string&);

    /* Copy the contents from the argument. Objects in active
       rendering cannot be copied.  */
    Artist(const Artist&);
    void operator=(const Artist&);

    /* Free resources. */
    ~Artist();

    /* Finish rendering and return the number of bytes. The background
       will be filled according to the current style. */
    unsigned long close();

    /* Start/stop an element group. If argument is positive, a new
       group is started, if negative the current active group is closed. 
       Returns the number of open groups. */
    medusa::mdsize group(const int);

    /* Render graphics from a frame. */
    bool paint(Frame&);
  };

  /* Version information. */
  extern std::string version();
};

#endif /* scriptum_INCLUDED */
