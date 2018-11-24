\name{nroKohonen}
\alias{nroKohonen}

\title{
Self-organizing map
}

\description{
Interpolates the initial district profiles of a self-organizing map based
on pre-determined seed profiles. The function is named after Teuvo Kohonen,
the inventor of the self-organizing map.
}

\usage{
nroKohonen(seeds, radius = 3)
}

\arguments{
  \item{seeds}{
A data matrix of K rows and N columns.
  }

\item{radius}{
Map radius.
  }
}

\value{
A list containing two named elements: 'centroids' contains the
N-dimensional district profiles, and 'topology' is an H x 6 matrix
that contains the 2D spatial layout for the map districts: the first
two columns (X, Y) indicate the positions of districts in Cartesian
coordinates, the other four columns (RADIUS1, RADIUS2, ANGLE1, ANGLE2)
define the perimeter of the district areas for visualisation on
a circular map.
}

\author{
Song Gao
Stefan Mutter
Ville-Petteri Makinen
}

\seealso{
Please see \code{\link{nroKmeans}} as a means to create the seed profiles.
}
