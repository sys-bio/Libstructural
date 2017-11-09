#include <stdio.h>           // for printf
#include <stdlib.h>          // for malloc
#include <string.h>          // for memset
#include <libstructural.h>   // the structural analysis library

// construct simple stoichiometry matrix
void GetMatrixFromSomeWhere(double** *oMatrix, int *nRows, int *nCols)
{
	int numCols, numRows, i;
	numRows = 3; numCols = 6;

	// initialize memory needed
	*oMatrix = (double**)malloc(sizeof(double*)*numRows);
	memset(*oMatrix, 0, sizeof(double*)*numRows);

	for (i = 0; i < numRows; i++)
	{
		(*oMatrix)[i] = (double*)malloc(sizeof(double)*numCols);
		memset((*oMatrix)[i], 0, sizeof(double)*numCols);
	}

	// set non zero entries of the stoichiometry matrix
	//(*oMatrix)[0][1] = -1.0;    (*oMatrix)[0][2] = 1.0;    // ES
	//(*oMatrix)[1][0] = 1.0;    (*oMatrix)[1][2] = -1.0;    // S2
	//(*oMatrix)[2][0] = -1.0;    (*oMatrix)[2][1] = 1.0;    // S1
	//(*oMatrix)[3][1] = 1.0;    (*oMatrix)[3][2] = -1.0;    // E
														   // be sure to return number of rows and columns
	//[[0, 3, -6, 6, 4, -5], [3, -7, 8, -5, 8, 9], [3, -9, 12, -9, 6, 16]]
														   // set non zero entries of the stoichiometry matrix
	(*oMatrix)[0][0] = 0;   (*oMatrix)[0][1] = 3;   (*oMatrix)[0][2] = -6;  (*oMatrix)[0][3] = 6;  (*oMatrix)[0][4] = 4; (*oMatrix)[0][5] = -5; 
	(*oMatrix)[1][0] = 3;   (*oMatrix)[1][1] = -7;   (*oMatrix)[1][2] = 8;  (*oMatrix)[1][3] = -5;  (*oMatrix)[1][4] = 8; (*oMatrix)[1][5] = 9;
	(*oMatrix)[2][0] = 3;   (*oMatrix)[2][1] = -9;   (*oMatrix)[2][2] = 12;  (*oMatrix)[2][3] = -9;  (*oMatrix)[2][4] = 6; (*oMatrix)[2][5] = 16;


	*nRows = numRows;
	*nCols = numCols;
}

int main(int argc, char** argv)
{
	int      i;
	int      nRows;
	int      nCols;
	double** oMatrix;
	char*    sMessage;
	int      nLength;
	int      outRows;
	int      outCols;
	double** outMatrix;
	LIB_STRUCTURAL::LibStructural *ls;
	LIB_LA::DoubleMatrix m;
	LIB_LA::DoubleMatrix *m2;


	// get matrix to analyze from another part of the code
	GetMatrixFromSomeWhere(&oMatrix, &nRows, &nCols);

	printf("Show matrix:\n");
	m = LIB_LA::DoubleMatrix(nRows, nCols);
	for (int i = 0; i < nRows; i++) {
		for (int j = 0; j < nCols; j++) {
			printf("%f ", oMatrix[i][j]);
			(m)(i, j) = oMatrix[i][j];
		}
		printf("\n");
	}


	ls = LIB_STRUCTURAL::LibStructural::getInstance();

	for (int i = 0; i < 20; i++) {
		printf("Matrix: %d\n", i);
		m2 = ls->rref(m, 1e-6);

		for (int i = 0; i < nRows; i++) {
			for (int j = 0; j < nCols; j++) {
				printf("%f ", (*m2)(i, j));
			}
			printf("\n");
		}
		printf("-------------------------------\n");
	}


//	FILE *fp;
//	char ch;
//	char *txt;
//	int len = 0;
//	int sz;
//
//	fp = fopen("C:\\Users\\yosef\\Documents\\Visual Studio 2017\\Projects\\structMain\\sbml_test_models\\test_model.txt", "r");
//	fseek(fp, 0L, SEEK_END);
//	sz = ftell(fp);
//	rewind(fp);
//	txt = (char *) malloc(sizeof(char*)*(sz + 1));
//
//	ch = fgetc(fp);
//	while (ch != EOF)
//	{
//		txt[len] = ch;
//		len++;
//		ch = fgetc(fp);
//	}
//	txt[len] = NULL;
//	
//	puts(txt);
//	fclose(fp);
//	
//	LibStruct::getInstance().loadSBMLFromString("abc");
//	if (LibStructural_loadSBML(txt, &sMessage, &nLength) == -1) {
//		printf("Error while reading SBML\n");
//		getchar();
//		exit(0);
//	}
//	else {
//		printf("Success\n\n");
//	}
//
//	LibStructural_getStoichiometryMatrix(&outMatrix, &outRows, &outCols);
//
//	// load it into the structural analysis library
//	//LibStructural_loadStoichiometryMatrix(oMatrix, nRows, nCols);
//
//	// analyze the stoichiometry matrix using the QR method
//	LibStructural_analyzeWithQR(&sMessage, &nLength);
//
//    //LibStructural_getL0Matrix(&outMatrix, &outRows, &outCols);
//	
//	//printf("L0 matrix = \n");
//	 //for (int i = 0; i < outRows; i++) {
//		// for (int j = 0; j < outCols; j++)
//		// printf("%f ", outMatrix[i][j]);
//		// printf("\n");
//	// }
//	// for (i = 0; i < outRows; i++)
//	//	 free(outMatrix[i]);
//	 //free(outMatrix);
//
//	 //LibStructural_getNrMatrix(&outMatrix, &outRows, &outCols);
//	 
//	 //printf("Nr matrix = \n");
//	// for (int i = 0; i < outRows; i++) {
//		 //for (int j = 0; j < outCols; j++)
//		//	 printf("%f ", outMatrix[i][j]);
//		// printf("\n");
//	// }
//
//	// for (i = 0; i < outRows; i++)
//	//		 free(outMatrix[i]);
//	// free(outMatrix);*/
//
//
//	// print model overview
//	printf("%s", sMessage);
//
//	// free the memory used by the message
//	//LibStructural_freeVector(sMessage);
//
//	// obtain and print the test results
//	//LibStructural_getTestDetails(&sMessage, &nLength);
//	//printf("%s", sMessage);
//
//	// finally free the memory used by the message
//	//LibStructural_freeVector(sMessage);
//
//	// and free the memory used to hold the stoichiometry matrix
////	for (i = 0; i < nRows; i++)
//		//free(oMatrix[i]);
//	//free(oMatrix);

	getchar();

	return 0;

}

//The program above returns the following output: 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//STRUCTURAL ANALYSIS MODULE : Results 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Size of Stochiometric Matrix: 4 x 3 (Rank is  2)
//Nonzero entries in Stochiometric Matrix: 8  (66.6667% full)
//
//Independent Species (2) :
//0, 1
//
//Dependent Species (2) :
//2, 3
//
//L0 : There are 2 dependencies. L0 is a 2x2 matrix.
//
//Conserved Entities
//1:  + 0 + 1 + 2
//2:  + 0 + 3
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Developed by the Computational Systems Biology Group at Keck Graduate Institute 
//and the Saurolab at the Bioengineering Departmant at  University of Washington.
//Contact : Frank T. Bergmann (fbergman@u.washington.edu) or Herbert M. Sauro.   
//
//(previous authors) Ravishankar Rao Vallabhajosyula                   
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
//Testing Validity of Conservation Laws.
//
//Passed Test 1 : Gamma*N = 0 (Zero matrix)
//Passed Test 2 : Rank(N) using SVD (2) is same as m0 (2)
//Passed Test 3 : Rank(NR) using SVD (2) is same as m0 (2)
//Passed Test 4 : Rank(NR) using QR (2) is same as m0 (2)
//Passed Test 5 : L0 obtained with QR matches Q21*inv(Q11)
//Passed Test 6 : N*K = 0 (Zero matrix)
