\name{nroColorize}
\alias{nroColorize}
\title{
Map Visualization
}
\description{
Assign colors to map districts based on the respective district values.
}
\usage{
nroColorize(values, amplitude = 1, palette = "rhodo" )
}
\arguments{
  \item{values}{
A numerical vector of unit values.
  }
  \item{amplitude}{
Available proportion of color range.
  }
  \item{palette}{
A sorted string vector of colors, see rgb() for additional details.
The input can also be the name of the colormap, see below.    
  }
}
\details{
The argument 'amplitude' controls the part of the color range that is
available for the district value range. For proportions below 1, the
minimum district value is assigned to a color that is between the first
and middle element in the color palette, and the maximum is assigned
to a color that is between the middle and the last element. If
'amplitude' is greater than 1, the extreme low and high values are
clipped to the first and last color in the palette, respectively.

Palette can also contain the name of a colormap: 'gray', 'fire',
'jungle', 'miami', 'rhodo' or 'tan'. Any other word will revert
to a rainbow colormap.
}
\value{
An character vector of color definitions.
}
\author{
Song Gao

Stefan Mutter

Ville-Petteri Makinen
}
\examples{
# Set random unit values.
uv <- sample.int(101, size = 100, replace = TRUE)

# Find hexadecimal color codings.
colorDef <- nroColorize(uv)
}

