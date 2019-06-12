\name{nroPlot}
\alias{nroPlot}

\title{
Plot a self-organizing map
}

\description{
Create a graphical interface for selecting subgroups from multiple
map colorings simultaneously.
}

\usage{
nroPlot(elements, colors, labels = NULL, values = NULL, subplot = NULL,
        interactive = FALSE, clear = NULL, file = NULL)
}

\arguments{
  \item{elements}{
A data frame with K rows and six columns that contains the district positions
of a self-organizing map.
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
  \item{clear}{
If TRUE, all graphics devices are cleared when the plot is refreshed.
}
  \item{interactive}{
If TRUE, an interactive version of the plot is launched.
  }
  \item{file}{
If non-empty, the figure is saved as a Scalable Vector Graphics file
instead of plotting on the R device.
  }
}

\details{
The input \code{elements} must follow the topology format
\code{\link{nroKohonen}()}, but may also contain the columns REGION,
REGION.label and REGION.color that specify the name for a subset of
districts, the single character labels to be shown on top of those districts,
and the color of the labels. The input can also be the list object as in the
output format of \code{\link{nroKohonen}()}.

Some non-alphanumeric characters are not supported and will be
automatically converted to '_'. Too long labels or column names will be
truncated.

The default value for \code{clear} is TRUE to prevent multiple plot windows
from accumulating within the RStudio. If you are running R from the terminal
or using detached devices, setting \code{clear} will retain the current
window when refreshing.
}

\value{
A data frame with K rows that contains the topology and subgrouping
information.
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

# Detect binary columns.
dataset <- nroPreprocess(dataset, method = "")

# Prepare training data.
trvars <- c("CHOL", "HDL2C", "TG", "CREAT", "uALB")
trdata <- scale.default(dataset[,trvars])

# K-means clustering.
km <- nroKmeans(data = trdata)

# Self-organizing map.
sm <- nroKohonen(seeds = km)
sm <- nroTrain(som = sm, data = trdata)

# Assign data points into districts.
matches <- nroMatch(centroids = sm, data = trdata)

# Select a subset of variables and detect binary data.
vars <- c("AGE", "MALE", "uALB", "CHOL", "DIAB_KIDNEY", "DECEASED")
selected <- nroPreprocess(dataset[,vars], method = "")

# Calculate district averages for seleted variables.
vars <- c("AGE", "MALE", "uALB", "CHOL", "DIAB_KIDNEY", "DECEASED")
planes <- nroAggregate(topology = sm, districts = matches, data = selected)

# Estimate statistics.
stats <- nroPermute(som = sm, districts = matches, data = selected)

# Set visuals.
colrs <- nroColorize(values = planes, amplitudes = stats)
labls <- nroLabel(topology = sm, values = planes)

# Plot colorings on screen.
nroPlot(elements = sm, colors = colrs, labels = labls)
}