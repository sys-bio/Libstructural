#include <iostream>
#include <libstructural.h>
#include <matrix.h>
#include <string>
#include <vector>

using namespace std;
using namespace LIB_STRUCTURAL;
using namespace LIB_LA;

void PrintLabeledMatrix (DoubleMatrix &oMatrix, 
						 vector<string> &rowLabels, 
						 vector<string> &colLabels)
{
	// return if we don't have the right dimensions
	if (oMatrix.numCols() != colLabels.size() 
		|| oMatrix.numRows() != rowLabels.size())
		return;
	
	// otherwise print 
	vector<string>::iterator it;
	cout << "\t";
	// print column labels
	for (it = colLabels.begin(); it != colLabels.end(); it++)
		cout << *it << "\t";
	cout << endl;
	
	for (unsigned int i = 0; i < oMatrix.numRows(); i++)
	{
		cout << rowLabels[i] << "\t";
		for (unsigned int j = 0; j < oMatrix.numCols(); j++)
			cout << oMatrix(i,j) << "\t";
		cout << endl;
	}
	
}

void PrintFullyReorderedStoichiometry(LibStructural &instance)
{
	DoubleMatrix *oMatrix = instance.getFullyReorderedStoichiometryMatrix();
	vector<string> sRows; vector<string> sCols;
	instance.getFullyReorderedStoichiometryMatrixLabels(sRows, sCols);
	cout << endl << "Fully Reordered Stoichiometry Matrix" << endl;
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}

void PrintReorderedStoichiometry(LibStructural &instance)
{
	DoubleMatrix *oMatrix = instance.getReorderedStoichiometryMatrix();
	vector<string> sRows; vector<string> sCols;
	instance.getReorderedStoichiometryMatrixLabels(sRows, sCols);
	cout << endl << "Reordered Stoichiometry Matrix" << endl;
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}

void PrintStoichiometry(LibStructural &instance)
{
	DoubleMatrix *oMatrix = instance.getStoichiometryMatrix();
	vector<string> sRows; vector<string> sCols;
	instance.getStoichiometryMatrixLabels(sRows, sCols);
	cout << endl << "Stoichiometry Matrix" << endl;
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}

void PrintKMatrix(LibStructural &instance)
{
	DoubleMatrix *oMatrix = instance.getKMatrix();
	vector<string> sRows; vector<string> sCols;
	instance.getKMatrixLabels(sRows, sCols);
	cout << endl << "K Matrix" << endl;
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}
void PrintK0Matrix(LibStructural &instance)
{
	DoubleMatrix *oMatrix = instance.getK0Matrix();
	vector<string> sRows; vector<string> sCols;
	instance.getK0MatrixLabels(sRows, sCols);
	cout << endl << "K0 Matrix" << endl;
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}
void PrintLinkMatrix(LibStructural &instance)
{
	DoubleMatrix *oMatrix = instance.getLinkMatrix();
	vector<string> sRows; vector<string> sCols;
	instance.getLinkMatrixLabels(sRows, sCols);
	cout << endl << "L Matrix" << endl;
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}
void PrintL0Matrix(LibStructural &instance)
{
	DoubleMatrix *oMatrix = instance.getL0Matrix();
	vector<string> sRows; vector<string> sCols;
	instance.getL0MatrixLabels(sRows, sCols);
	cout << endl << "L0 Matrix" << endl;
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}

// print a couple of matrices
void PrintMatrices(LibStructural &instance)
{
	PrintFullyReorderedStoichiometry(instance);		
	PrintReorderedStoichiometry(instance);		
	PrintStoichiometry(instance);		
	PrintKMatrix(instance);		
	PrintK0Matrix(instance);		
	PrintLinkMatrix(instance);		
	PrintL0Matrix(instance);
	
	// and so on for Gamma Matrix, NDC, NIC, 
	// ColumnReorderedNr ... 
}

int main(int argc, char*argv[])
{
	if (argc != 2)
	{
		cerr << "Need one argument, full path to SBML file." << endl;
		return -1;
	}
	
	// get an instance of the library
	LibStructural* instance = LibStructural::getInstance();

	// print out model overview
	cout << instance->loadSBMLFromFile(argv[1]);
	
	// print test details
	cout << instance->getTestDetails();
	
	PrintMatrices(*instance);
	
	return 0;
}


// Running the program for BorisEJB.xml (available from the SBW 
// distribution) yields:
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//STRUCTURAL ANALYSIS MODULE : Results 
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//Size of Stochiometric Matrix: 8 x 10 (Rank is  5)
//Nonzero entries in Stochiometric Matrix: 20  (25% full)
//
//Independent Species (5) :
//MKK_P, MAPK_P, MKKK, MKK, MAPK
//
//Dependent Species (3) :
//MKK_PP, MKKK_P, MAPK_PP
//
//L0 : There are 3 dependencies. L0 is a 3x5 matrix.
//
//Conserved Entities
//1:  + MKK_P + MKK + MKK_PP
//2:  + MKKK + MKKK_P
//3:  + MAPK_P + MAPK + MAPK_PP
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
//Passed Test 2 : Rank(N) using SVD (5) is same as m0 (5)
//Passed Test 3 : Rank(NR) using SVD (5) is same as m0 (5)
//Passed Test 4 : Rank(NR) using QR (5) is same as m0 (5)
//Passed Test 5 : L0 obtained with QR matches Q21*inv(Q11)
//Passed Test 6 : N*K = 0 (Zero matrix)
//
//
//Reordered Stoichiometry Matrix
//        J0      J1      J2      J3      J4      J5      J6      J7      J8      J9      
//MKK_P   0       0       1       -1      1       -1      0       0       0       0       
//MAPK_P  0       0       0       0       0       0       1       -1      1       -1      
//MKKK    -1      1       0       0       0       0       0       0       0       0       
//MKK     0       0       -1      0       0       1       0       0       0       0       
//MAPK    0       0       0       0       0       0       -1      0       0       1       
//MKK_PP  0       0       0       1       -1      0       0       0       0       0       
//MKKK_P  1       -1      0       0       0       0       0       0       0       0       
//MAPK_PP 0       0       0       0       0       0       0       1       -1      0       
//
//
//
//Stoichiometry Matrix
//        J0      J1      J2      J3      J4      J5      J6      J7      J8      J9      
//MKKK    -1      1       0       0       0       0       0       0       0       0       
//MKKK_P  1       -1      0       0       0       0       0       0       0       0       
//MKK     0       0       -1      0       0       1       0       0       0       0       
//MKK_P   0       0       1       -1      1       -1      0       0       0       0       
//MKK_PP  0       0       0       1       -1      0       0       0       0       0       
//MAPK    0       0       0       0       0       0       -1      0       0       1       
//MAPK_P  0       0       0       0       0       0       1       -1      1       -1      
//MAPK_PP 0       0       0       0       0       0       0       1       -1      0       
//
//
//
//K Matrix
//        J1      J5      J9      J8      J4      
//J1      1       0       0       0       0       
//J5      0       1       0       0       0       
//J9      0       0       1       0       0       
//J8      0       0       0       1       0       
//J4      0       0       0       0       1       
//J0      1       0       0       0       0       
//J2      0       1       0       0       0       
//J6      0       0       1       0       0       
//J3      0       0       0       0       1       
//J7      0       0       0       1       0       
//
//
//
//K0 Matrix
//        J1      J5      J9      J8      J4      
//J0      1       0       0       0       0       
//J2      0       1       0       0       0       
//J6      0       0       1       0       0       
//J3      0       0       0       0       1       
//J7      0       0       0       1       0       
//
//
//
//L Matrix
//        MKK_P   MAPK_P  MKKK    MKK     MAPK    
//MKK_P   1       0       0       0       0       
//MAPK_P  0       1       0       0       0       
//MKKK    0       0       1       0       0       
//MKK     0       0       0       1       0       
//MAPK    0       0       0       0       1       
//MKK_PP  -1      0       0       -1      0       
//MKKK_P  0       0       -1      0       0       
//MAPK_PP 0       -1      0       0       -1      
//
//
//
//L0 Matrix
//        MKK_P   MAPK_P  MKKK    MKK     MAPK    
//MKK_PP  -1      0       0       -1      0       
//MKKK_P  0       0       -1      0       0       
//MAPK_PP 0       -1      0       0       -1
