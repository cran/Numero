\name{nroAggregate}
\alias{nroAggregate}
\title{
Map Statistics
}
\description{
Estimate district averages based on assigned map locations for each data point.
}
\usage{
nroAggregate(map, bmus, x = double())
}
\arguments{
  \item{map}{
A list that must contain the element 'topology',
see \code{\link{nroKoho}} for details.  
  }
  \item{bmus}{ 
A vector of M best-matching districts for the data values 
in 'x', typically the POS column from the \link{nroMatch} output. See \code{\link{nroMatch}} for details.
  }
  \item{x}{
A vector of N data values, can be empty.
  }
}

\value{
A vector of average district values after smoothing. If 'x' is empty,
smoothed sample counts are returned.
}
\author{
Song Gao

Stefan Mutter

Ville-Petteri Makinen
}
\seealso{
\code{\link{nroKoho}} for details on the SOM and \code{\link{nroMatch}} for details on the best matching district labels 
}
\examples{
# Load input data.
dat = matrix(10:265, nrow = 64, ncol = 4)

# Create SOM topology.
sm <- nroKoho(matrix(1:16, nrow = 4, ncol = 4), radius = 4)

# Calculate smoothed sample counts.
sv <- nroAggregate(map = sm, bmus = sample.int(64, size = 64, replace = TRUE))
}
