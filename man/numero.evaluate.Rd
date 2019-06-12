\name{numero.evaluate}
\alias{numero.evaluate}

\title{Self-organizing map statistics}

\description{Evaluate regional variation of data values on a
self-organizing map}

\usage{
numero.evaluate(model, data)
}

\arguments{
  \item{model}{
A list object that contains a self-organizing map and a data layout.
  }

  \item{data}{
A matrix or a data frame.
  }
}

\details{
The input argument \code{model} can be the output from
\code{\link{numero.create}()} or from \code{\link{numero.quality}()}.
}

\value{
A list with named elements: \code{som} contains the self-organizing map,
\code{layout} contains the district assignments for data points,
\code{planes} contains smoothed district averages from
\code{\link{nroAggregate}()}, the element \code{ranges} contains the
reference ranges to be used in \code{\link{nroColorize}()},
the element \code{statistics} contains the output from
\code{\link{nroPermute}()}, the element \code{palette} is the name of
the colormap and the element \code{data} contains the data points
that were used for calculating the statistics.
}

\author{
Ville-Petteri Makinen
}

\examples{
# Import data.
fname <- system.file("extdata", "finndiane.txt", package = "Numero")
dataset <- read.delim(file = fname)

# Set identities and manage missing data.
dataset <- numero.clean(dataset, identity = "INDEX")

# Prepare training variables.
trvars <- c("CHOL", "HDL2C", "TG", "CREAT", "uALB")
trdata <- numero.prepare(data = dataset, variables = trvars)

# Create a self-organizing map.
modl <- numero.create(data = trdata)

# Evaluate map statistics for all variables.
stats <- numero.evaluate(model = modl, data = dataset)
}