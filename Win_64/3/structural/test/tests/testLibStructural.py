import structural
import pkg_resources
ls = structural.LibStructural()

# Test empty model

try:
    print(ls.getColumnReorderedNrMatrix())

    print(ls.getConditionNumber(ls.getStoichiometryMatrix()))
    print(ls.getEigenValues(ls.getNICMatrix())) # Requires square matrix
    print(ls.getEigenVectors(ls.getNICMatrix())) # Requires square matrix
    print(ls.getGammaMatrixGJ(ls.getNICMatrix()))
    print(ls.getLeftNullSpace(ls.getStoichiometryMatrix()))
    print(ls.getRConditionNumber(ls.getNICMatrix()))
    print(ls.getRank(ls.getStoichiometryMatrix()))
    print(ls.getRightNullSpace(ls.getStoichiometryMatrix()))
    print(ls.getKMatrix())
    print(ls.getN0Matrix())
    print(ls.getNDCMatrix())
    print(ls.getNICMatrix())
    print(ls.getNrMatrix())
    print(ls.getReorderedStoichiometryMatrix())
    print(ls.getStoichiometryMatrix())
except:
    pass

try:
  print(ls.getL0MatrixIds())
except Exception as e:
  print(e)


try:
  print(ls.getColumnReorderedNrMatrixIds())
except Exception as e:
  print(e)

print(ls.getConservedLaws())

print(ls.getConservedSums())
print(ls.getDependentReactionIds())
print(ls.getDependentSpeciesIds())

try:
  print(ls.getFullyReorderedN0StoichiometryMatrix())
except Exception as e:
  print(e)

try:
  print(ls.getFullyReorderedNrMatrix())
except Exception as e:
  print(e)

try:
  print(ls.getFullyReorderedStoichiometryMatrix())
except Exception as e:
  print(e)

try:
  print(ls.getFullyReorderedStoichiometryMatrixIds())
except Exception as e:
  print(e)

try:
  print(ls.getGammaMatrix())
except Exception as e:
  print(e)

try:
  print(ls.getGammaMatrixIds())
except Exception as e:
  print(e)

print(ls.getIndependentReactionIds())
print(ls.getIndependentSpeciesIds())
print(ls.getInitialConditions())

try:
  print(ls.getK0Matrix())
except Exception as e:
  print(e)

try:
  print(ls.getK0MatrixIds())
except Exception as e:
  print(e)

try:
  print(ls.getKMatrixIds())
except Exception as e:
  print(e)

try:
  print(ls.getL0Matrix())
except Exception as e:
  print(e)

try:
  print(ls.getLinkMatrix())
except Exception as e:
  print(e)

try:
  print(ls.getLinkMatrixIds())
except Exception as e:
  print(e)


print(ls.getModelName())

try:
  print(ls.getN0MatrixIds())
except Exception as e:
  print(e)

try:
  print(ls.getNDCMatrixIds())
except Exception as e:
  print(e)


try:
  print(ls.getNICMatrixIds())
except Exception as e:
  print(e)

print(ls.getNmatrixSparsity())

try:
  print(ls.getNrMatrixIds())
except Exception as e:
  print(e)

print(ls.getNumDepReactions())
print(ls.getNumDepSpecies())
print(ls.getNumIndReactions())
print(ls.getNumIndSpecies())
print(ls.getNumReactions())
print(ls.getNumSpecies())
print(ls.getReactionIds()) # Rename to Ids
print(ls.getReorderedReactionsIds())
print(ls.getReorderedSpeciesIds())
print(ls.getReorderedStoichiometryMatrixIds())
print(ls.getFloatingSpeciesIds())
print(ls.getStoichiometryMatrixIds())

try:
  print(ls.getTestDetails())
except Exception as e:
  print(e)

print(ls.getTolerance())

try:
  print(ls.getElementaryModes())
except Exception as e:
  print(e)

# Builtin test

ls.test()

# Test Conservation Analysis
print()
print("Conservation analysis example:")

ls = structural.LibStructural()
ls.loadSBMLFromFile(structural.__path__[0] + '/test/tests/Conservation_analysis_example.xml')
print((ls.getSummary()))


print("Test Manual Loading of a Model:")
matrix = [[  1, -1, -1], [  0, -1,  1], [  0,  1, -1]] # matrix can be a numpy 2d array
ls.loadStoichiometryMatrix(matrix)
print(ls.getStoichiometryMatrix())
print()
print("Species Ids: ", ls.getFloatingSpeciesIds())

print("Reaction Ids: ", ls.getReactionIds())
print()
print((ls.getSummary()))
print((ls.validateStructuralMatrices())) # Prints out if the model is passed some interna structural validation tests.
