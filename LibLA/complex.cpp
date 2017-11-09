#include "complex.h"

#include <ostream>

using namespace std;
using namespace LIB_LA;

std::basic_ostream<char>& Complex::operator<<(std::basic_ostream<char>& os)
{	
	return os << "(" << Real << " + " << Imag << "i)";
}

std::ostream & LIB_LA::operator << (std::ostream &os,  const Complex & complex)
{
	return os << "(" << complex.Real << " + " << complex.Imag << "i)";

}
