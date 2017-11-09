%module structural

// #pragma SWIG nowarn=516

%inline
%{
#include "libstructural.h"
#include "util.h"
#include "libla.h"
%}

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
%rename (_my_getColumnReorderedNrMatrix) getColumnReorderedNrMatrix;
%rename (_my_getFullyReorderedN0StoichiometryMatrix) getFullyReorderedN0StoichiometryMatrix;
%rename (_my_getFullyReorderedNrMatrix) getFullyReorderedNrMatrix;
%rename (_my_getFullyReorderedStoichiometryMatrix) getFullyReorderedStoichiometryMatrix;
%rename (_my_getGammaMatrix) getGammaMatrix;
%rename (_my_getGammaMatrixGJ) getGammaMatrixGJ;
%rename (_my_getK0Matrix) getK0Matrix;
%rename (_my_getKMatrix) getKMatrix;
%rename (_my_getL0Matrix) getL0Matrix;
%rename (_my_getLinkMatrix) getLinkMatrix;
%rename (_my_getN0Matrix) getN0Matrix;
%rename (_my_getNDCMatrix) getNDCMatrix;
%rename (_my_getNICMatrix) getNICMatrix;
%rename (_my_getNrMatrix) getNrMatrix;
%rename (_my_getReorderedStoichiometryMatrix) getReorderedStoichiometryMatrix;

%exception {
	 try {
     $action
   } catch (const LIB_LA::ApplicationException& e) {
     SWIG_exception(SWIG_RuntimeError, "app error");
   } catch (LIB_LA::ApplicationException* e) {
		 std::string msg = e->getDetailedMessage();
		 delete e;
     SWIG_exception(SWIG_RuntimeError, msg.c_str());
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

%template(StringDouble) std::pair<std::string,double>;
%template(StrDoubleVector) std::vector< std::pair<std::string,double> >;
%template(StringVector) std::vector<std::string>;
%template(DoubleVector) std::vector<double>;
%template(StringVectorx2) std::pair< std::vector<std::string>, std::vector<std::string> >;

%include "../include/matrix.h"

%template(DoubleMatrixStringVector) std::pair<LIB_LA::DoubleMatrix*,  std::vector< std::string> >;

// http://swig.10945.n7.nabble.com/replacing-a-real-class-method-with-SWIG-version-td11418.html
%extend LIB_STRUCTURAL::LibStructural {

	std::pair< std::vector<std::string>, std::vector<std::string> > getColumnReorderedNrMatrixLabels() {
		std::vector<std::string> rows;
		std::vector<std::string> cols;
		self->getColumnReorderedNrMatrixLabels(rows, cols);
		return std::make_pair(rows, cols);
	}
	
	void loadReactionNames (std::vector<std::string> reactionNames) {
		self->loadReactionNames(reactionNames);
	}
	
	void loadSpecies (std::vector<std::string> speciesNames, std::vector<double> speciesValues) {
		self->loadSpecies(speciesNames, speciesValues);
	}
	
	
	
	std::pair< std::vector<std::string>, std::vector<std::string> > getGammaMatrixLabels() {
		std::vector<std::string> rows;
		std::vector<std::string> cols;
		self->getGammaMatrixLabels(rows, cols);
		return std::make_pair(rows, cols);
	}

	std::pair< std::vector<std::string>, std::vector<std::string> > getK0MatrixLabels() {
		std::vector<std::string> rows;
		std::vector<std::string> cols;
		self->getK0MatrixLabels(rows, cols);
		return std::make_pair(rows, cols);
	}

	std::pair< std::vector<std::string>, std::vector<std::string> > getKMatrixLabels() {
		std::vector<std::string> rows;
		std::vector<std::string> cols;
		self->getKMatrixLabels(rows, cols);
		return std::make_pair(rows, cols);
	}

	std::pair< std::vector<std::string>, std::vector<std::string> > getL0MatrixLabels() {
		std::vector<std::string> rows;
		std::vector<std::string> cols;
		self->getL0MatrixLabels(rows, cols);
		return std::make_pair(rows, cols);
	}

	std::pair< std::vector<std::string>, std::vector<std::string> > getLinkMatrixLabels() {
		std::vector<std::string> rows;
		std::vector<std::string> cols;
		self->getLinkMatrixLabels(rows, cols);
		return std::make_pair(rows, cols);
	}

	std::pair< std::vector<std::string>, std::vector<std::string> > getLinkMatrixLabels() {
		std::vector<std::string> rows;
		std::vector<std::string> cols;
		self->getLinkMatrixLabels(rows, cols);
		return std::make_pair(rows, cols);
	}

	std::pair< std::vector<std::string>, std::vector<std::string> > getN0MatrixLabels() {
		std::vector<std::string> rows;
		std::vector<std::string> cols;
		self->getN0MatrixLabels(rows, cols);
		return std::make_pair(rows, cols);
	}

	std::pair< std::vector<std::string>, std::vector<std::string> > getNDCMatrixLabels() {
		std::vector<std::string> rows;
		std::vector<std::string> cols;
		self->getNDCMatrixLabels(rows, cols);
		return std::make_pair(rows, cols);
	}

	std::pair< std::vector<std::string>, std::vector<std::string> > getNICMatrixLabels() {
		std::vector<std::string> rows;
		std::vector<std::string> cols;
		self->getNICMatrixLabels(rows, cols);
		return std::make_pair(rows, cols);
	}

	std::pair< std::vector<std::string>, std::vector<std::string> > getNrMatrixLabels() {
		std::vector<std::string> rows;
		std::vector<std::string> cols;
		self->getNrMatrixLabels(rows, cols);
		return std::make_pair(rows, cols);
	}

	std::pair< std::vector<std::string>, std::vector<std::string> > getStoichiometryMatrixLabels() {
		std::vector<std::string> rows;
		std::vector<std::string> cols;
		self->getStoichiometryMatrixLabels(rows, cols);
		return std::make_pair(rows, cols);
	}

	std::pair< std::vector<std::string>, std::vector<std::string> > getFullyReorderedStoichiometryMatrixLabels() {
		std::vector<std::string> rows;
		std::vector<std::string> cols;
		self->getFullyReorderedStoichiometryMatrixLabels(rows, cols);
		return std::make_pair(rows, cols);
	}

	std::pair< std::vector<std::string>, std::vector<std::string> > getReorderedStoichiometryMatrixLabels() {
		std::vector<std::string> rows;
		std::vector<std::string> cols;
		self->getReorderedStoichiometryMatrixLabels(rows, cols);
		return std::make_pair(rows, cols);
	}

%pythoncode %{
	def getStoichiometryMatrix(self):
		return self._my_getStoichiometryMatrix().toNumpy()
		
	def getColumnReorderedNrMatrix(self):
		return self._my_getColumnReorderedNrMatrix().toNumpy()
		
	def getFullyReorderedN0StoichiometryMatrix(self):
		return self._my_getFullyReorderedN0StoichiometryMatrix().toNumpy()
	
	def getFullyReorderedNrMatrix(self):
		return self._my_getFullyReorderedNrMatrix().toNumpy()
		
	def getFullyReorderedStoichiometryMatrix(self):
		return self._my_getFullyReorderedStoichiometryMatrix().toNumpy()
		
	def getGammaMatrix(self):
		return self._my_getGammaMatrix().toNumpy()
		
	def getGammaMatrixGJ(self,oMatrix):
		import numpy as np

		if (type(oMatrix) is list or type(oMatrix) is np.ndarray):
			oMatrix = np.array(oMatrix)
			if oMatrix.ndim == 2:
				rows = len(oMatrix)
				cols = len(oMatrix[0])
				m = DoubleMatrix(rows,cols)
				for i in range(rows):
					for j in range (cols):
						m.set (i, j, oMatrix[i][j])
				return self._my_getGammaMatrixGJ(m).toNumpy()
			else:
				raise ValueError("Expecting 2 dimensional list or numpy array")
		else:
			raise ValueError("Expecting list or numpy array")
		
		
	def getK0Matrix(self):
		return self._my_getK0Matrix().toNumpy()
		
	def getKMatrix(self):
		return self._my_getKMatrix().toNumpy()
	
	def getL0Matrix(self):
		return self._my_getL0Matrix().toNumpy()
		
	def getLinkMatrix(self):
		return self._my_getLinkMatrix().toNumpy()
		
	def getN0Matrix(self):
		return self._my_getN0Matrix().toNumpy()
		
	def getNDCMatrix(self):
		return self._my_getNDCMatrix().toNumpy()
	
	def getNICMatrix(self):
		return self._my_getNICMatrix().toNumpy()
		
	def getNrMatrix(self):
		return self._my_getNrMatrix().toNumpy()
		
	def getReorderedStoichiometryMatrix(self):
		return self._my_getReorderedStoichiometryMatrix().toNumpy()
%}
	
%pythoncode %{
	def loadStoichiometryMatrix(self, data):
			import numpy as np

			if (type(data) is list or type(data) is np.ndarray):
				data = np.array(data)
				if data.ndim == 2:
					rows = len(data)
					cols = len(data[0])
					m = DoubleMatrix(rows,cols)
					for i in range(rows):
						for j in range (cols):
							m.set (i, j, data[i][j])
					return self._my_loadStoichiometryMatrix (m)
				else:
					raise ValueError("Expecting 2 dimensional list or numpy array")
			else:
  		        raise ValueError("Expecting list or numpy array")
%}

%pythoncode %{
	def rref(self, data, tolerance=1e-6):
			import numpy as np

			if (type(data) is list or type(data) is np.ndarray):
				data = np.array(data)
				if data.ndim == 2:
					rows = len(data)
					cols = len(data[0])
					m = DoubleMatrix(rows,cols)
					for i in range(rows):
						for j in range (cols):
							m.set (i, j, data[i][j])
					return self._my_rref (m, tolerance).toNumpy()
				else:
					raise ValueError("Expecting 2 dimensional list or numpy array")
			else:
  		        raise ValueError("Expecting list or numpy array")
%}
 
%pythoncode %{
	def rref_FB(self, data, tolerance=1e-6):
			import numpy as np

			if (type(data) is list or type(data) is np.ndarray):
				data = np.array(data)
				if data.ndim == 2:
					rows = len(data)
					cols = len(data[0])
					m = DoubleMatrix(rows,cols)
					for i in range(rows):
						for j in range (cols):
							m.set (i, j, data[i][j])
					return self._my_rref_FB (m, tolerance).toNumpy()
				else:
					raise ValueError("Expecting 2 dimensional list or numpy array")
			else:
  		        raise ValueError("Expecting list or numpy array")
%}

%pythoncode %{
	def getEigenValues (self, oMatrix):
			import numpy as np

			if (type(oMatrix) is list or type(oMatrix) is np.ndarray):
				oMatrix = np.array(oMatrix)
				if oMatrix.ndim == 2:
					rows = len(oMatrix)
					cols = len(oMatrix[0])
					m = DoubleMatrix(rows,cols)
					for i in range(rows):
						for j in range (cols):
							m.set (i, j, oMatrix[i][j])
					return self._my_getEigenValues(m).toNumpy()
				else:
					raise ValueError("Expecting 2 dimensional list or numpy array")
			else:
  		        raise ValueError("Expecting list or numpy array")
%}

%pythoncode %{
	def getEigenVectors (self, oMatrix):
			import numpy as np

			if (type(oMatrix) is list or type(oMatrix) is np.ndarray):
				oMatrix = np.array(oMatrix)
				if oMatrix.ndim == 2:
					rows = len(oMatrix)
					cols = len(oMatrix[0])
					m = DoubleMatrix(rows,cols)
					for i in range(rows):
						for j in range (cols):
							m.set (i, j, oMatrix[i][j])
					return self._my_getEigenVectors(m).toNumpy()
				else:
					raise ValueError("Expecting 2 dimensional list or numpy array")
			else:
  		        raise ValueError("Expecting list or numpy array")
%}

%pythoncode %{
	def getConditionNumber (self, oMatrix):
			import numpy as np

			if (type(oMatrix) is list or type(oMatrix) is np.ndarray):
				oMatrix = np.array(oMatrix)
				if oMatrix.ndim == 2:
					rows = len(oMatrix)
					cols = len(oMatrix[0])
					m = DoubleMatrix(rows,cols)
					for i in range(rows):
						for j in range (cols):
							m.set (i, j, oMatrix[i][j])
					return self._my_getConditionNumber(m)
				else:
					raise ValueError("Expecting 2 dimensional list or numpy array")
			else:
  		        raise ValueError("Expecting list or numpy array")
%}

%pythoncode %{
	def getRConditionNumber (self, oMatrix):
			import numpy as np

			if (type(oMatrix) is list or type(oMatrix) is np.ndarray):
				oMatrix = np.array(oMatrix)
				if oMatrix.ndim == 2:
					rows = len(oMatrix)
					cols = len(oMatrix[0])
					m = DoubleMatrix(rows,cols)
					for i in range(rows):
						for j in range (cols):
							m.set (i, j, oMatrix[i][j])
					return self.getRCond(m)
				else:
					raise ValueError("Expecting 2 dimensional list or numpy array")
			else:
  		        raise ValueError("Expecting list or numpy array")
%}


%pythoncode %{
	def getLeftNullSpace (self, oMatrix):
			import numpy as np

			if (type(oMatrix) is list or type(oMatrix) is np.ndarray):
				oMatrix = np.array(oMatrix)
				if oMatrix.ndim == 2:
					rows = len(oMatrix)
					cols = len(oMatrix[0])
					m = DoubleMatrix(rows,cols)
					for i in range(rows):
						for j in range (cols):
							m.set (i, j, oMatrix[i][j])
					return self._my_getLeftNullSpace(m).toNumpy()
				else:
					raise ValueError("Expecting 2 dimensional list or numpy array")
			else:
  		        raise ValueError("Expecting list or numpy array")
%}

%pythoncode %{
	def getRightNullSpace (self, oMatrix):
			import numpy as np

			if (type(oMatrix) is list or type(oMatrix) is np.ndarray):
				oMatrix = np.array(oMatrix)
				if oMatrix.ndim == 2:
					rows = len(oMatrix)
					cols = len(oMatrix[0])
					m = DoubleMatrix(rows,cols)
					for i in range(rows):
						for j in range (cols):
							m.set (i, j, oMatrix[i][j])
					return self._my_getRightNullSpace(m).toNumpy()
				else:
					raise ValueError("Expecting 2 dimensional list or numpy array")
			else:
  		        raise ValueError("Expecting list or numpy array")
%}

%pythoncode %{
	def getRank (self, oMatrix):
			import numpy as np

			if (type(oMatrix) is list or type(oMatrix) is np.ndarray):
				oMatrix = np.array(oMatrix)
				if oMatrix.ndim == 2:
					rows = len(oMatrix)
					cols = len(oMatrix[0])
					m = DoubleMatrix(rows,cols)
					for i in range(rows):
						for j in range (cols):
							m.set (i, j, oMatrix[i][j])
					return self._my_getRank(m)
				else:
					raise ValueError("Expecting 2 dimensional list or numpy array")
			else:
  		        raise ValueError("Expecting list or numpy array")
%}

}

%ignore LIB_STRUCTURAL::LibStructural::findPositiveGammaMatrix;
%ignore LIB_STRUCTURAL::LibStructural::getColumnReorderedNrMatrixLabels;
%ignore LIB_STRUCTURAL::LibStructural::getGammaMatrixLabels;
%ignore LIB_STRUCTURAL::LibStructural::getK0MatrixLabels;
%ignore LIB_STRUCTURAL::LibStructural::getKMatrixLabels;
%ignore LIB_STRUCTURAL::LibStructural::getL0MatrixLabels;
%ignore LIB_STRUCTURAL::LibStructural::getLinkMatrixLabels;
%ignore LIB_STRUCTURAL::LibStructural::getN0MatrixLabels;
%ignore LIB_STRUCTURAL::LibStructural::getNDCMatrixLabels;
%ignore LIB_STRUCTURAL::LibStructural::getNICMatrixLabels;
%ignore LIB_STRUCTURAL::LibStructural::getNrMatrixLabels;
%ignore LIB_STRUCTURAL::LibStructural::getStoichiometryMatrixLabels;
%ignore LIB_STRUCTURAL::LibStructural::getFullyReorderedStoichiometryMatrixLabels;
%ignore LIB_STRUCTURAL::LibStructural::getReorderedStoichiometryMatrixLabels;



%include "../include/libstructural.h"

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
#include "..\include\complex.h"
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
		return *(self->_Array + row * self->_Cols + col);
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
