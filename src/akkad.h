/* Created by Ville-Petteri Makinen 2003-2010
   Copyright (C) V-P Makinen
   All rights reserved */

#ifndef akkad_INCLUDED
#define akkad_INCLUDED

#include <string>
#include <vector>
#include "medusa.h"

namespace akkad {

  /*
   *
   */
  class Messenger {
  public:

    /* Advance internal counter according to the input
       and return the updated value. */
    virtual unsigned long counter(const medusa::mdsize) = 0;

    /* Returns true if the user has signalled the instructor to stop. */
    virtual bool stopping() const = 0;

    /* Print an error, plain message or warning. Returns the number of
       characters that were printed. */
    virtual medusa::mdsize alert(const std::string&) = 0;
    virtual medusa::mdsize print(const std::string&) = 0;
    virtual medusa::mdsize warn(const std::string&) = 0;

    /* Notify progress by proportion of completed work if a specific
       time interval has elapsed from the previous print event. Does
       nothing if input is below zero or above one. Returns non-zero
       if notification was accepted. */
    virtual medusa::mdsize progress(const medusa::mdreal) = 0;

    /* Print a plain message only if a specific time interval has
       elapsed from the previous print event. Returns the number of
       characters that were printed. */
    virtual medusa::mdsize report(const std::string&) = 0;

    /* Return the number of seconds elapsed since last call. */
    virtual medusa::mdreal seconds() = 0;

    /* The sum of message text sizes. */
    virtual unsigned long size() const = 0;

    /* Null device. All messages will be ignored. */
    static Messenger* null();

    /* Global implementation. The object is deleted
       automatically when the program ends. */
    static Messenger* standard();
  };

  /*
   * Organize instruction items into a spreadsheet.
   */
  class Scribe {
  private:
    void* buffer;
  public:
    Scribe();

    /* Import instructions from a file. */
    Scribe(const std::string&);

    /* Copy contents from the argument. */
    Scribe(const Scribe&);
    void operator=(const Scribe&);

    /* Free resources. */
    ~Scribe();

    /* Return the data fields of the first occurrence of an instruction.
       If none found, a two-element vector of empty strings is returned. */
    std::vector<std::string> operator[](const std::string&) const;

    /* Check if the first value for an instruction points to an
       accessible file. The second input sets the parameter flag on
       whether the file is meant for reading or writing. Returns an
       error message if failed. */
    std::string accessible(const std::string&, const std::string&) const;

    /* Add a new instruction. */
    bool add(const std::string&, const std::string&);
    bool add(const std::vector<std::string>&);

    /* Overwrite data fields of shared instructions in the current object
       by values from the argument. Returns the number of updates. */
    medusa::mdsize assign(const Scribe&);

    /* Return contents in a single string. */
    std::string contents() const;

    /* Return the data fields of the jth occurrence of an instruction.
       If none found, a vector with two empty strings is returned. */
    std::vector<std::string> get(const std::string&,
				 const medusa::mdsize) const;

    /* Find distinct elements according to an instruction (second
       input), and a target vector of words (third input). The element
       indices with respect to the word vector are stored in the first
       input. Return an error message if failed. */
    std::string find(std::vector<medusa::mdsize>&, const std::string&,
		     const std::vector<std::string>&) const;

    /* Return available instruction keywords. */
    std::vector<std::string> keys() const;

    /* Remove all occurrences of an instruction. */
    bool remove(const std::string&);

    /* Total number of instruction occurrences. */
    medusa::mdsize size() const;

    /* Save contents in a tab-delimited text file. */
    unsigned long save(const std::string&) const;
  };

  /* Version identification. */
  extern std::string version();
};

#endif /* akkad_INCLUDED */
