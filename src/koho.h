/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#ifndef koho_INCLUDED
#define koho_INCLUDED

#include <string>
#include <vector>
#include "medusa.h"
#include "punos.h"

namespace koho {

  /*
   *
   */
  class Model {
  private:
    void* buffer;
  public:
    Model();

    /* Set up model with the given topology. */
    Model(const punos::Topology&);

    /* Copy the contents from the input. */
    Model(const Model&);
    void operator=(const Model&);

    /* Free local resources. */
    ~Model();

    /* Set a prototype vector. The first inputs indicates the vertical
       level, the second sets the unit, and the third contains the data. */
    std::string configure(const medusa::mdsize, const medusa::mdsize,
			  const std::vector<medusa::mdreal>&);

    /* Return the distance in data space to the best matching unit. */
    medusa::mdreal distance(const std::string&) const;

    /* Return sample identities. */
    std::vector<std::string> identities() const;

    /* Insert a new data point. The first input is the sample identity
       and the second indicates the vertical position. The third input
       sets the values for each dimension. Returns a non-empty message
       if failed. */
    std::string insert(const std::string&, const medusa::mdreal,
		       const std::vector<medusa::mdreal>&);
    
    /* Return the best matching unit for a sample. */
    medusa::mdsize location(const std::string&) const;

    /* Return the prototype vector for the specified vertical
       level (1st) and map unit (2nd). */
    std::vector<medusa::mdreal> prototype(const medusa::mdsize,
					  const medusa::mdsize) const;

    /* Return the number of samples and data points. */
    std::pair<medusa::mdsize, medusa::mdsize> size() const;

    /* Return current map topology. */
    punos::Topology topology() const;

    /* Train the map according to the inserted data points. The input is
       filled with training errors from each cycle. */
    std::string train(std::vector<medusa::mdreal>&);
  };

  /*
   *
   */
  class Engine {
  private:
    void* buffer;
  public:
    Engine();

    /* Set up an engine with the given topology. */
    Engine(const punos::Topology&);

    /* Copy the contents from the input. */
    Engine(const Engine&);
    void operator=(const Engine&);

    /* Free local resources. */
    ~Engine();
    
    /* Calculate data point frequencies across the map volume
       using the current layout. */
    std::vector<std::vector<medusa::mdreal> > count() const;

    /* Insert a new data value. The first input is the sample identity
       and the second sets the vertical layer. The third input is the
       data point (no missing data). Returns an error if failed. */
    std::string insert(const std::string&, const medusa::mdreal,
		       const std::vector<medusa::mdreal>&);

    /* Put a sample on a map unit. Works only if the sample has
       at least one existing data point. Returns a non-empty
       message if failed. */
    std::string put(const std::string&, const medusa::mdsize);

    /* Estimate unit averages of data values using the current
       layout. The input indicates the target variable. */
    std::vector<std::vector<medusa::mdreal> >
    average(const medusa::mdsize) const;

    /* Shuffle unit labels randomly. If the input is true,
       pick with replacement. */
    void shuffle(const bool);
  };

  /* Utility functions. */
  extern std::vector<std::vector<medusa::mdreal> >
  operate(const std::vector<std::vector<medusa::mdreal> >&, const char,
	  const std::vector<std::vector<medusa::mdreal> >&);
  extern std::vector<std::vector<medusa::mdreal> >
  kmeans(std::vector<medusa::mdsize>&,
	 std::vector<medusa::mdreal>&,
	 const std::vector<std::vector<medusa::mdreal> >&,
	 const medusa::mdsize);

  /* Version information. */
  extern std::string version();
};

#endif /* koho_INCLUDED */
