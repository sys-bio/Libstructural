#ifndef LIB_LA_MATRIX_H
#define LIB_LA_MATRIX_H

#include <iosfwd>
#include "libutil.h"
#include "string.h"

#ifdef __cplusplus

namespace LIB_LA
{


	template<typename T> class Matrix;

	struct Complex;

	/*! \class LIB_LA::Matrix
		\brief LIB_LA::Matrix is the matrix class used by LIB_LA::LibLA and LIB_STRUTURAL::LibStructural
			
		This class implements a template to hold real, LIB_LA::Complex and integer matrices. It also implements basic
		operations on matrices.
	*/
	template <class T> class Matrix
	{
	public: 
		/*! \brief the element type for this matrix, will be real, LIB_LA::Complex or integer. 
		*/
		typedef T _ElementType;
	//protected:
		unsigned int _Rows;
		unsigned int _Cols;
		T * _Array;
	private:

	public:
		//! Creates a new matrix with the given numbers of rows and columns
		LIB_EXTERN Matrix(unsigned int rows = 0, unsigned int cols = 0) :
		  _Rows(rows),
			  _Cols(cols),
			  _Array(NULL)
		  {
			  if (_Rows && _Cols)
			  {
				  _Array = new T[_Rows * _Cols];
				  memset(_Array, 0, (sizeof(T)*_Rows*_Cols));
			  }
		  }

		  //! Copy constructor
		  LIB_EXTERN Matrix(const Matrix <T> & src):
		  _Rows(src._Rows),
			  _Cols(src._Cols),
			  _Array(NULL)
		  {
			  if (_Rows && _Cols)
			  {
				  _Array = new T[_Rows * _Cols];
				  memcpy(_Array, src._Array, _Rows * _Cols * sizeof(T));
			  }
		  }
	
		  //! Constructor taking a matrix mapped to a vector and reconstructing the 2D form
		  LIB_EXTERN Matrix( T* &oRawData, int nRows, int nCols, bool transpose = true) : 
		  _Rows(nRows),
			  _Cols(nCols),
			  _Array(NULL)

		  {
			  if (_Rows && _Cols) 
			  {
				  _Array = new T[_Rows * _Cols];
				  if (!transpose)
					memcpy(_Array, oRawData, sizeof(T)*nRows*nCols);
				  else
				  {
					  for (unsigned int i = 0; i < _Rows; i++)
					  {
						  for (unsigned int j = 0; j < _Cols; j++)
						  {
							  (*this)(i,j) = oRawData[i+_Rows*j];
						  }
					  }
				  }
			  }
		  }

		  //virtual Matrix <T> & operator + (const Matrix <T> & rhs)
		  //{			  
		  // unsigned int i, imax = _Rows * _Cols;
		  // T * tmp1 = _Array;
		  // T * tmp2 = rhs._Array;

		  // for (i = 0; i < imax; i++, tmp1++, tmp2++) *tmp1 += *tmp2;

		  // return *this;
		  //}

		  //! constructs a matrix from 2D data
		  LIB_EXTERN Matrix( T** &oRawData, int nRows, int nCols) : _Array(NULL), _Rows(0), _Cols(0)
		  {
			  initializeFrom2DMatrix(oRawData, nRows, nCols);
		  }

		  //! constructs a matrix from 2D const data
		  LIB_EXTERN Matrix( const T** oRawData, int nRows, int nCols) : _Array(NULL), _Rows(0), _Cols(0)
		  {
			  initializeFromConst2DMatrix(oRawData, nRows, nCols);
		  }


		  //! returns a pointer to the underlying 1D array
		  LIB_EXTERN T* getArray() { return _Array; };

		  //! returns a copy of the data, optionally transposing it
		  LIB_EXTERN T* getCopy(bool transpose = false) 
		  {
  				  T* result = new T[_Rows * _Cols];
				  if (_Rows * _Cols == 0) return result;
				  if (!transpose)				  
				  memcpy(result, _Array, sizeof(T)*_Rows*_Cols);
				  else
				  {
					  for (unsigned int i = 0; i < _Rows; i++)
					  {
						  for (unsigned int j = 0; j < _Cols; j++)
						  {
							  result[i+_Rows*j] = (*this)(i,j) ;
						  }
					  }
				  }
				  return result;
		  }

		  //! initializes the matrix from 2D data
		  LIB_EXTERN void initializeFrom2DMatrix( T** &oRawData, int nRows, int nCols);
		  //! initializes the matrix from 2D const data
		  LIB_EXTERN void initializeFromConst2DMatrix( const T** oRawData, int nRows, int nCols);

		  //! virtual destructor
		  LIB_EXTERN virtual ~Matrix()
		  {
			  if (_Array)
			  {
				  delete [] _Array;
				  _Array = NULL;
			  }
		  }

		  //! returns a 2D data array
		  LIB_EXTERN T** get2DMatrix(int &nRows, int &nCols);

		  //! swaps the given rows
		  LIB_EXTERN virtual void swapRows(unsigned int row1, unsigned int row2)
		  {			  
			  for (unsigned int i = 0; i < _Cols; i++)
			  {
				  T tmp = (*this)(row1,i);
				  (*this)(row1,i)=(*this)(row2,i);
				  (*this)(row2,i)=tmp;
			  }
		  }

		  //! swaps the given columns
		  LIB_EXTERN virtual void swapCols(unsigned int col1, unsigned int col2)
		  {
				for (unsigned int i = 0; i < _Rows; i++)
				{
					T tmp = (*this)(i,col1);
					(*this)(i,col1)=(*this)(i,col2);
					(*this)(i,col2)=tmp;
				}
		  }

		  //! resizes the matrix to the given number of rows and columns
		  LIB_EXTERN virtual void resize(unsigned int rows, unsigned int cols)
		  {
			  if (rows * cols != _Rows * _Cols)
			  {
				  if (_Array)
				  {
					  delete [] _Array;
					  _Array = NULL;
				  }
				  if (rows && cols)
					  _Array = new T[rows * cols];
			  }

			  _Rows = rows;
			  _Cols = cols;
		  }

		  //! creates a new matrix holding the transpose
		  LIB_EXTERN virtual Matrix <T> * getTranspose()
		  {
			  Matrix <T> *oResult = new Matrix <T>(_Cols, _Rows);
			  for (unsigned int i = 0; i < _Cols; i++)
			  {
				  for (unsigned int j = 0; j <_Rows; j++)
				  {
					  (*oResult)(i,j) = (*this)(j,i);
				  }
			  }
			  return oResult;

		  }

		  //! assignment operator
		  LIB_EXTERN virtual Matrix <T> & operator = (const Matrix <T> & rhs)
		  {
			  if (_Rows != rhs._Rows || _Cols != rhs._Cols)
				  resize(rhs._Rows, rhs._Cols);

			  memcpy(_Array, rhs._Array, _Rows * _Cols * sizeof(T));

			  return *this;
		  }

		  //! scalar assignment operator
		  LIB_EXTERN virtual Matrix <T> & operator = (const T & value)
		  {
			  unsigned int i, imax = _Rows * _Cols;
			  T * tmp = _Array;

			  for (i = 0; i < imax; i++, tmp++) *tmp = value;

			  return *this;
		  }

		  //! returns the size of the matrix
		  LIB_EXTERN virtual unsigned int size() const 
		  {
			  return _Rows * _Cols;
		  }

		  //! returns the number of rows
		  LIB_EXTERN virtual unsigned int numRows() const 
		  {
			  return _Rows;
		  }

		  //! returns the number of columns
		  LIB_EXTERN virtual unsigned int numCols() const 
		  {
			  return _Cols;
		  }

		  //! returns the selected row
		  LIB_EXTERN virtual inline T * operator[](unsigned int row)
		  {
			  return _Array + row * _Cols;
		  }

		  //! returns the selected row
		  LIB_EXTERN virtual inline const T * operator[](unsigned int row) const
		  {
			  return _Array + row * _Cols;
		  }

		  //! returns the selected matrix element
		  LIB_EXTERN virtual inline _ElementType & operator()(const unsigned int & row,
			  const unsigned int & col)
		  {      
			  return *(_Array + row * _Cols + col);
		  }

		  //! returns the selected matrix element (const)
		  LIB_EXTERN virtual inline const _ElementType & operator()(const unsigned int & row,
			  const unsigned int & col) const
		  {      
			  return *(_Array + row * _Cols + col);
		  }
	};

	//! defines a real matrix (hides the templates in signatures)
	typedef Matrix< double > DoubleMatrix;
	//! defines a integer matrix (hides the templates in signatures)
	typedef Matrix< int > IntMatrix;
	//! defines a complex matrix (hides the templates in signatures)
	typedef Matrix< Complex > ComplexMatrix;	
}

#endif // __cplusplus



#endif

