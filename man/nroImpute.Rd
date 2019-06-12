\name{nroImpute}
\alias{nroImpute}

\title{Impute missing values}

\description{Find nearest neighbors by Euclidean distance and impute missing values.}

\usage{
nroImpute(data, subsample = 500, standard = TRUE)
}

\arguments{
  \item{data}{
A matrix or a data frame.
  }
  
  \item{subsample}{
Maximum number of matchings to test per imputed row.
  }

  \item{standard}{
If \code{TRUE}, the scales of variables are standardized for processing.
  }
}

\details{
Non-numeric columns are excluded from processing and returned unaltered.

If \code{subsample} is less than the number of rows, an equivalent number
of randomly picked rows is selected to find the nearest neighbor.
}

\value{
A copy of the input argument where missing values have been imputed.
}

\author{
Ville-Petteri Makinen
}

\examples{
# Import data.
fname <- system.file("extdata", "finndiane.txt", package = "Numero")
dataset <- read.delim(file = fname)

# Convert identities to strings (produces a warning later).
ds <- dataset
ds$INDEX <- paste("K", ds$INDEX, sep=".")

# Introduce missing values to cholesterol.
missing <- seq(from = 1, to = nrow(ds), length.out = 40)
missing <- unique(round(missing))
ds$CHOL[missing] <- NA

# Impute missing values with and without standardization.
ds.std <- nroImpute(data = ds, standard = TRUE)
ds.orig <- nroImpute(data = ds, standard = FALSE)

# Compare against "true" cholesterol values.
rho.std <- cor(ds.std$CHOL[missing], dataset$CHOL[missing])
rho.orig <- cor(ds.orig$CHOL[missing], dataset$CHOL[missing])
cat("Correlation, standard = TRUE:  ", rho.std, "\n", sep="")
cat("Correlation, standard = FALSE: ", rho.orig, "\n", sep="")
}
