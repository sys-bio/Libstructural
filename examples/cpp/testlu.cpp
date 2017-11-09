#include <iostream>
#include <libstructural.h>
#include <matrix.h>
#include <util.h>
#include <vector>
#include <string>

using namespace std;
using namespace LIB_STRUCTURAL;
using namespace LIB_LA;

void PrintLU(DoubleMatrix &oMatrix)
{
	// get an instance of the library
	LibLA *laInstance = LibLA::getInstance();

	LU_Result* result = laInstance->getLU(oMatrix);
	cout << "L: " << endl;
	Util::print(*result->L);
	cout << "U: " << endl;
	Util::print(*result->U);
	cout <<"P: " << endl;
	Util::print(*result->P);
	cout <<"Info: " << result->nInfo << endl;	
}

int main(int argc, char*argv[])
{
        
        
			
		double TEST_MATRIX[5][3] = {
			{  17, -17, 34 },
			{   3,  -3,  6 }, 
			{  11,   8,  3 },  
			{   5,  -3,  8 },
			{  13,  -6, 19 }
		}; 
	
		// build stoichiometry matrix (this should be easier!!!)
		double *row1 = TEST_MATRIX[0], *row2 = TEST_MATRIX[1], 
		*row3 = TEST_MATRIX[2], *row4 = TEST_MATRIX[3], *row5 = TEST_MATRIX[4];
	
		const double* oMatrixRAW[] = {row1, row2, row3, row4, row5};
	
		DoubleMatrix oMatrix(oMatrixRAW, 5, 3);
		
		cout << "LU of 5x3 matrix: " << endl;
		PrintLU(oMatrix);
	
		cout << "LU of 3x5 matrix: " << endl;
		PrintLU(*oMatrix.getTranspose());
	
        return 0;
}
