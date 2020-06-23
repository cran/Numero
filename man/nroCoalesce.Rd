\name{nroCoalesce}
\alias{nroCoalesce}
\alias{nroCoalesce.prune}
\alias{nroCoalesce.split}
\alias{nroCoalesce.merge}

\title{Reduce collinearity within a dataset}

\description{Determine and merge collinear columns into the
first principal component.}

\usage{
nroCoalesce(data, threshold = 0.25, degree = 4.0)

nroCoalesce.prune(network, degree)

nroCoalesce.split(network, mtx = FALSE)

nroCoalesce.merge(data, modules)
}

\arguments{
  \item{data}{
A matrix or a data frame.
  }
  
  \item{network}{
A matrix or a data frame with three columns for edge end-points and weight.
  }

  \item{threshold}{
Minimum squared Pearson correlation to be counted as collinear.
  }
  
  \item{degree}{
Topological density parameter, see details.
  }
  
  \item{mtx}{
If TRUE, results are also returned in matrix form.
  }
  
  \item{modules}{
A named list of coefficient vectors that defines modules of
collinear columns.
  }
}

\details{
The prune function applies successive maximal spanning trees to separate
topologically important edges from the rest. The degree parameter
sets the number of spanning trees applied (non-integers are supported).

For high-dimensional datasets, the correlation network will be very
large. These functions will run in a few seconds for datasets with less
than 1,000 columns and 10,000 rows and they will work also for larger
datasets but completion may take a long time.

Memory usage is modest for high-dimensional datasets in most scenarios
as the correlation structures are first filtered before stored in a sparse
format (except when matrices are returned from the split function).

The merge function implements heuristic subsampling techniques that are
non-deterministic and may cause small variations between analyses.
}

\value{
The primary function returns a data frame or a matrix where any module of
collinear columns has been replaced by a single column. The aggregated values
are linear combinations of the module columns; the output includes the
attribute "modules" that contains coefficients for the principal components.
The output also includes the attribute "network" that contains the
pruned correlation network and "subnets" that contains the subnetworks for
each module and a separate entry for any remaining edges.

The prune function returns a subset of rows from the input.

The split function applies an agglomerative community detection algorithm
to the network topology and a list with the element "modules" that contains
vectors of module members and "subnets" that contains the subnetworks for
each module and a separate entry for any remaining edges. If requested,
an additional element "matrices" contains the subnetworks in matrix format.

The merge function combines data columns within modules into a single
principal component score. If PC1 coefficients are not available (e.g.
output from the split function), they are calculated from the data.
The output includes the attribute "modules" that contains the final
module definitions and PC1 coefficients.
}

\author{
Ville-Petteri Makinen
}

\examples{
# Random data matrix.
x <- matrix(rnorm(100000), ncol=100)

# Create correlation modules.
x[,12:20] <- (x[,12:20] + x[,11])
x[,32:40] <- (x[,32:40] + x[,31])
x[,62:90] <- (x[,62:90] + x[,61])
x[,50] <- (x[,20] + x[,90]) # connecting node

# Set column names.
cnames <- paste0("X", 1:ncol(x))
cnames[11:20] <- paste0("M1.", cnames[11:20])
cnames[31:40] <- paste0("M2.", cnames[31:40])
cnames[61:90] <- paste0("M3.", cnames[61:90])
colnames(x) <- cnames

# Merge collinear modules.
y <- nroCoalesce(x)

# Show merged columns.
modules <- attr(y,"modules")
mnames <- names(modules)
print(summary(y[,mnames]))

# Show module members.
lapply(mnames, function(k, x) {
    cat("\n", k, "\n", sep="")
    print(x[[k]]$weights)
}, x=modules)
}
