def run():
    
    import structural
    import pkg_resources
    ls = structural.LibStructural()
    
    # Test empty model
    print("=======================================================")
    print("Testing integrity of methods for empty model...")
    print("=======================================================\n")
    
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
        print(ls.getStoichiometryMatrixBoundary())
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
    print(ls.getNumFloatingSpecies())
    print(ls.getNumBoundarySpecies())
    print(ls.getReactionIds()) # Rename to Ids
    print(ls.getReorderedReactionIds())
    print(ls.getReorderedSpeciesIds())
    print(ls.getReorderedStoichiometryMatrixIds())
    print(ls.getFloatingSpeciesIds())
    print(ls.getBoundarySpeciesIds())
    print(ls.getStoichiometryMatrixIds())
    
    try:
      print(ls.getTestDetails())
    except Exception as e:
      print(e)
    
    print(ls.getTolerance())
    
    try:
      print(ls.getElementaryModesInteger())
    except Exception as e:
      print(e)
      
    try:
      print(ls.getElementaryModesDouble())
    except Exception as e:
      print(e)
    
    
    # Test Conservation Analysis
    print("\n\n=======================================================")
    print("Testing a model containing conserved moities...")
    print("=======================================================\n")
    ls = structural.LibStructural()
    model_path = pkg_resources.resource_filename('structural','/test/Conservation_analysis_example.xml')
    ls.loadSBMLFromFile(model_path)
    if not "Fail" in ls.validateStructuralMatrices():
        print ("***Pass***")
    else:
        print("***Fail***")
    
    print("\n\n===========================================================================================")
    print("Testing validity of structural matrices for test model BMID000000101155 from BioModels...")
    print("===========================================================================================\n")
    model_path = pkg_resources.resource_filename('structural','/test/BMID000000101155.xml')
    ls = structural.LibStructural()
    ls.loadSBMLFromFile(model_path)
    if not "Fail" in ls.validateStructuralMatrices():
        print ("***Pass***")
    else:
        print("***Fail***")
    
    
    print("\n\n========================================================================")
    print("Testing validity of structural matrices for test models #1-#31...")
    print("========================================================================\n")
    for i in range(31):
    	model_path = pkg_resources.resource_filename('structural','/test/testModel'+str(i+1)+'.xml')
    	ls = structural.LibStructural()
    	ls.loadSBMLFromFile(model_path)
    	
    	if not "Fail" in list(ls.validateStructuralMatrices()):
    		print ("Test Model #" + str(i+1) + " : ***Pass***")
    	else:
    		print ("Test Model #" + str(i+1) + " : ***Fail***")
    
    print("\n\n===========================================")
    print("Testing manual loading of a model...")
    print("===========================================\n")
    
    matrix = [[  1, -1, -1], [  0, -1,  1], [  0,  1, -1]] # matrix can be a numpy 2d array
    ls.loadStoichiometryMatrix(matrix)
    print(ls.getStoichiometryMatrix())
    print("Species Ids: ", ls.getFloatingSpeciesIds())
    
    print("Reaction Ids: ", ls.getReactionIds())
    print((ls.getSummary()))
    print((ls.validateStructuralMatrices())) # Prints out if the model is passed some interna structural validation tests.
