/* Created by Ville-Petteri Makinen 2014
   South Australian Health and Medical Research Institute */

#ifndef turnip_INCLUDED
#define turnip_INCLUDED

#include <string>
#include <vector>
#include <unordered_set>
#include "medusa.h"
#include "abacus.h"

namespace turnip {

  /*
   *
   */
  struct Edge {
    std::string tail;
    std::string head;
    medusa::mdreal weight;
  };

  /*
   *
   */
  class Network {
  protected:
    void* buffer;
  public:
    Network();

    /* Copy contents from the argument. */
    Network(const Network&);
    void operator=(const Network&);

    /* Free resources. */
    ~Network();

    /* Add a weighted directed edge between two vertices. If the
       edge already exists, increment the weight by the input value.
       Returns an error message if failed. */
    std::string add(const std::string&, const std::string&,
		    const medusa::mdreal);

    /* Add a weighted directed network to the current object.
       Returns the number of added edges. */
    medusa::mdsize add(const Network&);

    /* Create non-overlapping vertex communities according to current
       topology. Edge direction and sign of edge weight are ignored by
       the community detection algorithm. Returns community number. */
    medusa::mdsize agglomerate();
 
    /* Combine networks. If second input is negative, returns exclusion,
       if positive returns intersection, otherwise returns union (with
       sum weight for overlapping edges). */
    Network combine(const Network&, const int) const;
 
    /* Find vertex community. Returns the name of the hub. */
    std::string community(const std::string&) const;
    
    /* Return the component identifier for a vertex. The identifiers are
       numbered from 0 to N-1, where N is the total number of connected
       components. Note that if edges are added/removed from the network,
       the component indexing may change. */
    medusa::mdsize component(const std::string&) const;

    /* Return vertex degree. If the second input is negative (positive),
       incoming (outgoing) degree is returned, otherwise all adjacent
       edges are counted. */
    medusa::mdsize degree(const std::string&, const int) const;

    /* Return the current network topology. */
    std::vector<Edge> edges() const;

    /* Return edges that connect two sets of vertices. If the third
       input is negative, only edges from the second input to the first
       input are included. If the third input it positive, only edges
       from the first set to the second are included. */
    std::vector<Edge> edges(const std::vector<std::string>&,
			    const std::vector<std::string>&,
			    const int) const;

    /* Return the names of hub vertices. */
    std::vector<std::string> hubs() const;

    /* Insert a weighted directed edge between two vertices. Overwrites
       any existing data. Returns an error message if failed. */
    std::string insert(const std::string&, const std::string&,
		       const medusa::mdreal);

    /* Insert a weighted directed network to the current object.
       Overwrites any existing data. Returns the number of added edges. */
    medusa::mdsize insert(const Network&);

    /* Return vertex names in the subnetwork adjacent to a vertex. If the
       second input is negative (positive), only incoming (outgoing) edges
       are considered. If the second input is zero, the vertex itself is
       also included. */
    std::vector<std::string> neighbors(const std::string&, const int) const;

    /* Number of vertices. */
    medusa::mdsize order() const;

    /* Use spanning forest to remove less important edges. The first input
       sets the number of successive spanning trees for thinning the graph.
       Edge direction and sign of edge weight are ignored by the pruning
       algorithm. Returns the size of the pruned network. */
    medusa::mdsize prune(const medusa::mdreal);

    /* If set to true, edges with self are allowed. Default is false.
       Can be set only when network is empty. */
    void redundant(const bool);
    bool redundant() const;

    /* Number of edges. */
    medusa::mdsize size() const;

    /* Return subgraph that connects the set of vertices. For more
       efficient operation, use the Engine class instead. */
    Network subnetwork(const std::vector<std::string>&) const;

    /* Remove and return all the connected components. */
    std::vector<Network> split();

    /* Calculate vertex strength. If the second input is negative
       (positive), incoming (outgoing) strength is returned. */
    medusa::mdreal strength(const std::string&, const int) const;

    /* Test if a vertex is part of a triangle. */
    bool triangular(const std::string&) const;

    /* Return vertex names in the network. */
    std::vector<std::string> vertices() const;

    /* Return vertices that belong to the same community. */
    std::vector<std::string> vertices(const std::string&) const;

    /* Return the edge weight between two vertices. */
    medusa::mdreal weight(const std::string&, const std::string&) const;
  };

  /*
   *
   */
  class Engine {
  private:
    void* buffer;
  public:
    Engine();

    /* Optimize data structures. */
    Engine(const Network&);

    /* Copy contents from the argument. */
    Engine(const Engine&);
    void operator=(const Engine&);

     /* Free resources. */
    ~Engine();
       
    /* Unique numeric vertex identity. The identity codes start from
       zero and end with N-1 where N is the order of the network.
       A number larger than N-1 is returned if vertex is unknown. */
    medusa::mdsize operator[](const std::string&) const;

    /* Assign a numerical attribute to an existing vertex. */
    bool assign(const medusa::mdsize, const medusa::mdreal);

    /* Return all edges. */
    std::vector<abacus::Element> edges() const;

    /* Return edges that interconnect a set of vertices. */
    std::vector<abacus::Element>
    edges(const std::vector<medusa::mdsize>&) const;

    /* Collect adjacent vertices, including self. */
    std::unordered_set<medusa::mdsize>
    neighbors(const medusa::mdsize) const;

    /* Number of vertices. */
    medusa::mdsize order() const;

    /* Return a random subset of vertices. The input specifies the
       probability of picking a vertex. Note that the size of the
       set also fluctuates randomly. */
    std::vector<medusa::mdsize> pick(const medusa::mdreal) const;

    /* Set randomization pool for vertex values. The input must not
       contain unusable values. */
    void pool(const std::vector<medusa::mdreal>&);

    /* Remove any attributes assigned to a vertex. */
    void release(const medusa::mdsize);

    /* Return a random subset of vertices. The input specifies
       the number of vertices (up to the order of the graph). */
    std::vector<medusa::mdsize> shuffle(const medusa::mdsize) const;

    /* Return the values of all vertices. */
    std::vector<medusa::mdreal> values() const;
    
    /* Return values for a set of vertices. If randomization pool is set,
       missing values or vertices are replaced by random picks. */
    std::vector<medusa::mdreal>
    values(const std::vector<medusa::mdsize>&) const;

    /* Return values for the two end-points of edges. If randomization
       pool is set, missing values are replaced by random picks. */
    std::vector<std::pair<medusa::mdreal, medusa::mdreal> >
    values(const std::vector<abacus::Element>&) const;

    /* Return vertex identities. */
    std::vector<std::string> vertices() const;

    /* Number of edges. */
    medusa::mdsize size() const;
  };

  /*
   *
   */
  class Cohesion {
  private:
    void* buffer;
  public:
    Cohesion();

    /* Copy contents from the argument. */
    Cohesion(const Cohesion&);
    void operator=(const Cohesion&);

    /* Free resources. */
    ~Cohesion();

    /* Count the number of assignments for an item. */
    medusa::mdsize count(const std::string&) const;
    
    /* Convert a network of modules to a network of items. Suppose there
       is an edge between modules A and B, each item from A will be paired
       with an item in B in a way that maximizes edge magnitudes. */
    Network expand(const Network&) const;
 
    /* Add a module (1st) and an item in it (2nd). The third input sets
       the assignment weight (must be positive). Returns an error message
       if failed. */
    std::string insert(const std::string&, const std::string&,
		       const medusa::mdreal);

    /* Return all items. */
    std::vector<std::string> items() const;

    /* Return adjacent items for a module. */
    std::vector<std::string> items(const std::string&) const;

    /* Find the adjacent items for all modules and sort them from the most
       shared to the least shared. The names of items are stored in the
       first input. The second input is filled with the aggregated weight
       for modules that contain an item. Returns the number of items. */
    medusa::mdsize items(std::vector<std::string>&,
			 std::vector<medusa::mdreal>&) const;

    /* Find the adjacent items for a set of modules (3rd input) and sort
       them from the most shared to the least shared. The names of items
       are stored in the first input. The second input is filled with the
       aggregated weight for modules that contain an item. Returns the
       number of items. */
    medusa::mdsize items(std::vector<std::string>&,
			 std::vector<medusa::mdreal>&,
			 const std::vector<std::string>&) const;

    /* Return items that are adjacent to the selected modules. The second
       argument indicates the type of selection: 'union', 'exclusion'
       or 'intersection'. Duplicated items are excluded. */
    std::vector<std::string> items(const std::vector<std::string>&,
				   const std::string&) const;

    /* Return all modules. */
    std::vector<std::string> modules() const;

    /* Return the adjacent modules for an item. */
    std::vector<std::string> modules(const std::string&) const;

    /* Return the number of modules. */
    medusa::mdsize order() const;
    
    /* Collect pairs of modules and calculate respective sharing
       coefficients. Available methods include 'Jaccard' and 'S-S'
       for Szymkiewicz–Simpson (first input). The second input sets
       the threshold to include edges in the network. */
    Network overlaps(const std::string&, const medusa::mdreal) const;

    /* Return the number of items. */
    medusa::mdsize size() const;

    /* Return the number of items in a module. */
    medusa::mdsize size(const std::string&) const;

    /* Return the topology for a subset of modules. */
    Cohesion subset(const std::vector<std::string>&) const;

    /* Return the assignment weight between a module (first input)
       and an item (second input). */
    medusa::mdreal weight(const std::string&, const std::string&) const;
  };

  /* Version information. */
  extern std::string version();
}

#endif /* turnip_INCLUDED */
