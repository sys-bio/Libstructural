Getting Started with LibStructural
==================================

The following examples demonstrate how to load a biochemical reaction network in to LibStructural API for analyzing the stoichiometry matrix. A model should be available at least in one of the following formats: SBML model file (.xml format), a 2D array matrix or a string of SBML file.

----------------------
Testing LibStructural
----------------------
To test the imported structural module, you can use the **test()** method. This will print out an analysis summary of a Glycolysis/Gluconeogenesis SBML model (`BMID000000101155 <https://www.ebi.ac.uk/biomodels-main/BMID000000101155>`_) distributed with LibStructural.

.. code:: python

  import structural
  ls = structural.LibStructural()
  ls.test()

.. end

The following sections describe different ways of loading a model into Libstructural. Once a model is loaded it will automatically call ``analyzeWithQR``. At this point a summary of the analysis can be obtained by calling **getSummary()**:

.. code:: python

  ls.getSummary()

.. end

-------------------------
Loading a model
-------------------------

To load a model in to LibStructural, an instance variable must be created.

.. code:: python

    import structural
    ls = structural.LibStructural()

.. end

Loading a model from a string
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
An SBML model can be loaded from an .xml file format.

.. code:: python

    ls.loadSBMLFromFile("C:\Users\yosef\Documents\SBML_models\iYO844.xml") # This calls the analyzeWithQR implicitly.

.. end

Loading a model from a string
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

It's possible to load a model from an SBML string:

.. code:: python

    ls.loadSBMLFromString("example_SBMLstring")

.. end


Loading a model from a stoichiometric matrix
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

    ls = structural.LibStructural()
    matrix = [[  1, -1, -1], [  0, -1,  1], [  0,  1, -1]] # matrix can be a numpy 2d array
    ls.loadStoichiometryMatrix(matrix)

.. end

The load command will also by default add reaction ids of the form _Jx and species ids of the form Sx. To override these default names, see section below.

Assigning Reaction and Species Ids
----------------------------------

When loading a model from a stoichiometry matrix, a label can be added to reactions and species.

.. code:: python

  import structural
  ls = structural.LibStructural()
  matrix = [[  1, -1, -1], [  0, -1,  1], [  0,  1, -1]] # matrix can be a numpy 2d array

  print ls.getStoichiometryMatrix()
  print ls.getSpeciesIds()
  print ls.getReactionsIds()

  print('\n\n')

  # load Ids
  ls.loadSpeciesIdsWithValues (['a', 'b', 'c'], [0, 0, 0]) # The array length for both ids list and values list should be equal to the number of species
  ls.loadReactionIdsWithValues (['F1', 'F2', 'F3'],[0, 0, 0])

  ls.analyzeWithQR()

  print ls.getSpeciesIds()
  print ls.getReactionsIds()

.. end

Loading a model using the antimony model description language
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


If you use `tellurium <http://tellurium.analogmachine.org/>`_ you can load a model by converting an antimony string to SBML string or by using the Stoichiometry matrix of the antimony model.

.. code:: python

  import structural
  import tellurium as te

  r = te.loada('''
      model Test_Model ()
      species S1, S2, S3;

      // Reactions:
      J1: S1 -> S2; v;
      J2: -> S3; v;
      J3: S3 -> S1; v;

      S1 = 10; S2 = 10; S3 = 10;
      v = 0;

  end

  ''')

  sbmlstr = r.getSBML() # this creates an SBML string from the antimony model, r.
  ls = structural.LibStructural()
  ls.loadSBMLFromString(sbmlstr)
  print(ls.getSummary())

  # an antimony model can be converted in to SBML file as well
  r.exportToSBML('Test_model.xml') # creates an xml file in the current directory
  ls = structural.LibStructural()
  ls.loadSBMLFromFile('Test_model.xml') # loads the xml file from the current directory
  print(ls.getSummary())
.. end

-------------------------
Structural Analysis
-------------------------

The following snippets show some of LibStructural's methods on a model generated using antimony model description language.

.. code:: python

  import structural
  import tellurium as te

  r = te.loada('''

      // Reactions:
      J1: ES -> S1 + E; v;
      J2: S2 + E -> ES; v;
      J3: S1 -> S2; v;

      // Species Intialiazation
      S1 = 10; S2 = 10; ES = 10; E = 10;
      v = 0;

  end

  ''')

  sbmlstr = r.getSBML() # this creates an SBML string from the antimony model, r.
  ls = structural.LibStructural()
  ls.loadSBMLFromString(sbmlstr)

.. end


Once our model is loaded we can run methods.

.. code:: python
  print(ls.validateStructuralMatrices()) # Prints out if the model is passed some interna structural validation tests.

  # To see whats tests was performed call ls.test
  tests = ls.getTestDetails()
  print(tests)
.. end
