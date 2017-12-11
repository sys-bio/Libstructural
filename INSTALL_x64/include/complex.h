#ifndef LIB_LA_COMPLEX_H
#define LIB_LA_COMPLEX_H


#ifdef __cplusplus

#include <iosfwd>
#include "libutil.h"

namespace LIB_LA
{

	/*! \class LIB_LA::Complex
		\brief LIB_LA::Complex is the complex class used by LIB_LA::LibLA
			
		This class implements a basic complex type along with basic operations on it.
	*/
struct Complex
{
public:
	//! real part of the complex number
	double Real;	
	//! imaginary part of the complex number
	double Imag;
public:
	//! constructs a new complex number with given real and imaginary part
	Complex(double real = 0.0, double imag = 0.0): Real(real), Imag(imag) {}	
	////! virtual destructor
	////virtual ~Complex() {}

	//! returns the real part of the complex number
	LIB_EXTERN double getReal() { return Real; }
	//! return the complex part of the complex number
	LIB_EXTERN double getImag() { return Imag; }
	//! sets the real part of the complex number
	LIB_EXTERN void setReal(double real) { Real = real; }
	//! sets the imaginary part of the complex number
	LIB_EXTERN void setImag(double imag) { Imag = imag; }
	//! sets real and imaginary part of the complex number
	LIB_EXTERN void set(double real, double imag) { setReal(real); setImag(imag);}


	//! assignment operator (sets the real part only)
	LIB_EXTERN virtual Complex & operator = (const double rhs)
	{
		Real = rhs;
		return *this;
	}
	//! assignment operator
	LIB_EXTERN virtual Complex & operator = (const Complex & rhs)
	{
		Real = rhs.Real;
		Imag = rhs.Imag;
		return *this;
	}
	//! implements addition of complex numbers
	LIB_EXTERN virtual Complex & operator + (const Complex & rhs)
	{
		Real += rhs.Real;
		Imag += rhs.Imag;
		return *this;
	}
	//! implements subtraction of complex numbers
	LIB_EXTERN virtual Complex & operator - (const Complex & rhs)
	{
		Real -= rhs.Real;
		Imag -= rhs.Imag;
		return *this;
	}
	//! implements multiplication of complex numbers
	LIB_EXTERN virtual Complex & operator * (const Complex & rhs)
	{
		Real = Real * rhs.Real - Imag*rhs.Imag;
		Imag = Imag*rhs.Real + Real*rhs.Imag;
		return *this;
	}
	//! implements complex division 
	LIB_EXTERN virtual Complex & operator / (const Complex & rhs)
	{
		Real = (Real * rhs.Real + Imag*rhs.Imag)/(rhs.Real*rhs.Real+rhs.Imag*rhs.Imag);
		Imag = (Imag*rhs.Real - Real*rhs.Imag)/(rhs.Real*rhs.Real+rhs.Imag*rhs.Imag);
		return *this;
	}
	
	//! print the complex number on an output stream
	LIB_EXTERN virtual std::basic_ostream<char>& operator<<(std::basic_ostream<char>& os);

	
};
	/*! \brief overload that allows to print a complex number on a std::stream
	
		This function enables a complex number to be displayed on a stream. It will
		be formatted as: '(' + realpart + ' + ' + imaginaryPart + 'i)'. To use it
		invoke for example: 
		\par
		Complex number(1.0, 0.5); cout << number << endl;

		\param os output stream to print on
		\param complex the complex number to be printed

		\return the output stream containing the printed complex number
	*/
	LIB_EXTERN std::ostream &operator << (std::ostream &os,  const Complex & complex);

}

#endif // __cplusplus



#endif
