%feature("docstring") LIB_STRUCTURAL::LibStructural::getSummary "
LibStructural.getSummary(self, *args)

:returns: Returns the summary string of the last analysis.

";


%feature("docstring") LIB_STRUCTURAL::LibStructural::isReactionReversible "
LibStructural.isReactionReversible(self, *args)

:param: Reaction index
:returns: "True" if the reaction with given index is reversible.

Checks whether a given reaction is reversible or not.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::loadSBMLFromString "
LibStructural.loadSBMLFromString(self, *args)

:param: SBML string to load
:returns: information about the loaded model

Loads SBML model from a string variable.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::loadSBMLFromFile "
LibStructural.loadSBMLFromFile(self, *args)

:param: sFileName a file name to a SBML file to load
:returns: information about the loaded model

Loads SBML model from the specified file.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::loadSBMLwithTests "
LibStructural.loadSBMLwithTests(self, *args)

:param: a file name to an SBML model to load.
:returns: information about the loaded model and results of the internal test suite.

Loads an SBML model into the library and carries out tests using the internal test suite.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::analyzeWithQR "
LibStructural.analyzeWithQR(self)

Uses QR factorization for structural analysis. This method performs the actual analysis of the stoichiometry matrix loaded
either via

``LibStructural.loadStoichiometryMatrix`` or ``LibStructural.loadSBMLFromString``.

Only after one of the analysis methods below has been called are the structural matrices (L0, K0...) available.

| ``LibStructural.analyzeWithQR``,
| ``LibStructural.analyzeWithLU``,
| ``LibStructural.analyzeWithLU``,
  ``RunTests``,
| ``LibStructural.analyzeWithFullyPivotedLU``,
| ``LibStructural.analyzeWithFullyPivotedLUwithTests``


Remarks: This is the prefered method for structural analysis.

:returns: a string with information about the analysis process

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::analyzeWithLU "
LibStructural.analyzeWithLU(self)
Uses LU Decomposition for Conservation analysis. This method performs the actual analysis of the stoichiometry matrix (loaded either
via

``LibStructural.loadStoichiometryMatrix`` or ``LibStructural.loadSBMLFromString``.

Only after one of the analysis methods below has been called are the structural matrices (L0, K0...) available.

| ``LibStructural.analyzeWithQR``,
| ``LibStructural.analyzeWithLU``,
| ``LibStructural.analyzeWithLUandRunTests``,
| ``LibStructural.analyzeWithFullyPivotedLU`` or
| ``LibStructural.analyzeWithFullyPivotedLUwithTests``

:returns: a string with information about the analysis process

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::analyzeWithLUandRunTests "
LibStructural.analyzeWithLUandRunTests(self)
Uses LU Decomposition for Conservation analysis. This method performs the actual analysis of the stoichiometry matrix loaded either
via

``LibStructural.loadStoichiometryMatrix`` or ``LibStructural.loadSBMLFromString``.

Only after one of the analysis methods below has been called are the structural matrices (L0, K0...) available.

| ``LibStructural.analyzeWithQR``,
| ``LibStructural.analyzeWithLU``,
| ``LibStructural.analyzeWithLUandRunTests``,
| ``LibStructural.analyzeWithFullyPivotedLU`` or
| ``LibStructural.analyzeWithFullyPivotedLUwithTests''

This method additionally performs the integrated test suite and returns	those results.

:returns: a string with information about the analysis process

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLU "
LibStructural.analyzeWithFullyPivotedLU(self)
Uses fully pivoted LU Decomposition for Conservation analysis.This method performs the actual analysis of the stoichiometry matrix loaded either
via

``LibStructural.loadStoichiometryMatrix`` or ``LibStructural.loadSBMLFromString``.

Only after one of the analysis methods below has been called are the structural matrices (L0, K0...) available.

| ``LibStructural.analyzeWithQR``
| ``LibStructural.analyzeWithLU``
| ``LibStructural.analyzeWithLUandRunTests``
| ``LibStructural.analyzeWithFullyPivotedLU`` or
| ``LibStructural.analyzeWithFullyPivotedLUwithTests``

:returns: a string with information about the analysis process

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLUwithTests "
LibStructural.analyzeWithFullyPivotedLUwithTests(self)
Uses fully pivoted LU Decomposition for Conservation analysis. This method performs the actual analysis of the stoichiometry matrix loaded
either via

``LibStructural.loadStoichiometryMatrix`` or ``LibStructural.loadSBMLFromString``.

Only after one of the analysis methods below has been called are the structural matrices (L0, K0...) available.

| ``LibStructural.analyzeWithQR``,
| ``LibStructural.analyzeWithLU``,
| ``LibStructural.analyzeWithLU``andRunTests,
| ``LibStructural.analyzeWithFullyPivotedLU`` or
| ``LibStructural.analyzeWithFullyPivotedLUwithTests``

This method additionally performs the integrated test suite and returns	those results.

:returns: a string with information about the analysis process

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getReorderedSpeciesIds "
LibStructural.getReorderedSpeciesIds(self)

:returns: the reordered list of molecular species (choosing the SBML Id if possible).

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getFloatingSpeciesIds "
LibStructural.getFloatingSpeciesIds(self)

:returns: the unordered list of species Ids.

";


%feature("docstring") LIB_STRUCTURAL::LibStructural::getIndependentSpeciesIds "
LibStructural.getIndependentSpeciesIds(self)

:returns: the list of Ids for the independent species.

";


%feature("docstring") LIB_STRUCTURAL::LibStructural::getDependentSpeciesIds "
LibStructural.getDependentSpeciesIds(self)

:returns: the list of Ids for the dependent species.

";


%feature("docstring") LIB_STRUCTURAL::LibStructural::getReactionsIds "
LibStructural.getReactionsIds(self)

:returns: the list of Reaction ids.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getIndependentReactionIds "
LibStructural.getIndependentReactionIds(self)

:returns: the list of Ids for the independent reactions.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getDependentReactionIds "
LibStructural.getDependentReactionIds(self)

:returns: the list of Ids for the dependent reactions.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getReorderedReactionsIds "
LibStructural.getReorderedReactionsIds(self)

:returns: the reordered Id list of reactions.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::ConservedLaws "
LibStructural.ConservedLaws(self):



";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getConservedSums "
LibStructural.getConservedSums(self)

:returns: Total mass for each conserved cycle in the model. The sum is based on the current initial conditions.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getInitialConditions "
LibStructural.getInitialConditions(self)

:returns: Initial Conditions used in the model.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::validateStructuralMatrices "
LibStructural.validateStructuralMatrices(self)

Validates structural matrices. Calling this method will run the internal test suite against the structural
matrices those tests include:

| Test 1 : Gamma*N = 0 (Zero matrix)
| Test 2 : Rank(N) using SVD (5) is same as m0 (5)
| Test 3 : Rank(NR) using SVD (5) is same as m0 (5)
| Test 4 : Rank(NR) using QR (5) is same as m0 (5)
| Test 5 : L0 obtained with QR matches Q21*inv(Q11)
| Test 6 : N*K = 0 (Zero matrix)
";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getTestDetails "
LibStructural.getTestDetails(self)

:returns: Details about the validation tests.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getModelName "
LibStructural.getModelName(self)

:returns: the name of the model if SBML model has Name-tag, otherwise it returns the SBML id. If only a stoichiometry matrix was loaded 'untitled' will be returned.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNumSpecies "
LibStructural.getNumSpecies(self)

:returns: the total number of species

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNumIndSpecies "
LibStructural.getNumIndSpecies(self)

:returns: the number of independent species.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNumDepSpecies "
LibStructural.getNumDepSpecies(self)

:returns: the number of dependent species.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNumReactions "
LibStructural.getNumReactions(self)

:returns: the total number of reactions.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNumIndReactions "
LibStructural.getNumIndReactions(self)

:returns: the number of independent reactions

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNumDepReactions "
LibStructural.getNumDepReactions(self)

:returns: the number of dependent reactions

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNmatrixSparsity "
LibStructural.getNmatrixSparsity(self)

:returns: the number of nonzero values in Stoichiometry matrix

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getRank "
LibStructural.getRank(self);

:param: Matrix to find the rank of.

:returns: the rank as an integer

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getRCond "
LibStructural.getRCond(self, *args)


";

%feature("docstring") LIB_STRUCTURAL::LibStructural::setTolerance "
LibStructural.setTolerance(self, *args)

:param:  an integer or float

Sets user specified tolerance. This function sets the tolerance used by the library to determine what value is considered as zero. Any value with absolute value smaller than this tolerance is considered as zero and will be neglected.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getTolerance "
LibStructural.getTolerance(self)

:returns: the currently used tolerance for decided whether a small number is considered zero or not.

This function returns the tolerance currently used by the library to determine what value
is considered as zero. Any value with absolute value smaller than this tolerance is considered zero
and will be neglected.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getInstance "
LibStructural.getInstance(self)

static method to get an instance of LibStructural (allows use as singleton)

 import structural
 ls = structural.LibStructural.getInstance()
";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getColumnReorderedNrMatrixIds "
LibStructural.getColumnReorderedNrMatrixIds(self)

:returns: the Nr Matrix row and column labels (repartitioned into NIC and NDC).

";

/*%feature("docstring") LIB_STRUCTURAL::LibStructural::loadReactionNames "
LibStructural.loadReactionNames(self, *args):

:param: a vector of reaction names (ids)

Loads reaction names. This function should be used whenever labeled matrices are important as these labels will be used in labeling the structural matrices. This function sets the reaction names (ids).

Remarks: This method should only be called after ``LibStructural.loadStoichiometryMatrix``

";*/

%feature("docstring") LIB_STRUCTURAL::LibStructural::loadSpeciesIdsWithValues "
LibStructural.loadSpeciesIdsWithValues(self, *args):

:param: speciesIds a vector of species ids to load
:param: speciesValues a vector of initial concentrations

Loads species names and initial values. This function should be used whenever labeled matrices are important as these labels will be used in labeling the structural matrices. This function sets the species ids. It is also possible to provide an initial condition for each of the species. This will be used when calculating the conserved sums.

Remarks: This method should only be called after ``LibStructural.loadStoichiometryMatrix``. ``LibStructural.analyzeWithQR`` should be called for the function to take effect.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::loadReactionIdsWithValues "
LibStructural.loadReactionIdsWithValues(self, *args):

:param: reactionIds a vector of reaction ids to load
:param: reactionValues a vector of reaction rates

This function should be used whenever labeled matrices are important as these labels will be used in labeling the structural matrices. This function sets the reaction ids. It is also possible to provide reaction rate values. This will be used when calculating the conserved sums.

Remarks: This method should only be called after ``LibStructural.loadStoichiometryMatrix``. ``LibStructural.analyzeWithQR`` should be called for the function to take effect.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getGammaMatrixIds "
LibStructural.getGammaMatrixIds(self)

:returns: the row and column labels for Gamma, the conservation law array.
";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getK0MatrixIds "
LibStructural.getK0MatrixIds(self)

:returns: the K0 Matrix row and column labels.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getKMatrixIds "
LibStructural.getKMatrixIds(self)

:returns: the K matrix row and column labels.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getL0MatrixIds "
LibStructural.getL0MatrixIds(self)

:returns: the L0 Matrix row and column labels.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getLinkMatrixIds "
LibStructural.getLinkMatrixIds(self)

:returns: the row and column labels for the Link Matrix, L

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getN0MatrixIds "
LibStructural.getN0MatrixIds(self)

:returns: the N0 Matrix row and column labels.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNDCMatrixIds "
LibStructural.getNDCMatrixIds(self)

:returns: the NDC Matrix row and column labels.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNICMatrixIds "
LibStructural.getNICMatrixIds(self)

:returns: the NIC Matrix row and column labels.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getNrMatrixIds "
LibStructural.getNrMatrixIds(self)

:returns: the Nr Matrix row and column labels.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getStoichiometryMatrixIds "
LibStructural.getStoichiometryMatrixIds(self)

:returns: the row and column labels for the original and unaltered stoichiometry matrix.

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getFullyReorderedStoichiometryMatrixIds "
LibStructural.getFullyReorderedStoichiometryMatrixIds(self)

:returns: the row and column labels for the reordered stoichiometry matrix (row reordered stoichiometry matrix)

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getReorderedStoichiometryMatrixIds "
LibStructural.getReorderedStoichiometryMatrixIds(self)

:returns: the row and column labels for the reordered stoichiometry matrix (row reordered stoichiometry matrix)

";

%feature("docstring") LIB_STRUCTURAL::LibStructural::getConservedLaws "
LibStructural.getConservedLaws(self)

:returns: algebraic expressions for the conserved cycles. Returns empty if none.

";
