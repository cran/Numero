/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#ifndef abacus_INCLUDED
#define abacus_INCLUDED

#ifndef M_PI
#define M_PI 3.14159265359
#endif

#include <string>
#include <vector>
#include "medusa.h"

namespace abacus {

  /*
   *
   */
  struct Element {
    medusa::mdsize row;
    medusa::mdsize column;
    medusa::mdreal value;
  };

  /*
   *
   */
  class Matrix {
  protected:
    void* buffer;
  public:

    /* Create an asymmetric empty matrix. */
    Matrix();

    /* Copy contents from the input. */
    Matrix(const Matrix&);
    void operator=(const Matrix&);
  
    /* Free resources. */
    ~Matrix();

    /* Insert a new element or add the value to an existing one.
       If the element does not exist, the base value is zero. */
    bool add(const medusa::mdsize, const medusa::mdsize,
	     const medusa::mdreal);

    /* Return column vector. */
    std::vector<medusa::mdreal> column(const medusa::mdsize) const;

    /* Number of elements. */
    medusa::mdsize count() const;

    /* Return all elements with a value. For symmetric matrices,
       only one triangular half is returned. If the input is positive,
       the output is sorted according to ascending value. If negative,
       descending values are returned. */
    std::vector<Element> elements(const int) const;

    /* Insert a new element or set the value of an existing one.
       Returns true if the input was copied in the element. */
    bool insert(const medusa::mdsize, const medusa::mdsize,
		const medusa::mdreal);

    /* Determine the smallest set of elements that connect rows and
       columns. The elements are sorted according to the input flag
       before calling the static version of the function, see info
       at the end of the block. This function has a high overhead if
       called repeatedly. */
    Matrix kruskal(const int) const;

    /* Number of columns. */
    medusa::mdsize order() const;

    /* Return and erase a data element. */
    medusa::mdreal remove(const medusa::mdsize, const medusa::mdsize);

    /* Return row vector. */
    std::vector<medusa::mdreal> row(const medusa::mdsize) const;

    /* Number of rows. */
    medusa::mdsize size() const;

    /* If set to true, the rows and columns are interpreted as inter-
       changeable, akin to a symmetric square matrix. Can be set only
       for empty matrices. The default setting is asymmetric. */
    void symmetric(const bool);
    bool symmetric() const;

    /* Return a data value of an element. */
    medusa::mdreal value(const medusa::mdsize,
			 const medusa::mdsize) const;

    /* Copy data values for a batch of elements. The old values
       within the input array are discarded. */
    void values(std::vector<Element>&) const;

    /* Determine the smallest set of elements that connect rows and
       columns. The elements are checked in the order of the input,
       and moved to the output if they belong to the spanning tree. */
    static std::vector<Element> kruskal(std::vector<Element>&);
  };
  
  /*
   * Approximation of an empirical distribution. The samples should
   * be added in random order to ensure accurate estimates.
   */
  class Empirical {
  private:
    void* buffer;
  public:
    Empirical();

    /* Copy contents from the input. */
    Empirical(const Empirical&);
    void operator=(const Empirical&);

    /* Free resources. */
    ~Empirical();

    /* Add a sample. The first input containts the value and the
       second contains the weight. */
    bool add(const medusa::mdreal, const medusa::mdreal);

    /* Estimate area under the lower tail (2nd input negative) or under
       the higher tail (2nd input positive) or both (2nd input zero). */
    medusa::mdreal p(const medusa::mdreal, const int) const;

    /* Estimate quantile point. The input must be more than zero
       and less than one. */
    medusa::mdreal quantile(const medusa::mdreal) const;

    /* See the external function statistic(). */
    medusa::mdreal statistic(const std::string&) const;

    /* Number of samples. */
    medusa::mdsize size() const;

    /* Number of distinct values. */
    medusa::mdsize spread() const;

    /* Adjusted Gaussian approximation of the distance to distribution
       center. The returned value indicates the distance in standard
       deviations in optimized data space. */
    medusa::mdreal z(const medusa::mdreal) const;
  };

  /*
   *
   */
  class Minimizer {
  private:
    void* buffer;
  public:
    Minimizer();

    /* Copy contents from the input. */
    Minimizer(const Minimizer&);
    void operator=(const Minimizer&);

    /* Free resources. */
    ~Minimizer();

    /* Estimate the function value at a point. Derive a subclass
       and override this with a custom function. */
    virtual medusa::mdreal value(const medusa::mdreal);

     /* Set or get the number of divisions for the search algorithm, and
	set the maximum proportional tolerance with respect to the search
        space for the optimized point. */
    void algorithm(const medusa::mdsize, const medusa::mdreal);
    std::pair<medusa::mdsize, medusa::mdreal> algorithm() const;

    /* Return the last encountered error. */
    std::string error() const;

    /* Set or get the search space from lower to upper point. */
    void space(const medusa::mdreal, const medusa::mdreal);
    std::pair<medusa::mdreal, medusa::mdreal> space() const;

    /* Returns the parameter with the smallest value. */
    static medusa::mdreal optimize(Minimizer&);
  };

  /* A measure of stability for a sequence of numbers. Zero value
     means no convergence and one means full convergence. Use the
     second input to set the value threshold. */
  extern bool convergence(const std::vector<medusa::mdreal>&,
			  const medusa::mdreal);

  /* Pearson's correlation coefficient. */
  extern std::pair<medusa::mdreal, medusa::mdsize>
    correlation(const std::vector<medusa::mdreal>&,
		const std::vector<medusa::mdreal>&);

  /* Eliminate differences in distribution curves between groups of
     samples. The first input contains the sample values, and the
     second the group labels. */
  extern std::vector<medusa::mdreal>
  destratify(const std::vector<medusa::mdreal>&,
	     const std::vector<medusa::mdsize>&);

  /* Find the element indices for the smallest (first in pair) and
     the largest (second in pair) element. */
  extern std::pair<medusa::mdsize, medusa::mdsize>
  extrema(const std::vector<medusa::mdreal>&);

  /* Estimate sample histogram. The first input contains the sample values,
     the second contains the sample weights (can be omitted) and the third
     specifies the bin centers. Each sample value is added to the two
     closest bin centers according to the ratio of distances. */
  extern std::vector<medusa::mdreal>
  histogram(const std::vector<medusa::mdreal>&, 
	    const std::vector<medusa::mdreal>&, 
	    const std::vector<medusa::mdreal>&);
  extern std::vector<medusa::mdreal>
  histogram(const std::vector<medusa::mdreal>&, 
	    const std::vector<medusa::mdreal>&);

  /* Linear interpolation of points (x-coordinate in 1st and y-coordinate
     in 2nd input) at given positions (3rd input). */
  extern std::vector<medusa::mdreal>
  interpolate(const std::vector<medusa::mdreal>&, 
	      const std::vector<medusa::mdreal>&,
	      const std::vector<medusa::mdreal>&);

  /* Estimate quantile. The first input contains the sample values,
     the second sets the sample weights (can be omitted), and the third
     indicates the quantile point. */  
  extern medusa::mdreal quantile(const std::vector<medusa::mdreal>&,
				 const std::vector<medusa::mdreal>&,
				 const medusa::mdreal);
  extern medusa::mdreal quantile(const std::vector<medusa::mdreal>&,
				 const medusa::mdreal);

  /* Calculate the length and angle for a line in 2D Cartesian coordinates.
     The first two inputs specify the starting point and the two last
     specify the end-point.*/
  extern std::pair<medusa::mdreal, medusa::mdreal>
  polarize(const medusa::mdreal, const medusa::mdreal,
	   const medusa::mdreal, const medusa::mdreal);

  /* Calculate desriptive statistics: 'center', 'mean', 'median', 'mode',
     'min', 'max', 'range', 'sd', 'var' and 'number'. The first input
     contains the sample values, the second specifies the sample weights
     and the third indicates the statistic. */
  extern medusa::mdreal statistic(const std::vector<medusa::mdreal>&,
				  const std::vector<medusa::mdreal>&,
				  const std::string&);
  extern medusa::mdreal statistic(const std::vector<medusa::mdreal>&,
				  const std::string&);

  /* Apply transformations: 'z', 'uniform', 'balanced' or 'tapered'. */
  extern std::vector<medusa::mdreal>
  transform(const std::vector<medusa::mdreal>&,
	    const std::string&);

  /* Version information. */
  extern std::string version();
};

#endif /* abacus_INCLUDED */
