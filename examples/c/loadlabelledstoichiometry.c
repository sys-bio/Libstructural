#include <stdio.h>           // for printf
#include <stdlib.h>          // for malloc
#include <string.h>          // for memset
#include <libstructural.h>   // the structural analysis library

// construct simple stoichiometry matrix with labelled species and reactions
void GetMatrixFromSomeWhere(double** *oMatrix, int *nRows, int *nCols, 
         char** *speciesNames, double* *initialConcentrations, 
         char** *reactionNames);

// gets the reordered stoichiometry matrix from the library
void PrintReorderedStoichiometryMatrix();

// gets the gamma matrix from the librar
void PrintGammaMatrix();


int main (int argc, char** argv)
{
        int      i,j;
        int      nRows;
        int      nCols;
        double** oMatrix;
        char*    sMessage;
        char**   speciesNames;
        char**   reactionNames;
        double*  initialConcentrations;
        int      nLength;
                
        
        // get matrix to analyze from another part of the code as well 
        // as species and reaction names
        GetMatrixFromSomeWhere(&oMatrix, &nRows, &nCols, 
               &speciesNames, &initialConcentrations, &reactionNames);
        
        
        // load matrix into the structural analysis library
        LibStructural_loadStoichiometryMatrix (oMatrix, nRows, nCols);
        // load species names and initial concentrations
        LibStructural_loadSpecies(speciesNames, initialConcentrations, nRows);
        // load reaction names
        LibStructural_loadReactionNames(reactionNames, nCols);
        
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
        
        
        // Print Reordered Stoichiometry Matrix
        PrintReorderedStoichiometryMatrix();
        
        // Print Gamma Matrix
        PrintGammaMatrix();
        
                        
        // and free the memory used to hold the stoichiometry matrix
        for (i = 0; i < nRows; i++)
                free(oMatrix[i]);
        free(oMatrix);
        
        // free species names
        for (i = 0; i < nRows; i++)
                free(speciesNames[i]);
        free(speciesNames);

        // free reaction names
        for (i = 0; i < nCols; i++)
                free(reactionNames[i]);
        free(reactionNames);
        
        return 0;
        
}

void PrintReorderedStoichiometryMatrix()
{
        int      i,j;
        double** reorderedStoichiometryMatrix;
        int      reorderedNumRows;
        int      reorderedNumCols;
        char**   reorderedCols;
        char**   reorderedRows;
        
        printf("\nReordered Stoichiometry Matrix");

        // get Reordered Stoichiometry Matrix
        LibStructural_getReorderedStoichiometryMatrix
                (&reorderedStoichiometryMatrix, &reorderedNumRows, &reorderedNumCols);
        LibStructural_getReorderedStoichiometryMatrixLabels
                (&reorderedRows, &reorderedNumRows, &reorderedCols, &reorderedNumCols);
        
        // print Reordered stoichiometry matrix:
        printf("\n\t"); 
        for (i = 0; i < reorderedNumCols; i++)
                printf("%s\t", reorderedCols[i]);
        printf("\n");
        
        for (i = 0; i < reorderedNumRows; i++)
        {
                printf("%s\t", reorderedRows[i]);
                for (j = 0; j < reorderedNumCols; j++)
                        printf ("%2.1lf\t", reorderedStoichiometryMatrix[i][j]);
                printf("\n");
        }
                           
        // free reordered stoichiometry matrix and labels
        LibStructural_freeMatrix((void**)reorderedStoichiometryMatrix, reorderedNumRows);
        LibStructural_freeMatrix((void**)reorderedCols, reorderedNumCols);
        LibStructural_freeMatrix((void**)reorderedRows, reorderedNumRows);

        printf("\n");
}

void PrintGammaMatrix()
{
        int      i,j;   
        double** gammaMatrix;
        int      gammaNumRows;
        int      gammaNumCols;
        char**   gammaCols;
        char**   gammaRows;

        
        printf("\nGamma Matrix");
        
        // get Gamma Matrix and labels
        LibStructural_getGammaMatrix
                (&gammaMatrix, &gammaNumRows, &gammaNumCols);
        LibStructural_getGammaMatrixLabels
                (&gammaRows, &gammaNumRows, &gammaCols, &gammaNumCols);
        
        // print gamma matrix:
        printf("\n\t"); 
        for (i = 0; i < gammaNumCols; i++)
                printf("%s\t", gammaCols[i]);
        printf("\n");
        
        for (i = 0; i < gammaNumRows; i++)
        {
                printf("%s\t", gammaRows[i]);
                for (j = 0; j < gammaNumCols; j++)
                        printf ("%2.1lf\t", gammaMatrix[i][j]);
                printf("\n");
        }
                           
        // free gamma stoichiometry matrix and labels
        LibStructural_freeMatrix((void**)gammaMatrix, gammaNumRows);
        LibStructural_freeMatrix((void**)gammaCols, gammaNumCols);
        LibStructural_freeMatrix((void**)gammaRows, gammaNumRows);
        
        printf("\n");

}


void GetMatrixFromSomeWhere(double** *oMatrix, int *nRows, int *nCols, 
         char** *speciesNames, double* *initialConcentrations, 
         char** *reactionNames)
{
        int numCols, numRows, i;
        numRows = 4; numCols = 3;
        
        // initialize memory needed for the stoichiometry matrix
        *oMatrix = (double**)malloc(sizeof(double*)*numRows); 
        memset(*oMatrix, 0, sizeof(double*)*numRows);
        
        for (i = 0; i < numRows; i ++)
        {
                (*oMatrix)[i] = (double*)malloc(sizeof(double)*numCols); 
                memset((*oMatrix)[i], 0, sizeof(double)*numCols);
        }
        
        // initialize memory needed for speciesNames
        (*speciesNames) = (char**)malloc(sizeof(char*)*numRows); 
        memset(*speciesNames, 0, sizeof(char*)*numRows);
        
        *initialConcentrations = (double*)malloc(sizeof(double)*numRows); 
        memset(*initialConcentrations, 0, sizeof(double)*numRows);
                
        // initialize memory needed for reactionNames
        (*reactionNames) = (char**)malloc(sizeof(char*)*numCols); 
        memset(*reactionNames, 0, sizeof(char*)*numCols);
        
        // set non zero entries of the stoichiometry matrix
        (*oMatrix)[0][1] = -1.0;        (*oMatrix)[0][2] =  1.0;      // ES
        (*oMatrix)[1][0] =  1.0;        (*oMatrix)[1][2] = -1.0;      // S2
        (*oMatrix)[2][0] = -1.0;        (*oMatrix)[2][1] =  1.0;      // S1
        (*oMatrix)[3][1] =  1.0;        (*oMatrix)[3][2] = -1.0;      // E
        
        // set species names
        (*speciesNames)[0] = strdup("S2");  (*speciesNames)[1] = strdup("ES");
        (*speciesNames)[2] = strdup("S1");  (*speciesNames)[3] = strdup("E");
        
        // set reaction names
        (*reactionNames)[0] = strdup("J1"); (*reactionNames)[1] = strdup("J2");     
        (*reactionNames)[2] = strdup("J3");     
        
        // be sure to return number of rows and columns
        *nRows = numRows;
        *nCols = numCols;
}



//The above returns the following output: 
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//STRUCTURAL ANALYSIS MODULE : Results 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Size of Stochiometric Matrix: 4 x 3 (Rank is  2)
//Nonzero entries in Stochiometric Matrix: 8  (66.6667% full)
//
//Independent Species (2) :
//S2, ES
//
//Dependent Species (2) :
//S1, E
//
//L0 : There are 2 dependencies. L0 is a 2x2 matrix.
//
//Conserved Entities
//1:  + S2 + ES + S1
//2:  + S2 + E
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
//
//Reordered Stoichiometry Matrix
//J1      J2      J3      
//S2      0.0     -1.0    1.0     
//ES      1.0     0.0     -1.0    
//S1      -1.0    1.0     0.0     
//E       0.0     1.0     -1.0    
//
//
//Gamma Matrix
//S2      ES      S1      E       
//0       1.0     1.0     1.0     0.0     
//1       1.0     -0.0    0.0     1.0     
//
