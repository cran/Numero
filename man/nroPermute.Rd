\name{nroPermute}
\alias{nroPermute}

\title{
Permutation analysis
}

\description{
Estimate the dynamic range (and statistical significance) for
regional patterns on a self-organizing maps using permutations.
}

\usage{
nroPermute(som, districts, data, n = 10000)
}

\arguments{
  \item{som}{
A list that must contain the element 'topology',
see  \code{\link{nroKohonen}} for details.
  }
  
  \item{districts}{
A numeric vector of M best matching districts, typically the output
from /code{\link{nroMatch}}.  
  }
  
  \item{data}{
A numeric vector of M values or an M x N matrix, where M is the number of
data points and N is the number of variables.
  }
  
  \item{n}{
Maximum number of permutations.
  }
}

\value{
A data frame with eight columns. For example, P.z is a parametric estimate
for statistical significance, P.freq is the frequency-based estimate for
statistical signicance, and Z is the estimated z-score of how far the
observed map plane is from the average randomly generated layout.
N.data indicates how many data values were used and N.cycles tells the
number of completed permutations. AMPLITUDE is the dynamic range
for colors that can be used in \code{\link{nroColorize}}.
}

\author{
Song Gao
Stefan Mutter
Ville-Petteri Makinen
}
