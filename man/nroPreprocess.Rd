\name{nroPreprocess}
\alias{nroPreprocess}

\title{
Data cleaning and standardization
}

\description{
Convert to numerical values, remove unusable rows and columns, and
standardize scale of each variable.
}

\usage{
nroPreprocess(data, training = NULL, strata = NULL, key = NULL)
}

\arguments{
  \item{data}{
A matrix or a data frame. 
  }
  
  \item{training}{
A character vector that contains the headings of feature columns that
are intended for model training and data point matching.
  }

  \item{strata}{
The heading of the column that defines data batches; each batch will be
processed separately.
  }

  \item{key}{
The heading of the column that contains row identifiers.
  }
}

\value{
A list with three members is returned: 'original' contains a subset of the
original dataset were unusable rows were removed, 'values' contains those
columns that could be converted to numbers, and 'features' contains
the standardized training columns.
}

\author{
Song Gao
Stefan Mutter
Ville-Petteri Makinen
}
