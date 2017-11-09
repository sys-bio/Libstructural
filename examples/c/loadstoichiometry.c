#include <stdio.h>           // for printf
#include <stdlib.h>          // for malloc
#include <string.h>          // for memset
#include <libstructural.h>   // the structural analysis library

// construct simple stoichiometry matrix
void GetMatrixFromSomeWhere(double** *oMatrix, int *nRows, int *nCols)
{
        int numCols, numRows, i;
        numRows = 4; numCols = 3;
        
        // initialize memory needed
        *oMatrix = (double**)malloc(sizeof(double*)*numRows); 
        memset(*oMatrix, 0, sizeof(double*)*numRows);
        
        for (i = 0; i < numRows; i ++)
        {
                (*oMatrix)[i] = (double*)malloc(sizeof(double)*numCols); 
                memset((*oMatrix)[i], 0, sizeof(double)*numCols);
        }
        
        // set non zero entries of the stoichiometry matrix
        (*oMatrix)[0][1] = -1.0;    (*oMatrix)[0][2] =  1.0;    // ES
        (*oMatrix)[1][0] =  1.0;    (*oMatrix)[1][2] = -1.0;    // S2
        (*oMatrix)[2][0] = -1.0;    (*oMatrix)[2][1] =  1.0;    // S1
        (*oMatrix)[3][1] =  1.0;    (*oMatrix)[3][2] = -1.0;    // E
        
        
        // be sure to return number of rows and columns
        *nRows = numRows;
        *nCols = numCols;
}

int main (int argc, char** argv)
{
        int      i;
        int      nRows;
        int      nCols;
        double** oMatrix;
        char*    sMessage;
        int      nLength;
        
        
        // get matrix to analyze from another part of the code
        GetMatrixFromSomeWhere(&oMatrix, &nRows, &nCols);
        
        // load it into the structural analysis library
        LibStructural_loadStoichiometryMatrix (oMatrix, nRows, nCols);
        
        // analyze the stoichiometry matrix using the QR method
        LibStructural_analyzeWithQR( &sMessage, &nLength);
        
        // print model overview
        printf("%s", sMessage);
        
        // free the memory used by the message
        LibStructural_freeVector(sMessage);
        
        // obtain and print the test results
        LibStructural_getTestDetails( &sMessage, &nLength );
        printf("%s", sMessage);
        
        // finally free the memory used by the message
        LibStructural_freeVector(sMessage);
        
        // and free the memory used to hold the stoichiometry matrix
        for (i = 0; i < nRows; i++)
                free(oMatrix[i]);
        free(oMatrix);
        
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
