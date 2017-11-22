Getting Started with LibStructural
==================================

The following examples demonstrate how to load a biochemical reaction network in to LibStructural API for analyzing the stoichiometry matrix. A model should be available at least in one of the following formats: SBML model file (.xml format), a 2D array matrix or a string of SBML file.

-------------------------
Loading a model
-------------------------

To load a model in to LibStructural, an instance variable object

 be created.

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
    ls.loadStoichiometryMatrix("2D_array_matrix")
.. end
