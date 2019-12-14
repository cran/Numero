\name{nroPair}
\alias{nroPair}

\title{Match similar rows}

\description{Pair up closest matching rows from two datasets}

\usage{
nroPair(data.x, data.y, subsample = 500, standard = TRUE)
}

\arguments{
  \item{data.x}{
A matrix or a data frame with column names.
  }

  \item{data.y}{
A matrix or a data frame with column names.
  }

  \item{subsample}{
Maximum number of pairings to test per row.
  }

  \item{standard}{
If \code{TRUE}, the scales of variables are standardized for processing.
  }
}

\details{
The function detects columns that are shared between the two datasets by
their names. Pairs of rows across datasets are then compared using Euclidean
distance to determine the best matches.
}

\value{
A data frame that has up to five columns: ROW.x and ROW.y contain the pairings
using row indices and DISTANCE contains the distances in (standardized) data
space. If row names are available, the columns ROWNAME.x and ROWNAME.y are
added.

The output is sorted according to the matching distance.
}

\author{
Ville-Petteri Makinen
}

\examples{
# Import data.
fname <- system.file("extdata", "finndiane.txt", package = "Numero")
dataset <- read.delim(file = fname)

# Set row names.
rownames(dataset) <- paste("r", 1:nrow(dataset), sep="")

# Prepare training data.
trvars <- c("CHOL", "HDL2C", "TG", "CREAT", "uALB")
trdata <- scale.default(dataset[,trvars])

# Split by sex.
women <- which(dataset$MALE == 0)
men <- which(dataset$MALE == 1)

# Find best matches.
pairs <- nroPair(data.x = trdata[women,], data.y = trdata[men,])
print(head(pairs))
}