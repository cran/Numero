\name{nroMatch}
\alias{nroMatch}
\title{
Find best-matching districts
}
\description{
Compare the multivariate samples from a dataset against the districts
of a self-organizing map (SOM) or similarly formatted profiles.
}
\usage{
nroMatch(som, x)
}
\arguments{
  \item{som}{
A list with an element 'centroids' that contains the matrix of reference
profiles. If the list represents a SOM, the user needs to ensure the topology element from \code{\link{nroKoho}} is also present. If the list represents a trained SOM through \code{\link{nroTrain}}, then the presence of the history element will allow to evaluate the quality of the matching.
  }
  \item{x}{
A data matrix with identical column names to the centroids matrix.
  }
}

\details{
The matching error between a sample and a reference profile is defined as
the Euclidean distance in N-dimensional data space, where N is the number
of variables.
}
\value{
A data frame of three columns: POS contains the row positions within 'x'
of the best maching profiles, DIST is the Euclidean distance in data
space (shorter is better), QUALITY is a scale-independent measure
of the matching quality if training history is available (higher is
better), and COVER shows the percent of data elements that were
available for matching.
}
\author{
Song Gao

Stefan Mutter

Ville-Petteri Makinen
}
\seealso{
\code{\link{nroKmeans}} for detail on centroids, \code{\link{nroKoho}} for details on the SOM and \code{\link{nroTrain}} for details on training the SOM 
}
\examples{
# Create a list of centroids.
ctds <- list(matrix(1:256, nrow = 64, ncol = 4))
names(ctds) <- c('centroids')

# Match the samples against the centroids.
myMatches <- nroMatch(som = ctds, x = matrix(10:265, nrow = 64, ncol = 4))
}