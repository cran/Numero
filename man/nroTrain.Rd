\name{nroTrain}
\alias{nroTrain}
\title{
Train self-organizing map
}
\description{
Iterative algorithm to adapt a self-organizing map (SOM) to a set of
multivariable data.
}
\usage{
nroTrain(som, data, subsample = NULL, metric = "euclid", message = NULL)
}
\arguments{
  \item{som}{
A list object as returned by \code{\link{nroKohonen}()}.
  }
  \item{data}{
A matrix or a data frame.
  }
  \item{subsample}{
Number of rows used during a single training cycle.
  }
  \item{metric}{
Distance metric in data space, either "euclid" or "pearson".
  } 
  \item{message}{
If positive, progress information is printed at the specified
interval in seconds.
  }
}

\details{
The model is fitted according to columns that are found both in the
SOM centroids and the input data.

If \code{subsample} is less than the number of data rows, a random subset
of the specified size is used for each training cycle.
}

\value{
A copy of the list object \code{som}, where the element \code{centroids} is
updated according to the data patterns. The quantization errors during
training are stored in the element \code{history} and the element
\code{metric} is set to the distance measure used.
}

\references{
Gao S, Mutter S, Casey AE, Mäkinen V-P (2018) Numero: a
statistical framework to define multivariable subgroups in complex
population-based datasets, Int J Epidemiology,
https://doi.org/10.1093/ije/dyy113
}

\examples{
# Import data.
fname <- system.file("extdata", "finndiane.txt", package = "Numero")
dataset <- read.delim(file = fname)

# Prepare training data.
trvars <- c("CHOL", "HDL2C", "TG", "CREAT", "uALB")
trdata <- scale.default(dataset[,trvars]) 

# K-means clustering.
km <- nroKmeans(data = trdata)

# Train with full data.
sm <- nroKohonen(seeds = km)
sm <- nroTrain(som = sm, data = trdata)
print(sm$history)

# Train with subsampling.
sm <- nroKohonen(seeds = km)
sm <- nroTrain(som = sm, data = trdata, subsample = 200)
print(sm$history)
}
