%feature("docstring") LIB_STRUCTURAL::LibStructural::loadStoichiometryMatrix "
LibStructural.loadStoichiometryMatrix(self, data)
 Load a new stoichiometry matrix. 

Loads the stoichiometry matrix into the library. To analyze the stoichiometry 
call one of the following: 

\li ::LibStructural_analyzeWithQR, 
\li ::LibStructural_analyzeWithLU, 
\li ::LibStructural_analyzeWithLUandRunTests, 
\li ::LibStructural_analyzeWithFullyPivotedLU or
\li ::LibStructural_analyzeWithFullyPivotedLUwithTests

Remarks: if matrix labels are needed it is recommended to call LIB_STRUCTURAL::LibStructural::loadSpecies 
and LIB_STRUCTURAL::LibStructural::loadReactionNames after a call to this method.

:param: oMatrix the stoichiometry matrix to load

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::loadSBMLFromString "
LibStructural.loadSBMLFromString(self, *args)
 Load a SBML model.
:param: sSBML the SBML string to load
:returns: information about the loaded model
";

%feature("docstring") LIB_STRUCTURAL::LibStructural::loadSBMLFromFile "
LibStructural.loadSBMLFromFile(self, *args)

 Load a SBML model from the specified file. 
:param: sFileName a file name to a SBML file to load
:returns: information about the loaded model

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::loadSBMLwithTests "
LibStructural.loadSBMLwithTests(self, *args)

 Load an SBML model into the library and carry out tests using the internal test suite.  
:param: sSBML the SBML file to load
:returns: information about the loaded model and results of the internal test suite

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::analyzeWithQR "
LibStructural.analyzeWithQR(self)

 Uses QR factorization for structural analysis

This method performs the actual analysis of the stoichiometry matrix (loaded either
via LIB_STRUCTURAL::LibStructural::loadStoichiometryMatrix or LIB_STRUCTURAL::LibStructural::loadSBMLFromString. Only after 
one of the analysis methods below has been called are the structural matrices (L0, K0...)
available. 

\li LIB_STRUCTURAL::LibStructural::analyzeWithQR, 
\li LIB_STRUCTURAL::LibStructural::analyzeWithLU, 
\li LIB_STRUCTURAL::LibStructural::analyzeWithLUandRunTests, 
\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLU or
\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLUwithTests


Remarks: This is the prefered method for structural analysis.

:returns: a result string with information about the analysis process

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::analyzeWithLU "
LibStructural.analyzeWithLU(self)

 Uses LU Decomposition for Conservation analysis

This method performs the actual analysis of the stoichiometry matrix (loaded either
via LIB_STRUCTURAL::LibStructural::loadStoichiometryMatrix or LIB_STRUCTURAL::LibStructural::loadSBMLFromString. Only after 
one of the analysis methods below has been called are the structural matrices (L0, K0...)
available. 

\li LIB_STRUCTURAL::LibStructural::analyzeWithQR, 
\li LIB_STRUCTURAL::LibStructural::analyzeWithLU, 
\li LIB_STRUCTURAL::LibStructural::analyzeWithLUandRunTests, 
\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLU or
\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLUwithTests

:returns: a result string with information about the analysis process

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::analyzeWithLUandRunTests "
LibStructural.analyzeWithLUandRunTests(self)

 Uses LU Decomposition for Conservation analysis

This method performs the actual analysis of the stoichiometry matrix (loaded either
via LIB_STRUCTURAL::LibStructural::loadStoichiometryMatrix or LIB_STRUCTURAL::LibStructural::loadSBMLFromString. Only after 
one of the analysis methods below has been called are the structural matrices (L0, K0...)
available. 

\li LIB_STRUCTURAL::LibStructural::analyzeWithQR, 
\li LIB_STRUCTURAL::LibStructural::analyzeWithLU, 
\li LIB_STRUCTURAL::LibStructural::analyzeWithLUandRunTests, 
\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLU or
\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLUwithTests

This method additionally performs the integrated test suite and returns	those results.


:returns: a result string with information about the analysis process

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLU "
LibStructural.analyzeWithFullyPivotedLU(self)

 Uses fully pivoted LU Decomposition for Conservation analysis

This method performs the actual analysis of the stoichiometry matrix (loaded either
via LIB_STRUCTURAL::LibStructural::loadStoichiometryMatrix or LIB_STRUCTURAL::LibStructural::loadSBMLFromString. Only after 
one of the analysis methods below has been called are the structural matrices (L0, K0...)
available. 

\li LIB_STRUCTURAL::LibStructural::analyzeWithQR, 
\li LIB_STRUCTURAL::LibStructural::analyzeWithLU, 
\li LIB_STRUCTURAL::LibStructural::analyzeWithLUandRunTests, 
\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLU or
\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLUwithTests


:returns: a result string with information about the analysis process

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLUwithTests "
LibStructural.analyzeWithFullyPivotedLUwithTests(self)

 Uses fully pivoted LU Decomposition for Conservation analysis

This method performs the actual analysis of the stoichiometry matrix (loaded either
via LIB_STRUCTURAL::LibStructural::loadStoichiometryMatrix or LIB_STRUCTURAL::LibStructural::loadSBMLFromString. Only after 
one of the analysis methods below has been called are the structural matrices (L0, K0...)
available. 

\li LIB_STRUCTURAL::LibStructural::analyzeWithQR, 
\li LIB_STRUCTURAL::LibStructural::analyzeWithLU, 
\li LIB_STRUCTURAL::LibStructural::analyzeWithLUandRunTests, 
\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLU or
\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLUwithTests

This method additionally performs the integrated test suite and returns	those results.

:returns: a result string with information about the analysis process

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getReorderedSpecies "
LibStructural.getReorderedSpecies(self)

Returns the reordered list of molecular species. (choosing the SBML Id if possible )

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getSpecies "
LibStructural.getSpecies(self)

Returns the unordered list of species Ids

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getReorderedSpeciesNamesList "
LibStructural.getReorderedSpeciesNamesList(self)

Returns the reordered list of molecular species.  (choosing the SBML Name if possible )

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getIndependentSpecies "
LibStructural.getIndependentSpecies(self)

Returns the list of independent species 

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getIndependentSpeciesNamesList "
LibStructural.getIndependentSpeciesNamesList(self)

Returns the actual names of the independent species 

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getDependentSpecies "
LibStructural.getDependentSpecies(self)

Returns the list of dependent species 

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getDependentSpeciesNamesList "
LibStructural.getDependentSpeciesNamesList(self)

Returns the actual names of the dependent species 

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getReactions "
LibStructural.getReactions(self)

Returns the list of Reactions 

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getIndependentReactionIds "
LibStructural.getIndependentReactionIds(self)

Returns the list of independent reactions 

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getDependentReactionIds "
LibStructural.getDependentReactionIds(self)

Returns the list of dependent reactions 


";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getReactionsNamesList "
LibStructural.getReactionsNamesList(self)

Returns actual names of the Reactions 

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getReorderedReactions "
LibStructural.getReorderedReactions(self)

Returns the reordered list of reactions

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::ConservedLaws "
LibStructural.ConservedLaws(self):



";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getConservedSums "
LibStructural.getConservedSums(self)

Returns values for conservation laws using the current initial conditions

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getInitialConditions "
LibStructural.getInitialConditions(self)

Returns Initial Conditions used in the model

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::validateStructuralMatrices "
LibStructural.validateStructuralMatrices(self)

 Validates structural matrices.

Calling this method will run the internal test suite against the structural 
matrices those tests include:\n

\li Test 1 : Gamma*N = 0 (Zero matrix)
\li Test 2 : Rank(N) using SVD (5) is same as m0 (5)
\li Test 3 : Rank(NR) using SVD (5) is same as m0 (5)
\li Test 4 : Rank(NR) using QR (5) is same as m0 (5)
\li Test 5 : L0 obtained with QR matches Q21*inv(Q11)
\li Test 6 : N*K = 0 (Zero matrix)
";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getTestDetails "
LibStructural.getTestDetails(self)

Return Return Details about validation tests.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getModelName "
LibStructural.getModelName(self)

Returns the name of the model. 

Returns the name of the model if SBML model has Name-tag, otherwise it returns the 
SBML id. If only a stoichiometry matrix was loaded 'untitled' will be returned.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNumSpecies "
LibStructural.getNumSpecies(self)

Returns the total number of species

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNumIndSpecies "
LibStructural.getNumIndSpecies(self)

Returns the number of independent species

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNumDepSpecies "
LibStructural.getNumDepSpecies(self)

Returns the number of dependent species

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNumReactions "
LibStructural.getNumReactions(self)

Returns the total number of reactions

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNumIndReactions "
LibStructural.getNumIndReactions(self)

Returns the number of independent reactions

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNumDepReactions "
LibStructural.getNumDepReactions(self)

Returns the number of dependent reactions

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNmatrixSparsity "
LibStructural.getNmatrixSparsity(self)

Returns the number of nonzero values in Stoichiometry matrix

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getRank "
LibStructural.getRank(self);

 Returns the rank of the matrix argument

:param: oMatrix Matrix to find the rank of. 

\result Returns the rank as an integer

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getRCond "
LibStructural.getRCond(self, *args)


";

%feature("docstring") LIB_STRUCTURAL::LibStructural::setTolerance "
LibStructural.setTolerance(self, *args)

 Set user specified tolerance

This function sets the tolerance used by the library to determine what value 
is considered as zero. Any value with absolute value smaller than this tolerance is considered as zero 
and will be neglected. 

:param: dTolerance Sets the tolerance used by the library to determine a  value close to zero


";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getTolerance "
LibStructural.getTolerance(self)

 Returns the currently used tolerance

This function returns the tolerance currently used by the library to determine what value 
is considered as zero. Any value with absolute value smaller than this tolerance is considered zero 
and will be neglected. 

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getInstance "
LibStructural.getInstance()

static method to get an instance of LibStructural (allows use as singleton)

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getColumnReorderedNrMatrixLabels "
LibStructural.getColumnReorderedNrMatrixLabels(self)

brief Returns the Nr Matrix row and column labels (repartitioned into NIC and NDC).

:param: outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
:param: outRowCount after the call this variable will hold the number of row labels 
returned.
:param: outColLabels a pointer to a string array where the column labels will be allocated
and written.
:param: outColCount after the call this variable will hold the number of column labels
returned.

:returns: The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

Remarks: To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::loadReactionNames "
LibStructural.loadReactionNames(self, *args):

 Load reaction names.

This function should be used whenever labeled matrices are important as these
labels will be used in labeling the structural matrices. This function sets the reaction 
names (ids). 

Remarks: This method should only be called after ::LibStructural_loadStoichiometryMatrix

:param: reactionNames a vector of reaction names (ids)

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::loadSpecies "
LibStructural.loadSpecies(self, *args):

 Load species names and initial values. 

This function should be used whenever labeled matrices are important as these
labels will be used in labeling the structural matrices. This function sets the species 
names (ids). It is also possible to provide an initial condition for each of 
the species. This will be used when calculating the conserved sums.

Remarks: This method should only be called after ::LibStructural_loadStoichiometryMatrix

:param: speciesNames a vector of species names (ids) to load
:param: speciesValues a vector of initial concentrations 

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getGammaMatrixLabels "
LibStructural.getGammaMatrixLabels(self)

 Returns the row and column labels for Gamma, the conservation law array.
:param: oRows a string vector that will be overwritten to hold the row labels
:param: oCols a string vector that will be overwritten to hold the column labels.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getK0MatrixLabels "
LibStructural.getK0MatrixLabels(self)

 Returns the K0 Matrix row and column labels.

:param: outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
:param: outRowCount after the call this variable will hold the number of row labels 
returned.
:param: outColLabels a pointer to a string array where the column labels will be allocated
and written.
:param: outColCount after the call this variable will hold the number of column labels
returned.

:returns: The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

Remarks: To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getKMatrixLabels "
LibStructural.getKMatrixLabels(self)

 Returns the K matrix row and column labels.

:param: outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
:param: outRowCount after the call this variable will hold the number of row labels 
returned.
:param: outColLabels a pointer to a string array where the column labels will be allocated
and written.
:param: outColCount after the call this variable will hold the number of column labels
returned.

:returns: The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

Remarks: To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getL0MatrixLabels "
LibStructural.getL0MatrixLabels(self)

 Returns the L0 Matrix row and column labels.
:param: oRows a string vector that will be overwritten to hold the row labels
:param: oCols a string vector that will be overwritten to hold the column labels.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getLinkMatrixLabels "
LibStructural.getLinkMatrixLabels(self)

 Returns the row and column labels for the Link Matrix, L
:param: oRows a string vector that will be overwritten to hold the row labels
:param: oCols a string vector that will be overwritten to hold the column labels.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getN0MatrixLabels "
LibStructural.getN0MatrixLabels(self)

 Returns the N0 Matrix row and column labels.
:param: oRows a string vector that will be overwritten to hold the row labels
:param: oCols a string vector that will be overwritten to hold the column labels

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNDCMatrixLabels "
LibStructural.getNDCMatrixLabels(self)

 Returns the NDC Matrix row and column labels.

:param: outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
:param: outRowCount after the call this variable will hold the number of row labels 
returned.
:param: outColLabels a pointer to a string array where the column labels will be allocated
and written.
:param: outColCount after the call this variable will hold the number of column labels
returned.

:returns: The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

Remarks: To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNICMatrixLabels "
LibStructural.getNICMatrixLabels(self)

Returns the NIC Matrix row and column labels.
:param: oRows a string vector that will be overwritten to hold the row labels
:param: oCols a string vector that will be overwritten to hold the column labels.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNrMatrixLabels "
LibStructural.getNrMatrixLabels(self)

Returns the Nr Matrix row and column labels.

:param: outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
:param: outRowCount after the call this variable will hold the number of row labels 
returned.
:param: outColLabels a pointer to a string array where the column labels will be allocated
and written.
:param: outColCount after the call this variable will hold the number of column labels
returned.

:returns: The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

Remarks: To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getStoichiometryMatrixLabels "
LibStructural.getStoichiometryMatrixLabels(self)

 Returns the row and column labels for the original and unaltered stoichiometry matrix.

:param: outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
:param: outRowCount after the call this variable will hold the number of row labels 
returned.
:param: outColLabels a pointer to a string array where the column labels will be allocated
and written.
:param: outColCount after the call this variable will hold the number of column labels
returned.

:returns: The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

Remarks: To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getFullyReorderedStoichiometryMatrixLabels "
LibStructural.getFullyReorderedStoichiometryMatrixLabels(self)

 Returns the row and column labels for the reordered stoichiometry matrix (row reordered stoichiometry matrix)

:param: outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
:param: outRowCount after the call this variable will hold the number of row labels 
returned.
:param: outColLabels a pointer to a string array where the column labels will be allocated
and written.
:param: outColCount after the call this variable will hold the number of column labels
returned.

:returns: The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

Remarks: To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getReorderedStoichiometryMatrixLabels "
LibStructural.getReorderedStoichiometryMatrixLabels(self)

 Returns the row and column labels for the reordered stoichiometry matrix (row reordered stoichiometry matrix)

:param: outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
:param: outRowCount after the call this variable will hold the number of row labels 
returned.
:param: outColLabels a pointer to a string array where the column labels will be allocated
and written.
:param: outColCount after the call this variable will hold the number of column labels
returned.

:returns: The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

Remarks: To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)

";

