#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <cmath>
#include <sstream>

#include <string.h>

#include "libla.h"
#include "matrix.h"
#include "util.h"

extern "C" {
#include "f2c.h"
#include "clapack.h"
}


using namespace std;
using namespace LIB_LA;

//#define ENABLE_DEBUG_OUTPUT 1


vector< LIB_LA::Complex> LibLA::getEigenValues(DoubleMatrix &oMatrix)
{
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== getEigenValues " << endl;
	cout << "======================================================" << endl << endl;
#endif
	vector< Complex > oResult;

	integer numRows = oMatrix.numRows();
	integer numCols = oMatrix.numCols();
	integer lwork = 2*numRows; integer info;

	if (numRows != numCols)
		throw ApplicationException ("Input Matrix must be square", "Expecting a Square Matrix");

	if (numRows == 0) return oResult;

	doublecomplex* A = new doublecomplex[numRows*numRows]; memset(A, 0, sizeof(doublecomplex)*numRows*numRows);
	doublecomplex* eigVals = new doublecomplex[numRows]; memset(eigVals, 0, sizeof(doublecomplex)*numRows);
	doublecomplex* work = new doublecomplex[lwork]; memset(work, 0, sizeof(doublecomplex)*lwork);
	doublereal* rwork = new doublereal[lwork]; memset(rwork, 0, sizeof(doublereal)*lwork);

	int index;
	for(int i=0; i<numRows; i++) 
	{       	
		for(int j=0; j<numCols; j++) 
		{ 	
			index = (j+numRows*i);
			A[index].r = oMatrix(i,j);
		}
	}

	char job = 'N'; // do not compute eigenvectors
	zgeev_(&job,&job, &numRows, A, &numRows, eigVals, NULL, &numRows, NULL,&numRows, work, &lwork, rwork, &info);


	for (int i = 0; i < numRows; i++)
	{
		Complex complex( Util::RoundToTolerance( eigVals[i].r, _Tolerance ), Util::RoundToTolerance( eigVals[i].i, _Tolerance ));
		oResult.push_back(complex);
	}

	delete [] eigVals; delete[] A;delete[] work; delete[] rwork;

	return oResult;

}
vector< LIB_LA::Complex > LibLA::ZgetEigenValues(ComplexMatrix &oMatrix)
{
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== ZgetEigenValues " << endl;
	cout << "======================================================" << endl << endl;
#endif
	vector< Complex > oResult;

	integer numRows = oMatrix.numRows();
	integer numCols = oMatrix.numCols();
	integer lwork = 2*numRows; integer info;

	if (numRows != numCols)
		throw ApplicationException ("Input Matrix must be square", "Expecting a Square Matrix");

	doublecomplex* A = new doublecomplex[numRows*numRows]; memset(A, 0, sizeof(doublecomplex)*numRows*numRows);
	doublecomplex* eigVals = new doublecomplex[numRows]; memset(eigVals, 0, sizeof(doublecomplex)*numRows);
	doublecomplex* work = new doublecomplex[lwork]; memset(work, 0, sizeof(doublecomplex)*lwork);
	doublereal* rwork = new doublereal[lwork]; memset(rwork, 0, sizeof(doublereal)*lwork);

	int index;
	for(int i=0; i<numRows; i++) 
	{       	
		for(int j=0; j<numCols; j++) 
		{ 	
			index = (j+numRows*i);
			A[index].r = oMatrix(j,i).getReal();
			A[index].i = oMatrix(j,i).getImag();
		}
	}

	char job = 'N'; // do not compute eigenvectors
	zgeev_(&job,&job, &numRows, A, &numRows, eigVals, NULL, &numRows, NULL,&numRows, work, &lwork, rwork, &info);


	for (int i = 0; i < numRows; i++)
	{
		Complex complex( Util::RoundToTolerance( eigVals[i].r, _Tolerance ), Util::RoundToTolerance( eigVals[i].i, _Tolerance ));
		oResult.push_back(complex);
	}

	delete [] eigVals; delete[] A;delete[] work; delete[] rwork;

	return oResult;
}

vector< DoubleMatrix* > LibLA::getQRWithPivot(DoubleMatrix &oMatrix)
{
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== getQRWithPivot " << endl;
	cout << "======================================================" << endl << endl;
#endif

	vector < DoubleMatrix* > oResult;

	integer row = oMatrix.numRows();
	integer col = oMatrix.numCols();

	if (row * col == 0)
	{
		DoubleMatrix* oMatrixQ = new DoubleMatrix(row, row);
		DoubleMatrix* oMatrixR = new DoubleMatrix(row, col);
		DoubleMatrix* oMatrixP = new DoubleMatrix(col, col);
		oResult.push_back( oMatrixQ );
		oResult.push_back( oMatrixR );
		oResult.push_back( oMatrixP );
		return oResult;
	}

	integer minRowCol = min(row,col);
	integer lwork = 16*col;

	doublereal* A = oMatrix.getCopy(true);


	doublereal *Q = NULL;
	if (row*row)
	{
		Q = new doublereal[row*row]; memset(Q, 0, sizeof(doublereal)*row*row);
	}
	doublereal *R = NULL;
	if (row*col)
	{
		R = new doublereal[row*col]; memset(R, 0, sizeof(doublereal)*row*col);
	}
	doublereal *P = NULL;
	if (col*col)
	{
		P = new doublereal[col*col]; memset(P, 0, sizeof(doublereal)*col*col);
	}


	doublereal* tau = NULL; 
	if (minRowCol)
	{
		tau = new doublereal[minRowCol]; memset(tau, 0, sizeof(doublereal)*minRowCol);
	}
	integer* jpvt = NULL; 
	if (col)
	{
		jpvt = new integer[col]; memset(jpvt, 0, sizeof(integer)*col);
	}
	doublereal* work = NULL; 
	if (lwork)
	{
		work = new doublereal[lwork]; memset(work, 0, lwork);
	}

	integer info;
	int out;

#ifdef ENABLE_DEBUG_OUTPUT
	cout << "Input:\n"; Util::print(row, col, A);	
#endif

	// call lapack routine dgepq3_ to generate householder reflections
	out = dgeqp3_ (&row, &col, A, &row, jpvt, tau, work, &lwork, &info);	


#ifdef ENABLE_DEBUG_OUTPUT
	cout << "before permutation" << endl;	
#endif


	// Building permutation matrix P and	
	for (int i=0; i<col; i++) 
	{
		size_t pos = i*col+(jpvt[i]-1);
		if (pos < col*col)
		P[pos] = 1;		
	}

#ifdef ENABLE_DEBUG_OUTPUT
	cout << "before memcpy" << endl;	
#endif

	// set R to A before calling dorgqr
	memcpy(R, A, sizeof(doublereal)*row*col);

#ifdef ENABLE_DEBUG_OUTPUT
	cout << "after memcpy" << endl;	
#endif

	// make R a trapezoidal matrix
	// set Q to A before calling dorgqr
	int index = 0;
	for (int i = 0; i < row; i++)
	{
		for (int j=0; j<minRowCol; j++) 
		{
			index = i+row*j;
			Q[index] = A[index];
		}

		if (i >= 1) for (int j=0; j<min(i,col); j++) 
		{
			R[i+row*j] = 0.0;
		}

	}

#ifdef ENABLE_DEBUG_OUTPUT
	cout << "before dorgqr:\n"; Util::print(row, col, A);	
#endif


	// call routine dorgqr_ to build orthogonal matrix Q
	out = dorgqr_ (&row, &row, &minRowCol, Q, &row, tau, work, &lwork, &info);

#ifdef ENABLE_DEBUG_OUTPUT
	cout << endl << endl << "Q: )\n"; Util::print(row, row, Q);	
	cout << endl << endl << "R: )\n"; Util::print(row, col, R);	
	cout << endl << endl << "P: )\n"; Util::print(col, col, P);	
#endif

	DoubleMatrix* oMatrixQ = new DoubleMatrix(Q, row, row, true); Util::RoundMatrixToTolerance(*oMatrixQ, _Tolerance);
	DoubleMatrix* oMatrixR = new DoubleMatrix(R, row, col, true); Util::RoundMatrixToTolerance(*oMatrixR, _Tolerance);
	DoubleMatrix* oMatrixP = new DoubleMatrix(P, col, col, true); Util::RoundMatrixToTolerance(*oMatrixP, _Tolerance);
	oResult.push_back( oMatrixQ );
	oResult.push_back( oMatrixR );
	oResult.push_back( oMatrixP );


	// free
	if (row*col) delete[] A; if (row*row) delete[] Q; if (row*col) delete[] R; if (col*col) delete[] P;
	if (tau) delete[] tau; if (jpvt) delete[] jpvt; if (work) delete[] work;

	return oResult;
}

vector< DoubleMatrix* > LibLA::getQR(DoubleMatrix &oMatrix)
{
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== getQR " << endl;
	cout << "======================================================" << endl << endl;
#endif

	integer row			= oMatrix.numRows();
	integer col			= oMatrix.numCols();		
	if (row * col == 0)
	{
		vector< DoubleMatrix* > oResult;
		DoubleMatrix* oMatrixQ = new DoubleMatrix(row, row);
		DoubleMatrix* oMatrixR = new DoubleMatrix(row, col);
		oResult.push_back( oMatrixQ );
		oResult.push_back( oMatrixR );
		return oResult;
	}

	integer lwork		= 16*col;
	integer minRowCol	= min(row,col);

	doublereal* Q		= new doublereal[row*row];
	doublereal* R		= new doublereal[row*col];
	doublereal* tau		= new doublereal[minRowCol];
	doublereal* work	= new doublereal[lwork];

	doublereal* A = (doublereal*)oMatrix.getCopy(true);


#ifdef ENABLE_DEBUG_OUTPUT
	cout << "Input:\n"; Util::print(row, col, A);			
#endif

	integer info;
	int out = dgeqrf_ (&row, &col, A, &row, tau, work, &lwork, &info);

#ifdef ENABLE_DEBUG_OUTPUT
	cout << "A: after dgeqrt)\n"; Util::print(row, col, A);
	cout << "tau: after dgeqrt)\n"; Util::print(1, minRowCol, tau);
#endif
	// set R to A before calling dorgqr	
	memcpy(R,A,sizeof(double)*row*col);
	int index;
	for (int i=0; i<row; i++)
	{
		if (i > 0) for (int j=0; j<min(i,col); j++) {
			R[i+row*j] = 0.0;
		}
		for (int j=0; j<minRowCol; j++) 
		{
			index = i+row*j;
			Q[index] = A[index];
		}
	}

	out = dorgqr_ (&row, &row, &minRowCol, Q, &row, tau, work, &lwork, &info);		

	Util::checkTolerance(row*row, Q, LibLA::getInstance()->getTolerance());
	Util::checkTolerance(row*col, R,LibLA::getInstance()->getTolerance());

#ifdef ENABLE_DEBUG_OUTPUT
	cout << endl << endl << "Q: )\n"; Util::print(row, row, Q);	
	cout << endl << endl << "R: )\n"; Util::print(row, col, R);	
#endif

	vector< DoubleMatrix* > oResult;

	DoubleMatrix* oMatrixQ = new DoubleMatrix(Q, row, row, true); Util::RoundMatrixToTolerance(*oMatrixQ, _Tolerance);
	DoubleMatrix* oMatrixR = new DoubleMatrix(R, row, col, true); Util::RoundMatrixToTolerance(*oMatrixR, _Tolerance);
	oResult.push_back( oMatrixQ );
	oResult.push_back( oMatrixR );

	delete [] A; delete [] Q; delete [] R; delete [] tau; delete [] work;

	return oResult;
}


DoubleMatrix* LibLA::getLeftNullSpace(DoubleMatrix &oMatrixIn)
{
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== getLeftNullSpace " << endl;
	cout << "======================================================" << endl << endl;
#endif
	DoubleMatrix *oMatrix = oMatrixIn.getTranspose();
	DoubleMatrix *oMatrixResult = getRightNullSpace(*oMatrix);
	delete oMatrix;
	DoubleMatrix *oFinalMatrix = oMatrixResult->getTranspose();
	delete oMatrixResult;
	return oFinalMatrix;	
}

DoubleMatrix* LibLA::getScaledLeftNullSpace(DoubleMatrix &oMatrixIn)
{
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== getScaledLeftNullSpace " << endl;
	cout << "======================================================" << endl << endl;
#endif
	DoubleMatrix *oMatrix = oMatrixIn.getTranspose();
	DoubleMatrix *oMatrixResult = getScaledRightNullSpace(*oMatrix);
	delete oMatrix;
	DoubleMatrix *oFinalMatrix = oMatrixResult->getTranspose();
	delete oMatrixResult;
	return oFinalMatrix;	

}

DoubleMatrix* LibLA::getScaledRightNullSpace(DoubleMatrix &oMatrix)
{
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== getScaledRightNullSpace " << endl;
	cout << "======================================================" << endl << endl;
#endif
	DoubleMatrix* oTemp = getRightNullSpace(oMatrix);
	DoubleMatrix* oTranspose = oTemp->getTranspose();
	delete oTemp;
	Util::GaussJordan(*oTranspose, _Tolerance);
	DoubleMatrix* oResult = oTranspose->getTranspose();
	delete oTranspose;

	Util::RoundMatrixToTolerance(oMatrix, _Tolerance);

	return oResult;
}

DoubleMatrix* LibLA::getRightNullSpace(DoubleMatrix &oMatrix)
{
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== getRightNullSpace " << endl;
	cout << "======================================================" << endl << endl;
#endif
	DoubleMatrix *oTranspose = oMatrix.getTranspose();

	integer numRows = oTranspose->numRows();
	integer numCols = oTranspose->numCols();

	// determine sizes
	integer min_MN	= min(numRows,numCols);
	integer max_MN	= max(numRows,numCols);
	integer lwork   =  3*min_MN*min_MN + max(max_MN,4*min_MN*min_MN+4*min_MN); // 'A'	


	// allocate arrays for lapack
	doublereal* A		= oTranspose->getCopy(true);		
	doublereal* S		= new doublereal[min_MN]; memset(S,0,sizeof(doublereal)*min_MN);
	doublereal* work	= new doublereal[lwork]; memset(work,0,sizeof(doublereal)*lwork);
	doublereal* U		= new doublereal[numRows*numRows]; memset(U, 0, sizeof(doublereal)*numRows*numRows);
	doublereal* VT		= new doublereal[numCols*numCols]; memset(VT, 0, sizeof(doublereal)*numCols*numCols);
	integer* iwork		= new integer[8*min_MN];

	integer info; char jobz = 'A';
	dgesdd_(&jobz, &numRows, &numCols, A, &numRows, S, U, &numRows, VT, &numCols, work, &lwork, iwork, &info);

	// now we have everything we could get, now extract the nullspace. In Matlab this would look like: 
	//     [U,S,V] = svd(A');
	//     r = rank(A)
	//     Z = U(:,r+1:end)

	int rank = getRank(oMatrix);
	int nResultColumns = numRows-rank;

	DoubleMatrix* oMatrixU = new DoubleMatrix(U, numRows, numRows, true); 


#ifdef ENABLE_DEBUG_OUTPUT
	cout << " SVD: U " << endl;
	Util::print(*oMatrixU);
#endif
	DoubleMatrix *oResultMatrix = new DoubleMatrix(numRows, nResultColumns);
	for (int i = 0; i < nResultColumns; i++)
	{
		for (int j = 0; j < numRows; j++)
		{
			(*oResultMatrix)(j,i) = (*oMatrixU)(j,rank+i);
		}
	}
#ifdef ENABLE_DEBUG_OUTPUT
	cout << " Nullspace: " << endl;
	Util::print(*oResultMatrix);
#endif
	delete[] S; delete[]
	work; delete[] U; delete[] VT; delete[] iwork; delete[] A; delete oTranspose; delete oMatrixU;

	Util::RoundMatrixToTolerance(*oResultMatrix, _Tolerance);
	return oResultMatrix ;

}

int LibLA::getRank(DoubleMatrix &oMatrix)
{
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== getRank " << endl;
	cout << "======================================================" << endl << endl;
#endif
	int rank = 0;
	vector<double> oSingularVals = LibLA::getSingularValsBySVD(oMatrix);

	for (unsigned int i = 0; i < oSingularVals.size(); i++)
	{
		if (fabs(oSingularVals[i]) >  _Tolerance )
		{
			rank++;
		}
	}
	return rank;
}

vector< double> LibLA::getSingularValsBySVD(DoubleMatrix &oMatrix)
{
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== getSingularValsBySVD " << endl;
	cout << "======================================================" << endl << endl;
#endif

	vector <double> oResult;

	integer numRows = oMatrix.numRows();
	integer numCols = oMatrix.numCols();

	integer min_MN	= min(numRows,numCols);
	integer max_MN	= max(numRows,numCols);

	if (min_MN == 0) return oResult;

	integer lwork	= 3*min_MN + max(max_MN, 7*min_MN);	// specified in dgesdd description

	doublereal* A		= oMatrix.getCopy(true);		
	doublereal* S		= new doublereal[min_MN]; memset(S,0,sizeof(doublereal)*min_MN);
	doublereal* work	= new doublereal[lwork]; memset(work,0,sizeof(doublereal)*lwork);
	integer* iwork		= new integer[8*min_MN];


	integer info; char jobz = 'N';
	dgesdd_(&jobz, &numRows, &numCols, A, &numRows, S, NULL, &numRows, NULL, &numCols, work, &lwork, iwork, &info);

	for (int i=0; i<min_MN; i++) 
	{
		oResult.push_back( Util::RoundToTolerance( S[i], _Tolerance ) );
	}

	// free memory
	delete [] A; delete [] S; delete [] work; delete [] iwork;

	return oResult;
}

ComplexMatrix *LibLA::getEigenVectors(DoubleMatrix &oMatrix)
{
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== getEigenVectors " << endl;
	cout << "======================================================" << endl << endl;
#endif


	integer numRows = oMatrix.numRows();
	integer numCols = oMatrix.numCols();
	integer lwork = 2*numRows; integer info;

	if (numRows != numCols)
		throw ApplicationException ("Input Matrix must be square", "Expecting a Square Matrix");

	if (numRows == 0) return new ComplexMatrix();

	doublecomplex* A = new doublecomplex[numRows*numRows]; memset(A, 0, sizeof(doublecomplex)*numRows*numRows);
	doublecomplex* eigVals = new doublecomplex[numRows]; memset(eigVals, 0, sizeof(doublecomplex)*numRows);
	doublecomplex* vr = new doublecomplex[numRows*numRows]; memset(vr, 0, sizeof(doublecomplex)*numRows*numRows);
	doublecomplex* work = new doublecomplex[lwork]; memset(work, 0, sizeof(doublecomplex)*lwork);
	doublereal* rwork = new doublereal[lwork]; memset(rwork, 0, sizeof(doublereal)*lwork);

	int index;
	for(int i=0; i<numRows; i++) 
	{       	
		for(int j=0; j<numCols; j++) 
		{ 	
			index = (j+numRows*i);
			A[index].r = oMatrix(j,i);
		}
	}

	char job = 'N'; char jobR = 'V'; // compute the right eigenvectors
	zgeev_(&job,&jobR, &numRows, A, &numRows, eigVals, NULL, &numRows, vr,&numRows, work, &lwork, rwork, &info);

	ComplexMatrix *oResult = new ComplexMatrix (numRows, numRows);
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numRows; j++)
		{
			index = (j+numRows*i);
			Complex complex( 
				Util::RoundToTolerance( vr[index].r, _Tolerance ), 
				Util::RoundToTolerance( vr[index].i, _Tolerance ));

			(*oResult)(i,j).set(complex.Real, complex.Imag);
		}
	}

	delete [] eigVals; delete[] A;delete[] work; delete[] rwork; delete[] vr;

	return oResult;
}

ComplexMatrix* LibLA::ZgetEigenVectors(ComplexMatrix &oMatrix)
{
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== getEigenVectors " << endl;
	cout << "======================================================" << endl << endl;
#endif


	integer numRows = oMatrix.numRows();
	integer numCols = oMatrix.numCols();
	integer lwork = 2*numRows; integer info;

	if (numRows != numCols)
		throw ApplicationException ("Input Matrix must be square", "Expecting a Square Matrix");

	if (numRows == 0) return new ComplexMatrix();

	doublecomplex* A = new doublecomplex[numRows*numRows]; memset(A, 0, sizeof(doublecomplex)*numRows*numRows);
	doublecomplex* eigVals = new doublecomplex[numRows]; memset(eigVals, 0, sizeof(doublecomplex)*numRows);
	doublecomplex* vr = new doublecomplex[numRows*numRows]; memset(vr, 0, sizeof(doublecomplex)*numRows*numRows);
	doublecomplex* work = new doublecomplex[lwork]; memset(work, 0, sizeof(doublecomplex)*lwork);
	doublereal* rwork = new doublereal[lwork]; memset(rwork, 0, sizeof(doublereal)*lwork);

	int index;
	for(int i=0; i<numRows; i++) 
	{       	
		for(int j=0; j<numCols; j++) 
		{ 	
			index = (j+numRows*i);
			A[index].r = oMatrix(j,i).Real;
			A[index].i = oMatrix(j,i).Imag;
		}
	}

	char job = 'N'; char jobR = 'V'; // compute the right eigenvectors
	zgeev_(&job,&jobR, &numRows, A, &numRows, eigVals, NULL, &numRows, vr,&numRows, work, &lwork, rwork, &info);

	ComplexMatrix *oResult = new ComplexMatrix (numRows, numRows);
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numRows; j++)
		{
			index = (j+numRows*i);
			Complex complex( 
				Util::RoundToTolerance( vr[index].r, _Tolerance ), 
				Util::RoundToTolerance( vr[index].i, _Tolerance ));

			(*oResult)(i,j).set(complex.Real, complex.Imag);
		}
	}

	delete [] eigVals; delete[] A;delete[] work; delete[] rwork; delete[] vr;

	return oResult;
}

void LibLA::getSVD(DoubleMatrix &inputMatrix, DoubleMatrix* &outU, std::vector<double>* &outSingularVals, DoubleMatrix* &outV)
{
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== getSingularValsBySVD " << endl;
	cout << "======================================================" << endl << endl;
#endif


	integer numRows = inputMatrix.numRows();
	integer numCols = inputMatrix.numCols();

	integer min_MN	= min(numRows,numCols);
	integer max_MN	= max(numRows,numCols);

	if (min_MN == 0) return;

	//integer lwork	= 3*min_MN + max(max_MN, 7*min_MN);	// specified in dgesdd description
	integer lwork	= 3*min_MN*min_MN +
		max(max_MN,4*min_MN*min_MN+4*min_MN); // specified in dgesdd description for job 'A'

	doublereal* A		= inputMatrix.getCopy(true);		
	doublereal* U		= new doublereal[numRows*numRows]; memset(U, 0, sizeof(doublereal)*numRows*numRows);
	doublereal* VT		= new doublereal[numCols*numCols]; memset(VT, 0, sizeof(doublereal)*numCols*numCols);
	doublereal* S		= new doublereal[min_MN]; memset(S,0,sizeof(doublereal)*min_MN);
	doublereal* work	= new doublereal[lwork]; memset(work,0,sizeof(doublereal)*lwork);
	integer* iwork		= new integer[8*min_MN];


	integer info; char jobz = 'A';
	dgesdd_(&jobz, &numRows, &numCols, A, &numRows, S, U, &numRows, VT, &numCols, work, &lwork, iwork, &info);

	outU = new DoubleMatrix(numRows, numRows);
	int index;
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numRows; j++)
		{
			index = (j+numRows*i);
			(*outU)(j,i) = U[index];
		}
	}

	Util::RoundMatrixToTolerance(*outU, _Tolerance);

	outV = new DoubleMatrix(numCols, numCols);
	for (int i = 0; i < numCols; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			index = (j+numCols*i);
			(*outV)(i,j) = VT[index];
		}
	}

	Util::RoundMatrixToTolerance(*outV, _Tolerance);

	outSingularVals = new std::vector<double>();
	for (int i=0; i<min_MN; i++) 
	{
		outSingularVals ->push_back( Util::RoundToTolerance( S[i], _Tolerance ) );
	}	

	// free memory
	delete [] A; delete [] S; delete [] work; delete [] iwork; delete[] U; delete[] VT;

	return;
}

void LibLA::ZgetSVD(ComplexMatrix &inputMatrix, ComplexMatrix* &outU, std::vector<double>* &outSingularVals, ComplexMatrix* &outV)
{
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== getSingularValsBySVD " << endl;
	cout << "======================================================" << endl << endl;
#endif


	integer numRows = inputMatrix.numRows();
	integer numCols = inputMatrix.numCols();

	integer min_MN	= min(numRows,numCols);
	integer max_MN	= max(numRows,numCols);

	if (min_MN == 0) return;

	integer lwork	        =  min_MN*min_MN+2*min_MN+max_MN; // specified in dgesdd description for job 'A'
	integer lrwork          = 5*min_MN*min_MN + 7*min_MN; 
	doublecomplex* A		= new doublecomplex[numRows*numCols]; memset(A, 0, sizeof(doublecomplex)*numRows*numCols);
	doublecomplex* U		= new doublecomplex[numRows*numRows]; memset(U, 0, sizeof(doublecomplex)*numRows*numRows);
	doublecomplex* VT		= new doublecomplex[numCols*numCols]; memset(VT, 0, sizeof(doublecomplex)*numCols*numCols);
	doublereal* S			= new doublereal[min_MN]; memset(S,0,sizeof(doublereal)*min_MN);
	doublecomplex* work		= new doublecomplex[lwork]; memset(work,0,sizeof(doublecomplex)*lwork);
	doublereal* rwork		= new doublereal[lrwork]; memset(rwork, 0, sizeof(doublereal)*lrwork);
	integer* iwork			= new integer[8*min_MN];


	int index;
	for(int i=0; i<numRows; i++) 
	{       	
		for(int j=0; j<numCols; j++) 
		{ 	
			index = (j+numRows*i);
			A[index].r = inputMatrix(j,i).Real;
			A[index].i = inputMatrix(j,i).Imag;
		}
	}
	

	integer info; char jobz = 'A';
	zgesdd_(&jobz, &numRows, &numCols, A, &numRows, S, U, &numRows, VT, &numCols, work, &lwork, rwork, iwork, &info);

	outU = new ComplexMatrix(numRows, numRows);
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numRows; j++)
		{
			index = (j+numRows*i);
			(*outU)(j,i).set( Util::RoundToTolerance( U[index].r, _Tolerance),
				Util::RoundToTolerance( U[index].i, _Tolerance) );
		}
	}
	
	outV = new ComplexMatrix(numCols, numCols);
	for (int i = 0; i < numCols; i++)
	{
		for (int j = 0; j < numCols; j++)
		{
			index = (j+numCols*i);
			(*outV)(i,j).set( Util::RoundToTolerance( VT[index].r, _Tolerance),
				Util::RoundToTolerance( -VT[index].i, _Tolerance));
		}
	}

	outSingularVals = new std::vector<double>();
	for (int i=0; i<min_MN; i++) 
	{
		outSingularVals ->push_back( Util::RoundToTolerance( S[i], _Tolerance ) );
	}

	// free memory
	delete [] A; delete [] S; delete [] work; delete [] iwork; delete[] U; delete[] VT;

	return;
}


double LibLA::getRCond(DoubleMatrix &oMatrix)
{
	integer numRows = oMatrix.numRows();
	integer numCols = oMatrix.numCols();

	integer minRC = min(numRows, numCols);

	if (minRC == 0)
	{
		return 0.0;
	}

	doublereal *A = (doublereal*)oMatrix.getCopy(true);
	integer* vecP = new integer[minRC]; memset(vecP, 0, (sizeof(integer)*minRC));

	integer info; 
	
	
	char norm = '1';
	integer order = numRows*numCols;

	double* work = new doublereal[4*order]; memset(work, 0, sizeof(double)*4*order);

	double dnorm = dlange_(&norm, &numRows, &numCols, A, &numRows, work  );
	dgetrf_ (&numRows, &numCols, A, &numRows, vecP, &info);
	Util::checkTolerance(numRows*numCols, A, _Tolerance);
	
	integer* iwork = new integer[numRows]; memset(iwork, 0, sizeof(integer)*numRows);

	memset(work, 0, sizeof(double)*4*order);
	
	double rcond = 0.0; 
	dgecon_(&norm, &numRows, A, &numRows, &dnorm, &rcond, work, iwork, &info);

	delete[] vecP; delete[] A; delete[] work; delete[] iwork; 

	return rcond;

}

LU_Result* LibLA::getLU(DoubleMatrix &oMatrix)
{
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== getLU " << endl;
	cout << "======================================================" << endl << endl;
#endif	

	integer numRows = oMatrix.numRows();
	integer numCols = oMatrix.numCols();

	int minRC = min(numRows, numCols);

	if (minRC == 0)
	{
		LU_Result* oResult = new LU_Result();
		DoubleMatrix *L = new DoubleMatrix(numRows, minRC);
		DoubleMatrix *U = new DoubleMatrix(minRC,numCols);
		IntMatrix *P = new IntMatrix(numRows, numRows);

		oResult->L = L; oResult->U = U;
		oResult->P = P; oResult->nInfo = -1;
		return oResult;
	}

	doublereal *A = (doublereal*)oMatrix.getCopy(true);
	integer* vecP = new integer[minRC]; memset(vecP, 0, (sizeof(integer)*minRC));

	integer info; dgetrf_ (&numRows, &numCols, A, &numRows, vecP, &info);


#ifdef ENABLE_DEBUG_OUTPUT
	Util::print(numRows, numCols, A);
#endif	
	
	DoubleMatrix *L = new DoubleMatrix(numRows, minRC);
	DoubleMatrix *U = new DoubleMatrix(minRC,numCols);

	// Assign values to Lmat and Umat
	for (int i=0; i<minRC; i++) 
	{
		(*L)(i,i) = 1.0;
		(*U)(i,i) = A[i+numRows*i];
		for (int j=0; j<i; j++) 
		{
			(*L)(i,j) = A[i+numRows*j];
		}
		for (int j=i+1; j<minRC; j++) 
		{
			(*U)(i,j) = A[i+numRows*j];
		}
	}

	if (numRows > numCols) 
	{
		for (int i = numCols; i < numRows; i++) 
		{
			for (int j=0; j<numCols; j++) 
			{
				(*L)(i,j) = A[i+numRows*j];
			}
		}
	}
	else  
	{
		for (int i=0; i<numRows; i++) 
		{
			for (int j=numRows; j<numCols; j++) 
			{
				(*U)(i,j) = A[i+numRows*j];
			}
		}
	}

	// build permutation matrix
	IntMatrix *P = new IntMatrix(numRows, numRows);
	for (int i = 0; i < numRows; i++)
		(*P)(i,i) = 1;
	for (int i = 0; i < minRC; i++)
	{		
		if (vecP[i] != 0 && vecP[i]-1 != i)
			P->swapRows(i, vecP[i]-1);			
	}

	LU_Result* oResult = new LU_Result();

	Util::RoundMatrixToTolerance(*L, _Tolerance);
	Util::RoundMatrixToTolerance(*U, _Tolerance);

	oResult->L = L; oResult->U = U;
	oResult->P = P; oResult->nInfo = info;

	delete[] A; delete [] vecP;	

	return oResult;
}

LU_Result* LibLA::getLUwithFullPivoting(DoubleMatrix &oMatrix)
{

#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== getLUwithFullPivoting " << endl;
	cout << "======================================================" << endl << endl;
#endif


	integer numRows = oMatrix.numRows();
	integer numCols = oMatrix.numCols();

	if (numRows != numCols)
		throw ApplicationException("Input Matrix must be square", "Expecting a Square Matrix");


	doublereal *A = (doublereal*)oMatrix.getCopy(true);
	integer* vecP = new integer[numRows]; memset(vecP, 0, (sizeof(integer)*numRows));
	integer* vecQ = new integer[numRows]; memset(vecQ, 0, (sizeof(integer)*numRows));

	integer info; dgetc2_ (&numRows, A, &numRows, vecP, vecQ, &info);

	DoubleMatrix *L = new DoubleMatrix(numRows, numRows);
	DoubleMatrix *U = new DoubleMatrix(numRows, numCols);

	// Assign values to Lmat and Umat
	for (int i=0; i<numRows; i++) 
	{
		(*L)(i,i) = 1.0;
		(*U)(i,i) = A[i+numRows*i];
		for (int j=0; j<i; j++) 
		{
			(*L)(i,j) = A[i+numRows*j];
		}
		for (int j=i+1; j<numRows; j++) 
		{
			(*U)(i,j) = A[i+numRows*j];
		}
	}

	if (numRows > numCols) 
	{
		for (int i = numCols; i < numRows; i++) 
		{
			for (int j=0; j<numCols; j++) 
			{
				(*L)(i,j) = A[i+numRows*j];
			}
		}
	}
	else  
	{
		for (int i=0; i<numRows; i++) 
		{
			for (int j=numRows; j<numCols; j++) 
			{
				(*U)(i,j) = A[i+numRows*j];
			}
		}
	}

	// build permutation matrix
	IntMatrix *P = new IntMatrix(numRows, numRows);
	for (int i = 0; i < numRows; i++)
		(*P)(i,i) = 1;	
	for (int i = 0; i < numRows; i++)
	{		
		if (vecP[i] != 0 && vecP[i]-1 != i)
			P->swapRows(i, vecP[i]-1);			
	}

	IntMatrix *Q = new IntMatrix(numRows, numRows);
	for (int i = 0; i < numRows; i++)
		(*Q)(i,i) = 1;
	for (int i = 0; i < numRows; i++)
	{		
		if (vecQ[i] != 0 && vecQ[i]-1 != i)
			Q->swapCols(i,vecQ[i]-1);
	}

	LU_Result* oResult = new LU_Result();

	Util::RoundMatrixToTolerance(*L, _Tolerance);
	Util::RoundMatrixToTolerance(*U, _Tolerance);

	oResult->L = L; oResult->U = U;
	oResult->P = P; oResult->Q = Q; oResult->nInfo = info;

	delete[] A; delete [] vecP;	delete[] vecQ;

	return oResult;
}

DoubleMatrix* LibLA::inverse(DoubleMatrix &oMatrix)
{	
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== inverse " << endl;
	cout << "======================================================" << endl << endl;
#endif
	DoubleMatrix* oResultMatrix = NULL;

	integer numRows = oMatrix.numRows();
	integer numCols = oMatrix.numCols();


	if (numRows != numCols)
		throw ApplicationException ("Input Matrix must be square", "Expecting a Square Matrix");	


	doublereal* A = oMatrix.getCopy(true);
	integer* ipvt = new integer[numRows];		memset(ipvt,0,sizeof(integer)*numRows);
	doublereal* work = new doublereal[numRows];	memset(work,0,sizeof(doublereal)*numRows);

#ifdef ENABLE_DEBUG_OUTPUT
	cout << "Input Matrix 1D: \n"; Util::print(numRows, numRows, A);
#endif


	// Carry out LU Factorization
	integer info; dgetrf_ (&numRows, &numRows, A, &numRows, ipvt, &info);
	if (info < 0) 
		throw ApplicationException("Error in dgetrf : LU Factorization", "Illegal Value");
	if (info > 0) 
		throw ApplicationException("Exception in LIB_LA while computing Inverse", "Input Matrix is Singular.");


#ifdef ENABLE_DEBUG_OUTPUT
	cout << "After dgetrf: \n"; Util::print(numRows, numRows, A);
#endif

	dgetri_ (&numRows, A, &numRows, ipvt, work, &numRows, &info);

#ifdef ENABLE_DEBUG_OUTPUT
	cout << "After dgetri: \n"; Util::print(numRows, numRows, A);
#endif

	oResultMatrix = new DoubleMatrix(A,numRows,numRows, true);
	Util::RoundMatrixToTolerance(*oResultMatrix, _Tolerance);

	delete [] A; delete [] ipvt; delete [] work; 

	return oResultMatrix;
}

ComplexMatrix* LibLA::Zinverse (ComplexMatrix &oMatrix)
{
#ifdef ENABLE_DEBUG_OUTPUT
	cout << "======================================================" << endl;
	cout << "=== Zinverse " << endl;
	cout << "======================================================" << endl;
#endif

	ComplexMatrix *oResultMatrix = NULL;
	integer numRows = oMatrix.numRows();
	integer numCols = oMatrix.numCols();		

	if (numRows != numCols)
		throw ApplicationException ("Input Matrix must be square", "Expecting a Square Matrix");


	doublecomplex* A = new doublecomplex[numRows*numRows];
	for (int i=0; i<numRows; i++) 
	{
		for (int j=0; j<numRows; j++) 
		{
			A[i+numRows*j].r = oMatrix(i,j).getReal();
			A[i+numRows*j].i = oMatrix(i,j).getImag();
		}
	}

#ifdef ENABLE_DEBUG_OUTPUT
	//cout << "Input Matrix 1D: \n";
	//Util::print(numRows, numRows, A);
#endif

	integer* ipvt = new integer[numRows];				memset(ipvt,0,sizeof(integer)*numRows);
	doublecomplex* work = new doublecomplex[numRows];	memset(work,0,sizeof(doublecomplex)*numRows);

	// Carry out LU Factorization
	integer info; zgetrf_ (&numRows, &numRows, A, &numRows, ipvt, &info);

	if (info < 0) 
		throw ApplicationException("Error in dgetrf : LU Factorization", "Illegal Value");

	if (info > 0) 
		throw ApplicationException("Exception in LIB_LA while computing Inverse", "Input Matrix is Sinuglar.");

#ifdef ENABLE_DEBUG_OUTPUT
	//cout << "After dgetrf: \n";
	//Util::print(numRows, numRows, A);
#endif

	zgetri_ (&numRows, A, &numRows, ipvt, work, &numRows, &info);

#ifdef ENABLE_DEBUG_OUTPUT
	cout << "After dgetri: \n";
	//Util::print(numRows, numRows, A);
#endif


	oResultMatrix = new ComplexMatrix(numRows,numRows);
	for (int i=0; i<numRows; i++) 
	{
		for (int j=0; j<numRows; j++) 
		{
			(*oResultMatrix )(i,j).Real = Util::RoundToTolerance( A[(i+numRows*j)].r, _Tolerance);
			(*oResultMatrix )(i,j).Imag = Util::RoundToTolerance( A[(i+numRows*j)].i, _Tolerance);
		}
	}

	// free memory
	delete [] A; delete [] ipvt; delete [] work; 

	return oResultMatrix;
}



LibLA* LibLA::getInstance()
{
	if (_Instance == NULL)
		_Instance = new LibLA();
	return _Instance;
}


// static variable definitions
LibLA* LibLA::_Instance = NULL;


// ---------------------------------------------------------------------------------------------
/// the C-API
// ---------------------------------------------------------------------------------------------

LIB_EXTERN double LibLA_getTolerance()
{
	return LibLA::getInstance()->getTolerance();
}

LIB_EXTERN void LibLA_setTolerance(const double value)
{
	LibLA::getInstance()->setTolerance(value);
}

LIB_EXTERN int LibLA_getEigenValues(double** inMatrix, int numRows, int numCols, 
									double* *outReal, double * *outImag, int *outLength)
{
	try
	{
		DoubleMatrix oMatrix(inMatrix, numRows, numCols);
		vector< Complex > oVector = LibLA::getInstance()->getEigenValues( oMatrix );

		Util::CopyComplexVector(oVector, *outReal, *outImag, *outLength);

		return 0;
	}
	catch(...)
	{
		return -1;
	}
}

LIB_EXTERN int LibLA_ZgetEigenValues(double** inMatrixReal, double** inMatrixImag, int numRows, int numCols, 
									 double* *outReal, double * *outImag, int *outLength)
{
	try
	{
		ComplexMatrix oMatrix(numRows, numCols);

		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numCols; j++)
			{				
				oMatrix(i,j).set( inMatrixReal[i][j],inMatrixImag[i][j]);
			}
		}

		vector< Complex > oVector = LibLA::getInstance()->ZgetEigenValues( oMatrix );

		Util::CopyComplexVector(oVector, *outReal, *outImag, *outLength);

		oVector.clear();
		return 0;
	}
	catch(...)
	{
		return -1;
	}
}

LIB_EXTERN int LibLA_getQRWithPivot(double** inMatrix, int numRows, int numCols, 
									double** *outQ, int *outQRows, int * outQCols, 
									double** *outR, int *outRRows, int * outRCols, 
									double** *outP, int *outPRows, int * outPCols)
{
	DoubleMatrix oMatrix(inMatrix, numRows, numCols);
	vector< DoubleMatrix * > oVector = LibLA::getInstance()->getQRWithPivot( oMatrix );

	Util::CopyMatrix(*oVector[0], *outQ, *outQRows, *outQCols); delete oVector[0];
	Util::CopyMatrix(*oVector[1], *outR, *outRRows, *outRCols); delete oVector[1];
	Util::CopyMatrix(*oVector[2], *outP, *outPRows, *outPCols); delete oVector[2];


	return 0;
}

LIB_EXTERN int LibLA_getQR(double** inMatrix, int numRows, int numCols, 
						   double** *outQ, int *outQRows, int * outQCols, 
						   double** *outR, int *outRRows, int * outRCols)
{
	DoubleMatrix oMatrix(inMatrix, numRows, numCols);
	vector< DoubleMatrix * > oVector = LibLA::getInstance()->getQR( oMatrix );

	Util::CopyMatrix(*oVector[0], *outQ, *outQRows, *outQCols); delete oVector[0];
	Util::CopyMatrix(*oVector[1], *outR, *outRRows, *outRCols); delete oVector[1];

	return 0;
}

LIB_EXTERN int LibLA_getSingularValsBySVD(double** inMatrix, int numRows, int numCols, 
										  double* *outSingularVals, int *outLength)
{
	DoubleMatrix oMatrix(inMatrix, numRows, numCols);
	vector< double > oVector = LibLA::getInstance()->getSingularValsBySVD( oMatrix );

	Util::CopyDoubleVector(oVector, *outSingularVals, *outLength);

	return 0;
}

LIB_EXTERN int LibLA_getLUwithFullPivoting(double** inMatrix, int numRows, int numCols, 
										   double** *outL, int *outLRows, int * outLCols, 
										   double** *outU, int *outURows, int * outUCols, 
										   int** *outP, int *outPRows, int * outPCols, 
										   int** *outQ, int *outQRows, int * outQCols, 
										   int *info)
{

	try
	{
		DoubleMatrix oMatrix(inMatrix, numRows, numCols);
		LU_Result* oResult = LibLA::getInstance()->getLUwithFullPivoting( oMatrix );

		Util::CopyMatrix(*(oResult->L), *outL, *outLRows, *outLCols);
		Util::CopyMatrix(*(oResult->U), *outU, *outURows, *outUCols);
		Util::CopyMatrix(*(oResult->P), *outP, *outPRows, *outPCols);
		Util::CopyMatrix(*(oResult->Q), *outQ, *outQRows, *outQCols);

		*info = oResult->nInfo;

		delete oResult;

		return 0;
	}
	catch(...)
	{
		return -1;
	}
}

LIB_EXTERN int LibLA_getLU(double** inMatrix, int numRows, int numCols, 
						   double** *outL, int *outLRows, int * outLCols, 
						   double** *outU, int *outURows, int * outUCols, 
						   int** *outP, int *outPRows, int * outPCols, 										 
						   int *info)
{
	DoubleMatrix oMatrix(inMatrix, numRows, numCols);
	LU_Result* oResult = LibLA::getInstance()->getLU( oMatrix );

	Util::CopyMatrix(*(oResult->L), *outL, *outLRows, *outLCols);
	Util::CopyMatrix(*(oResult->U), *outU, *outURows, *outUCols);
	Util::CopyMatrix(*(oResult->P), *outP, *outPRows, *outPCols);

	*info = oResult->nInfo;

	delete oResult;
	return 0;
}

LIB_EXTERN int LibLA_inverse(double** inMatrix, int numRows, int numCols, 
							 double** *outMatrix, int *outRows, int *outCols)
{

	try
	{
		DoubleMatrix oMatrix(inMatrix, numRows, numCols);
		DoubleMatrix *oResult = LibLA::getInstance()->inverse( oMatrix );

		Util::CopyMatrix(*oResult, *outMatrix, *outRows, *outCols); delete oResult;

		return 0;
	}
	catch (...)
	{
		return -1;
	}
}


LIB_EXTERN int LibLA_leftNullspace(double** inMatrix, int numRows, int numCols, 
								   double** *outMatrix, int *outRows, int *outCols)
{
	DoubleMatrix oMatrix(inMatrix, numRows, numCols);
	DoubleMatrix *oResult = LibLA::getInstance()->getLeftNullSpace( oMatrix );

	Util::CopyMatrix(*oResult, *outMatrix, *outRows, *outCols); delete oResult;

	return 0;
}
LIB_EXTERN int LibLA_rightNullspace(double** inMatrix, int numRows, int numCols, 
									double** *outMatrix, int *outRows, int *outCols)
{
	DoubleMatrix oMatrix(inMatrix, numRows, numCols);
	DoubleMatrix *oResult = LibLA::getInstance()->getRightNullSpace( oMatrix );

	Util::CopyMatrix(*oResult, *outMatrix, *outRows, *outCols); delete oResult;

	return 0;
}

LIB_EXTERN int LibLA_scaledLeftNullspace(double** inMatrix, int numRows, int numCols, 
										 double** *outMatrix, int *outRows, int *outCols)
{
	DoubleMatrix oMatrix(inMatrix, numRows, numCols);
	DoubleMatrix *oResult = LibLA::getInstance()->getScaledLeftNullSpace( oMatrix );

	Util::CopyMatrix(*oResult, *outMatrix, *outRows, *outCols); delete oResult;

	return 0;
}
LIB_EXTERN int LibLA_scaledRightNullspace(double** inMatrix, int numRows, int numCols, 
										  double** *outMatrix, int *outRows, int *outCols)
{
	DoubleMatrix oMatrix(inMatrix, numRows, numCols);
	DoubleMatrix *oResult = LibLA::getInstance()->getScaledRightNullSpace( oMatrix );

	Util::CopyMatrix(*oResult, *outMatrix, *outRows, *outCols); delete oResult;

	return 0;
}

LIB_EXTERN int LibLA_getRank(double** inMatrix, int numRows, int numCols)
{
	DoubleMatrix oMatrix(inMatrix, numRows, numCols);
	int nRank = LibLA::getInstance()->getRank( oMatrix );
	return nRank;
}


LIB_EXTERN double LibLA_getRCond(double** inMatrix, int numRows, int numCols)
{
	DoubleMatrix oMatrix(inMatrix, numRows, numCols);
	double dRcond = LibLA::getInstance()->getRCond( oMatrix );
	return dRcond;
}


LIB_EXTERN int LibLA_Zinverse(double** inMatrixReal, double **inMatrixImag, int numRows, int numCols, 
							  double** *outMatrixReal, double ** *outMatrixImag, int *outRows, int *outCols)
{
	try
	{
		ComplexMatrix oMatrix(numRows, numCols);

		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numCols; j++)
			{
				oMatrix(i,j).set(inMatrixReal[i][j],inMatrixImag[i][j]);				
			}
		}

		ComplexMatrix *oResult = LibLA::getInstance()->Zinverse( oMatrix );
		Util::CopyMatrix(*oResult, *outMatrixReal, *outMatrixImag, *outRows, *outCols); delete oResult;

		return 0;
	}
	catch(...)
	{
		return -1;
	}
}

LIB_EXTERN void LibLA_freeVector(void* vector)
{
	if (vector) free(vector);
}

LIB_EXTERN void LibLA_freeMatrix(void** matrix, int numRows)
{
	for (int i = 0; i < numRows; i++)
	{
		if (matrix[i]) free(matrix[i]);
	}
	free(matrix);
}



LIB_EXTERN int LibLA_gaussJordan(double** inMatrix, int numRows, int numCols, 
								 double** *outMatrix, int *outRows, int *outCols, int* *oPivots, int *nLength)
{
	DoubleMatrix oMatrix(inMatrix, numRows, numCols);
	vector<int> oPivotVec  = Util::GaussJordan(oMatrix, LibLA::getInstance()->getTolerance());
	Util::CopyMatrix(oMatrix, *outMatrix, *outRows, *outCols);

	Util::CopyIntVector(oPivotVec, *oPivots, *nLength);
	return 0;
}

std::vector<int> LibLA::gaussJordan(DoubleMatrix &oMatrix)
{
	return Util::GaussJordan(oMatrix, _Tolerance);
}



void LibLA::fullyPivotedGaussJordan(DoubleMatrix &oMatrix, vector<int> &rowPivots, vector<int> &colPivots)
{
	return Util::FullyPivotedGaussJordan(oMatrix, _Tolerance, rowPivots, colPivots);
}


LIB_EXTERN int LibLA_fullyPivotedGaussJordan(double** inMatrix, int numRows, int numCols, 
											 double** *outMatrix, int *outRows, int *outCols, 
											 int* *oRowPivots, int *nRowLength, 
											 int* *oColPivots, int *nColLength)
{

	DoubleMatrix oMatrix(inMatrix, numRows, numCols);
	vector< int > vecRowPivots;  vector< int> vecColPivots;
	Util::FullyPivotedGaussJordan(oMatrix, LibLA::getInstance()->getTolerance(), vecRowPivots, vecColPivots);

	Util::CopyMatrix(oMatrix, *outMatrix, *outRows, *outCols);

	Util::CopyIntVector(vecRowPivots, *oRowPivots, *nRowLength);
	Util::CopyIntVector(vecColPivots, *oColPivots, *nColLength);

	return 0;

}


LIB_EXTERN int LibLA_getEigenVectors(double** inMatrix, int numRows, int numCols, 
									 double** *outMatrixReal, double** *outMatrixImag, int *outRows, int *outCols)
{

	try
	{
		DoubleMatrix oMatrix(inMatrix, numRows, numCols);
		ComplexMatrix *oResult = LibLA::getInstance()->getEigenVectors( oMatrix );

		Util::CopyMatrix(*oResult, *outMatrixReal, *outMatrixImag, *outRows, *outCols); delete oResult;

		return 0;
	}
	catch (...)
	{
		return -1;
	}
}

LIB_EXTERN int LibLA_ZgetEigenVectors(double** inMatrixReal,   double **  inMatrixImag, int numRows, int numCols, 
									  double** *outMatrixReal, double** *outMatrixImag, int *outRows, int *outCols)
{
	try
	{
		ComplexMatrix oMatrix(numRows, numCols);

		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numCols; j++)
			{
				oMatrix(i,j).set(inMatrixReal[i][j],inMatrixImag[i][j]);				
			}
		}

		ComplexMatrix *oResult = LibLA::getInstance()->ZgetEigenVectors( oMatrix );
		Util::CopyMatrix(*oResult, *outMatrixReal, *outMatrixImag, *outRows, *outCols); delete oResult;

		return 0;
	}
	catch(...)
	{
		return -1;
	}
}

LIB_EXTERN int LibLA_getSVD(double** inMatrix, int numRows, int numCols, 
							double** *outU, int *outRowsU, int *outColsU, 
							double* *outSingVals, int *outLength, 
							double** *outV, int *outRowsV, int *outColsV)
{
	try
	{
		DoubleMatrix oMatrix(inMatrix, numRows, numCols);
		
		DoubleMatrix*   matrixU  = NULL; 
		vector<double>* singVals = NULL; 
		DoubleMatrix*   matrixV  = NULL;
		
		LibLA::getInstance()->getSVD(oMatrix, matrixU, singVals, matrixV);

		Util::CopyMatrix(*matrixU, *outU, *outRowsU, *outColsU); delete matrixU;
		Util::CopyDoubleVector(*singVals, *outSingVals, *outLength); delete singVals;
		Util::CopyMatrix(*matrixV, *outV, *outRowsV, *outColsV); delete matrixV;

		return 0;
	}
	catch(...)
	{
		return -1;
	}
	
}

LIB_EXTERN int LibLA_ZgetSVD(double** inMatrixReal, double** inMatrixImag, int numRows, int numCols, 
							 double** *outUReal, double** *outUImag, int *outRowsU, int *outColsU, 
							 double* *outSingVals, int *outLength, 
							 double** *outVReal, double** *outVImag, int *outRowsV, int *outColsV)
{
	try
	{
		ComplexMatrix oMatrix(numRows, numCols);

		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j < numCols; j++)
			{
				oMatrix(i,j).set(inMatrixReal[i][j],inMatrixImag[i][j]);				
			}
		}

		ComplexMatrix *matrixU = NULL; vector<double>* singVals= NULL; ComplexMatrix *matrixV= NULL;
		
		LibLA::getInstance()->ZgetSVD(oMatrix, matrixU, singVals, matrixV);

		Util::CopyMatrix(*matrixU, *outUReal, *outUImag, *outRowsU, *outColsU); delete matrixU;
		Util::CopyDoubleVector(*singVals, *outSingVals, *outLength); delete singVals;
		Util::CopyMatrix(*matrixV, *outVReal, *outVImag, *outRowsV, *outColsV); delete matrixV;
	
		return 0;
	}
	catch(...)
	{
		return -1;
	}
}


