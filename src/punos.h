/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#ifndef punos_INCLUDED
#define punos_INCLUDED

#include <string>
#include <vector>
#include <map>
#include "medusa.h"
#include "scriptum.h"

namespace punos {

  /*
   * Coordinate information for visualization.
   */
  struct Unit {
    medusa::mdreal x;
    medusa::mdreal y;
    std::pair<medusa::mdreal, medusa::mdreal> radii;
    std::pair<medusa::mdreal, medusa::mdreal> angles;
  };

  /*
   *
   */
  class Topology {
  private:
    void* buffer;
  public:
    Topology();

    /* Create a set of unconnected centroids. */
    Topology(const medusa::mdsize);
    
    /* Create a 3D network structure. The first input sets the vertical
       positions of the planes. The second input sets the map radius
       (as the number of concentric circles of units). Use the size()
       function to verify the total number of units and radius() to
       check the exact space occupied by the circular plane.  */
    Topology(const std::vector<medusa::mdreal>&, const medusa::mdsize);

    /* Create a 3D network structure according to the specified units. */
    Topology(const std::vector<medusa::mdreal>&, const std::vector<Unit>&);

    /* Copy contents from the input. */
    Topology(const Topology&);
    void operator=(const Topology&);
    
    /* Free resources. */
    ~Topology();
    
    /* Return unit characteristics for visualization. */
    Unit operator[](const medusa::mdsize) const;

    /* Number of horizontal levels. */
    medusa::mdsize depth() const;

    /* Estimate smoothed sum of values on the network. The first input
       contains the best-matching units, and the second contains the
       sample values. */
    std::vector<medusa::mdreal>
    diffuse(const std::vector<medusa::mdsize>&,
	    const std::vector<medusa::mdreal>&) const;

    /* Estimate smoothed sum of values on the network. The first input
       specifies the sample depths, see stratify(). The second input
       contains the best-matching units, and the third contains the
       sample values. */
    std::vector<std::vector<medusa::mdreal> >
    diffuse(const std::vector<medusa::Site>&,
	    const std::vector<medusa::mdsize>&,
	    const std::vector<medusa::mdreal>&) const;

    /* Calculate spatial distance between two map units. */
    medusa::mdreal distance(const medusa::mdsize,
			    const medusa::mdsize) const;    

    /* Visualize the horizontal topology by writing single-character
       highlighting labels on the map unit areas. The first two inputs
       define the center point of the map on the canvas. The size of
       the label and color vectors must match the number of units. */
    scriptum::Frame highlight(const medusa::mdreal, const medusa::mdreal,
			      const std::vector<char>&,
			      const std::vector<scriptum::Color>&,
			      const scriptum::Style&) const;

    /* Import a network structure from a file. Any previous
       contents is discarded. Returns an error message if failed. */
    std::string import(const std::string&);

    /* Interpolate unit prototypes according to pivot points. The input
       contains seed profiles that will be spread over the map. */
    std::vector<std::vector<medusa::mdreal> >
    interpolate(const std::vector<std::vector<medusa::mdreal> >&) const;
    
    /* Vertical positions of horizontal planes. */
    std::vector<medusa::mdreal> levels() const;

    /* List of connected units, including self. */
    std::vector<medusa::mdsize> neighbors(const medusa::mdsize) const;

    /* Visualize the horizontal topology by painting the map unit areas.
       The first two inputs define the center point of the map on the
       canvas. The size of the color vector must match the map size. */
    scriptum::Frame paint(const medusa::mdreal, const medusa::mdreal,
			  const std::vector<scriptum::Color>&) const;
    
    /* Maximum extent from origin. */
    medusa::mdreal radius() const;

    /* Set neighborhood radius. The input must be positive and at
       most half of map radius. Returns true if at least one edge
       was created. */
    bool rewire(const medusa::mdreal);

    /* Save network structure in a text file. */
    unsigned long save(const std::string&) const;

    /* Current neighborhood radius. */
    medusa::mdreal sigma() const;

    /* Number of units. */
    medusa::mdsize size() const;

    /* Determine adjacent planes (i.e. the layer in between)
       according to vertical position. */
    medusa::Site stratify(const medusa::mdreal) const;

    /* Link weight between two map units. */
    medusa::mdreal weight(const medusa::mdsize,
			  const medusa::mdsize) const;

    /* Visualize the horizontal topology by writing labels on the map unit
       areas. The first two inputs define the center point of the map on
       the canvas. The size of the string and color vectors must match the
       number of units. */
    scriptum::Frame write(const medusa::mdreal, const medusa::mdreal,
			  const std::vector<std::string>&,
			  const std::vector<scriptum::Color>&,
			  const scriptum::Style&) const;
  };

  /* Version information. */
  extern std::string version();
};

#endif /* punos_INCLUDED */
