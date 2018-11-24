\name{nroAggregate}
\alias{nroAggregate}

\title{
Regional averages on a self-organizing map
}

\description{
Estimate district averages based on assigned map locations for each data point.
}

\usage{
nroAggregate(topology, districts, data = NULL)
}

\arguments{
  \item{topology}{
A data frame with K rows and six columns that contains the district positions
of a self-organizing map, please see \code{\link{nroKohonen}} for details.  
  }

  \item{districts}{ 
A vector of M best-matching districts for each row in the data matrix,
please see \link{nroMatch} for a typical scenario.
  }

  \item{data}{
A vector of M elements or an M x N matrix of data values. 
  }
}

\value{
A data frame of K rows and N columns that contains the average district
values after smoothing. The data frame has also the attribute 'histogram'
that contains a K x 1 vector of estimated sample counts after smoothing.
}

\author{
Song Gao
Stefan Mutter
Ville-Petteri Makinen
}
