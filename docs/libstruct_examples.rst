Getting Started with LibStructural
==================================

The following examples demonstrate how to load a biochemical reaction network in to LibStructural API for analyzing the stoichiometry matrix. A model should be available at least in one of the following formats: SBML model file (.xml format), a 2D array matrix or a string of SBML file.

-------------------------
Loading a model
-------------------------

To load a model in to LibStructural, an instance variable must be created.

.. code:: python

    import structural
    ls = structural.LibStructural.getInstance()
    ls.loadSBMLFromFile("example.xml") # or full path to model

.. end

Loading a model from a string
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Its possible to load a model using a SBML string:

.. code:: python

    import structural

    ls = structural.LibStructural.getInstance()
    ls.loadSBMLFromString("example_SBMLstring")
.. end

 
Loading a model from a stoichiometric matrix
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code:: python

    ls = structural.LibStructural.getInstance()
    matrix = [[  1, -1, -1], [  0, -1,  1], [  0,  1, -1]] # matrix can be a numpy 2d array
    ls.loadStoichiometryMatrix(matrix)
    ls.analyzeWithQR()
.. end

Loading a model using the antiomy model description language
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


If you use `tellurium <http://tellurium.analogmachine.org/>`_ you can load a model by converting the antimony model to SBML string or by using the Stoichiometry matrix of the antimony model.
Note: loading a stoichiometry matrix from antimony will result in losing some information such as, species and reaction names.

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
  ls = structural.LibStructural.getInstance()
  ls.loadSBMLFromString(sbmlstr)

  # an antimony model can be converted in to SBML file as well
  r.exportToSBML('Test_model.xml') # creates an xml file in the current directory
  ls = structural.LibStructural.getInstance()
  ls.loadSBMLFromFile('Test_model.xml') # loads the xml file from the current directory
  print(ls.analyzeWithQR())
.. end
