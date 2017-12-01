\name{nroKmeans}
\alias{nroKmeans}
\title{
Data Clustering
}
\description{
K-means clustering for multi-dimensional data.
}
\usage{
nroKmeans(x, k = 3)
}
\arguments{
  \item{x}{
Numerical data frame or matrix with M rows and N columns.
  }
  \item{k}{
Number of centroids.
  }
}
\details{
 The K centroids are determined by Lloyd's algorithm with Euclidean
 distances.
}
\value{
A list of three named elements: 'centroids' is a k x N matrix of the main
results and 'bmcs' contains the M best matching centroid labels for each sample from the original dataset. Finally, 'history' is the chronological record of
training errors.
}
\author{
Song Gao

Stefan Mutter

Ville-Petteri Makinen
}
\examples{
# Cluster data into 4 clusters.
clust <- nroKmeans(matrix(1:10), k = 4)

# Show the centroids of each cluster.
print(clust$centroids)
}
