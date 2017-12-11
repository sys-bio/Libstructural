/*! \file   constraint.h
\brief  basic storage class for constraints used by LIB_STRUCTURAL::FluxBalance

\par
LIB_STRUCTURAL::LPResult consists of an flux list and correspending values. 

\author  Frank T. Bergmann (fbergman@u.washington.edu)

*/
#ifndef LP_RESULT_H
#define LP_RESULT_H

#include <vector>
#include <string>
#include "libutil.h"

namespace LIB_STRUCTURAL
{
	/*! \class LIB_STRUCTURAL::LPResult
	\brief basic storage class for constraints used by LIB_STRUCTURAL::FluxBalance

	\par 
	LIB_STRUCTURAL::LPResult consists of an flux list and correspending values. 


	*/
	class LPResult
	{
	public:
		//! construct a new result
		LIB_EXTERN LPResult(void);

		//! construct a new result with given id and value list
		LIB_EXTERN LPResult(std::vector<std::string> &ids, std::vector<double> &values) 
			: Ids(ids), Values(values) {}

		//! virtual destructor
		LIB_EXTERN virtual ~LPResult(void);

		//! return the number of result variables
		LIB_EXTERN int getNumVariables() { return Ids.size(); } 
		//! return the nth flux name
		LIB_EXTERN std::string getNthId(int index) { return Ids[index]; } 
		//! return the nth flux value
		LIB_EXTERN double getNthValue(int index) { return Values[index]; } 

		std::vector< std::string >  Ids;
		std::vector< double >		Values;

	};

}

#endif
