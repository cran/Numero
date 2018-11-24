\name{nroTrain}
\alias{nroTrain}
\title{
Train Self-Organizing Map
}
\description{
Iterative algorithm to adapt a self-organizing map (SOM) to a set of
multivariable data.
}
\usage{
nroTrain(som, data, subsample = NULL)
}
\arguments{
  \item{som}{
A list of two elements: 'centroids' and 'topology', see \code{\link{nroKohonen}} for additional details.
  }
  \item{data}{
A data matrix with the same column names as the 'centroids'.
  }
  \item{subsample}{
Number of rows used during a single training cycle.
  }
}

\value{
A copy of 'som', where the 'centroids' list element is updated
according to the data patterns. In addition, the quantization
errors during training is stored in the element 'history'.
}
\author{
Song Gao
Stefan Mutter
Ville-Petteri Makinen
}
\seealso{
\code{\link{nroKohonen}} for details on the SOM
}
