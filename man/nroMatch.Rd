\name{nroMatch}
\alias{nroMatch}

\title{
Best-matching districts
}

\description{
Compare the multivariate samples from a dataset against the district
profiles of a self-organizing map (SOM).
}

\usage{
nroMatch(som, data)
}

\arguments{
  \item{som}{
Either a matrix or a list with an element 'centroids' that contains the
matrix of reference profiles, such as outputs from \code{\link{nroKmeans}}
and \code{\link{nroTrain}}.
  }
  
  \item{data}{
A data matrix with identical column names to the centroid matrix.
  }
}

\details{
The matching error between a sample and a reference profile is defined as
the Euclidean distance in N-dimensional data space, where N is the number
of variables.
}

\value{
A vector of integers with elements corresponding to the rows in 'data'.
Each element contains the index of the best matching centroid from 'som'.
The vector also has the attribute 'quality' that contains three columns:
RESIDUAL is the Euclidean distance in data space (shorter is better),
QUALITY is a scale-independent measure of the matching quality if training
history is available (higher is better, 1 means equal quality to the
training set), and COVERAGE shows the proportion of data elements that were
available for matching. Finally, the names of the columns that were used
for matching are stored in the attribute 'features'.
}

\author{
Song Gao
Stefan Mutter
Ville-Petteri Makinen
}
