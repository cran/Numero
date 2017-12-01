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
nroTrain(som, x)
}
\arguments{
  \item{som}{
A list of two elements: 'centroids' and 'topology', see \code{\link{nroKoho}} for additional details.
  }
  \item{x}{
A data matrix with the same column names as the 'centroids'.
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
\code{\link{nroKoho}} for details on the SOM
}
\examples{
# Define seeds for the SOM; typically they are obtained from nroKmeans.
mySeeds <- matrix(1:256, nrow = 64, ncol = 4)

# Create an initial, untrained SOM.
sm <- nroKoho(mySeeds, radius = 4)

# Train the SOM.
sm <- nroTrain(som = sm, x = matrix(1001:1256, nrow = 64, ncol = 4))
}