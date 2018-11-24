\name{nroLabel}
\alias{nroLabel}

\title{
Label pruning
}

\description{
Optimize the selection of labels on map districts.
}

\usage{
nroLabel(topology, values, gap = 2.3)
}

\arguments{
  \item{topology}{
A matrix with six columns that contain the geometric properties of
map districts, please see \code{\link{nroKohonen}} for details.
  }
  \item{values}{
A vector of K elements or an K x N matrix of smoothed district values,
where K is the number of map districts and N is the number of variables,
please see \code{\link{nroAggregate}} for details.
  }
  \item{gap}{
Minimum distance between map districts with non-empty labels.
  }
}

\details{
The function assigns non-empty labels for districts based on the absolute
deviations from the average district value. The most extreme districts are
picked first, and then the remaining districts are prioritized based on
their value and distance to the other districts already labeled.
}

\value{
A data frame with K rows and N columns that contains labels for
all the districts.
}

\author{
Song Gao
Stefan Mutter
Ville-Petteri Makinen
}
