\name{nroStatistic}
\alias{nroStatistic}

\title{Estimate average value}

\description{Calculate weighted descriptive statistics for datasets with
missing and duplicated values.}

\usage{
nroStatistic(data, weights = NULL, method = "center")
}

\arguments{
  \item{data}{
A matrix or a data frame.
  }
  
  \item{weights}{
A matrix or a data frame of positive values with the same row and column names as \code{data}.
  }

  \item{method}{
The name of the test statistic, see details.
}
}

\details{
Methods include "center", "mean", "median", "mode", "min", "max", "range",
"sd","iqr", "var" and "number".

For binary columns, the method "center" returns the mean. If there are more
than two distinct values (nuniq > 2), the formula
rho * mean + (1 - rho) * median, rho = 2.2 / nuniq / log(nuniq + 1) is used.
If a column contains duplicated values, the duplicates are expanded linearly
prior to calculating the median in the formula. This mitigates numerical
artefacts from bootstrapping or permutations.

Range is defined the difference between the maximum and minimum value and
"iqr" stands for inter-quartile range.

The method "number" returns the number of usable values.
}

\value{
A numeric vector with elements named according to data columns.
}

\examples{
# Import data.
fname <- system.file("extdata", "finndiane.txt", package = "Numero")
dataset <- read.delim(file = fname)

# Calculate centers.
mu <- nroStatistic(dataset, method = "center")
print(mu)

# Calculate inter-quartile ranges.
iqr <- nroStatistic(dataset, method = "iqr")
print(iqr)
}
