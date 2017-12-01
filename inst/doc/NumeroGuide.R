## ----setUpLibrary, eval=TRUE, include=FALSE------------------------------
library(Numero)

## ----legumes, eval=TRUE, echo=FALSE, out.width='100%', fig.cap="A conceptual example of data-driven subgrouping. The example shows how to organize objects with multiple features into a two-dimensional layout. The images were obtained from @cardoso2014taxonomic."----
knitr::include_graphics("legumes.jpg")

## ----splitFig, eval=TRUE, echo=FALSE, out.width='100%', fig.cap="Application of the split-by-variable study design in the diabetic kidney disease example. Of note, the training set is adjusted for sex differences; hence the ‘MALE’ column is not included in the evaluation set."----
knitr::include_graphics("split.jpg")

## ----pipeline, eval=TRUE, echo=FALSE, out.width= '100%', fig.cap="Analysis steps in the diabetic kidney disease example."----
knitr::include_graphics("pipeline.svg")

## ----setUpExample, eval=TRUE---------------------------------------------
fname <- system.file("extdata", "finndiane_dataset.txt", package = "Numero")

## ----loadExample, eval=TRUE----------------------------------------------
db <- nroMatrix(file = fname, keyvars = "INDEX")

## ----showData, eval=TRUE-------------------------------------------------
summary(db)

## ----preproc1, eval=TRUE-------------------------------------------------
trdata <- db[,c("uALB_log", "TG_log", "CHOL", "HDL2C", "CREAT_log")]

## ----preproc2, eval=TRUE-------------------------------------------------
men <- which(db[, "MALE"] == 1)
women <- which(db[, "MALE"] == 0)

## ----preproc3, eval=TRUE-------------------------------------------------
trdata[men,] <- scale.default(trdata[men,])
trdata[women,] <- scale.default(trdata[women,])

## ----preproc4, eval=TRUE-------------------------------------------------
print(summary(trdata[men,]))
print(summary(trdata[women,]))

## ----init1, eval=TRUE----------------------------------------------------
km <- nroKmeans(x = trdata, k = 3)

## ----init2, eval=TRUE----------------------------------------------------
print(km$centroids)

## ----init3, eval=TRUE----------------------------------------------------
sm <- nroKoho(seeds = km$centroids, radius = 3)

## ----init4, eval=TRUE----------------------------------------------------
print(head(sm$centroids)) 

## ----train1, eval=TRUE---------------------------------------------------
sm <- nroTrain(som = sm, x = trdata)

## ----train2, out.width='100%', fig.cap = "SOM training history in the diabetic kidney disease example.", eval=TRUE----
plot(sm$history)

## ----match1, eval=TRUE---------------------------------------------------
matches <- nroMatch(som = sm, x = trdata)

## ----match2, eval=TRUE---------------------------------------------------
print(head(matches))

## ----match3, eval=TRUE---------------------------------------------------
t <- table(matches$POS)
counts <- data.frame(DISTRICT = names(t), N = as.integer(t))
print(counts, row.names = FALSE)

## ----permute1, eval=TRUE-------------------------------------------------
stats <- matrix(NA, ncol(db), 5)
rownames(stats) <- colnames(db)
for( vname in colnames(db) ) {
    
    # Check if a training variable.
    nsim <- NA
    pos <- match(vname, colnames(trdata))
    if(is.na(pos)) nsim <- 10000
 
    # Estimate the dynamic range of regional variation.
    tmp <- nroPermute(map = sm, x =db[, vname], bmus = matches$POS, n = nsim)
    colnames(stats) <- colnames(tmp)
    stats[vname,] <- as.matrix(tmp)
}

## ----permute2, eval=TRUE-------------------------------------------------
print(stats)

## ----coloramp1, eval=TRUE------------------------------------------------
trmask <- which(is.na(stats[, "P.z"]))
evmask <- which(stats[, "P.z"] >= 0.0)

## ----coloramp2, eval=TRUE------------------------------------------------
zbase <- 0.5*(mean(stats[trmask, "Z"], na.rm = TRUE) +
              max(stats[evmask, "Z"], na.rm = TRUE))
zbase <- max(zbase, 2.0)

## ----coloramp3, eval=TRUE------------------------------------------------
amplitudes <- stats[, "Z"] / zbase
amplitudes <- pmax(amplitudes, 0.02)

## ----color1, eval=TRUE---------------------------------------------------
vname <- "DIAB_KIDNEY"

## ----color2, eval=TRUE---------------------------------------------------
plane <- nroAggregate(map = sm, x = db[, vname], bmus = matches$POS)

## ----color3, eval=TRUE---------------------------------------------------
colrs <- nroColorize(values = plane, amplitude = amplitudes[vname])

## ----color4, eval=TRUE---------------------------------------------------
labls <- nroLabel(map = sm, values = plane)

## ----color5, eval=TRUE---------------------------------------------------
print(data.frame(VALUE = plane, COLOR = colrs, LABEL = labls)[1:10,])

## ----gout1, eval=TRUE----------------------------------------------------
pval <- stats[vname, "P.z"]
ttxt <- sprintf("%s, P = %.2e", vname, pval)

## ----gout2, eval=TRUE----------------------------------------------------
smfig <- nroCircus(map = sm, colors = colrs, labels = labls, title = ttxt)

## ----gout3, eval=TRUE----------------------------------------------------
fpath <- paste(vname, ".svg", sep = "")
nbytes <- nroFigure(file = fpath, scene = smfig)

## ----gout4, eval=TRUE----------------------------------------------------
for( vname in colnames(db) ) {

     # Estimate district values.
     plane <- nroAggregate(map = sm, x = db[, vname], bmus = matches$POS)
     
     # Determine district colors.
     colrs <- nroColorize(values = plane, amplitude = amplitudes[vname])

     # Determine which district labels should be shown.
     labls <- nroLabel(map = sm, values = plane)
     
     # Create a vector graphics object.
     pval <- stats[vname, "P.z"]
     ttxt <- vname # only the name if training variable
     if(is.na(pval) == FALSE) { # add p-value for evaluation variables
         ttxt <- sprintf("%s, P = %.2e", vname, pval);
     }
     smfig <- nroCircus(map = sm, colors = colrs, labels = labls, title = ttxt)
     
     # Save figure.
     fpath <- paste(vname, ".svg", sep = "")
     nroFigure(file = fpath, scene = smfig)
}

## ----quality1, eval=TRUE-------------------------------------------------
plane <- nroAggregate(map = sm, bmus = matches$POS)

## ----quality2, eval=TRUE-------------------------------------------------
colrs <- nroColorize(values = plane, palette = "fire")
labls <- nroLabel(map = sm, values = plane)
smfig <- nroCircus(map = sm, colors = colrs, labels = labls,
                   title = "Data point histogram")
nbytes <- nroFigure(file = "histogram.svg", scene = smfig)

## ----quality3, eval=TRUE-------------------------------------------------
nroAggregate(map = sm, bmus = matches$POS, x = matches$COVER)
colrs <- nroColorize(values = plane, palette = "fire")
labls <- nroLabel(map = sm, values = plane)
smfig <- nroCircus(map = sm, colors = colrs, labels = labls,
                   title = "Data coverage")
nbytes <- nroFigure(file = "coverage.svg", scene = smfig)

## ----quality4, eval=TRUE-------------------------------------------------
plane <- nroAggregate(map = sm, bmus =  matches$POS, x = matches$QUALITY)
colrs <- nroColorize(values = plane, palette = "fire")
labls <- nroLabel(map = sm, values = plane)
smfig <- nroCircus(map = sm, colors = colrs, labels = labls,
                   title = "Matching quality")
nbytes <- nroFigure(file = "quality.svg", scene = smfig)

## ----quality5, eval=TRUE-------------------------------------------------
sorted <- order(matches$QUALITY)
print(matches[sorted[1:20],])

## ----mapQuality, eval=TRUE, echo=FALSE, out.width= '100%', fig.cap="Map quality metrics in the diabetic kidney disease example."----
knitr::include_graphics("qualityOverview.svg")

## ----biochemistry, eval=TRUE, echo=FALSE, out.width= '100%', fig.cap="Map colorings for the training set. The colorings for urinary albumin, serum creatinine and triglycerides in Plots A,B and F were created based on the logarithmic values, whereas the numbers show absolute concentrations after reversing the logarithmic transformation."----
knitr::include_graphics("biochemistry.svg")

## ----subgroups, eval=TRUE, echo=FALSE, out.width= '100%', fig.cap="Non-overlapping metabolic subgroups. The numbers in Plot E are the unique district identifiers."----
knitr::include_graphics("subgroups.svg")

## ----endpoints, eval=TRUE, echo=FALSE, out.width= '100%', fig.cap="Overlay of metabolic subgroup boundaries on selected map colorings of clinical end-points."----
knitr::include_graphics("endpoints.svg")

## ----sg1, eval=TRUE------------------------------------------------------
subgrp <- c(2,8,10,11,12,36,37,38,39)

## ----sg2, eval=TRUE------------------------------------------------------
rows <- which(match(matches$POS, subgrp) > 0)

## ----sg3, eval=TRUE------------------------------------------------------
print(summary(db[rows,]))

## ----sg4, eval=TRUE------------------------------------------------------
subgroups <- list()
subgroups[["HighCreat"]] <- c(2,8,10:12,36:39)
subgroups[["HighChol"]] <- c(3,13,14,22,40)
subgroups[["HighHDL2"]] <- c(15,16,23:28)
subgroups[["HighTGRatio"]] <- c(9,21,33:35)

## ----sg5, eval=TRUE------------------------------------------------------
subgrp <- c(1,4:7,17:20,29:32)
controls <- which(match(matches$POS, subgrp) > 0)

## ----sg6, eval=TRUE------------------------------------------------------
for(name in names(subgroups)) {
    subgrp <- subgroups[[name]]
    rows <- which(match(matches$POS, subgrp) > 0)

    # T-test for continous traits.
    for(vname in c("AGE", "T1D_DURAT")) {
        x <- na.omit(db[controls, vname])
        y <- na.omit(db[rows, vname])
        stats <- t.test(y, x)
        
        # only print some results
        if(name == "HighCreat" & vname == "T1D_DURAT"){
          cat("\n", name, ", ", vname, ":\n", sep = "")
          cat("Difference:", (mean(y) - mean(x)), "\n")
          cat("CI:", as.double(stats$conf.int), "\n")
          cat("P-value:", stats$p.value, "\n")
        }
    }
    
    # Fisher's test for binary traits.
    for(vname in c("DECEASED", "DIAB_KIDNEY", "METAB_SYNDR")) {
        x <- na.omit(db[controls, vname])
        y <- na.omit(db[rows, vname])
        bits <- c(0*x, 0*y + 1)
        stats <- fisher.test(c(x, y), bits)
        
        # only print some results
        if(name == "HighCreat"){
          cat("\n", name, ", ", vname, ":\n", sep="")
          cat("Odds ratio:", stats$estimate, "\n")
          cat("CI:", as.double(stats$conf.int), "\n")
          cat("P-value:", stats$p.value, "\n")
        }
    }
}

## ----sg7, eval=TRUE, echo=FALSE, warning=FALSE---------------------------
#create table for binary traits
df <- data.frame()
for(name in names(subgroups)) {
   subgrp <- subgroups[[name]]
   rows <- which(match(matches$POS, subgrp) > 0)
   for(vname in c("DECEASED", "DIAB_KIDNEY", "METAB_SYNDR")) {
        x <- na.omit(db[controls, vname])
        y <- na.omit(db[rows, vname])
        bits <- c(0*x, 0*y + 1)
        stats <- fisher.test(c(x, y), bits)
        
        #format number and strings
        conf <- round(as.double(stats$conf.int), digits = 1)
        conf <- paste(paste0(conf[1],','),conf[2])
        odds <- round(stats$estimate, digits = 1)
        odds <-paste0(paste0(paste(odds,'('),conf),')')
        p <- ifelse(stats$p.value < 0.001, '<0.001',as.character(round(stats$p.value, digits = 4)))
        df1 <- data.frame(vname,name,odds,p)
        df <- as.data.frame(rbind(df,df1))
   }
}
names(df) <- c('Variable', 'Subgroup', 'Odds ratio', 'P-value')

#choose variables that appear in the text
o1 <- df[[which(df$Variable == 'DECEASED' & df$Subgroup == 'HighCreat'),'Odds ratio']]
p1 <- df[[which(df$Variable == 'DECEASED' & df$Subgroup == 'HighCreat'),'P-value']]
o2 <- df[[which(df$Variable == 'DIAB_KIDNEY' & df$Subgroup == 'HighCreat'),'Odds ratio']]
p2 <- df[[which(df$Variable == 'DIAB_KIDNEY' & df$Subgroup == 'HighCreat'),'P-value']]
o3 <- df[[which(df$Variable == 'METAB_SYNDR' & df$Subgroup == 'HighTGRatio'),'Odds ratio']]
p3 <- df[[which(df$Variable == 'METAB_SYNDR' & df$Subgroup == 'HighTGRatio'),'P-value']]

#rename for better readability
df$Variable <- gsub(pattern = "DECEASED", replacement = "Deceased at follow-up", x = df$Variable, fixed = TRUE)
df$Variable <- gsub(pattern = "DIAB_KIDNEY", replacement = "Diabetic kidney disease", x = df$Variable, fixed = TRUE)
df$Variable <- gsub(pattern = "METAB_SYNDR", replacement = "Metabolic syndrome", x = df$Variable, fixed = TRUE)
df$Subgroup <- gsub(pattern = "HighCreat", replacement = "High Creatinine", x = df$Subgroup, fixed = TRUE)
df$Subgroup <- gsub(pattern = "HighChol", replacement = "High Cholesterol", x = df$Subgroup, fixed = TRUE)
df$Subgroup <- gsub(pattern = "HighHDL2", replacement = "High HDL2 Cholesterol", x = df$Subgroup, fixed = TRUE)
df$Subgroup <- gsub(pattern = "HighTGRatio", replacement = "High Triglyceride Ratio", x = df$Subgroup, fixed = TRUE)

## ----sg8, eval=TRUE, echo=FALSE------------------------------------------
knitr::kable(
 df[order(df$Variable),], row.names = FALSE, caption = 'Comparison of metabolic subgroups in individuals with type 1 diabetes. Odds ratios with the Low Lipids Subgroup are reported. In addition, 95% confidence intervals are reported for the descriptive statistics. P-values were estimated by Fisher’s test.'
)

## ----rplot, eval=FALSE---------------------------------------------------
#  # Install and activate libraries.
#  require(rsvg)
#  require(grid)
#  
#  # List all SVG files in the specified directory. Adapt path if required.
#  fnames <- list.files(path = "/tmp/",
#                       full.names = TRUE,
#                       pattern = "*.svg")
#  
#  # Four colorings per one figure.
#  pages <- seq.int(from = 1, to = length(fnames), by = 4)
#  
#  # Close all current graphics devices to clear the screen.
#  graphics.off()
#  
#  # Show all figures on screen.
#  for(p in pages) {
#      last <- min(length(fnames), (p + 3))
#      fbatch <- fnames[p:last]
#  
#      # Set up a new figure.
#      dev.new()
#      density <- 500
#  
#      # Import color data.
#      colors <- list()
#      for(i in 1:length(fbatch)) {
#          bmap <- rsvg(fbatch[i], width = density, height = density)
#          cmap <- rgb(bmap[,,1], bmap[,,2], bmap[,,3])
#          cmap <- matrix(cmap, nrow = density, ncol = density)
#          colors[[i]] <- cmap
#      }
#  
#      # Add empty space if missing files.
#      while(length(colors) < 4) {
#          whites <- matrix("#FFFFFF", nrow = density, ncol = density)
#          colors[[length(colors)+1]] <- whites
#      }
#  
#      # Reshape into a 2 x 2 grid of colorings.
#      colors <- rbind(cbind(colors[[1]], colors[[2]]),
#                      cbind(colors[[3]], colors[[4]]))
#  
#      # Show image in the figure.
#      grid.raster(colors)
#  }

