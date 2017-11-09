#include <stdio.h>           // for printf
#include <stdlib.h>          // for malloc
#include <string.h>          // for memset
#include <libstructural.h>   // the structural analysis library

// construct simple matrix
void GetMatrixFromSomeWhere(double** *oMatrix, int *nRows, int *nCols)
{
	int numCols, numRows, i;
	numRows = 5; numCols = 3;
	
	// initialize memory needed
	*oMatrix = (double**)malloc(sizeof(double*)*numRows); 
	memset(*oMatrix, 0, sizeof(double*)*numRows);
	
	for (i = 0; i < numRows; i ++)
	{
		(*oMatrix)[i] = (double*)malloc(sizeof(double)*numCols); 
		memset((*oMatrix)[i], 0, sizeof(double)*numCols);
	}
		
	// set non zero entries of the matrix
	(*oMatrix)[0][0] =  17.0;    (*oMatrix)[0][1] = -17.0;   (*oMatrix)[0][2] =  34.0; 
	(*oMatrix)[1][0] =   3.0;    (*oMatrix)[1][1] =  -3.0;   (*oMatrix)[1][2] =   6.0;
	(*oMatrix)[2][0] =  11.0;    (*oMatrix)[2][1] =   8.0;   (*oMatrix)[2][2] =   3.0;
	(*oMatrix)[3][0] =   5.0;    (*oMatrix)[3][1] =  -3.0;   (*oMatrix)[3][2] =   8.0;
	(*oMatrix)[4][0] =  13.0;    (*oMatrix)[4][1] =  -6.0;   (*oMatrix)[4][2] =  19.0;	
	
	// be sure to return number of rows and columns
	*nRows = numRows;
	*nCols = numCols;
}

void PrintDoubleMatrix(double** matrix, int numRows, int numCols)
{
	int i;
	int j;
	for (i = 0; i < numRows; i++) {
		for (j = 0; j < numCols; j++) {
			printf ("%3.5lf\t", matrix[i][j]);
		}
		printf ("\n");
	}
}

void PrintIntMatrix(int** matrix, int numRows, int numCols)
{
	int i; 
	int j;
	
	for (i = 0; i < numRows; i++) {
		for (j = 0; j < numCols; j++) {
			printf ("%d\t", matrix[i][j]);
		}
		printf ("\n");
	}
}

int main(int argc, char*argv[])
{
	int      nRows;
	int      nCols;
	double** oMatrix;
	
	int		nLRows; int nLCols; double** dL;
	int		nURows; int nUCols; double** dU;
	int		nPRows; int nPCols; int** dP;
	
	int		nInfo;
	
	GetMatrixFromSomeWhere(&oMatrix, &nRows, &nCols);

	LibLA_getLU(oMatrix, nRows, nCols, 
				&dL, &nLRows, &nLCols, 
				&dU, &nURows, &nUCols, 
				&dP, &nPRows, &nPCols, &nInfo);
		
	printf("L matrix \n");
	PrintDoubleMatrix(dL, nLRows, nLCols);

	printf("U matrix \n");
	PrintDoubleMatrix(dU, nURows, nUCols);

	printf("P matrix \n");
	PrintIntMatrix(dP, nPRows, nPCols);

	printf("Info: %d", nInfo);
	
	return 0;
}
