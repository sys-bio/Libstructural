%module structural

// #pragma SWIG nowarn=516

//%feature("autodoc");
%inline
%{
#include "libstructural.h"
#include "util.h"
#include "libla.h"
#include "libMetaToolInt4_3.h"

#define SWIG_FILE_WITH_INIT
static PyObject* pNoModelException;  /* add this! */
%}

%rename (_my_getElementaryModesInteger) getElementaryModesInteger;
%rename (_my_loadStoichiometryMatrix) loadStoichiometryMatrix;
%rename (_my_rref) rref;
%rename (_my_rref_FB) rref_FB;
%rename (_my_getEigenValues) getEigenValues;
%rename (_my_getEigenVectors) getEigenVectors;
%rename (_my_getConditionNumber) getConditionNumber;
%rename (_my_getLeftNullSpace) getLeftNullSpace;
%rename (_my_getRightNullSpace) getRightNullSpace;
%rename (_my_getRank) getRank;
%rename (_my_getStoichiometryMatrix) getStoichiometryMatrix;
%rename (_my_getStoichiometryMatrixBoundary) getStoichiometryMatrixBoundary;
%rename (_my_getColumnReorderedNrMatrix) getColumnReorderedNrMatrix;
%rename (_my_getFullyReorderedN0StoichiometryMatrix) getFullyReorderedN0StoichiometryMatrix;
%rename (_my_getFullyReorderedNrMatrix) getFullyReorderedNrMatrix;
%rename (_my_getFullyReorderedStoichiometryMatrix) getFullyReorderedStoichiometryMatrix;
%rename (_my_getGammaMatrix) getGammaMatrix;
%rename (_my_getGammaMatrixGJ) getGammaMatrixGJ;
%rename (_my_getK0Matrix) getK0Matrix;
%rename (_my_getKMatrix) getKMatrix;
%rename (_my_getL0Matrix) getL0Matrix;
%rename (_my_getN0Matrix) getN0Matrix;
%rename (_my_getNDCMatrix) getNDCMatrix;
%rename (_my_getNICMatrix) getNICMatrix;
%rename (_my_getNrMatrix) getNrMatrix;
%rename (_my_getReorderedStoichiometryMatrix) getReorderedStoichiometryMatrix;
%rename (_my_getLinkMatrix) getLinkMatrix;
%rename (_my_getRCond) getRCond;



%init %{
    pNoModelException = PyErr_NewException ("_structural.NoModelException", NULL, NULL);
    Py_INCREF (pNoModelException);
    PyModule_AddObject (m, "NoModelException", pNoModelException);
%}

%pythoncode %{
    NoModelException = _structural.NoModelException
%}

%exception {
     try {
     $action
   } catch (LIB_LA::ApplicationException& e) {
         std::string msg = e.getDetailedMessage();
         PyErr_SetString (PyExc_Exception, msg.c_str());
		 return NULL;
   } catch (LIB_LA::NoModelException& e) {
         std::string msg = e.getMessage();
         PyErr_SetString (PyExc_Exception, msg.c_str());
         return NULL;
   } catch (const std::exception& e) {
         SWIG_exception(SWIG_RuntimeError, e.what());
   } catch (...) {
         SWIG_exception(SWIG_RuntimeError, "Unknown exception");
     }
}

%include "carrays.i"
%include "typemaps.i"
%include "stl.i"
%include "std_vector.i"
%include "exception.i"
%include "st_docstrings.i"
%include "../include/matrix.h"


%template(StringDouble) std::pair<std::string,double>;
%template(StrDoubleVector) std::vector< std::pair<std::string,double> >;
%template(StringVector) std::vector<std::string>;
%template(DoubleVector) std::vector<double>;
%template(StringVectorx2) std::pair< std::vector<std::string>, std::vector<std::string> >;

//*
%template(DoubleMatrixStringVector) std::pair<LIB_LA::DoubleMatrix*,  std::vector< std::string> >;

// http://swig.10945.n7.nabble.com/replacing-a-real-class-method-with-SWIG-version-td11418.html
%extend LIB_STRUCTURAL::LibStructural {

    std::pair< std::vector<std::string>, std::vector<std::string> > getColumnReorderedNrMatrixIds() {
        std::vector<std::string> rows;
        std::vector<std::string> cols;
        self->getColumnReorderedNrMatrixIds(rows, cols);
        return std::make_pair(rows, cols);
    }

    void loadReactionIds (std::vector<std::string> reactionIds) {
        self->loadReactionIds(reactionIds);
    }

    void loadReactionIdsWithValues (std::vector<std::string> reactionIds, std::vector<double> reactionValues) {
        self->loadReactionIdsWithValues(reactionIds, reactionValues);
    }

    void loadSpeciesIdsWithValues (std::vector<std::string> speciesIds, std::vector<double> speciesValues) {
        self->loadSpeciesIdsWithValues(speciesIds, speciesValues);
    }

    /*void loadSpeciesIds (std::vector<std::string> speciesIds) {
        self->loadSpeciesIds(speciesIds);
    }*/

    std::pair< std::vector<std::string>, std::vector<std::string> > getGammaMatrixIds() {
        std::vector<std::string> rows;
        std::vector<std::string> cols;
        self->getGammaMatrixIds(rows, cols);
        return std::make_pair(rows, cols);
    }

    std::pair< std::vector<std::string>, std::vector<std::string> > getK0MatrixIds() {
        std::vector<std::string> rows;
        std::vector<std::string> cols;
        self->getK0MatrixIds(rows, cols);
        return std::make_pair(rows, cols);
    }

    std::pair< std::vector<std::string>, std::vector<std::string> > getKMatrixIds() {
        std::vector<std::string> rows;
        std::vector<std::string> cols;
        self->getKMatrixIds(rows, cols);
        return std::make_pair(rows, cols);
    }

    std::pair< std::vector<std::string>, std::vector<std::string> > getL0MatrixIds() {
        std::vector<std::string> rows;
        std::vector<std::string> cols;
        self->getL0MatrixIds(rows, cols);
        return std::make_pair(rows, cols);
    }

    std::pair< std::vector<std::string>, std::vector<std::string> > getLinkMatrixIds() {
        std::vector<std::string> rows;
        std::vector<std::string> cols;
        self->getLinkMatrixIds(rows, cols);
        return std::make_pair(rows, cols);
    }

    std::pair< std::vector<std::string>, std::vector<std::string> > getLinkMatrixIds() {
        std::vector<std::string> rows;
        std::vector<std::string> cols;
        self->getLinkMatrixIds(rows, cols);
        return std::make_pair(rows, cols);
    }

    std::pair< std::vector<std::string>, std::vector<std::string> > getN0MatrixIds() {
        std::vector<std::string> rows;
        std::vector<std::string> cols;
        self->getN0MatrixIds(rows, cols);
        return std::make_pair(rows, cols);
    }

    std::pair< std::vector<std::string>, std::vector<std::string> > getNDCMatrixIds() {
        std::vector<std::string> rows;
        std::vector<std::string> cols;
        self->getNDCMatrixIds(rows, cols);
        return std::make_pair(rows, cols);
    }

    std::pair< std::vector<std::string>, std::vector<std::string> > getNICMatrixIds() {
        std::vector<std::string> rows;
        std::vector<std::string> cols;
        self->getNICMatrixIds(rows, cols);
        return std::make_pair(rows, cols);
    }

    std::pair< std::vector<std::string>, std::vector<std::string> > getNrMatrixIds() {
        std::vector<std::string> rows;
        std::vector<std::string> cols;
        self->getNrMatrixIds(rows, cols);
        return std::make_pair(rows, cols);
    }

    std::pair< std::vector<std::string>, std::vector<std::string> > getStoichiometryMatrixIds() {
        std::vector<std::string> rows;
        std::vector<std::string> cols;
        self->getStoichiometryMatrixIds(rows, cols);
        return std::make_pair(rows, cols);
    }

    std::pair< std::vector<std::string>, std::vector<std::string> > getFullyReorderedStoichiometryMatrixIds() {
        std::vector<std::string> rows;
        std::vector<std::string> cols;
        self->getFullyReorderedStoichiometryMatrixIds(rows, cols);
        return std::make_pair(rows, cols);
    }

    std::pair< std::vector<std::string>, std::vector<std::string> > getReorderedStoichiometryMatrixIds() {
        std::vector<std::string> rows;
        std::vector<std::string> cols;
        self->getReorderedStoichiometryMatrixIds(rows, cols);
        return std::make_pair(rows, cols);
    }

%pythoncode %{
    global exitCodeDict
    exitCodeDict = {
      -1 : "MetaTool Error: Not enough memory: Programm prematurely finished",
    -2 : "MetaTool Error: amount for allocation is zero : Programm prematurely finished",
    -3 : "MetaTool Error: A metabolite could not be found in the stoichiometric equations! Program prematurely terminated",
    -4 : "MetaTool Error: Enzyme name could not be found in the stoichiometric equations! Program prematurely finished",
    -5 : "MetaTool Error: File error",
    -6 : "MetaTool Error: Enlarge the array size of the struct enc ri.Program prematurely finished",
    -7 : "MetaTool Error: An enzyme reaction is defined more than once. Please delete the extra defined reaction in the input file. Program prematurly finished.",
    -8 : "MetaTool Error: The system comprises only external metabolites.",
    -9 : "MetaTool Error: File error",
    -10 : "MetaTool Error: The input file contains interlocking commentaries. (To many closing marks)",
    -11 : "MetaTool Error: The input file contains interlocking commentaries.",
    -12 : "MetaTool Error: ERROR IN FUNCTION CUTCOL",
    -13 : "MetaTool Error: An intermediate result exceeds the allowed integer range: Program prematurely finished.",
    -14 : "MetaTool Error: The name of input file is the same as the name of the output file: Program prematurely finished.",
    -15 : "MetaTool Error: An enzyme is delared twice: Program prematurely finished.",
    -16 : "MetaTool Error: There are metabolites in the stoichiometric equations and are declared as -METINT or -METEXT: Program prematurely terminated.",
    -17 : "MetaTool Error: File error",
    }
    def getStoichiometryMatrix(self):
      """
      LibStructural.getStoichiometryMatrix(self)
      :returns: Unaltered stoichiometry matrix.
      """
      return self._my_getStoichiometryMatrix().toNumpy();

    def getStoichiometryMatrixBoundary(self):
      """
      LibStructural.getStoichiometryMatrixBoundary(self)
      :returns: Unaltered stoichiometry matrix.
      """
      return self._my_getStoichiometryMatrixBoundary().toNumpy();

    def getColumnReorderedNrMatrix(self):
        """
        LibStructural.getColumnReorderedNrMatrix(self)
        :returns: The Nr Matrix repartitioned into NIC (independent columns) and NDC (dependent columns). The Nr matrix contains the independent rows of the stoichiometry matrix
        """
        return self._my_getColumnReorderedNrMatrix().toNumpy()

    def getFullyReorderedN0StoichiometryMatrix(self):
        """
        LibStructural.getFullyReorderedN0StoichiometryMatrix(self)

        Computes the N0 matrix if possible. The N0 matrix will contain all the dependent rows of the stoichiometry matrix.

        :returns: The N0 Matrix.

        """
        return self._my_getFullyReorderedN0StoichiometryMatrix().toNumpy()

    def getFullyReorderedNrMatrix(self):
        """
        LibStructural.getFullyReorderedNrMatrix(self)

        The Nr matrix contains all the linearly independent rows of the stoichiometry matrix.

        :returns: The Nr Matrix.
        """
        return self._my_getFullyReorderedNrMatrix().toNumpy()

    def getFullyReorderedStoichiometryMatrix(self):
        """
        LibStructural.getFullyReorderedStoichiometryMatrix(self)
        :returns: The fully reordered stoichiometry matrix. Rows and columns are reordered so all indepedent rows
        of the stoichiometry matrix are brought to the top and left side of the matrix.
        """
        return self._my_getFullyReorderedStoichiometryMatrix().toNumpy()

    def getGammaMatrix(self):
        """
        LibStructural.getGammaMatrix(self)
        :returns: Gamma, the conservation law array.
        Each row represents a single conservation law where the column indicates the participating molecular species.
        The number of rows is therefore equal to the number of conservation laws. Columns are ordered according to the
        rows in the reordered stoichiometry matrix, see ``LibStructural.getReorderedSpeciesId`` and ``LibStructural.getReorderedStoichiometryMatrix``.

        """
        return self._my_getGammaMatrix().toNumpy()

    def getGammaMatrixGJ(self, oMatrix):
        """
        LibStructural.getGammaMatrixGJ(self,matrix)

        :param: The stoichiometry matrix that will be used to calculate gamma matrix.
        :returns: Gamma, the conservation law array.

        Each row represents a single conservation law where the column indicate the participating molecular species. The number of rows is therefore equal to the number of conservation laws. In this case the Gamma Matrix is recalculated for the given stoichiometry matrix. The column label will be the same label as the stoichiometry matrix.
        """
        import numpy as np

        if (type(oMatrix) is list or type(oMatrix) is np.ndarray):
            oMatrix = np.array(oMatrix)
            if oMatrix.ndim == 2:
                rows = len(oMatrix)
                cols = len(oMatrix[0])
                m = DoubleMatrix(rows,cols)
                for i in range(rows):
                    for j in range (cols):
                        m.set (i, j, float(oMatrix[i][j]))
                return self._my_getGammaMatrixGJ(m).toNumpy()
            else:
                raise ValueError("Expecting 2 dimensional list or numpy array")
        else:
            raise ValueError("Expecting list or numpy array")


    def getK0Matrix(self):
        """
        LibStructural.getK0Matrix(self)

        :returns: The K0 Matrix.
        K0 is defined such that K0 = -(NIC)\ :sup:`-1`\ * NDC, or equivalently, [NDC NIC][I K0]' = 0 where [NDC NIC] = Nr
        """
        return self._my_getK0Matrix().toNumpy()

    def getKMatrix(self):
        """
        LibStructural.getKMatrix(self)
        :returns: The K matrix (right nullspace of Nr)
        The K matrix has the structure, [I K0]'
        """
        return self._my_getKMatrix().toNumpy()

    def getL0Matrix(self):
        """
        LibStructural.getL0Matrix(self)

        :returns: The L0 Matrix.

        L0 is defined such that  L0*Nr = N0. L0 forms part of the link matrix, L.  N0 is the set of linear dependent rows from the lower portion of the reordered stoichiometry matrix.

        """
        return self._my_getL0Matrix().toNumpy()

    def getLinkMatrix(self):
        """
        LibStructural.getLinkMatrix(self)
        :returns: L, the Link Matrix, left nullspace (aka nullspace of the transpose Nr).

        L will have the structure, [I L0]', such that L*Nr = N
        """
        return self._my_getLinkMatrix().toNumpy()

    def getN0Matrix(self):
        """
        LibStructural.getN0Matrix(self)
        :returns: The N0 Matrix.
        The N0 matrix is the set of linearly dependent rows of N where L0 Nr = N0.
        """
        return self._my_getN0Matrix().toNumpy()

    def getNDCMatrix(self):
        """
        LibStructural.getNDCMatrix(self)
        :returns: The NDC Matrix (the set of linearly dependent columns of Nr).

        """
        return self._my_getNDCMatrix().toNumpy()

    def getNICMatrix(self):
        """
        LibStructural.getNICMatrix(self)
        :returns: The NIC Matrix (the set of linearly independent columns of Nr)
        """
        return self._my_getNICMatrix().toNumpy()

    def getNrMatrix(self):
        """
        LibStructural.getNrMatrix(self)

        :returns: The Nr Matrix.

        The rows of the Nr matrix will be linearly independent.
        """
        return self._my_getNrMatrix().toNumpy()

    def getReorderedStoichiometryMatrix(self):
        """
        LibStructural.getReorderedStoichiometryMatrix(self)

        :returns: The reordered stoichiometry matrix (row reordered stoichiometry matrix, columns are not reordered!)

        """
        return self._my_getReorderedStoichiometryMatrix().toNumpy()

    def loadStoichiometryMatrix(self, data):
            """
            LibStructural.loadStoichiometryMatrix(self, Matrix)

            :param: 2D array stoichiometry matrix

            Loads a stoichiometry matrix into the library.
            To analyze the stoichiometry call one of the following:

            | ``LibStructural.analyzeWithQR``,
            | ``LibStructural.analyzeWithLU``,
            | ``LibStructural.analyzeWithLUandRunTests``,
            | ``LibStructural.analyzeWithFullyPivotedLU``,
            | ``LibStructural.analyzeWithFullyPivotedLUwithTests``,

            """
            import numpy as np

            if (type(data) is list or type(data) is np.ndarray):
                data = np.array(data)
                if data.ndim == 2:
                    rows = len(data)
                    cols = len(data[0])
                    m = DoubleMatrix(rows,cols)
                    for i in range(rows):
                        for j in range (cols):
                            m.set (i, j, float(data[i][j]))
                    return self._my_loadStoichiometryMatrix (m)
                else:
                    raise ValueError("Expecting 2 dimensional list or numpy array")
            else:
                raise ValueError("Expecting list or numpy array")

    def getElementaryModesInteger (self):
      """
      LibStructural.getElementaryModesInteger(self)

      :returns: An array where each column is an elementary mode
      """
      import numpy as np
      elementaryModes =  self._my_getElementaryModesInteger().toNumpy()
      if np.any(elementaryModes):
        return elementaryModes
      else:
        return np.empty(0)


    def rref(self, data, tolerance=1e-6):
      """
      LibStructural.rref(self, matrix)

      Computes the reduced row echelon of the given matrix. Tolerance is set to indicate the smallest number consider to be zero.

      :param: A matrix (numpy)
      :param: Optional: tolerance (float), default is 1E-6
      :returns: A reduced row echelon form of the matrix
      """
      import numpy as np
      if (type(data) is list or type(data) is np.ndarray):
            data = np.array(data)
            if data.ndim == 2:
                rows = len(data)
                cols = len(data[0])
                m = DoubleMatrix(rows,cols)
                for i in range(rows):
                    for j in range (cols):
                        m.set (i, j, float(data[i][j]))
                return self._my_rref (m, tolerance).toNumpy()
            else:
                raise ValueError("Expecting 2 dimensional list or numpy array")
      else:
          raise ValueError("Expecting list or numpy array")

    def rref_FB(self, data, tolerance=1e-6):
          """
          LibStructural.rref_FB(self, matrix)

          :param: A matrix
          :returns: The reduced row echelon form of the matrix.
          """

          import numpy as np

          if (type(data) is list or type(data) is np.ndarray):
              data = np.array(data)
              if data.ndim == 2:
                  rows = len(data)
                  cols = len(data[0])
                  m = DoubleMatrix(rows,cols)
                  for i in range(rows):
                      for j in range (cols):
                          m.set (i, j, float(data[i][j]))
                  return self._my_rref_FB (m, tolerance).toNumpy()
              else:
                  raise ValueError("Expecting 2 dimensional list or numpy array")
          else:
              raise ValueError("Expecting list or numpy array")

    def getEigenValues (self, oMatrix):
            """
            LibStructural.getEigenValues(self, matrix)

            :param: Matrix to find the eigenvalues for.
            :returns: An array, first column are the real values and second column are imaginary values
            """

            import numpy as np

            if (type(oMatrix) is list or type(oMatrix) is np.ndarray):
                oMatrix = np.array(oMatrix)
                if oMatrix.ndim == 2:
                    rows = len(oMatrix)
                    cols = len(oMatrix[0])
                    m = DoubleMatrix(rows,cols)
                    for i in range(rows):
                        for j in range (cols):
                            m.set (i, j, float(oMatrix[i][j]))
                    return self._my_getEigenValues(m).toNumpy()
                else:
                    raise ValueError("Expecting 2 dimensional list or numpy array")
            else:
                raise ValueError("Expecting list or numpy array")

    def getEigenVectors (self, oMatrix):
            """
            LibStructural.getEigenVectors(self, matrix)

            :param: Matrix to find the eigenvectors for
            :returns: An array where each columns is an eigenvector
            """

            import numpy as np

            if (type(oMatrix) is list or type(oMatrix) is np.ndarray):
                oMatrix = np.array(oMatrix)
                if oMatrix.ndim == 2:
                    rows = len(oMatrix)
                    cols = len(oMatrix[0])
                    m = DoubleMatrix(rows,cols)
                    for i in range(rows):
                        for j in range (cols):
                            m.set (i, j, float(oMatrix[i][j]))
                    return self._my_getEigenVectors(m).toNumpy()
                else:
                    raise ValueError("Expecting 2 dimensional list or numpy array")
            else:
                raise ValueError("Expecting list or numpy array")

    def getConditionNumber (self, oMatrix):
            '''
            LibStructural.getConditionNumber(self, matrix)

            :param: Takes a matrix (numpy) as an argument. Find the condition number of the matrix.
            :returns: The condition number
            '''
            import numpy as np

            if (type(oMatrix) is list or type(oMatrix) is np.ndarray):
                oMatrix = np.array(oMatrix)
                if oMatrix.ndim == 2:
                    rows = len(oMatrix)
                    cols = len(oMatrix[0])
                    m = DoubleMatrix(rows,cols)
                    for i in range(rows):
                        for j in range (cols):
                            m.set (i, j, float(oMatrix[i][j]))
                    return self._my_getConditionNumber(m)
                else:
                    raise ValueError("Expecting 2 dimensional list or numpy array")
            else:
                raise ValueError("Expecting list or numpy array")

    def getRConditionNumber (self, oMatrix):
            '''
            LibStructural.getRConditionNumber(self, matrix)

            Find the condition number of a matrix.

            :param: A matrix as an argument.
            :returns: The condition number
            '''

            import numpy as np

            if (type(oMatrix) is list or type(oMatrix) is np.ndarray):
                oMatrix = np.array(oMatrix)
                if oMatrix.ndim == 2:
                    rows = len(oMatrix)
                    cols = len(oMatrix[0])
                    m = DoubleMatrix(rows,cols)
                    for i in range(rows):
                        for j in range (cols):
                            m.set (i, j, float(oMatrix[i][j]))
                    return self._my_getRCond(m)
                else:
                    raise ValueError("Expecting 2 dimensional list or numpy array")
            else:
                raise ValueError("Expecting list or numpy array")

    def getLeftNullSpace (self, oMatrix):
            """
            LibStructural.getLeftNullSpace(self, matrix)

            :param: Matrix to find the left nullspace of.
            :returns: The Left Nullspace of the matrix argument.

            """

            import numpy as np

            if (type(oMatrix) is list or type(oMatrix) is np.ndarray):
                oMatrix = np.array(oMatrix)
                if oMatrix.ndim == 2:
                    rows = len(oMatrix)
                    cols = len(oMatrix[0])
                    m = DoubleMatrix(rows,cols)
                    for i in range(rows):
                        for j in range (cols):
                            m.set (i, j, float(oMatrix[i][j]))
                    return self._my_getLeftNullSpace(m).toNumpy()
                else:
                    raise ValueError("Expecting 2 dimensional list or numpy array")
            else:
                raise ValueError("Expecting list or numpy array")

    def getRightNullSpace (self, oMatrix):
            """
            LibStructural.getRightNullSpace(self, matrix)

            :param: Matrix to find the right nullspace of.
            :returns: The Right Nullspace of the matric argument.
            """

            import numpy as np

            if (type(oMatrix) is list or type(oMatrix) is np.ndarray):
                oMatrix = np.array(oMatrix)
                if oMatrix.ndim == 2:
                    rows = len(oMatrix)
                    cols = len(oMatrix[0])
                    m = DoubleMatrix(rows,cols)
                    for i in range(rows):
                        for j in range (cols):
                            m.set (i, j, float(oMatrix[i][j]))
                    return self._my_getRightNullSpace(m).toNumpy()
                else:
                    raise ValueError("Expecting 2 dimensional list or numpy array")
            else:
                raise ValueError("Expecting list or numpy array")

    def getRank (self, oMatrix):
            """
            LibStructural.getRank(self, matrix)

            :param: Matrix to find the rank of.
            :returns: The rank as an integer.
            """
            import numpy as np

            if (type(oMatrix) is list or type(oMatrix) is np.ndarray):
                oMatrix = np.array(oMatrix)
                if oMatrix.ndim == 2:
                    rows = len(oMatrix)
                    cols = len(oMatrix[0])
                    m = DoubleMatrix(rows,cols)
                    for i in range(rows):
                        for j in range (cols):
                            m.set (i, j, float(oMatrix[i][j]))
                    return self._my_getRank(m)
                else:
                    raise ValueError("Expecting 2 dimensional list or numpy array")
            else:
                raise ValueError("Expecting list or numpy array")


    def runLibstructTests(self):
      """
      LibStructural.runLibstructTests(self)

      :returns: A summary of various a tests.

      """
      import sys
      if sys.version_info[0] < 3:
          import test.testLibStructuralSBML
          test.testLibStructuralSBML.run()
      else:
          from structural.test import testLibStructuralSBML
          testLibStructuralSBML.run()


    def runElementaryModeTests(self):
      """
      LibStructural.runElementaryModeTests(self)

      :returns: An elementary modes tests for 25 models.

      """
      import sys
      if sys.version_info[0] < 3:
          import test.testElementaryModesUsingSBML
          test.testElementaryModesUsingSBML.run()
      else:
          from structural.test import testElementaryModesUsingSBML
          testElementaryModesUsingSBML.run()


    def getElementaryModesDouble(self):
      """
      LibStructural.getElementaryModesDouble(self)

      :returns: Returns in an array where each column is an elementary mode (Generated from MetaTool)

      """
      import numpy as np
      import tempfile
      import subprocess
      import site
      import os
      import pkg_resources
      import platform

      mStr = ''

      rxn_ids = self.getReactionIds()
      flt_ids = self.getFloatingSpeciesIds()
      bnd_ids = self.getBoundarySpeciesIds()
      matx_bnd = self.getStoichiometryMatrixBoundary()
      matx_flt = self.getStoichiometryMatrix()

      if len(bnd_ids) == 0:
        matx = matx_flt
        spec_ids = list(flt_ids)
      else:
        matx = matx_bnd
        spec_ids = list(flt_ids) + list(bnd_ids)


      mStr += "-ENZREV" + "\n"

      for i in range(len(rxn_ids)):
          if self.isReactionReversible(i):
              mStr += rxn_ids[i] + " "

      mStr += "\n\n"+"-ENZIRREV" + "\n"

      for i in range(len(rxn_ids)):
          if not self.isReactionReversible(i):
              mStr += rxn_ids[i] + " "

      mStr += "\n\n"+"-METINT"+"\n"

      for  i in flt_ids:
          mStr += str (i) + " "

      mStr += "\n\n"+"-METEXT"+"\n"

      if len(bnd_ids) != 0:
        for i in bnd_ids:
          mStr += i + " "

      mStr += "\n\n"+"-CAT"+"\n"

      for i in range(len(rxn_ids)):
          react_list = []
          col = matx[:,i]
          mStr += rxn_ids[i] + " : "

          for j in range(len(matx)):
              if matx[j,i] < 0:
                  stStr = ''
                  if abs(matx[j,i]) > 1:
                      stStr = str(abs(matx[j,i])) + ' '
                  react_list.append(stStr + spec_ids[j])
          mStr += react_list[0]
          for k in range(1,len(react_list)):
              mStr += " + " + react_list[k]
          mStr += " = "

          prod_list = []
          for j in range(len(matx)):
              if matx[j,i] > 0:
                  stStr = ''
                  if matx[j,i] > 1:
                      stStr = str(matx[j,i]) + ' '
                  prod_list.append(stStr + spec_ids[j])

          if len(prod_list) != 0:
            mStr += prod_list[0]
            for k in range(1,len(prod_list)):
              mStr += " + " + prod_list[k]

          mStr += " .\n"


      f = tempfile.NamedTemporaryFile(delete=False)
      d = tempfile.gettempdir()

      resultFile = d+"\\MetaToolResult.txt"
      metatoolFile = f.name
      with open(metatoolFile, "w") as f:
        f.write (mStr)
      f.close()

      if platform.system == 'Windows':
          pathToMetatool = pkg_resources.resource_filename('structural', 'metaToolDouble.exe')
      else:
          pathToMetatool = pkg_resources.resource_filename('structural', 'metaToolDouble')

      with open(os.devnull, "w") as f:
          exit_code = subprocess.call ([pathToMetatool, metatoolFile, resultFile], stdout=f)


      if os.path.isfile(resultFile):
        if exit_code == 0:
            line_array = []
            with open(resultFile) as f:
                for lines in f:
                    line_array.append(lines)

            start_pt = line_array.index("ELEMENTARY MODES\n")

            if line_array[start_pt+1] == ' \n':
                row_num = int(line_array[start_pt+2].split()[2].replace("r", ""))

                elementaryModeMatrix = []
                for i in range(row_num):
                    elementaryModeMatrix.append(line_array[start_pt+3+i].split())
                elementaryModeMatrix = np.array(elementaryModeMatrix, dtype=float)
                f.close()
                return elementaryModeMatrix
            else:
                return np.empty([0,0])
        else:
            raise RuntimeError(exitCodeDict[exit_code])
      else:
         raise RuntimeError ("Internal Error: Result file from MetaTool not found")


    def getElementaryModesIntegerRxnIds(self):
      """
      LibStructural.getElementaryModesIntegerRxnIds(self)

      :returns: An array of reaction Ids corresponding with the columns of getElementaryModesInteger() matrix.

      """
      return list(self.getReactionIds())


    def getElementaryModesDoubleRxnIds(self):
      """
      LibStructural.getElementaryModesIntegerRxnIds(self)

      :returns: An array of reaction Ids corresponding with the columns of getElementaryModesDouble() matrix.

      """

      import os
      import tempfile

      rxn_ids = self.getReactionIds()
      rxnId_lst = []

      for i in range(len(rxn_ids)):
          if self.isReactionReversible(i):
              rxnId_lst.append(rxn_ids[i])

      for i in range(len(rxn_ids)):
          if not self.isReactionReversible(i):
              rxnId_lst.append(rxn_ids[i])

      return rxnId_lst

      d = tempfile.gettempdir()
      resultFile = d+"\\MetaToolResult.txt"
      if os.path.isfile(resultFile):
          line_array = []
          with open(resultFile) as f:
              for lines in f:
                  line_array.append(lines)
      f.close()

      line_dict = {}
      for i in range(len(line_array)):
          line_dict[i+1] = line_array[i]

      index_list = [k for k,v in line_dict.items() if v == ' enzymes\n']
      start_pt = index_list[1]-1
      col_num = int(line_array[6].split()[1])

      if line_array[start_pt+1] == ' - not found -\n':
          return []

      if col_num > 0:
          rxnId_lst = []
          for i in range(col_num):
              rxnId = (line_array[start_pt+2+i].split())[1]
              rxnId_lst.append(rxnId)
          return rxnId_lst
      else:
          return []
%}

}

%ignore LIB_STRUCTURAL::LibStructural::findPositiveGammaMatrix;
%ignore LIB_STRUCTURAL::LibStructural::getColumnReorderedNrMatrixIds;
%ignore LIB_STRUCTURAL::LibStructural::getGammaMatrixIds;
%ignore LIB_STRUCTURAL::LibStructural::getK0MatrixIds;
%ignore LIB_STRUCTURAL::LibStructural::getKMatrixIds;
%ignore LIB_STRUCTURAL::LibStructural::getL0MatrixIds;
%ignore LIB_STRUCTURAL::LibStructural::getLinkMatrixIds;
%ignore LIB_STRUCTURAL::LibStructural::getN0MatrixIds;
%ignore LIB_STRUCTURAL::LibStructural::getNDCMatrixIds;
%ignore LIB_STRUCTURAL::LibStructural::getNICMatrixIds;
%ignore LIB_STRUCTURAL::LibStructural::getNrMatrixIds;
%ignore LIB_STRUCTURAL::LibStructural::getStoichiometryMatrixIds;
%ignore LIB_STRUCTURAL::LibStructural::getFullyReorderedStoichiometryMatrixIds;
%ignore LIB_STRUCTURAL::LibStructural::getReorderedStoichiometryMatrixIds;
%ignore LIB_STRUCTURAL::_sResultStream;
%ignore LIB_STRUCTURAL::LibStructural::getResultString;

%include "../include/libstructural.h"
%include "../include/util.h"

%rename(assign) operator=;
%rename(add) operator+;
%rename(sub) operator-;
%rename(div) operator/;
%rename(mult) operator*;
%ignore operator<<;
%ignore operator();
%ignore operator[];

%ignore getReal;
%ignore setReal;
%ignore getImag;
%ignore setImag;



%inline
%{
#include "complex.h"
%}

%array_class(LIB_LA::Complex, complexArray);
%array_class(double, doubleArray);
%array_class(int, intArray);
%array_function(LIB_LA::Complex, complexArray);
%array_function(double, doubleArray);
%array_function(int, intArray);

using LIB_LA::Matrix;

%template(DoubleMatrix) LIB_LA::Matrix<double>;
%template(IntMatrix) LIB_LA::Matrix<int>;
%template(ComplexMatrix) LIB_LA::Matrix<LIB_LA::Complex>;

// #ifdef SWIGCSHARP
// %template(StringDouble) std::pair< std::string, double >;
// %template(StringDoubleVector) std::vector< std::pair< std::string, double > >;
// #endif

%extend LIB_LA::Matrix<double>
{
    virtual double get(const unsigned int row, const unsigned int col)
  {
        return (*self)(row,col);
        //return *(self->_Array + row * self->_Cols + col);
    }

    virtual void set(const unsigned int row, const unsigned int col, double value)
  {
        (*self)(row,col) = value;
    }

%pythoncode %{
        def toNumpy(self):
                import numpy as np
                result = np.zeros((self.numRows(), self.numCols()))
                for i in range(self.numRows()):
                        for j in range(self.numCols()):
                                result[i,j] = self.get(i,j)
                return result
        def __repr__(self):
                return self.toNumpy().__repr__()
%}

}


%extend LIB_LA::Matrix<int>
{
    virtual int get(const unsigned int row, const unsigned int col)
    {
        return (*self)(row,col);
    }

    virtual void set(const unsigned int row, const unsigned int col, int value)
    {
        (*self)(row,col) = value;
    }

%pythoncode %{
        def toNumpy(self):
                import numpy as np
                result = np.zeros((self.numRows(), self.numCols()), dtype=np.int)
                for i in range(self.numRows()):
                        for j in range(self.numCols()):
                                result[i,j] = self.get(i,j)
                return result
        def __repr__(self):
                return self.toNumpy().__repr__()
%}
}

%extend LIB_LA::Matrix<LIB_LA::Complex>
{
    virtual LIB_LA::Complex get(const unsigned int row, const unsigned int col)
    {
        return (*self)(row,col);
    }

    virtual double getReal(const unsigned int row, const unsigned int col)
    {
        return (*self)(row,col).Real;
    }
    virtual double getImag(const unsigned int row, const unsigned int col)
    {
        return (*self)(row,col).Imag;
    }

    virtual void set(const unsigned int row, const unsigned int col, LIB_LA::Complex value)
    {
        (*self)(row,col) = value;
    }

    virtual void setReal(const unsigned int row, const unsigned int col, double value)
    {
        (*self)(row,col).Real = value;
    }
    virtual void setImag(const unsigned int row, const unsigned int col, double value)
    {
        (*self)(row,col).Imag = value;
    }

%pythoncode %{
        def toNumpy(self):
                import numpy as np
                result = np.zeros((self.numRows(), self.numCols()), dtype=np.complex_)
                for i in range(self.numRows()):
                        for j in range(self.numCols()):
                                result[i,j] = self.getReal(i,j) + self.getImag(i,j)*1j
                return result
        def __repr__(self):
                return self.toNumpy().__repr__()
%}
}

// %ignore LIB_STRUCTURAL::LibStructural::getInitialConditions;
// %rename (_getInitialConditions) LIB_STRUCTURAL::LibStructural::getInitialConditions;

/*%extend LIB_STRUCTURAL::LibStructural {
    std::vector< std::pair<std::string, double> > _getInitialConditions() {
        return *self->getInitialConditions();
    }
%pythoncode %{
        def getInitialConditions(*args):
                return _getInitialConditions(*args)
%}
}*/
