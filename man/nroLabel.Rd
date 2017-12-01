\name{nroLabel}
\alias{nroLabel}
\title{
Map Visualization
}
\description{
Optimize the selection of labels on map districts.
}
\usage{
nroLabel(map, values, gap = 2.3)
}
\arguments{
  \item{map}{
A list with the element 'topology', see \code{\link{nroKoho}} for details.
  }
  \item{values}{
A vector of map district values.
  }
  \item{gap}{
Minimum distance between labeled map districts.
  }
}
\details{
The function assigns non-empty labels for districts based on the absolute
deviations from the average district value. The most extreme districts are
picked first, and then the remaining districts are prioritized based on
their value and distance to the other districts already labeled.
}
\value{
A vector of character labels for each district.
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
# Create SOM topology for map of radius 4 with 66 units.
sm <- nroKoho(matrix(1:16, nrow = 4, ncol = 4),
  radius = 4)

# Get labels.
labels <- nroLabel(map = sm, 
  values = sample.int(101, size = 66, replace = TRUE),
  gap = 2.3)
}
