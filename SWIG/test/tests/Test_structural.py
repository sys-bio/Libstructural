import structural
import tellurium as te
import pkg_resources
ls = structural.LibStructural()
#ls.test()
#%%
r = te.loada('''
    
    // Reactions:
    J1: ES -> E  + S1; v;
    J2: S1 -> S2 ; v;
    J3: S2 + E -> ES; v;

    
    // Variable initializations:
        v = 0;
    
    // Species initializations:
    S1 = 100; E = 20;
    
''')

sbmlstr = r.getSBML()
ls = structural.LibStructural()
ls.loadSBMLFromString(sbmlstr)
print(ls.getSummary())
#%%

model_path = pkg_resources.resource_filename('structural','test_models/BMID000000101155.xml')
ls = structural.LibStructural()
ls.loadSBMLFromFile(model_path)
print(ls.getSummary())

#%%
#print ls.analyzeWithFullyPivotedLU()
print ls.analyzeWithFullyPivotedLUwithTests()
print ls.analyzeWithLU()
print ls.analyzeWithLUandRunTests()
print ls.analyzeWithQR()
#%%
import structural
ls = structural.LibStructural()
#%%
try:
    print ls.getColumnReorderedNrMatrix()
    
    print ls.getConditionNumber(ls.getStoichiometryMatrix())
    print ls.getEigenValues(ls.getNICMatrix()) # Requires square matrix
    print ls.getEigenVectors(ls.getNICMatrix()) # Requires square matrix
    print ls.getGammaMatrixGJ(ls.getNICMatrix())
    print ls.getLeftNullSpace(ls.getStoichiometryMatrix())
    print ls.getRConditionNumber(ls.getNICMatrix())
    print ls.getRank(ls.getStoichiometryMatrix())
    print ls.getRightNullSpace(ls.getStoichiometryMatrix())
    print ls.getKMatrix()
    print ls.getN0Matrix()
    print ls.getNDCMatrix()
    print ls.getNICMatrix()
    print ls.getNrMatrix()
    print ls.getReorderedStoichiometryMatrix()
    print ls.getStoichiometryMatrix()

except:
    pass
print ls.getColumnReorderedNrMatrixIds()

print ls.getConservedLaws()
print ls.getConservedSums()
print ls.getDependentReactionIds()
print ls.getDependentSpeciesIds()
print ls.getFullyReorderedN0StoichiometryMatrix()
print ls.getFullyReorderedNrMatrix()
print ls.getFullyReorderedStoichiometryMatrix()
print ls.getFullyReorderedStoichiometryMatrixIds()
print ls.getGammaMatrix()
print ls.getGammaMatrixIds()
print ls.getIndependentReactionIds()
print ls.getIndependentSpeciesIds()
print ls.getInitialConditions()
print ls.getK0Matrix()
print ls.getK0MatrixIds()
print ls.getKMatrixIds()
print ls.getL0Matrix()
print ls.getL0MatrixIds()
print ls.getLinkMatrix() 
print ls.getLinkMatrixIds()
print ls.getModelName()
print ls.getN0MatrixIds()
print ls.getNDCMatrixIds()
print ls.getNICMatrixIds()
print ls.getNmatrixSparsity()
print ls.getNrMatrixIds()
print ls.getNumDepReactions()
print ls.getNumDepSpecies()
print ls.getNumIndReactions()
print ls.getNumIndSpecies()
print ls.getNumReactions()
print ls.getNumSpecies()
print ls.getReactionsIds() # Rename to Ids
print ls.getReorderedReactionsIds()
print ls.getReorderedSpeciesIds()
print ls.getReorderedStoichiometryMatrixIds()
print ls.getSpeciesIds()
print ls.getStoichiometryMatrixIds()
print ls.getTestDetails()
print ls.getTolerance()
print ls.getElementaryModes()
#%%

print ls.rref(ls.getNICMatrix())
print ls.rref_FB(ls.getNICMatrix()) # change to rrefReordered
print ls.setTolerance(1e-7)
print ls.validateStructuralMatrices()


#%%

import structural

ls = structural.LibStructural()
matrix = [[  1, -1, -1], [  0, -1,  1], [  0,  1, -1]] # matrix can be a numpy 2d array
ls.loadStoichiometryMatrix(matrix)
#ls.loadSBMLFromString(sbmlstr)
print ls.getStoichiometryMatrix()
print ls.getSpeciesIds(), "Sp Ids"

print ls.getReactionsIds(), "Rxn Ids"


print('\n\n')
# load Names
ls.loadSpeciesIdsWithValues (['a', 'b', 'c'], [0, 0, 0])
ls.loadReactionIdsWithValues (['F1', 'F2', 'F3'],[0, 0, 0])

ls.analyzeWithQR()

print ls.getSpeciesIds(), "Sp Ids"
print ls.getReactionsIds(), "Rxn Ids"

#%%
print(ls.validateStructuralMatrices()) # Prints out if the model is passed some interna structural validation tests.

  # To see whats tests was performed call ls.test
tests = ls.getTestDetails()
print(tests)
