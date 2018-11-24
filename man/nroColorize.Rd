\name{nroColorize}
\alias{nroColorize}
\title{
Assign colors based on value
}
\description{
Assign colors to map districts based on the respective district values.
}
\usage{
nroColorize(values, amplitudes = 1, palette = "rhodo" )
}
\arguments{
  \item{values}{
A vector of K values or a K x N matrix, where K is the number of map
districts and N is the number of variables.
  }
  \item{amplitudes}{
Single value or a vector of N elements to specify available proportion of
color range for each variable.
  }
  \item{palette}{
One of pre-defined colormap names (see details) or a sorted vector of
hexadecimal color codes as strings, see rgb() for additional details. 
  }
}
\details{
The argument 'amplitudes' controls the part of the color range that is
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
A data frame with K rows and N columns that contains color definitions
as character strings.
}
\author{
Song Gao
Stefan Mutter
Ville-Petteri Makinen
}
