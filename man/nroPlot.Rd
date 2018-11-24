\name{nroPlot}
\alias{nroPlot}

\title{
Plot a self-organizing map
}

\description{
Create a graphical interface for selecting subgroups from multiple
map colorings simultaneusly.
}

\usage{
nroPlot(elements, colors, labels = NULL, values = NULL, subplot = NULL,
        interactive = FALSE, file = NULL)
}

\arguments{
  \item{elements}{
A data frame with K rows and six columns that contains the district positions
of a self-organizing map (i.e. the topology), please see
\code{\link{nroKohonen}} for details. May also contain additional columns
for visualization of subgroup information.
  }
  \item{colors}{
A character vector with K elements or a K x N matrix of hexadecimal
color codes as strings.
  }
  \item{labels}{
A character vector with K elements or a K x N matrix of district labels.
  }
  \item{values}{
A vector with K elements or a K x N matrix of district values.
  }
  \item{subplot}{
A two-element vector that sets out the number of rows and columns for
a grid layout of multiple colorings.
  }
  \item{interactive}{
If TRUE, an interactive version of the plot is launched.
  }
  \item{file}{
If non-empty, the figure is saved as a Scalable Vector Graphics file.
  }
}

\details{
Some non-alphanumeric characters are not supported and will be
automatically converted to '_'. Too long labels or column names will be
truncated.
}

\value{
A data frame with K rows that contains the topology and subgrouping
information.
}

\author{
Song Gao
Stefan Mutter
Ville-Petteri Makinen
}

\seealso{
Please see \code{\link{nroColorize}} for converting values into color codes,
and \code{\link{nroLabel}} for optimizing which labels to show on the map.
}
