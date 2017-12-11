///
/// This file contains several useful utility functions
///
///

#ifndef SBW_CLAPACK_UTIL_H
#define SBW_CLAPACK_UTIL_H

#include "libla.h"
#include "libutil.h"

#define DELETE_2D_ARRAY(oArray, nLength)\
	{\
		for (int i=0; i<nLength; i++) {\
			delete[] oArray[i];\
		}\
		delete[] oArray;\
		oArray = NULL;\
	}

#define FREE_2D_ARRAY(oArray, nLength)\
	{\
		for (int i=0; i<nLength; i++) {\
			free(oArray[i]);\
		}\
		free(oArray);\
		oArray = NULL;\
	}

#define CREATE_ARRAY(variableName,type,length)\
	if(variableName) { delete[] variableName; variableName = NULL;}\
	variableName = new type[length]; memset(variableName, 0, sizeof(type)*length);//

#define DELETE_ARRAY_IF_NON_NULL(target)\
	if(target) { delete[] target; target = NULL;}
#define DELETE_IF_NON_NULL(target)\
	if(target) { delete target; target = NULL;}


#include <string>

//#include "f2c.h"

namespace LIB_LA
{

	class ApplicationException
	{

	private:
		std::string _Message;
		std::string _DetailedMessage;

	public: 
		LIB_EXTERN ApplicationException() : _Message(""), _DetailedMessage("") {}
		LIB_EXTERN ApplicationException(const std::string &message) : _Message(message), _DetailedMessage("") {}
		LIB_EXTERN ApplicationException(const std::string &message, const std::string &detailedMessage) : 
		_Message(message), _DetailedMessage(detailedMessage) {}

		LIB_EXTERN std::string getMessage() { return _Message; }
		LIB_EXTERN std::string getDetailedMessage() { return _DetailedMessage; } 

	};
	

	class Util
	{
	public:

		LIB_EXTERN static int** matMult(int mA, int nA, int **A, int **B, int nB);
		LIB_EXTERN static double** matMult(int mA, int nA, double **A, double **B, int nB);
		LIB_EXTERN static void checkTolerance(int nrows, double *A, double dTolerance);
		LIB_EXTERN static void checkTolerance(int nrows, int ncols, double **A, double dTolerance);
		LIB_EXTERN static DoubleMatrix* getSubMatrix(int Mb, int Nb, int ms, int ns, int mi, int nj, DoubleMatrix& A);
		LIB_EXTERN static DoubleMatrix* matMult(unsigned int mA, unsigned int nA, DoubleMatrix &A, DoubleMatrix &B, unsigned int nB);
		LIB_EXTERN static DoubleMatrix* matMult(DoubleMatrix &A, DoubleMatrix &B);
		LIB_EXTERN static DoubleMatrix* matMult(IntMatrix &A, DoubleMatrix &B);
		LIB_EXTERN static IntMatrix* matMult(IntMatrix &A, IntMatrix &B);		

		LIB_EXTERN static bool isPositive(DoubleMatrix &A, double dTolerance);

		LIB_EXTERN static void CopyMatrix(DoubleMatrix& oMatrix, double** &outMatrix, int &numRows, int &numCols);
		LIB_EXTERN static void CopyMatrix(IntMatrix& oMatrix, int** &outMatrix, int &outNumRows, int &outNumCols);
		LIB_EXTERN static void CopyMatrix(ComplexMatrix& oMatrix, double** &outMatrixReal,double** &outMatrixImag, int &outNumRows, int &outNumCols);
		LIB_EXTERN static void CopyStringVector(const std::vector< std::string > &vector, char** &outVector, int &outLength);
		LIB_EXTERN static void CopyDoubleVector(const std::vector< double > &vector, double* &outVector, int &outLength);
		LIB_EXTERN static void CopyIntVector(const std::vector< int > &vector, int* &outVector, int &outLength);
		LIB_EXTERN static void CopyComplexVector(const std::vector< Complex> &vector, double* &outVectorReal, double* &outVectorImag, int &outLength);

		LIB_EXTERN static void RoundMatrixToTolerance(DoubleMatrix& oMatrix, double dTolerance);
		LIB_EXTERN static double RoundToTolerance(double dValue, double dTolerance);

		LIB_EXTERN static std::vector<int> GaussJordan(DoubleMatrix &oMatrix, double dTolerance);
		LIB_EXTERN static void FullyPivotedGaussJordan(DoubleMatrix &oMatrix, double dTolerance, std::vector< int > &rowPivots, std::vector< int > &colPivots);

		LIB_EXTERN static void gaussJordan(DoubleMatrix &oMatrix, double dTolerance);
		LIB_EXTERN static int findRank(DoubleMatrix &oMatrix, double dTolerance);

		LIB_EXTERN static void print(int mr, int nc, int *A);
		//static void print(int mr, int nc, integer *A);
		LIB_EXTERN static void print(int mr, int nc, int **A);
		LIB_EXTERN static void print(int mr, int nc, double *A);
		LIB_EXTERN static void print(int mr, int nc, double **A);
		LIB_EXTERN static void print(IntMatrix& A);
		LIB_EXTERN static void print(DoubleMatrix& A);
		LIB_EXTERN static void print(ComplexMatrix& A);
		//static void print(int mr, int nc, doublecomplex *A);
		//static void print(int mr, int nc, doublecomplex **A);
		LIB_EXTERN static void print(int mr, int nc, LIB_LA::Complex *A);
		LIB_EXTERN static void print(int mr, int nc, LIB_LA::Complex **A);
		LIB_EXTERN static void print(int mr, int nc, int *A, int *B);
		LIB_EXTERN static void print(int mr, int nc, int **A, int **B);
		LIB_EXTERN static void print(int mr, int nc, double **A, double **B);
	};
}

#endif
