#include <iostream>
#include <libstructural.h>
#include <matrix.h>
#include <vector>
#include <string>

using namespace std;
using namespace LIB_STRUCTURAL;
using namespace LIB_LA;


int main(int argc, char*argv[])
{
        
        
        // get an instance of the library
		LibLA *laInstance = LibLA::getInstance();
			
		// lets try something easy the condition number of [1 2; 3 4]
		DoubleMatrix aMatrix(2,2); aMatrix(0,0) = 1; aMatrix(0,1) = 2; aMatrix(1,0) = 3; aMatrix(1,1) = 4;
		double rcond = laInstance->getRCond(aMatrix);
		cout << "reciprocal condition number is: " << rcond << endl;
		cout << "condition number is: " << (1/rcond) << endl;

        return 0;
}
