\name{nroMatrix}
\alias{nroMatrix}
\title{
Data Input
}
\description{
Import a data matrix from a tab-delimited text file.
}
\usage{
nroMatrix(file, vars = c(), keyvars = TRUE)
}
\arguments{
  \item{file}{
Path to the tab-delimited text file to be imported.
}
  \item{vars}{
Headings of columns to be imported (optional)
}
  \item{keyvars}{
Either a boolean, a character string or a vector of character strings. If TRUE, the first column will be assumed to contain row names, if FALSE, all columns will be assumed to contain data, if a character string or vector, the named column(s) will be assumed to contain row identity info.
  }
}
\details{
  The first row of the (tab-delimited) text file must contain the column headings. If 'vars' is unavailable, all columns in the file will be selected. If
  'keyvars' is TRUE, the first column is assumed to contain the row
  names but no column heading (hence the first row is shifted one
  element to the left with respect to the subsequent data rows). If
  'keyvars' is FALSE, all columns are assumed to contain data and the
  row ranks will be set as row identities. If 'keyvars' is set to a character string, the identity key will be set to the named column (hence this column will be read in as rownames). If 'keyvars' is set to a vector of character strings, the identity key for a row will be a tab-delimited composite of the corresponding elements.
}
\value{
  A numerical matrix with column headings copied from the file and row
  names constructed from identity columns if available. 
}
\author{
Song Gao

Stefan Mutter

Ville-Petteri Makinen
}
\examples{
# Load the tab-delimited dataset.
# The INDEX column contains the key
fileName <- system.file("extdata", "finndiane_dataset.txt", package = "Numero")

# Load all columns.
dat <- nroMatrix(file = fileName, keyvars = FALSE)
print(summary(dat))

# Load all columns except key column(s).
dat <- nroMatrix(file = fileName, keyvars = "INDEX")
print(summary(dat))

# Load specific columns only.
dat <- nroMatrix(file = fileName, vars = c('AGE','MALE'), keyvars = FALSE)
print(summary(dat))
}