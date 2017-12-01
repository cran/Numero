\name{nroPermute}
\alias{nroPermute}
\title{
Map Statistcs
}
\description{
Estimate the dynamic range (and statistical significance) for a
regional pattern on a two-dimensional map using permutations.
}
\usage{
nroPermute(map, bmus, x, n = 10000)
}
\arguments{
  \item{map}{
A list that must contain the element 'topology',
see  \code{\link{nroKoho}} for details.
  }
  \item{bmus}{
A numeric vector of M best matching districts, typically the POS column from the \link{nroMatch} output. See \code{\link{nroMatch}} for details.  
  }
  \item{x}{
A numeric vector of M values.
  }
  \item{n}{
Maximum number of permutations. If NA, a limited number of permutations
is performed without estimating statistical significance.
  }
}
\details{
This function repeats the following procedure: i) re-assign best-matching district randomly in accordance with the null hypothesis, ii) recalculate the average district values across the map and iii) summarize the regional variation with a single descriptive statistic. When a sufficient number of cycles has been achieved, the null distribution of the descriptive statistic is analyzed to determine how far, in terms of standard deviations, the observed value is from the mean prediction by the null hypothesis. This distance is reported as the Z-score of regional variation. Furthermore, the function also estimates how frequently a permuted layout produced a regional variation that exceeded the observation (frequency-based P-value).
}
\value{
  A data frame with one row and five columns: P.z is a parametric estimate
  for statistcal significance. P.freq is the frequency-based estimate for
  statistical signicance, Z is the estimated z-score of how far the
  observed map plane is from the average randomly generated layout,
  N.data indicates how many data values were used, and N.cycles tells the
  number of completed permutations.
}
\author{
Song Gao

Stefan Mutter

Ville-Petteri Makinen
}
\seealso{
\code{\link{nroKoho}} for details on the SOM and \code{\link{nroMatch}} for details on the best matching district labels 
}
\examples{
# Create a data vector.
dat = c(101:164)

# Create the SOM topology.
sm <- nroKoho(matrix(1:16, nrow = 4, ncol = 4), 
  radius = 4)

# Permute labels for the data vector using 10 permutations.
stats <- nroPermute(map = sm, bmus = c(1:64), x = dat, n = 10)
}