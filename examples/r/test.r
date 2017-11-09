dyn.load(paste("structural", .Platform$dynlib.ext, sep=""))
source("structural.R")
cacheMetaData(1)

ConvertMatrix <- function(matrixIn, rowLabels, colLabels) 
{
	numRows <- DoubleMatrix_numRows(matrixIn)
	numCols <- DoubleMatrix_numCols(matrixIn)
	
	out <- matrix(nrow=numRows, ncol=numCols)
	for (i in 1:numRows)
	{
		for (j in 1:numCols)
		{
			out[i,j] <- DoubleMatrix_get(matrixIn, i-1,j-1 )
		}
	}
	
	rowNames1 <- vector("character", numRows)
	colNames1 <- vector("character", numCols)
	
	for (i in 1:numRows)
	{
		rowNames1[i] <- rowLabels[i-1]
	}
	
	for (j in 1:numCols)
	{
		colNames1[j] <- colLabels[j-1]
	}
	
	colnames(out) <- colNames1;
	rownames(out) <- rowNames1;
	
	return(out)
	
}

LoadSBMLAndAnalyze <- function(struct, fileName)
{
	
	print(strsplit(LibStructural_loadSBMLFromFile(struct,fileName), "\n"))
	print(strsplit(LibStructural_getTestDetails(struct), "\n"))

	PrintFullyReorderedStoichiometryMatrix(struct)
}

PrintFullyReorderedStoichiometryMatrix <- function(struct)
{
	
	x <- LibStructural_getFullyReorderedStoichiometryMatrix(struct)
	rowLabels <- StringVector()
	colLabels <- StringVector()	
	LibStructural_getFullyReorderedStoichiometryMatrixLabels(struct, rowLabels, colLabels)

	result <- ConvertMatrix(x, rowLabels, colLabels)
	print("Fully Reordered Stoichiometry Matrix")
	print(result)

}

LoadAndAnalyzeMatrix <- function(struct, stoich, speciesNames, speciesConcentrations, reactionNames)
{	
	dMatrix <- CreateDoubleMatrix(stoich)
	vSpeciesNames <- CreateStringVector(speciesNames)
	vSpeciesConcentrations <- CreateDoubleVector(speciesConcentrations)
	vReactionNames <- CreateStringVector(reactionNames)
		
	LibStructural_loadStoichiometryMatrix(struct, dMatrix)
#	LibStructural_loadSpecies(struct,vSpeciesNames, vSpeciesConcentrations)
#	LibStructural_loadReactionNames(struct,vReactionNames)
	
	print(strsplit(LibStructural_analyzeWithQR(struct), "\n"))
	print(strsplit(LibStructural_getTestDetails(struct), "\n"))	
	
	PrintFullyReorderedStoichiometryMatrix(struct)
	
}

CreateDoubleVector <- function(vector)
{
	
	vLength <- length(vector)
	result <- DoubleVector(vLength)
	for (i in (1:vLength))
	{
		result[i-1] <- vector[i]
	}
	return(result)
}

CreateStringVector <- function(vector)
{	
	vLength <- length(vector)
	result <- StringVector(vLength)
	
	for (i in (1:vLength))
	{
		result[i-1] <- vector[i]
	}
	return(result)
}

CreateDoubleMatrix <- function(stoich)
{
	numRows <- dim(as.matrix(stoich))[1]
	numCols <- dim(as.matrix(stoich))[2]
	
	dMatrix <- DoubleMatrix(numRows, numCols)
	for (i in (1:numRows))
	{
		for (j in (1:numCols))
		{
			DoubleMatrix_set(dMatrix,i-1,j-1, stoich[i,j])
		}
	}
	return(dMatrix)
}

struct <- LibStructural()
LoadSBMLAndAnalyze(struct, "/Applications/SBW-2.7.8/SBML Models/BorisEJB.xml")

speciesNames <- c( "S2", "S3", "S4" )
speciesConcentrations <- c( 1.0, 0.0, 1.0 )
reactionNames <- c( "J1", "J2", "J3", "J4" )
stoich <- data.frame( 
			c(1.0,              -1.0,               0.0,               0.0), 
			c(0.0,               1.0,              -1.0,               0.0), 
			c(0.0,               0.0,               1.0,              -1.0)) 

LoadAndAnalyzeMatrix(struct, stoich, speciesNames, speciesConcentrations, reactionNames)