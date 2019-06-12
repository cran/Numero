\name{nroPrune}
\alias{nroPrune}

\title{Reduce collinearity within a dataset}

\description{Detect and merge collinear columns into first principal
components.}

\usage{
nroPrune(data, modules)
}

\arguments{
  \item{data}{
A matrix or a data frame.
  }
  
  \item{modules}{
Pruning parameter, see details.
  }
}

\details{
The pruning parameter \code{modules} is an integer that sets the desired
number of columns in the pruned dataset. If necessary, the number is
automatically revised if the original value cannot be applied to the dataset.

The input argument \code{modules} can also be a list object that is attached
to the output of a previous call to the function, see the description of
the return value.

To determine modules of collinear variables, the function uses K-means
clustering with Spearman correlation as the distance metric.
}

\value{
A data frame or a matrix where a module of collinear columns has been
replaced by a single column. The aggregated values are linear combinations
of the module columns; the coefficients define the principal component of
the module data.

The output also contains the attribute "modules", which can be passed to
the function to replicate the same pruning procedure for another dataset.
}

\author{
Ville-Petteri Makinen
}

\examples{
# Import data.
fname <- system.file("extdata", "finndiane.txt", package = "Numero")
dataset <- read.delim(file = fname)

# Split into men and women.
ds.men <- dataset[which(dataset$MALE == 1),]
ds.women <- dataset[which(dataset$MALE == 0),]

# Exclude unusable columns.
ds.men$INDEX <- NULL
ds.women$INDEX <- NULL
ds.men$MALE <- NULL
ds.women$MALE <- NULL

# Merge collinear variables in one dataset according to the other.
results.men <- nroPrune(data = ds.men, modules = 3)
results.women <- nroPrune(data = ds.women, modules = results.men)
print(attr(results.men, "modules"))
print(summary(results.men$MODULE.1))
print(summary(results.women$MODULE.1))
}
