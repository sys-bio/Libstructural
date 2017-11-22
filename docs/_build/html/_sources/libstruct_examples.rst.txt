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

For loading a model in 2D array format or SBML string format:

.. code:: python

    import structural

    ls = structural.LibStructural.getInstance()
    ls.loadSBMLFromString("example_SBMLstring")

    # or

    ls = structural.LibStructural.getInstance()
    matrix = [[  1, -1, -1], [  0, -1,  1], [  0,  1, -1]] # matrix can be a numpy 2d array
    ls.loadStoichiometryMatrix(matrix)
.. end

If you use `tellurium<http://tellurium.analogmachine.org/>`_ you can load a model by converting the antimony model to SBML string or by using the Stoichiometry matrix of the antimony model.
Note: loading a stoichiometry matrix from antimony will result in losing some information such as, species and reaction names.

.. code:: python

  import structural
  import tellurium as te

  r = te.loada('''
      model Test_Model ()
      species S1, S2, S3, S4;

      // Reactions:
      J1: S1 -> S2;  k1*S1;
      J2: S2 -> S3; k2*S2 - k3*S3;
      J3: S2 -> S3; k2*S2;
      J4: S3 -> S2; k3*S3;
      J5: S3 -> S4; k4*S3;

      // Variable initializations:
      k1 = 0.1; k2 = 0.5; k3 = 0.5; k4 = 0.5;

      // Species initializations:
      S1 = 100;

      // Display Names:
      J1 is "Reaction one";
      J2 is "Reaction two";
      J3 is "Reaction three";
      J4 is "Reaction four";
      J5 is "Reaction five";
      S1 is "Species one";
      S2 is "Species two";
      S3 is "Species three";
      S4 is "Species four";
  end

  ''')

  sbmlstr = r.getSBML() # this creates an SBML string from the antimony model, r.
  ls = structural.LibStructural.getInstance()
  ls.loadSBMLFromString(sbmlstr)

  # an antimony model can be converted in to SBML file as well
  r.exportToSBML('Test_model.xml') # creates an xml file in the current directory
  ls = structural.LibStructural.getInstance()
  ls.loadSBMLFromFile('Test_model.xml') # loads the xml file from the current directory

.. end
