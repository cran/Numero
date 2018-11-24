\name{nroKmeans}
\alias{nroKmeans}

\title{
K-means clustering
}

\description{
K-means clustering for multi-dimensional data.
}

\usage{
nroKmeans(data, k = 3, subsample = NULL, balance = 0)
}

\arguments{
  \item{data}{
Numerical data frame or matrix with M rows and N columns.
  }
  \item{k}{
Number of centroids.
  }  
  \item{subsample}{
Number of randomly selected rows used during a single training cycle.
  }    
  \item{balance}{
If 0, the algorithm is applied with no balancing, if 1 all the clusters
will be forced to be of equal size.
  }
}

\details{
 The K centroids are determined by Lloyd's algorithm with Euclidean
 distances. If 'subsample' is less than the number of data rows, a random
 subset of rows is used for each training cycle.
}

\value{
A list of three named elements: 'centroids' is a k x N matrix of the main
results and 'layout' contains the M best-matching centroid labels for each sample from the original dataset. Finally, 'history' is the chronological record of
training errors.
}

\author{
Song Gao
Stefan Mutter
Ville-Petteri Makinen
}
