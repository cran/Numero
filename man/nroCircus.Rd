\name{nroCircus}
\alias{nroCircus}
\title{
Map Visualization
}
\description{
Create Scalable Vector Graphics code for a map coloring.
}
\usage{
nroCircus(map, colors, labels = character(), title = "")
}
\arguments{
  \item{map}{
A list with the element 'topology', see \code{\link{nroKoho}} for details.
  }
  \item{colors}{
A character vector of N color codes, see \code{\link[grDevices]{rgb}} for details.
  }
  \item{labels}{
A character vector of H unit labels
  }
  \item{title}{
Text for the plot title
  }
}
\details{
Some non-alphanumeric characters are not supported and will be
automatically converted to '_'. Too long labels or title will be
truncated.
}
\value{
A list containing three values: svg, bbox and tight. 'svg' is a
character vector that contains the SVG code. 'bbox' (a vector of
four elements x1, y1, x2, y2) indicates the position of the top
left corner (first two elements), and the bottom right corner
(last two elements) of the bounding box, when using the default
SVG coordinate system with left-to-right horizontal and up-to-down
vertical axes. 'tight' is the bounding box of the map without labels.
}
\author{
Song Gao

Stefan Mutter

Ville-Petteri Makinen
}
\seealso{
\code{\link{nroKoho}} for details on the SOM and \code{\link[grDevices]{rgb}} for details colors
}
\examples{
# Set map unit values for a map of 66 units.
uv <- sample.int(101, size = 66, replace = TRUE)

# Create the SOM topology for a map of radius 4 with 66 units.
sm <- nroKoho(matrix(1:16, nrow = 4, ncol = 4),
  radius = 4)

# Find hexadecimal color codings.
colorDef <- nroColorize(uv)

# Define Labels.
myLabels <- nroLabel(map = sm, values = uv)

# Create the SVG output.
svg <- nroCircus(map = sm, colors = colorDef, 
  labels = myLabels, title = "Example")
}

