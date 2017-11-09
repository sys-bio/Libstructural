#!/usr/bin/env python

# This is a python test program for the libstructural SWIG Python bindings.
# 
# In this test we run through the following use cases: 
#
#	- loading an SBML file and analyzing it
#	- loading a raw Stoichiometry Matrix and analyzing it
#

import sys
from structural import *
struct = LibStructural()

# Utility function printing a labeled 
def printLabeledMatrix(matrix,rowLables,colLabels):
	
	numCols = matrix.numCols();
	numRows = matrix.numRows();
	
	print '%-12s' % '  ','\t',
	for i in range(0,numCols):
		print colLabels[i], '\t',
	print ''
	for i in range(0, numRows):
		print '%-12s' % rowLables[i], '\t',
		for j in range(0, numCols):
			print matrix.get(i,j), '\t',
		print ''
	
# Utility function printing the fully reordered stoichiometry matrix
def printFullyReorderedStoichiometryMatrix(struct):
	
	print
	print 'Fully Reordered Stoichiometry Matrix'
	species = StringVector()
	reactions = StringVector()
	matrix = struct.getFullyReorderedStoichiometryMatrix()
	struct.getFullyReorderedStoichiometryMatrixLabels(species, reactions)
	
	printLabeledMatrix(matrix, species, reactions)
	
	print
	
	
# Utility function loading a SBML model from a file and testing it	
def loadSBMLFromFileAndAnalyze(fileName):
	print struct.loadSBMLFromFile(fileName)
	print struct.getTestDetails()
	printFullyReorderedStoichiometryMatrix(struct)
	
# Utility function creating a SWIG generated DoubleMatrix out of a python matrix
def CreateDoubleMatrixFromPythonMatrix(matrix): 
	numRows = len(matrix)
	numCols = len(matrix[0])
	dMatrix = DoubleMatrix(numRows, numCols)
	
	for i in range(numRows):
		for j in range(numCols):
			dMatrix.set(i,j,matrix[i][j])
	return dMatrix

# Utility function creating a SWIG DoubleVector out of a python vector
def CreateDoubleVectorFromPythonArray(vector): 	
	result = DoubleVector(len(vector))
	for i in range(len(vector)):
		result[i] = vector[i]
	return result

# Utility function creating a SWIG StringVector out of a python vector
def CreateStringVectorFromPythonArray(vector): 	
	result = StringVector(len(vector))
	for i in range(len(vector)):
		result[i] = vector[i]
	return result
	
# Utility function loading a stoichiometry matrix and labels and then analyzing it	
def loadStoichiometryMatrixandAnalyze(matrix, speciesNames, speciesConcentrations, reactionNames):
	
	dMatrix = CreateDoubleMatrixFromPythonMatrix(matrix)
	vSpeciesNames = CreateStringVectorFromPythonArray(speciesNames)
	vSpeciesConcentrations = CreateDoubleVectorFromPythonArray(speciesConcentrations)
	vReactionNames = CreateStringVectorFromPythonArray(reactionNames)
	
	struct.loadStoichiometryMatrix(dMatrix)
	struct.loadSpecies(vSpeciesNames, vSpeciesConcentrations)
	struct.loadReactionNames(vReactionNames)
	
	print struct.analyzeWithQR()
	print struct.getTestDetails()
	
	printFullyReorderedStoichiometryMatrix(struct)

# now after all those definitions lets run through them 

# test case 1: loading and analyzing a SBML model
loadSBMLFromFileAndAnalyze('/Applications/SBW-2.7.8/SBML Models/BorisEJB.xml')


# constructing a Stoichiometry matrix
matrix = [None]*3;
matrix[0] = [1.0,              -1.0,               0.0,               0.0]
matrix[1] = [0.0,               1.0,              -1.0,               0.0]
matrix[2] = [0.0,               0.0,               1.0,              -1.0]

speciesNames = [ "S2", "S3", "S4" ];
speciesConcentrations = [ 1.0, 0.0, 1.0 ];
reactionNames = [ "J1", "J2", "J3", "J4" ];

# test case 2: loading the stoichiometry matrix
loadStoichiometryMatrixandAnalyze(matrix, speciesNames, speciesConcentrations, reactionNames)