/*!  \file		libla.h
\brief		All definitions needed for the LibLA (Linear Algebra) library

\par 
The current scope of the library encompasses matrix factorizations (QR and LU 
factorization) as well as commonly needed matrix operations, such as calculating
the inverse of a matrix, computing eigen values and singular values as well as 
the null space of a matrix (both left and right null space) along with a method 
for the computation of the row echelon or Gauss Jordan form of a matrix.

\author     Frank T. Bergmann (fbergman@u.washington.edu)
\author	 Herbert M. Sauro
\author	 Ravishankar Rao Vallabhajosyula (developed a previous version of the sructural analysis code)		

*/
#ifndef LIB_LA_LIBCLAPACK_H
#define LIB_LA_LIBCLAPACK_H

#include "libutil.h"

#ifdef __cplusplus

#include <vector>
#include "matrix.h"
#include "complex.h"


/*!	\namespace LIB_LA
\brief	   The LIB_LA namespace contains all functions and classes enabling the Linear Algebra functions.

The namespace consists mainly of three classes LIB_LA::Complex, a straight forward implementation of a complex type, 
LIB_LA::Matrix a template matrix class used by the C++ API and of course LIB_LA:LibLA the entry point of the 
LA library which encapsulates all functionality. 
*/
namespace LIB_LA
{	

	/*! \class LIB_LA::LU_Result
	\brief LUResult is intended to hold the return values of the Clapack LU decomposition methods

	This class will hold the result from the methods LIB_LA::LibLA::getLU and 
	LIB_LA::LibLA::getLUwithFullPivoting.
	*/
	class LU_Result
	{
	public:
		//! Constructor of a new result object (all result variables are NULL)
		LU_Result() : L(NULL), U(NULL), P(NULL),Q(NULL) {};
		//! Destructor deletes all non-NULL matrices
		~LU_Result() 
		{ 
			if (L) delete L; L = NULL; 
			if (U) delete U; U = NULL;
			if (P) delete P; P = NULL;
			if (Q) delete Q; Q = NULL;
		}

		//! \brief L is a lower triangular matrix 
		DoubleMatrix* L;
		//! \brief U is an upper triangular matrix 
		DoubleMatrix* U;
		//! \brief P is a permutation matrix representing row permutations 
		IntMatrix* P;
		/*! \brief Q is a permutation matrix representing column permutations.
		\remarks and is only available after a call to LIB_LA::LibLA::getLUwithFullPivoting and NULL otherwise 
		*/
		IntMatrix* Q;
		/*! \brief  Info represents status information about the LU factorization its value is to be interpreted as:

		\li 0: successful exit
		\li < 0: if INFO = -i, the i-th argument had an illegal value
		\li > 0: if INFO = i, U(i,i) is exactly zero. The factorization has been completed, 
		but the factor U is exactly singular, and division by zero will occur if it is used to solve a system of equations.

		*/
		int nInfo;

	};


	/*! \brief The LIB_LA::LibLA class represents the entry point to a variety of useful functionality operating on double and complex matrices.

	The current scope of the library encompasses matrix factorizations (QR and LU factorization) 
	as well as commonly needed matrix operations, such as calculating the inverse of a matrix, 
	computing eigen values and singular values as well as the null space of a matrix 
	(both left and right null space) along with a method for the computation of the row echelon or 
	Gauss Jordan form of a matrix.

	*/
	class LibLA
	{
	public:

		/*! \brief Constructor of a new instance of LIB_LA::LibLA with a default tolerance of 1E-12

		See also LIB_LA::LibLA::setTolerance
		*/
		LIB_EXTERN LibLA() : _Tolerance(1.0E-12) {}
		//! Provides access to a singleton of this class
		LIB_EXTERN  static LibLA* getInstance();

		/*! \brief Returns the currently used tolerance

		This function returns the tolerance currently used by the library to determine what value 
		is considered as zero. Any value with absolute value smaller than this tolerance is considered zero 
		and will be neglected. 
		*/
		LIB_EXTERN double getTolerance() 
		{ return _Tolerance; }
		/*! \brief Set user specified tolerance

		This function sets the tolerance used by the library to determine what value 
		is considered as zero. Any value with absolute value smaller than this tolerance is considered as zero 
		and will be neglected. 

		\param dTolerance Sets the tolerance used by the library to determine a 
		value close to zero
		*/
		LIB_EXTERN void setTolerance(double dTolerance) 
		{ _Tolerance = dTolerance; }

		/*! \brief Calculates the eigen-values of a square real matrix.

		This function calculates the complex eigenvalues of the given real matrix. The complex vector
		of eigenvalues will be returned in two real vectors, one for the real and one for the imaginary part.

		\param oMatrix a real matrix
		\return a vector of LIB_LA::Complex numbers representing the eigen-values of the matrix
		*/
		LIB_EXTERN std::vector< Complex > getEigenValues(DoubleMatrix &oMatrix);

		/*! \brief Calculates the eigen-vectors of a square real matrix. 

		This function calculates the complex (right)eigenvectors of the given real matrix. The complex matrix 
		returned contains the eigenvectors in the columns, in the same order as LIB_LA::LibLA::getEigenValues.

		The right eigenvector v(j) of A satisfies:
		\par
		A * v(j) = lambda(j) * v(j)

		\param oMatrix a real matrix
		\return a matrix of LIB_LA::Complex numbers representing the eigen-vectors of the matrix
		
		*/
		LIB_EXTERN ComplexMatrix *getEigenVectors(DoubleMatrix &oMatrix);
		/*! \brief Calculates the eigen-vectors of a square nonsymmetrix complex matrix. 

		This function calculates the complex (right)eigenvectors of the given real matrix. The complex matrix 
		returned contains the eigenvectors in the columns, in the same order as LIB_LA::LibLA::ZgetEigenValues.
		The right eigenvector v(j) of A satisfies:
		\par
		A * v(j) = lambda(j) * v(j)

		\param oMatrix a complex matrix
		\return a matrix of LIB_LA::Complex numbers representing the eigen-vectors of the matrix
		
		*/
		LIB_EXTERN ComplexMatrix *ZgetEigenVectors(ComplexMatrix &oMatrix);
		/*! \brief Factorizes the given matrix using SVD

		This function computes the singular value decomposition (SVD) of the given real matrix. 


		The SVD is written

		\par
		A = U * SIGMA * transpose(V)

		\remarks this function returns the transpose of V

		\param inputMatrix the real matrix to be decomposed (input) (With size M by N)
		\param outU (output) will be initialized with the M by M orthogonal matrix U
		\param outSingularVals (output) will be initialized with the min(M,N) singular values, returned in descending order
		\param outV (output) will be initialized with the N by N orthogonal matrix V
		*/
		LIB_EXTERN void getSVD(DoubleMatrix &inputMatrix, DoubleMatrix* &outU, std::vector<double>* &outSingularVals, DoubleMatrix* &outV);
		/*! \brief Factorizes the given matrix using SVD

		This function computes the singular value decomposition (SVD) of the given complex matrix. 


		The SVD is written

		\par
		A = U * SIGMA * conjugate-transpose(V)

		\remarks this function returns the conjugate-transpose of V

		\param inputMatrix the complex matrix to be decomposed (input) (With size M by N)
		\param outU (output) will be initialized with the M by M orthogonal matrix U
		\param outSingularVals (output) will be initialized with the min(M,N) singular values, returned in descending order
		\param outV (output) will be initialized with the N by N orthogonal matrix V
		*/
		LIB_EXTERN void ZgetSVD(ComplexMatrix &inputMatrix, ComplexMatrix* &outU, std::vector<double>* &outSingularVals, ComplexMatrix* &outV);


		/*! \brief Calculates the eigen-values of a square complex matrix.

		This function calculates the complex eigenvalues of the given complex matrix. The input matrix
		should be broken up into two matrices representing the real and imaginary parts respectively. 


		\param oMatrix a complex  matrix
		\return a vector of LIB_LA::Complex numbers representing the eigen-values of the matrix
		*/
		LIB_EXTERN std::vector< Complex > ZgetEigenValues(ComplexMatrix &oMatrix);

		/*!	\brief 	This function computes the QR factorization of the given real M-by-N matrix A with column pivoting. 

		The LAPACK method dgeqp3 is used followed by an orthonormalization of Q through the use of DORGQR.

		The factorized form is:

		\par
		A = Q * R


		\return This call yields a vector of matrices. These matrices are (in order): 
		\li Q an orthogonal matrix.
		\li R an upper triangular matrix 
		\li P a permutation matrix,

		\remarks free all matrices using 'delete'.
		*/
		LIB_EXTERN std::vector< DoubleMatrix* > getQRWithPivot(DoubleMatrix &oMatrix);
		/*!	\brief 	This function computes the QR factorization of the given real M-by-N matrix A. 

		The LAPACK method dgeqp3 is used followed by an orthonormalization of Q through the use of DORGQR.

		The factorized form is:

		\par
		A = Q * R

		In order to also perform column pivoting use LIB_LA::LibLA::getQRWithPivot

		\return This call yields a vector of matrices. These matrices are (in order): 
		\li Q an orthogonal matrix.
		\li R an upper triangular matrix 

		\remarks free all matrices using 'delete'.
		*/
		LIB_EXTERN std::vector< DoubleMatrix* > getQR(DoubleMatrix &oMatrix);
		/*! \brief 	This method performs the Singular Value Decomposition of the given real matrix, returning only the singular values. 

		This procedure is carried out by the LAPACK method dgesdd.


		\param oMatrix a real matrix
		\return a vector of (real) singular values
		*/
		LIB_EXTERN std::vector< double > getSingularValsBySVD(DoubleMatrix &oMatrix);
		/*! \brief This method computes the rank of the given matrix. 

		The singular values of the matrix are calculated and the rank is determined by the number of non-zero values.

		Note that zero here is defined as any value whose absolute value is bigger than the set tolerance (see
		LIB_LA::LibLA::setTolerance )

		\param oMatrix a real matrix
		\return the rank of the matrix
		*/
		LIB_EXTERN int getRank(DoubleMatrix &oMatrix);


	
		/*! \brief  reciprocal condition number estimate

		 returns an estimate for the reciprocal of the condition of A in 1-norm using the LAPACK condition estimator. 
		 If A is well conditioned, getRCond(A) is near 1.0. If A is badly conditioned, getRCond(A) is near 0.0.

		*/
		LIB_EXTERN double getRCond(DoubleMatrix &oMatrix);


		/*! \brief This method calculates the Gauss Jordan or row echelon form of the given matrix. 

		Only row swaps are used. These permutations will be returned in the 'pivots' vector.

		If no permutations have occurred this vector will be in ascending form [ 0, 1, 2, 3 ]; 
		However if say row one and three would be swapped this vector would look like: [ 0, 3, 2, 1 ];

		\return the pivots vector

		*/
		LIB_EXTERN std::vector<int> gaussJordan(DoubleMatrix &oMatrix);

		/*! \brief This method calculates the fully pivoted Gauss Jordan form of the given matrix. 

		Fully pivoted means, that rows as well as column swaps will be used. These permutations 
		are captured in the integer vectors rowPivots and colPivots.

		If no permutations have occurred those vectors will be in ascending form [ 0, 1, 2, 3 ]; 
		However if say row one and three would be swapped this vector would look like: [ 0, 3, 2, 1 ];
		*/
		LIB_EXTERN void fullyPivotedGaussJordan(DoubleMatrix &oMatrix, std::vector<int> &rowPivots, std::vector<int> &colPivots);


		/*! \brief This function calculates the left null space of a given real matrix. 

		That is:
		\par 
		null(A)*A = 0

		\remarks This function is equivalent to returning the right null space of the transposed matrix. 
		See LIB_LA::LibLA::getRightNullSpace. It should also be noted that the values are 
		unscaled yielding rational numbers. For a scaled version see LIB_LA::LibLA::getScaledLeftNullSpace

		\param oMatrix a real matrix
		\return a matrix representing the left null space
		*/
		LIB_EXTERN DoubleMatrix* getLeftNullSpace(DoubleMatrix &oMatrix);
		/*! \brief This function calculates the scaled left null space of a given real matrix. 

		This function is equivalent to calling LIB_LA::LibLA::getLeftNullSpace however the resulting
		matrix will be scaled (employing Gauss Jordan factorization) to yield whole numbered
		entries wherever possible. 

		\param oMatrix a real matrix
		\return a matrix representing the scaled left null space
		*/
		LIB_EXTERN DoubleMatrix* getScaledLeftNullSpace(DoubleMatrix &oMatrix);
		/*! \brief This function calculates the right null space of a given real matrix. 

		That is:

		\par 
		A*null(A) = 0

		In order to calculate the (right) null space, we first calculate the full singular value decomposition (employing dgesdd) of the matrix:

		\par
		[U,S,V] = svd(A');

		then calculate the rank:

		\par
		r = rank(A)

		and finally return the last columns of the U matrix (r+1...n) as the null space matrix.

		\remarks It should also be noted that the values are unscaled yielding rational numbers. 
		For a scaled version see LIB_LA::LibLA::getScaledRightNullSpace

		\param oMatrix a real matrix
		\return a matrix representing the right null space

		*/
		LIB_EXTERN DoubleMatrix* getRightNullSpace(DoubleMatrix &oMatrix);
		/*! \brief This function calculates the scaled right null space of a given real matrix. 

		This function is equivalent to calling LIB_LA::LibLA::getRightNullSpace however the resulting
		matrix will be scaled (employing Gauss Jordan factorization) to yield whole numbered
		entries wherever possible. 

		\param oMatrix a real matrix
		\return a matrix representing the scaled right null space
		*/
		LIB_EXTERN DoubleMatrix* getScaledRightNullSpace(DoubleMatrix &oMatrix);
		/*! \brief This function computes the LU factorization of the given real M-by-N matrix A 

		using partial pivoting with row interchanges. This procedure is carried out by the LAPACK method dgetrf .
		A is factorized into:

		\par
		A = P * L * U

		Here P is the row permutation matrix.

		\param oMatrix a real matrix
		\return a LIB_LA::LU_Result object with empty LIB_LA::LU_Result#Q matrix

		\remarks The LU factorization is unstable, please check the status flag: 
		LIB_LA::LU_Result#nInfo
		*/
		LIB_EXTERN LU_Result* getLU(DoubleMatrix &oMatrix);
		/*! \brief This function computes the LU factorization of the given real N-by-N matrix A using complete pivoting (with row and column interchanges). 

		This procedure is carried out by the LAPACK method dgetc2.\n
		A is factorized into:

		\par
		A = P * L * U * Q

		Here P and Q are permutation matrices for the rows and columns respectively.

		\remarks This function supports only square matrices (N-by-N), choose ::LibLA_getQRWithPivot for a stable method
		operating on N-by-M matrices.
		\param oMatrix a real matrix
		\return a LIB_LA::LU_Result object 
		\remarks The LU factorization is unstable, please check the status flag: 
		LIB_LA::LU_Result#nInfo
		*/
		LIB_EXTERN LU_Result* getLUwithFullPivoting(DoubleMatrix &oMatrix);
		/*! \brief This function calculates the inverse of a square real matrix. 

		This procedure is carried out by the LAPACK methods dgetrf and dgetri. This means that the matrix will be 
		factorized using LU decomposition first, followed by the calculation of the inverse based on:

		\par 
		inv(A)*L = inv(U) for inv(A).
		\param oMatrix a real matrix
		\return the inverse of the real matrix

		*/
		LIB_EXTERN DoubleMatrix* inverse(DoubleMatrix &oMatrix);
		/*! \brief This function calculates the inverse of a square complex matrix. 

		This procedure is carried out by the LAPACK methods: zgetrf and zgetri. This means that the matrix will be 
		factorized using LU decomposition first, followed by the calculation of the inverse based on:

		\par 
		inv(A)*L = inv(U) for inv(A).
		\param oMatrix a complex matrix
		\return the inverse of the complex matrix
		*/
		LIB_EXTERN ComplexMatrix* Zinverse (ComplexMatrix & oMatrix);

	private:
		double _Tolerance;
		static LibLA* _Instance;
	};

}

#endif // __cplusplus

BEGIN_C_DECLS;

/*! \brief Returns the currently used tolerance

This function returns the tolerance currently used by the library to determine what value 
is considered as zero. Any value with absolute value smaller than this tolerance is considered zero 
and will be neglected. 
*/
LIB_EXTERN double LibLA_getTolerance();

/*! \brief Set user specified tolerance

This function sets the tolerance used by the library to determine what value 
is considered as zero. Any value with absolute value smaller than this tolerance is considered as zero 
and will be neglected. 

\param value Sets the tolerance used by the library to determine a  value close to zero
*/
LIB_EXTERN void LibLA_setTolerance(const double value);

/*! \brief Calculates the eigen-values of a square real matrix.

This function calculates the complex eigenvalues of the given real matrix. The complex vector
of eigenvalues will be returned in two real vectors, one for the real and one for the imaginary part.

\param inMatrix real matrix to calculate the eigen-values for. 
\param numRows the number of rows of the input matrix
\param numCols the number of columns of the input matrix
\param outReal pointer to the output array for the eigenvalues (real part)
\param outImag pointer to the output array for the eigenvalues (imaginary part)
\param outLength the number of eigenvalues written to outReal and outImag

\return The return value will be zero (0) when successful, and negative (-1) in case
an error occurred (for example because the caller supplied a non-square matrix)

\remarks free outReal and outImag using ::LibLA_freeVector

*/
LIB_EXTERN int LibLA_getEigenValues(double** inMatrix, int numRows, int numCols, double* *outReal, double * *outImag, int *outLength);

/*! \brief Calculates the eigen-values of a square complex matrix.

This function calculates the complex eigenvalues of the given complex matrix. The input matrix
should be broken up into two matrices representing the real and imaginary parts respectively. 
The complex vector of eigenvalues will be returned in two  real vectors, one for the real and 
one for the imaginary part.

\param inMatrixReal real part of the complex matrix to calculate the eigen-values for. 
\param inMatrixImag imaginary part of the complex matrix to calculate the eigen-values for
\param numRows the number of rows of the input matrix
\param numCols the number of columns of the input matrix
\param outReal pointer to the output array for the eigenvalues (real part)
\param outImag pointer to the output array for the eigenvalues (imaginary part)
\param outLength the number of eigenvalues written to outReal and outImag

\return The return value will be zero (0) when successful, and negative (-1) in case
an error occurred (for example non square matrix)

\remarks free outReal and outImag using ::LibLA_freeVector

*/
LIB_EXTERN int LibLA_ZgetEigenValues(double** inMatrixReal, double** inMatrixImag, int numRows, int numCols, double* *outReal, double * *outImag, int *outLength);

/*!	\brief This function computes the QR factorization of the given real M-by-N matrix A with column pivoting.

The LAPACK method dgeqp3 is used followed by an orthonormalization of Q through the use of DORGQR.
The factorized form is:

\par 
A = Q * R

this method also returns the column pivots used in the P matrix.

\param inMatrix real matrix to factorize
\param numRows number of rows of the matrix
\param numCols number of columns of the matrix
\param outQ pointer to a real matrix where Q will be written
\param outQRows number of rows of the Q matrix
\param outQCols number of columns of the Q matrix
\param outR pointer to a real matrix where R will be written
\param outRRows number of rows of the R matrix
\param outRCols number of columns of the R matrix
\param outP pointer to a real matrix where P will be written
\param outPRows number of rows of the P matrix
\param outPCols number of columns of the P matrix

\return The return value will be zero (0) when successful, and negative (-1) in case
an error occurred

\remarks free outP, outQ and outR using ::LibLA_freeMatrix

*/
LIB_EXTERN int LibLA_getQRWithPivot(double** inMatrix, int numRows, int numCols, 
									double** *outQ, int *outQRows, int * outQCols, 
									double** *outR, int *outRRows, int * outRCols, 
									double** *outP, int *outPRows, int * outPCols);

/*!	\brief This function computes the QR factorization of the given real M-by-N matrix A with column pivoting.

The LAPACK method dgeqp3 is used followed by an orthonormalization of Q through the use of DORGQR.
The factorized form is:

\par 
A = Q * R

\param inMatrix real matrix to factorize
\param numRows number of rows of the matrix
\param numCols number of columns of the matrix
\param outQ pointer to a real matrix where Q will be written
\param outQRows number of rows of the Q matrix
\param outQCols number of columns of the Q matrix
\param outR pointer to a real matrix where R will be written
\param outRRows number of rows of the R matrix
\param outRCols number of columns of the R matrix

\return The return value will be zero (0) when successful, and negative (-1) in case
an error occurred

\remarks free outQ and outR using ::LibLA_freeMatrix

*/
LIB_EXTERN int LibLA_getQR(double** inMatrix, int numRows, int numCols, 
						   double** *outQ, int *outQRows, int * outQCols, 
						   double** *outR, int *outRRows, int * outRCols);

/*! \brief This method performs the Singular Value Decomposition of the given real matrix, returning only the singular values. 

This procedure is carried out by the LAPACK method dgesdd.

\param inMatrix real matrix 
\param numRows number of rows of the matrix
\param numCols number of columns of the matrix

\param outSingularVals pointer to the double array where the singular values will be stored
\param outLength number of singular values

\return The return value will be zero (0) when successful, and negative (-1) in case
an error occurred 

\remarks free outSingularVals using ::LibLA_freeVector
*/

LIB_EXTERN int LibLA_getSingularValsBySVD(double** inMatrix, int numRows, int numCols, double* *outSingularVals, int *outLength);

/*! \brief This function computes the LU factorization of the given real N-by-N matrix A using complete pivoting (with row and column interchanges). 

This procedure is carried out by the LAPACK method dgetc2.

A is factorized into:

\par
A = P * L * U * Q

Here P and Q are permutation matrices for the rows and columns respectively.

\remarks This function supports only square matrices (N-by-N), choose ::LibLA_getQRWithPivot for a stable method
operating on N-by-M matrices.
*/
LIB_EXTERN int LibLA_getLUwithFullPivoting(double** inMatrix, int numRows, int numCols, 
										   double** *outL, int *outLRows, int * outLCols, 
										   double** *outU, int *outURows, int * outUCols, 
										   int** *outP, int *outPRows, int * outPCols, 
										   int** *outQ, int *outQRows, int * outQCols, 
										   int *info);

/*! \brief This function computes the LU factorization of the given real M-by-N matrix A 

using partial pivoting with row interchanges. This procedure is carried out by the 
LAPACK method dgetrf.

A is factorized into:

\par
A = P * L * U

Here P is the row permutation matrix.
*/
LIB_EXTERN int LibLA_getLU(double** inMatrix, int numRows, int numCols, 
						   double** *outL, int *outLRows, int * outLCols, 
						   double** *outU, int *outURows, int * outUCols, 
						   int** *outP, int *outPRows, int * outPCols, 										 
						   int *info);

/*! \brief This function calculates the inverse of a square real matrix. 

This procedure is carried out by the LAPACK methods dgetrf and dgetri. This means that the matrix will be 
factorized using LU decomposition first, followed by the calculation of the inverse based on:

\par 
inv(A)*L = inv(U) for inv(A).
*/
LIB_EXTERN int LibLA_inverse(double** inMatrix, int numRows, int numCols, 
							 double** *outMatrix, int *outRows, int *outCols);

/*! \brief This function calculates the left null space of a given real matrix. 

That is:
\par 
null(A)*A = 0

\remarks This function is equivalent to returning the right null space of the transposed matrix. 
See ::LibLA_rightNullspace
*/
LIB_EXTERN int LibLA_leftNullspace(double** inMatrix, int numRows, int numCols, 
								   double** *outMatrix, int *outRows, int *outCols);

/*! \brief This function calculates the right null space of a given real matrix. 

That is:

\par 
A*null(A) = 0

In order to calculate the (right) null space, we first calculate the full singular value decomposition (employing dgesdd) of the matrix:

\par
[U,S,V] = svd(A');

then calculate the rank:

\par
r = rank(A)

and finally return the last columns of the U matrix (r+1...n) as the null space matrix.
*/
LIB_EXTERN int LibLA_rightNullspace(double** inMatrix, int numRows, int numCols, 
									double** *outMatrix, int *outRows, int *outCols);

/*! \brief This function calculates the scaled left null space of a given real matrix. 

This function is equivalent to calling ::LibLA_leftNullspace however the resulting
matrix will be scaled (employing Gauss Jordan factorization) to yield whole numbered
entries wherever possible. 
*/
LIB_EXTERN int LibLA_scaledLeftNullspace(double** inMatrix, int numRows, int numCols, 
										 double** *outMatrix, int *outRows, int *outCols);

/*! \brief This function calculates the scaled right null space of a given real matrix. 

This function is equivalent to calling ::LibLA_rightNullspace however the resulting
matrix will be scaled (employing Gauss Jordan factorization) to yield whole numbered
entries wherever possible. 
*/
LIB_EXTERN int LibLA_scaledRightNullspace(double** inMatrix, int numRows, int numCols, 
										  double** *outMatrix, int *outRows, int *outCols);


/*! \brief This method computes the rank of the given matrix. 

The singular values of the matrix are calculated and the rank is determined by the number of non-zero values.

Note that zero here is defined as any value whose absolute value is bigger than the set tolerance (see
::LibLA_setTolerance )
*/
LIB_EXTERN int LibLA_getRank(double** inMatrix, int numRows, int numCols);

/*! \brief  reciprocal condition number estimate

returns an estimate for the reciprocal of the condition of A in 1-norm using the LAPACK condition estimator. 
If A is well conditioned, getRCond(A) is near 1.0. If A is badly conditioned, getRCond(A) is near 0.0.

*/
LIB_EXTERN double LibLA_getRCond(double** inMatrix, int numRows, int numCols);

/*! \brief This method calculates the Gauss Jordan or row echelon form of the given matrix. 

Only row swaps are used. These permutations will be returned in the 'pivots' vector.

If no permutations have occurred this vector will be in ascending form [ 0, 1, 2, 3 ]; 
However if say row one and three would be swapped this vector would look like: [ 0, 3, 2, 1 ];

*/
LIB_EXTERN int LibLA_gaussJordan(double** inMatrix, int numRows, int numCols, 
								 double** *outMatrix, int *outRows, int *outCols, 
								 int* *oPivots, int *nLength);

/*! \brief This method calculates the fully pivoted Gauss Jordan form of the given matrix. 

Fully pivoted means, that rows as well as column swaps will be used. These permutations 
are captured in the integer vectors rowPivots and colPivots.

If no permutations have occurred those vectors will be in ascending form [ 0, 1, 2, 3 ]; 
However if say row one and three would be swapped this vector would look like: [ 0, 3, 2, 1 ];
*/
LIB_EXTERN int LibLA_fullyPivotedGaussJordan(double** inMatrix, int numRows, int numCols, 
											 double** *outMatrix, int *outRows, int *outCols, 
											 int* *oRowPivots, int *nRowLength, 
											 int* *oColPivots, int *nColLength);



/*! \brief This function calculates the inverse of a square complex matrix. 

This procedure is carried out by the LAPACK methods: zgetrf and zgetri. This means that the matrix will be 
factorized using LU decomposition first, followed by the calculation of the inverse based on:

\par 
inv(A)*L = inv(U) for inv(A).
*/
LIB_EXTERN int LibLA_Zinverse(double** inMatrixReal, double **inMatrixImag, int numRows, int numCols, 
							  double** *outMatrixReal, double ** *outMatrixImag, int *outRows, int *outCols);

//! Frees a vector previously allocated by this library.
LIB_EXTERN void LibLA_freeVector(void* vector);

//! Frees a matrix previously allocated by this library.
LIB_EXTERN void LibLA_freeMatrix(void** matrix, int numRows);


/*! \brief Calculates the eigen-vectors of a square real matrix. 

This function calculates the complex (right)eigenvectors of the given real matrix. The complex matrix 
returned contains the eigenvectors in the columns, in the same order as LibLA_getEigenValues.

The right eigenvector v(j) of A satisfies:
\par
A * v(j) = lambda(j) * v(j)


*/
LIB_EXTERN int LibLA_getEigenVectors(double** inMatrix, int numRows, int numCols, 
									 double** *outMatrixReal, double** *outMatrixImag, int *outRows, int *outCols);
/*! \brief Calculates the eigen-vectors of a square nonsymmetrix complex matrix. 

This function calculates the complex (right)eigenvectors of the given real matrix. The complex matrix 
returned contains the eigenvectors in the columns, in the same order as LibLA_ZgetEigenValues.
The right eigenvector v(j) of A satisfies:
\par
A * v(j) = lambda(j) * v(j)


*/
LIB_EXTERN int LibLA_ZgetEigenVectors(double** inMatrixReal,   double **  inMatrixImag, int numRows, int numCols, 
									   double** *outMatrixReal, double** *outMatrixImag, int *outRows, int *outCols);
/*! \brief Factorizes the given matrix using SVD

This function computes the singular value decomposition (SVD) of the given real matrix. 


The SVD is written

\par
A = U * SIGMA * transpose(V)

\remarks this function returns the transpose of V

*/
LIB_EXTERN int LibLA_getSVD(double** inMatrix, int numRows, int numCols, 
					   double** *outU, int *outRowsU, int *outColsU, 
					   double* *outSingVals, int *outLength, 
					   double** *outV, int *outRowsV, int *outColsV);
					   					   
/*! \brief Factorizes the given matrix using SVD

This function computes the singular value decomposition (SVD) of the given complex matrix. 


The SVD is written

\par
A = U * SIGMA * conjugate-transpose(V)

\remarks this function returns the conjugate-transpose of V

*/
LIB_EXTERN int LibLA_ZgetSVD(double** inMatrixReal, double** inMatrixImag, int numRows, int numCols, 
					   double** *outUReal, double** *outUImag, int *outRowsU, int *outColsU, 
					   double* *outSingVals, int *outLength, 
					   double** *outVReal, double** *outVImag, int *outRowsV, int *outColsV);

END_C_DECLS;

#endif

