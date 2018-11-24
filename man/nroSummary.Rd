\name{nroSummary}
\alias{nroSummary}

\title{
Estimate subgroup statistics
}

\description{
Combine subgrouping information for districts with the data points that
reside in the districts, and estimate statistics for each subgroup and
variable.
}

\usage{
nroSummary(data, districts, regions, categlim = 8)
}

\arguments{
  \item{data}{
A vector of M elements or an M x N matrix of data values. 
  }
  
  \item{districts}{ 
A vector of M best-matching districts for each row in the data matrix,
please see \link{nroMatch} for a typical usage case.
  }

  \item{regions}{
An vector of K elements that defines if a district belongs to a
larger region (i.e. a subgroup).
  }
  \item{categlim}{
The threshold for the number of unique values before a variable is
considered continuous.
  }
}

\details{
The region vector must have K elements where K is the total number of
map districts. Accordingly, the value at element [i] indicates the region
for the district [i].
}

\value{
A data frame of summary statistics that contains a row for every
combination of subgroups and variables.
}

\author{
Song Gao
Stefan Mutter
Ville-Petteri Makinen
}
