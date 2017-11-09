/*! \file   libstructural.h
	\brief  All definitions needed for the Structural Analysis Library

	\par
	The structural analysis of stoichiometric networks is an important step in a number of computational methods in systems biology. The structure of a network based on the stoichiometry matrix is divided into two areas, structural constraints imposed by moiety conservation and constraints imposed by flux distributions at steady state. The former constraints have important applications in numerical methods for simulation and the analysis of control, while the later constraints have important applications in flux balance analysis. The LibStructural API provides a wide variety of methods that permit access to the constraint information in the stoichiometry matrix. 
	\par Stoichiometric Constraints
	Moiety constraints concern the conservation of molecular subgroups in stoichiometric networks. Their existence results in dependencies among the model differential equations and the emergence of additional model parameters in the form of moiety mass totals.  In the API we provide robust methods for extracting the constraint information and include specific methods to obtain for example the number of moiety cycles, the number of independent and dependent species and all the pertinent matrices such as the link matrix, reduced stoichiometry matrix etc.  In addition to moiety constraints the library also provides robust methods for determining the flux constraints in a model. These include the dependent and independent flux, and the K matrix (and corresponding terms) that relates the two.  
	\par
	All matrices provided by the API are fully labeled with reaction and species labels. The API can accept models either directly from standard SBML or by specifying the stoichiometry matrix. In the case of SBML the species and reaction labels are obtained directly from the SBML otherwise they are entered manually.
	\par	
	Further and more detailed information on this work can be found in Reder (1988), Sauro and Ingalls (2004), Vallabhajosyula et al. (2005).

	\author  Frank T. Bergmann (fbergman@u.washington.edu)
	\author	 Herbert M. Sauro
	\author	 Ravishankar Rao Vallabhajosyula (developed a previous version of the sructural analysis code)		

*/

//   \par 
//  LibStructural represents the main class for all structural analysis on 
//   either http://sbml.org/ models or directly on a provided stoichiometry matrix.

//   \par 
//  The model can be either analyzed employing QR factorization with householder reflections, 
//  or with LU(P) factorization. The QR factorization is the superior method.
//     
//   \par 
//  For further information please also see:
//\par
//  Vallabhajosyula RR, Chickarmane V, Sauro HM.
//  Conservation analysis of large biochemical networks
//  Bioinformatics, 2005 Nov 29
//  http://bioinformatics.oxfordjournals.org/cgi/content/abstract/bti800v1

//   \par 
//  For examples on how to use the library see LIB_STRUCTURAL::LibStructural::loadSBMLFromString and LIB_STRUCTURAL::LibStructural::loadStoichiometryMatrix


#ifndef LIBCONSERVATION_LIBCONSERVATION_H
#define LIBCONSERVATION_LIBCONSERVATION_H

#include "libutil.h"

#ifdef __cplusplus

#include <vector>
#include <string>
#include <map>

#include "libla.h"
#include "matrix.h"
#include "complex.h"

/*!	\namespace LIB_STRUCTURAL
	\brief	   The LIB_STRUCTURAL namespace contains all functions and classes directly related to Structural Analysis.

	The namespace consists mainly of two classes LIB_STRUCTURAL::LibStructural, the class performing all the structural
	analysis of SBML models, or Stoichiometry matrices, and LIB_STRUCTURAL::SBMLmodel, a small utility class for easy
	access of the needed information.
*/
namespace LIB_STRUCTURAL
{
#ifndef NO_SBML
	class SBMLmodel;
#endif
	/*! \class LIB_STRUCTURAL::LibStructural
		\brief Entrypoint for the C++ API of the Structural Analysis Library. 

		\par 
		LIB_STRUCTURAL::LibStructural represents the main class for all structural analyses on 
		either http://sbml.org/ models or directly on a provided stoichiometry matrix.

		\par 
		The model can be either analyzed by employing QR factorization with householder reflections, 
		or with LU(P) factorization. The QR factorization is the superior method.

		\par Further Information
		Vallabhajosyula RR, Chickarmane V, Sauro HM.
		Conservation analysis of large biochemical networks
		Bioinformatics, 2005 Nov 29
		http://bioinformatics.oxfordjournals.org/cgi/content/abstract/bti800v1

		\par Examples
		For examples on how to use the library see LIB_STRUCTURAL::LibStructural::loadSBMLFromString and LIB_STRUCTURAL::LibStructural::loadStoichiometryMatrix

	*/
	class LibStructural
	{
	public:
		typedef LIB_LA::Matrix< double > DoubleMatrix;
		typedef LIB_LA::Matrix< int > IntMatrix;
		typedef LIB_LA::Matrix< LIB_LA::Complex > ComplexMatrix;		

	private:

		int _NumRows;
		int _NumCols;

		DoubleMatrix* _K0;
		DoubleMatrix* _N0;
		DoubleMatrix* _Nr;
		DoubleMatrix* _L0;
		DoubleMatrix* _L;		// Link Matrix
		DoubleMatrix* _K;		// Null Space Matrix
		DoubleMatrix* _NullN;
		DoubleMatrix* _G;		// conservation law array		

		DoubleMatrix* _Nmat;
		DoubleMatrix* _Nmat_orig;
		DoubleMatrix* _NmatT;
		DoubleMatrix* _NmatT_orig;


		double*								_T; // conserved totals
		double*								_IC;
		double*								_BC;


		int*								spVec;
		int*								colVec;
		std::vector < std::string >         _consv_list;
		double								_Sparsity;
		double								_Pvalue;
		int									_svd_rank_Nmat;
		int									_svd_rank_Nrmat;
		int									_qr_rank_Nrmat;
		int									_NumIndependent;					// number of independent species;
		int									_NumDependent;

		int									nz_count;
		int numFloating;
		int numReactions;
		int numBoundary;
		bool zero_nmat;


		int _SvdRankNr;
		int _SvdRankNmat;
		int _QrRankNmat;

		std::string									_sModelName;

		std::map<int, std::string>					_speciesIndexList;
		std::map<std::string, int>					_speciesIndexList2;
		std::map<int, std::string>					_speciesNamesList;
		std::map<std::string, int>					_speciesNamesList2;

		std::map<int, std::string>					_reactionIndexList;
		std::map<int, std::string>					_reactionNamesList;

		std::map<std::string, int>					_modSpeciesIndexList;
		std::map<std::string, int>					_modSpeciesNamesList;

		std::map<std::string, double>				_speciesValueList;
		std::map<std::string, double>				_variableList;

		std::map<int, std::string>					_bSpeciesIndexList;
		std::map<std::string, int>					_bSpeciesIndexList2;
		std::map<int, std::string>					_bSpeciesNamesList;
		std::map<std::string, int>					_bSpeciesNamesList2;
		std::map<std::string, double>				_bSpeciesValueList;


		std::vector<std::string>			        _inputSpeciesNames;
		std::vector<std::string>			        _inputReactionNames;
		std::vector<double>		                    _inputValues;

	private: 

		std::string GenerateResultString();

		void Initialize();

#ifndef NO_SBML
		void InitializeFromModel(LIB_STRUCTURAL::SBMLmodel& oModel);
#endif
		void InitializeFromStoichiometryMatrix(DoubleMatrix& oMatrix);

#ifndef NO_SBML
		void BuildStoichiometryMatrixFromModel(LIB_STRUCTURAL::SBMLmodel& oModel);
#endif

		void InitializeFromStoichiometryMatrix(DoubleMatrix& oMatrix, 
			std::vector<std::string>& speciesNames, 
			std::vector<std::string>& reactionNames,
			std::vector<double>& inputValues);

		void FreeMatrices();

		void reorderNmatrix();
		void computeNrMatrix();
		void computeN0Matrix();
		void computeLinkMatrix();
		void computeConservedSums();
		void computeConservedEntities();
		void computeK0andKMatrices();


		bool testConservationLaw_1();
		bool testConservationLaw_2();
		bool testConservationLaw_3();
		bool testConservationLaw_4();
		bool testConservationLaw_5();
		bool testConservationLaw_6();

	public:

		/*!	\example examples/cpp/loadstoichiometry.cpp
			This is an example of how to load a stoichiometry matrix and read test details.
		*/
		/*!	\example examples/cpp/loadsbmlfromfile.cpp
			This is an example of how to load a SBML file and print structural analysis test results.
		*/
		/*!	\example examples/cpp/printmatrices.cpp
			This example demonstrates how to access the matrices calculated by the library from C++
		*/

		/*! \brief Load a new stoichiometry matrix. 

			Loads the stoichiometry matrix into the library. To analyze the stoichiometry 
			call one of the following: 

			\li ::LibStructural_analyzeWithQR, 
			\li ::LibStructural_analyzeWithLU, 
			\li ::LibStructural_analyzeWithLUandRunTests, 
			\li ::LibStructural_analyzeWithFullyPivotedLU or
			\li ::LibStructural_analyzeWithFullyPivotedLUwithTests

			\remarks if matrix labels are needed it is recommended to call LIB_STRUCTURAL::LibStructural::loadSpecies 
			and LIB_STRUCTURAL::LibStructural::loadReactionNames after a call to this method.

			\param oMatrix the stoichiometry matrix to load
		*/
		LIB_EXTERN void loadStoichiometryMatrix (DoubleMatrix& oMatrix);
		/*! \brief Load species names and initial values. 

			This function should be used whenever labeled matrices are important as these
			labels will be used in labeling the structural matrices. This function sets the species 
			names (ids). It is also possible to provide an initial condition for each of 
			the species. This will be used when calculating the conserved sums.

			\remarks This method should only be called after ::LibStructural_loadStoichiometryMatrix

			\param speciesNames a vector of species names (ids) to load
			\param speciesValues a vector of initial concentrations 
		*/
		LIB_EXTERN void loadSpecies ( std::vector< std::string > &speciesNames, std::vector<double> &speciesValues);
		/*! \brief Load reaction names.

			This function should be used whenever labeled matrices are important as these
			labels will be used in labeling the structural matrices. This function sets the reaction 
			names (ids). 

			\remarks This method should only be called after ::LibStructural_loadStoichiometryMatrix

			\param reactionNames a vector of reaction names (ids)
		*/
		LIB_EXTERN void loadReactionNames ( std::vector< std::string > &reactionNames);

		/*! \brief Load reaction names.

		This function should be used whenever labeled matrices are important as these
		labels will be used in labeling the structural matrices. This function sets the reaction
		names (ids).

		\remarks This method should only be called after ::LibStructural_loadStoichiometryMatrix

		\param reactionNames a vector of reaction names (ids)

		\return a result string with information about the analysis process

		*/
		LIB_EXTERN DoubleMatrix* rref(DoubleMatrix& oMatrix, double tolerance);
		
		LibStructural::DoubleMatrix* LibStructural::rref_FB(DoubleMatrix &oMatrix, double tolerance);
	
#ifndef NO_SBML

		/*! \brief Load a SBML model.
			\param sSBML the SBML string to load
			\return information about the loaded model
		*/
		LIB_EXTERN std::string loadSBMLFromString(std::string sSBML);

		/*! \brief Load a SBML model from the specified file. 
			\param sFileName a file name to a SBML file to load
			\return information about the loaded model
		*/
		LIB_EXTERN std::string loadSBMLFromFile(std::string sFileName);

		/*! \brief Load an SBML model into the library and carry out tests using the internal test suite.  
			\param sSBML the SBML file to load
			\return information about the loaded model and results of the internal test suite
		*/
		LIB_EXTERN std::string loadSBMLwithTests(std::string sSBML); 
#endif
		/*! \brief Uses QR factorization for structural analysis

			This method performs the actual analysis of the stoichiometry matrix (loaded either
			via LIB_STRUCTURAL::LibStructural::loadStoichiometryMatrix or LIB_STRUCTURAL::LibStructural::loadSBMLFromString. Only after 
			one of the analysis methods below has been called are the structural matrices (L0, K0...)
			available. 

			\li LIB_STRUCTURAL::LibStructural::analyzeWithQR, 
			\li LIB_STRUCTURAL::LibStructural::analyzeWithLU, 
			\li LIB_STRUCTURAL::LibStructural::analyzeWithLUandRunTests, 
			\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLU or
			\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLUwithTests


			\remarks This is the prefered method for structural analysis.

			\return a result string with information about the analysis process
		*/
		LIB_EXTERN std::string analyzeWithQR();
		/*! \brief Uses LU Decomposition for Conservation analysis

			This method performs the actual analysis of the stoichiometry matrix (loaded either
			via LIB_STRUCTURAL::LibStructural::loadStoichiometryMatrix or LIB_STRUCTURAL::LibStructural::loadSBMLFromString. Only after 
			one of the analysis methods below has been called are the structural matrices (L0, K0...)
			available. 

			\li LIB_STRUCTURAL::LibStructural::analyzeWithQR, 
			\li LIB_STRUCTURAL::LibStructural::analyzeWithLU, 
			\li LIB_STRUCTURAL::LibStructural::analyzeWithLUandRunTests, 
			\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLU or
			\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLUwithTests

			\return a result string with information about the analysis process
		*/
		LIB_EXTERN std::string analyzeWithLU(); 
		/*! \brief Uses LU Decomposition for Conservation analysis

			This method performs the actual analysis of the stoichiometry matrix (loaded either
			via LIB_STRUCTURAL::LibStructural::loadStoichiometryMatrix or LIB_STRUCTURAL::LibStructural::loadSBMLFromString. Only after 
			one of the analysis methods below has been called are the structural matrices (L0, K0...)
			available. 

			\li LIB_STRUCTURAL::LibStructural::analyzeWithQR, 
			\li LIB_STRUCTURAL::LibStructural::analyzeWithLU, 
			\li LIB_STRUCTURAL::LibStructural::analyzeWithLUandRunTests, 
			\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLU or
			\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLUwithTests

			This method additionally performs the integrated test suite and returns	those results.


			\return a result string with information about the analysis process
		*/
		LIB_EXTERN std::string analyzeWithLUandRunTests(); 
		/*! \brief Uses fully pivoted LU Decomposition for Conservation analysis

			This method performs the actual analysis of the stoichiometry matrix (loaded either
			via LIB_STRUCTURAL::LibStructural::loadStoichiometryMatrix or LIB_STRUCTURAL::LibStructural::loadSBMLFromString. Only after 
			one of the analysis methods below has been called are the structural matrices (L0, K0...)
			available. 

			\li LIB_STRUCTURAL::LibStructural::analyzeWithQR, 
			\li LIB_STRUCTURAL::LibStructural::analyzeWithLU, 
			\li LIB_STRUCTURAL::LibStructural::analyzeWithLUandRunTests, 
			\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLU or
			\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLUwithTests


			\return a result string with information about the analysis process
		*/
		LIB_EXTERN std::string analyzeWithFullyPivotedLU(); 
		/*! \brief Uses fully pivoted LU Decomposition for Conservation analysis

			This method performs the actual analysis of the stoichiometry matrix (loaded either
			via LIB_STRUCTURAL::LibStructural::loadStoichiometryMatrix or LIB_STRUCTURAL::LibStructural::loadSBMLFromString. Only after 
			one of the analysis methods below has been called are the structural matrices (L0, K0...)
			available. 

			\li LIB_STRUCTURAL::LibStructural::analyzeWithQR, 
			\li LIB_STRUCTURAL::LibStructural::analyzeWithLU, 
			\li LIB_STRUCTURAL::LibStructural::analyzeWithLUandRunTests, 
			\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLU or
			\li LIB_STRUCTURAL::LibStructural::analyzeWithFullyPivotedLUwithTests

			This method additionally performs the integrated test suite and returns	those results.

			\return a result string with information about the analysis process
		*/
		LIB_EXTERN std::string analyzeWithFullyPivotedLUwithTests(); 

		/*! \brief Returns the L0 Matrix. 

			L0 is defined such that  L0 Nr = N0. L0 forms part of the link matrix, L.  N0 is the set of 
			linear dependent rows from the lower portion of the reordered stoichiometry matrix.

		*/
		LIB_EXTERN DoubleMatrix* getL0Matrix(); 

		/*! \brief Returns the L0 Matrix row and column labels.
			\param oRows a string vector that will be overwritten to hold the row labels
			\param oCols a string vector that will be overwritten to hold the column labels.
		*/
		LIB_EXTERN void getL0MatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols ); 

		/*! \brief Returns the Nr Matrix. 

			The rows of the Nr matrix will be linearly independent.

		*/
		LIB_EXTERN DoubleMatrix* getNrMatrix(); 

		/*! \brief Returns the Nr Matrix.

		The rows of the Nr matrix will be linearly independent.

		*/
		LIB_EXTERN DoubleMatrix* getFullyReorderedNrMatrix();

		/*! \brief Returns the N0 Matrix.

		The rows of the Nr matrix will be linearly Dependent.

		*/
		LIB_EXTERN DoubleMatrix* getFullyReorderedN0StoichiometryMatrix();

		/*! \brief Returns the Nr Matrix row and column labels.
			\param oRows a string vector that will be overwritten to hold the row labels
			\param oCols a string vector that will be overwritten to hold the column labels.
		*/
		LIB_EXTERN void getNrMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols ); 

		//! Returns the Nr Matrix repartitioned into NIC (independent columns) and NDC (dependent columns).
		LIB_EXTERN DoubleMatrix* getColumnReorderedNrMatrix(); 

		/*! \brief Returns the Nr Matrix row and column labels (repartitioned into NIC and NDC).
			\param oRows a string vector that will be overwritten to hold the row labels
			\param oCols a string vector that will be overwritten to hold the column labels.
		*/
		LIB_EXTERN void getColumnReorderedNrMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols ); 

		//!  Returns the NIC Matrix (the set of linearly independent columns of Nr)
		LIB_EXTERN DoubleMatrix* getNICMatrix(); 

		/*! \brief Returns the NIC Matrix row and column labels.
			\param oRows a string vector that will be overwritten to hold the row labels
			\param oCols a string vector that will be overwritten to hold the column labels.
		*/
		LIB_EXTERN void getNICMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols ); 

		//! Returns the NDC Matrix (the set of linearly dependent columns of Nr).
		LIB_EXTERN DoubleMatrix* getNDCMatrix(); 
		/*!  \brief Returns the NDC Matrix row and column labels.

			\param oRows a string vector that will be overwritten to hold the row labels
			\param oCols a string vector that will be overwritten to hold the column labels.
		*/
		LIB_EXTERN void getNDCMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols ); 

		/*! \brief Returns the N0 Matrix. 

			The N0 matrix is the set of linearly dependent rows of N where L0 Nr = N0.
		*/
		LIB_EXTERN DoubleMatrix* getN0Matrix(); 

		/*! \brief Returns the N0 Matrix row and column labels.
			\param oRows a string vector that will be overwritten to hold the row labels
			\param oCols a string vector that will be overwritten to hold the column labels.
		*/
		LIB_EXTERN void getN0MatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols ); 

		/*! \brief Returns L, the Link Matrix, left nullspace (aka nullspace of the transpose Nr). 

			L will have the structure, [I L0]', such that L Nr  = N
		*/
		LIB_EXTERN DoubleMatrix* getLinkMatrix(); 

		/*! \brief Returns the row and column labels for the Link Matrix, L
			\param oRows a string vector that will be overwritten to hold the row labels
			\param oCols a string vector that will be overwritten to hold the column labels.
		*/
		LIB_EXTERN void getLinkMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols ); 

		/*! \brief Returns the K0 Matrix. 

			K0 is defined such that K0 = -(NIC)^-1 NDC, or equivalently, [NDC NIC][I K0]' = 0 where [NDC NIC] = Nr
		*/
		LIB_EXTERN DoubleMatrix* getK0Matrix(); 

		/*! \brief  Returns the K0 Matrix row and column labels.
			\param oRows a string vector that will be overwritten to hold the row labels
			\param oCols a string vector that will be overwritten to hold the column labels.
		*/
		LIB_EXTERN void getK0MatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols ); 

		/*! \brief Returns the K matrix (right nullspace of Nr) 

			The K matrix has the structure, [I K0]'  
		*/
		LIB_EXTERN DoubleMatrix* getKMatrix(); 

		/*! \brief  Returns the K matrix row and column labels.
		    \param oRows a string vector that will be overwritten to hold the row labels
			\param oCols a string vector that will be overwritten to hold the column labels.
		*/
		LIB_EXTERN void getKMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols ); 

		/*! \brief Returns Gamma, the conservation law array. 

			Each row represents a single conservation law where the column indicate the 
			participating molecular species. The number of rows is therefore equal to the 
			number of conservation laws. Columns are ordered according to the rows in the 
			reordered stoichiometry matrix, see LIB_STRUCTURAL::LibStructural::getReorderedSpeciesId and 
			LIB_STRUCTURAL::LibStructural::getReorderedStoichiometryMatrix. 

			Gamma is calculated based on: Gamma = [ -L0 I ]


		*/
		LIB_EXTERN DoubleMatrix* getGammaMatrix(); 


		/*! \brief Returns Gamma, the conservation law array. 

			Each row represents a single conservation law where the column indicate the 
			participating molecular species. The number of rows is therefore equal to the 
			number of conservation laws. 

			In this case the Gamma Matrix is recalculated for the given stoichiometry matrix.

			Gamma is calculated based on R =   GaussJordan ( [ stoichiometry  I ] ), where 
			R has the form 

			               R = [ R11 R12  
						          0  GAMMA ]


		    The RowLabels should be an increasing number, to numerate the conservation law, the 
			column label will be the same label as the stoichiometry matrix;

			\param stoichiometry the stoichiometry matrix that will be used to calculate gamma
			
			

		*/
		LIB_EXTERN DoubleMatrix* getGammaMatrixGJ(DoubleMatrix &stoichiometry); 

		/*! \brief try and find a positive Gamma Matrix

		    This function uses LIB_STRUCTURAL::LibStructural::getGammaMatrixGJ on 
			row permutations of the given stoichiometries to find a positive gamma
			matrix. 

			\param stoichiometry the stoichiometry matrix that will be used to calculate gamma
			\param rowLabels as the rows will be permutated b this method, the initial row names should
							 be provided. This vector will be permuted and will present the column
							 labels for the gamma matrix.
		*/
		LIB_EXTERN DoubleMatrix* findPositiveGammaMatrix(DoubleMatrix &stoichiometry, 
														 std::vector< std::string> &rowLabels); 
		
		
		/*! \brief Returns the row and column labels for Gamma, the conservation law array.
			\param oRows a string vector that will be overwritten to hold the row labels
			\param oCols a string vector that will be overwritten to hold the column labels.
		*/
		LIB_EXTERN void getGammaMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols ); 

		//! Returns the original, unaltered stoichiometry matrix.
		LIB_EXTERN DoubleMatrix* getStoichiometryMatrix(); 

		/*! \brief Returns the row and column labels for the original and unaltered stoichiometry matrix.
			\param oRows a string vector that will be overwritten to hold the row labels
			\param oCols a string vector that will be overwritten to hold the column labels.
		*/
		LIB_EXTERN void getStoichiometryMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols ); 

		//! Returns the reordered stoichiometry matrix (row reordered stoichiometry matrix, columns are not reordered!)
		LIB_EXTERN DoubleMatrix* getReorderedStoichiometryMatrix(); 

		/*! \brief Returns the row and column labels for the reordered stoichiometry matrix (row reordered stoichiometry matrix)

			\param oRows a string vector that will be overwritten to hold the row labels
			\param oCols a string vector that will be overwritten to hold the column labels.
		*/
		LIB_EXTERN void getReorderedStoichiometryMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols ); 

		//! Returns the fully reordered stoichiometry matrix (row and column reordered stoichiometry matrix)
		LIB_EXTERN DoubleMatrix* getFullyReorderedStoichiometryMatrix(); 

		/*! \brief Returns the row and column labels for the fully reordered stoichiometry matrix (row and column reordered stoichiometry matrix)

			\param oRows a string vector that will be overwritten to hold the row labels
			\param oCols a string vector that will be overwritten to hold the column labels.
		*/
		LIB_EXTERN void getFullyReorderedStoichiometryMatrixLabels(std::vector< std::string > &oRows, std::vector< std::string > &oCols ); 

		//! Returns the reordered list of molecular species. (choosing the SBML Id if possible )
		LIB_EXTERN std::vector< std::string > getReorderedSpecies(); 

		//!  Returns the unordered list of species Ids
		LIB_EXTERN std::vector< std::string > getSpecies();

		//! Returns the reordered list of molecular species.  (choosing the SBML Name if possible )
		LIB_EXTERN std::vector< std::string > getReorderedSpeciesNamesList(); 

		//! Returns the list of independent species 
		LIB_EXTERN std::vector< std::string > getIndependentSpecies(); 

		//! Returns the actual names of the independent species 
		LIB_EXTERN std::vector< std::string > getIndependentSpeciesNamesList(); 

		//! Returns the list of dependent species 
		LIB_EXTERN std::vector< std::string > getDependentSpecies(); 

		//! Returns the actual names of the dependent species 
		LIB_EXTERN std::vector< std::string > getDependentSpeciesNamesList(); 

		//! Returns the list of Reactions 
		LIB_EXTERN std::vector< std::string > getReactions(); 

		//! Returns the list of independent reactions 
		LIB_EXTERN std::vector< std::string > getIndependentReactionIds(); 

		//! Returns the list of dependent reactions 
		LIB_EXTERN std::vector< std::string > getDependentReactionIds(); 

		//! Returns actual names of the Reactions 
		LIB_EXTERN std::vector< std::string > getReactionsNamesList(); 

		//! Returns the reordered list of reactions 
		LIB_EXTERN std::vector< std::string > getReorderedReactions(); 

		//! Returns algebraic expressions for conserved cycles 
		LIB_EXTERN std::vector< std::string > getConservedLaws(); 

		//! Returns values for conservation laws using the current initial conditions 
		LIB_EXTERN std::vector< double > getConservedSums(); 

		//! Returns Initial Conditions used in the model
		LIB_EXTERN std::vector< std::pair <std::string, double> > getInitialConditions(); 

		/*! \brief Validates structural matrices.

			Calling this method will run the internal test suite against the structural 
			matrices those tests include:\n

			\li Test 1 : Gamma*N = 0 (Zero matrix)
			\li Test 2 : Rank(N) using SVD (5) is same as m0 (5)
			\li Test 3 : Rank(NR) using SVD (5) is same as m0 (5)
			\li Test 4 : Rank(NR) using QR (5) is same as m0 (5)
			\li Test 5 : L0 obtained with QR matches Q21*inv(Q11)
			\li Test 6 : N*K = 0 (Zero matrix)
		*/
		LIB_EXTERN std::vector< std::string > validateStructuralMatrices(); 

		//! Return Return Details about validation tests.
		LIB_EXTERN std::string getTestDetails(); 

		/*! \brief Returns the name of the model. 

			Returns the name of the model if SBML model has Name-tag, otherwise it returns the 
			SBML id. If only a stoichiometry matrix was loaded 'untitled' will be returned.
		*/
		LIB_EXTERN std::string getModelName(); 

		//! Returns the total number of species
		LIB_EXTERN int getNumSpecies(); 
		//! Returns the number of independent species
		LIB_EXTERN int getNumIndSpecies(); 
		//! Returns the number of dependent species
		LIB_EXTERN int getNumDepSpecies(); 
		//! Returns the total number of reactions
		LIB_EXTERN int getNumReactions(); 
		//! Returns the number of independent reactions
		LIB_EXTERN int getNumIndReactions(); 
		//! Returns the number of dependent reactions
		LIB_EXTERN int getNumDepReactions(); 

		//! Returns rank of stoichiometry matrix
		LIB_EXTERN int getRank(); 
		//! Returns the number of nonzero values in Stoichiometry matrix
		LIB_EXTERN double getNmatrixSparsity(); 

		/*! \brief Get Eigenvalues for a matrix

		\param oMatrix Matrix to find the eigenvalues for

		\result Returns in an array, first column are the real values and second column imaginary values
		*/
		LIB_EXTERN LibStructural::DoubleMatrix* getEigenValues(DoubleMatrix &oMatrix);


		/*! \brief Get Eigenvectors for a matrix

		\param oMatrix Matrix to find the eigenvectors for

		\result Returns in an array where each columns is an eigenvector
		*/
		LIB_EXTERN LibStructural::DoubleMatrix* LibStructural::getEigenVectors(DoubleMatrix &oMatrix);


		/*! \brief Get the condition number for a matrix

		\param oMatrix Matrix to find the condition of

		\result Returns the condition number
		*/
		LIB_EXTERN double LibStructural::getConditionNumber(DoubleMatrix &oMatrix);


		/*! \brief Returns the Left Nullspace of the matric argument

		\param oMatrix Matrix to find the left nullspace of

		\result Returns the left null space
		*/
		LIB_EXTERN LibStructural::DoubleMatrix* LibStructural::getLeftNullSpace(DoubleMatrix &oMatrix);

		/*! \brief Returns the Right Nullspace of the matric argument

		\param oMatrix Matrix to find the right nullspace of

		\result Returns the right null space
		*/
		LIB_EXTERN LibStructural::DoubleMatrix* LibStructural::getRightNullSpace(DoubleMatrix &oMatrix);


		/*! \brief Returns the rank of the matrix argument

		\param oMatrix Matrix to find the rank of. 

		\result Returns the rank as an integer
		*/
		LIB_EXTERN int LibStructural::getRank(DoubleMatrix &oMatrix);

		LIB_EXTERN double LibStructural::getRCond(DoubleMatrix &oMatrix);


		/*! \brief Set user specified tolerance

			This function sets the tolerance used by the library to determine what value 
			is considered as zero. Any value with absolute value smaller than this tolerance is considered as zero 
			and will be neglected. 

			\param dTolerance Sets the tolerance used by the library to determine a  value close to zero
		*/
		LIB_EXTERN void setTolerance(double dTolerance); 


		/*! \brief Returns the currently used tolerance

			This function returns the tolerance currently used by the library to determine what value 
			is considered as zero. Any value with absolute value smaller than this tolerance is considered zero 
			and will be neglected. 
		*/
		LIB_EXTERN double getTolerance() { return _Tolerance; }


	public:
		//! Constructor of a new instance of LibStructural
#ifndef NO_SBML
		LIB_EXTERN LibStructural() :   _NumRows(0), _NumCols(0),
			_K0(NULL), _N0(NULL), _Nr(NULL), _L0(NULL), _L(NULL),_K(NULL),_NullN(NULL),_G(NULL),
			_Nmat(NULL), _Nmat_orig(NULL), _NmatT(NULL), _NmatT_orig(NULL), 
			_T(NULL), _IC(NULL), _BC(NULL), spVec(NULL), colVec(NULL), _sModelName("untitled"),_Tolerance(1.0E-9),_Model(NULL)
#else
		LIB_EXTERN LibStructural() :   _NumRows(0), _NumCols(0),
			_K0(NULL), _N0(NULL), _Nr(NULL), _L0(NULL), _L(NULL),_K(NULL),_NullN(NULL),_G(NULL),
			_Nmat(NULL), _Nmat_orig(NULL), _NmatT(NULL), _NmatT_orig(NULL), 
			_T(NULL), _IC(NULL), _BC(NULL), spVec(NULL), colVec(NULL), _sModelName("untitled"),_Tolerance(1.0E-9)
#endif
		{}

		//! static method to get an instance of LibStructural (allows use as singleton)
		LIB_EXTERN static LibStructural* getInstance();
	private: 
		double _Tolerance;
		static LibStructural* _Instance;
#ifndef NO_SBML
		SBMLmodel* _Model;
#endif
	};
}

#endif //__cplusplus

#ifndef SWIG

BEGIN_C_DECLS;

/*! \brief Load a new stoichiometry matrix. 

Loads the stoichiometry matrix into the library. To analyze the stoichiometry 
call one of the following: 

\li ::LibStructural_analyzeWithQR, 
\li ::LibStructural_analyzeWithLU, 
\li ::LibStructural_analyzeWithLUandRunTests, 
\li ::LibStructural_analyzeWithFullyPivotedLU or
\li ::LibStructural_analyzeWithFullyPivotedLUwithTests


\remarks if matrix labels are needed it is recommended to call 
::LibStructural_loadSpecies and ::LibStructural_loadReactionNames after
a call to this function.

\param oMatrix a pointer to a double** matrix
\param nRows the number of rows of the matrix
\param nCols the number of columns of the matrix

\return The return value will be zero (0) when successful, and negative (-1) in case
an error occurred
*/
LIB_EXTERN  int LibStructural_loadStoichiometryMatrix (double ** oMatrix, const int nRows, const int nCols);
/*!	\example examples/c/loadstoichiometry.c
This is an example of how to load a (unlabeled) stoichiometry matrix and read test details.
*/

/*!	\example examples/c/loadlabelledstoichiometry.c
This is an example of how to load a labeled stoichiometry matrix and read test results.
The example also shows how to print the reordered stoichiometry matrix as well as the 
Gamma matrix.
*/

/*!	\example examples/c/loadsbmlfromfile.c
This is an example of how to load a SBML file and print structural analysis test results.
*/

/*! \brief Load species names and initial values. 

This function should be used whenever labeled matrices are important as these
labels will be used in labeling the structural matrices. This function sets the species 
names (ids). It is also possible to provide an initial condition for each of 
the species. This will be used when calculating the conserved sums.

\param speciesNames an array of strings of species names with length nLength
\param speciesValues an array of real numbers of species concentrations corresponding
to the speciesName with the same index
\param nLength number of elements in speciesNames and speciesValues

\return The return value will be zero (0) when successful, and negative (-1) in case
an error occurred

\remarks This method should only be called after ::LibStructural_loadStoichiometryMatrix

*/
LIB_EXTERN  int LibStructural_loadSpecies ( const char** speciesNames, const double* speciesValues, const int nLength);

/*! \brief Load reaction names.

This function should be used whenever labeled matrices are important as these
labels will be used in labeling the structural matrices. This function sets the reaction 
names (ids). 

\param reactionNames an array of strings of reaction names with length nLength
\param nLength number of elements in reactionNames

\return The return value will be zero (0) when successful, and negative (-1) in case
an error occurred

\remarks This method should only be called after ::LibStructural_loadStoichiometryMatrix

*/
#ifndef NO_SBML

LIB_EXTERN  int LibStructural_loadReactionNames ( const char** reactionNames, const int nLength);

/*! \brief Load a SBML model. 
\param sSBML the SBML string to load into the library
\param outMessage a pointer to a string that the library can use to provide information
about the loaded SBML
\param nLength is the length of the above message

\return The return value will be zero (0) when successful, and negative (-1) in case
an error occurred (invalid SBML)

*/
LIB_EXTERN  int LibStructural_loadSBMLFromString(const char* sSBML, char* *outMessage, int *nLength);

/*! \brief Load a SBML model from the specified file. 
\param sFileName the full path to the SBML file to be loaded.
\param outMessage a pointer to a string that the library can use to provide information
about the loaded SBML
\param nLength is the length of the above message

\remarks To avoid unintentional errors be sure to pass in the full path to the SBML file.

\return The return value will be zero (0) when successful, and negative (-1) in case
an error occurred (invalid SBML, file not readable ...).

*/
LIB_EXTERN  int LibStructural_loadSBMLFromFile(const char* sFileName, char* *outMessage, int *nLength);

/*! \brief Load an SBML model into the library and carry out tests using the internal test suite.  
\param sSBML the SBML string to load into the library
\param outMessage a pointer to a string that contains information about the loaded
model as well as the test results of the internal test suite.
\param nLength is the length of the above message

\return The return value will be zero (0) when successful, and negative (-1) in case
an error occurred (invalid SBML)

*/
LIB_EXTERN  int LibStructural_loadSBMLwithTests(const char* sSBML, char* *outMessage, int *nLength);
#endif
/*! \brief Uses QR factorization for structural analysis

This method performs the actual analysis of the stoichiometry matrix (loaded either
via ::LibStructural_loadStoichiometryMatrix or ::LibStructural_loadSBMLFromString. Only after 
one of the analysis methods below has been called are the structural matrices (L0, K0...)
available. 

\li ::LibStructural_analyzeWithQR, 
\li ::LibStructural_analyzeWithLU, 
\li ::LibStructural_analyzeWithLUandRunTests, 
\li ::LibStructural_analyzeWithFullyPivotedLU or
\li ::LibStructural_analyzeWithFullyPivotedLUwithTests


\remarks This is the prefered method for structural analysis.

\param outMessage a pointer to a string where status information of the analysis 
will be returned. 
\param nLength the length of the message.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand see ::LibStructural_loadStoichiometryMatrix
or ::LibStructural_loadSBMLFromString or ::LibStructural_loadSBMLFromFile

*/
LIB_EXTERN  int LibStructural_analyzeWithQR(char* *outMessage, int *nLength);
/*! \brief Uses LU Decomposition for structural analysis

This method performs the actual analysis of the stoichiometry matrix (loaded either
via ::LibStructural_loadStoichiometryMatrix or ::LibStructural_loadSBMLFromString. Only after 
one of the analysis methods below has been called are the structural matrices (L0, K0...)
available. 

\li ::LibStructural_analyzeWithQR, 
\li ::LibStructural_analyzeWithLU, 
\li ::LibStructural_analyzeWithLUandRunTests, 
\li ::LibStructural_analyzeWithFullyPivotedLU or
\li ::LibStructural_analyzeWithFullyPivotedLUwithTests


\param outMessage a pointer to a string where status information of the analysis 
will be returned. 
\param nLength the length of the message.
\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand see ::LibStructural_loadStoichiometryMatrix
or ::LibStructural_loadSBMLFromString or ::LibStructural_loadSBMLFromFile
*/
LIB_EXTERN  int LibStructural_analyzeWithLU(char* *outMessage, int *nLength); 
/*! \brief Uses LU Decomposition for structural analysis

This method performs the actual analysis of the stoichiometry matrix (loaded either
via ::LibStructural_loadStoichiometryMatrix or ::LibStructural_loadSBMLFromString. Only after 
one of the analysis methods below has been called are the structural matrices (L0, K0...)
available. 

\li ::LibStructural_analyzeWithQR, 
\li ::LibStructural_analyzeWithLU, 
\li ::LibStructural_analyzeWithLUandRunTests, 
\li ::LibStructural_analyzeWithFullyPivotedLU or
\li ::LibStructural_analyzeWithFullyPivotedLUwithTests


This method additionally performs the integrated test suite and returns	those results.

\param outMessage a pointer to a string where status information of the analysis 
will be returned. 
\param nLength the length of the message.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand see ::LibStructural_loadStoichiometryMatrix
or ::LibStructural_loadSBMLFromString or ::LibStructural_loadSBMLFromFile

*/
LIB_EXTERN  int LibStructural_analyzeWithLUandRunTests(char* *outMessage, int *nLength); 
/*! \brief Uses fully pivoted LU decomposition for structural analysis.

This method performs the actual analysis of the stoichiometry matrix (loaded either
via ::LibStructural_loadStoichiometryMatrix or ::LibStructural_loadSBMLFromString. Only after 
one of the analysis methods below has been called are the structural matrices (L0, K0...)
available. 

\li ::LibStructural_analyzeWithQR, 
\li ::LibStructural_analyzeWithLU, 
\li ::LibStructural_analyzeWithLUandRunTests, 
\li ::LibStructural_analyzeWithFullyPivotedLU or
\li ::LibStructural_analyzeWithFullyPivotedLUwithTests

\remarks Unlike the other methods, this method handles only square stoichiometry 
matrices. This method was only included for backward compatibility use
::LibStructural_analyzeWithQR

\param outMessage a pointer to a string where status information of the analysis 
will be returned. 
\param nLength the length of the message.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand. See ::LibStructural_loadStoichiometryMatrix
or ::LibStructural_loadSBMLFromString or ::LibStructural_loadSBMLFromFile
*/
LIB_EXTERN  int LibStructural_analyzeWithFullyPivotedLU(char* *outMessage, int *nLength); 
/*! \brief Uses fully pivoted LU decomposition for structural analysis

This method performs the actual analysis of the stoichiometry matrix (loaded either
via ::LibStructural_loadStoichiometryMatrix or ::LibStructural_loadSBMLFromString. Only after 
one of the analysis methods below has been called are the structural matrices (L0, K0...)
available. 

\li ::LibStructural_analyzeWithQR, 
\li ::LibStructural_analyzeWithLU, 
\li ::LibStructural_analyzeWithLUandRunTests, 
\li ::LibStructural_analyzeWithFullyPivotedLU or
\li ::LibStructural_analyzeWithFullyPivotedLUwithTests


This method additionally performs the integrated test suite and returns those results.

\remarks Unlike the other methods, this method handles only square stoichiometry 
matrices. For non-square matrices use a method like ::LibStructural_analyzeWithQR.

\param outMessage a pointer to a string where status information of the analysis 
will be returned. 
\param nLength the length of the message.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand see ::LibStructural_loadStoichiometryMatrix
or ::LibStructural_loadSBMLFromString or ::LibStructural_loadSBMLFromFile
*/
LIB_EXTERN  int LibStructural_analyzeWithFullyPivotedLUwithTests(char* *outMessage, int *nLength); 

/*! \brief Returns the L0 Matrix. 

L0 is defined such that  L0 Nr = N0. L0 forms part of the link matrix, L.  N0 is the set of 
linear dependent rows from the lower portion of the reordered stoichiometry matrix.

\param outMatrix a pointer to a double array that holds the output
\param outRows will be overwritten with the number of rows
\param outCols will be overwritten with the number of columns.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods have 
been called yet.

\remarks To free the returned matrix call ::LibStructural_freeMatrix with the outMatrix 
and outRows as parameter.
*/
LIB_EXTERN  int LibStructural_getL0Matrix(double** *outMatrix, int* outRows, int *outCols); 

/*! \brief Returns the L0 Matrix row and column labels.

\param outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
\param outRowCount after the call this variable will hold the number of row labels 
returned.
\param outColLabels a pointer to a string array where the column labels will be allocated
and written.
\param outColCount after the call this variable will hold the number of column labels
returned.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)
*/
LIB_EXTERN  int LibStructural_getL0MatrixLabels(char** *outRowLabels, int *outRowCount, 
												char** *outColLabels, int *outColCount);

/*! \brief Returns the Nr Matrix. 
The rows of the Nr matrix will be linearly independent.
\param outMatrix a pointer to a double array that holds the output
\param outRows will be overwritten with the number of rows
\param outCols will be overwritten with the number of columns.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the returned matrix call ::LibStructural_freeMatrix with the outMatrix 
and outRows as parameter.
*/
LIB_EXTERN  int LibStructural_getNrMatrix(double** *outMatrix, int* outRows, int *outCols); 
/*! \brief Returns the Nr Matrix row and column labels.

\param outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
\param outRowCount after the call this variable will hold the number of row labels 
returned.
\param outColLabels a pointer to a string array where the column labels will be allocated
and written.
\param outColCount after the call this variable will hold the number of column labels
returned.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)
*/
LIB_EXTERN  int LibStructural_getNrMatrixLabels(char** *outRowLabels, int *outRowCount, 
												char** *outColLabels, int *outColCount);

/*! \brief Returns the Nr Matrix repartitioned into NIC (independent columns) and NDC (dependent columns).

\param outMatrix a pointer to a double array that holds the output
\param outRows will be overwritten with the number of rows
\param outCols will be overwritten with the number of columns.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the returned matrix call ::LibStructural_freeMatrix with the outMatrix 
and outRows as parameter.
*/
LIB_EXTERN  int LibStructural_getColumnReorderedNrMatrix(double** *outMatrix, int* outRows, int *outCols); 
/*! \brief Returns the Nr Matrix row and column labels (repartitioned into NIC and NDC).

\param outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
\param outRowCount after the call this variable will hold the number of row labels 
returned.
\param outColLabels a pointer to a string array where the column labels will be allocated
and written.
\param outColCount after the call this variable will hold the number of column labels
returned.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)
*/
LIB_EXTERN  int LibStructural_getColumnReorderedNrMatrixLabels(char** *outRowLabels, int *outRowCount, 
															   char** *outColLabels, int *outColCount);

/*! \brief Returns the NIC Matrix (the set of linearly independent columns of Nr)

\param outMatrix a pointer to a double array that holds the output
\param outRows will be overwritten with the number of rows
\param outCols will be overwritten with the number of columns.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the returned matrix call ::LibStructural_freeMatrix with the outMatrix 
and outRows as parameter.
*/
LIB_EXTERN  int LibStructural_getNICMatrix(double** *outMatrix, int* outRows, int *outCols); 
/*! \brief Returns the NIC Matrix row and column labels.

\param outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
\param outRowCount after the call this variable will hold the number of row labels 
returned.
\param outColLabels a pointer to a string array where the column labels will be allocated
and written.
\param outColCount after the call this variable will hold the number of column labels
returned.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)
*/
LIB_EXTERN  int LibStructural_getNICMatrixLabels(char** *outRowLabels, int *outRowCount, 
												 char** *outColLabels, int *outColCount);
/*! \brief Returns the NDC Matrix (the set of linearly dependent columns of Nr).

\param outMatrix a pointer to a double array that holds the output
\param outRows will be overwritten with the number of rows
\param outCols will be overwritten with the number of columns.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the returned matrix call ::LibStructural_freeMatrix with the outMatrix 
and outRows as parameter.
*/
LIB_EXTERN  int LibStructural_getNDCMatrix(double** *outMatrix, int* outRows, int *outCols); 
/*! \brief Returns the NDC Matrix row and column labels.

\param outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
\param outRowCount after the call this variable will hold the number of row labels 
returned.
\param outColLabels a pointer to a string array where the column labels will be allocated
and written.
\param outColCount after the call this variable will hold the number of column labels
returned.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)
*/
LIB_EXTERN  int LibStructural_getNDCMatrixLabels(char** *outRowLabels, int *outRowCount, 
												 char** *outColLabels, int *outColCount);
/*! \brief Returns the N0 Matrix. 

The N0 matrix is the set of linearly dependent rows of N where L0 Nr = N0.

\param outMatrix a pointer to a double array that holds the output
\param outRows will be overwritten with the number of rows
\param outCols will be overwritten with the number of columns.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the returned matrix call ::LibStructural_freeMatrix with the outMatrix 
and outRows as parameter.
*/
LIB_EXTERN  int LibStructural_getN0Matrix(double** *outMatrix, int* outRows, int *outCols); 
/*! \brief Returns the N0 Matrix row and column labels.

\param outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
\param outRowCount after the call this variable will hold the number of row labels 
returned.
\param outColLabels a pointer to a string array where the column labels will be allocated
and written.
\param outColCount after the call this variable will hold the number of column labels
returned.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)
*/
LIB_EXTERN  int LibStructural_getN0MatrixLabels(char** *outRowLabels, int *outRowCount, 
												char** *outColLabels, int *outColCount);

/*! \brief Returns L, the Link Matrix, left nullspace (aka nullspace of the transpose Nr). 

L will have the structure, [I L0]', such that L Nr  = N

\param outMatrix a pointer to a double array that holds the output
\param outRows will be overwritten with the number of rows
\param outCols will be overwritten with the number of columns.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the returned matrix call ::LibStructural_freeMatrix with the outMatrix 
and outRows as parameter.
*/
LIB_EXTERN  int LibStructural_getLinkMatrix(double** *outMatrix, int* outRows, int *outCols); 
/*! \brief Returns the row and column labels for the Link Matrix, L

\param outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
\param outRowCount after the call this variable will hold the number of row labels 
returned.
\param outColLabels a pointer to a string array where the column labels will be allocated
and written.
\param outColCount after the call this variable will hold the number of column labels
returned.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)
*/
LIB_EXTERN  int LibStructural_getLinkMatrixLabels(char** *outRowLabels, int *outRowCount, 
												  char** *outColLabels, int *outColCount);

/*! \brief Returns the K0 Matrix. 

K0 is defined such that K0 = -(NIC)^-1 NDC, or equivalently, [NDC NIC][I K0]' = 0 where [NDC NIC] = Nr

\param outMatrix a pointer to a double array that holds the output
\param outRows will be overwritten with the number of rows
\param outCols will be overwritten with the number of columns.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the returned matrix call ::LibStructural_freeMatrix with the outMatrix 
and outRows as parameter.
*/
LIB_EXTERN  int LibStructural_getK0Matrix(double** *outMatrix, int* outRows, int *outCols); 
/*! \brief Returns the K0 Matrix row and column labels.

\param outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
\param outRowCount after the call this variable will hold the number of row labels 
returned.
\param outColLabels a pointer to a string array where the column labels will be allocated
and written.
\param outColCount after the call this variable will hold the number of column labels
returned.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)
*/
LIB_EXTERN  int LibStructural_getK0MatrixLabels(char** *outRowLabels, int *outRowCount, 
												char** *outColLabels, int *outColCount);
/*! \brief Returns the K matrix (right nullspace of Nr) 
The K matrix has the structure, [I K0]'  

\param outMatrix a pointer to a double array that holds the output
\param outRows will be overwritten with the number of rows
\param outCols will be overwritten with the number of columns.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the returned matrix call ::LibStructural_freeMatrix with the outMatrix 
and outRows as parameter.
*/
LIB_EXTERN  int LibStructural_getKMatrix(double** *outMatrix, int* outRows, int *outCols); 
/*! \brief Returns the K matrix row and column labels.

\param outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
\param outRowCount after the call this variable will hold the number of row labels 
returned.
\param outColLabels a pointer to a string array where the column labels will be allocated
and written.
\param outColCount after the call this variable will hold the number of column labels
returned.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)
*/
LIB_EXTERN  int LibStructural_getKMatrixLabels(char** *outRowLabels, int *outRowCount, 
											   char** *outColLabels, int *outColCount);

/*! \brief Returns Gamma, the conservation law array. 

Each row represents a single conservation law where the column indicate the 
participating molecular species. The number of rows is therefore equal to the 
number of conservation laws. Columns are ordered according to the rows in the 
reordered stoichiometry matrix, see ::LibStructural_getReorderedSpeciesId and 
::LibStructural_getReorderedStoichiometryMatrix. 

\param outMatrix a pointer to a double array that holds the output
\param outRows will be overwritten with the number of rows
\param outCols will be overwritten with the number of columns.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the returned matrix call ::LibStructural_freeMatrix with the outMatrix 
and outRows as parameter.
*/
LIB_EXTERN  int LibStructural_getGammaMatrix(double** *outMatrix, int* outRows, int *outCols); 

LIB_EXTERN int LibStructural_getGammaMatrixGJ(double** inMatrix, int numRows, int numCols, 
										  double** *outMatrix, int *outRows, int *outCols);

LIB_EXTERN int LibStructural_findPositiveGammaMatrix(double** inMatrix, int numRows, int numCols, 
				const char** inRowLabels,
				double** *outMatrix, int *outRows, int *outCols, 
				char** *outRowLabels, int *outRowCount);


/*! \brief Returns the row and column labels for Gamma, the conservation law array.

\param outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
\param outRowCount after the call this variable will hold the number of row labels 
returned.
\param outColLabels a pointer to a string array where the column labels will be allocated
and written.
\param outColCount after the call this variable will hold the number of column labels
returned.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)
*/
LIB_EXTERN  int LibStructural_getGammaMatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount);
/*! \brief Returns the original, unaltered stoichiometry matrix.
\param outMatrix a pointer to a double array that holds the output
\param outRows will be overwritten with the number of rows
\param outCols will be overwritten with the number of columns.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the returned matrix call ::LibStructural_freeMatrix with the outMatrix 
and outRows as parameter.
*/
LIB_EXTERN  int LibStructural_getStoichiometryMatrix(double** *outMatrix, int* outRows, int *outCols); 
/*! \brief Returns the row and column labels for the original and unaltered stoichiometry matrix.

\param outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
\param outRowCount after the call this variable will hold the number of row labels 
returned.
\param outColLabels a pointer to a string array where the column labels will be allocated
and written.
\param outColCount after the call this variable will hold the number of column labels
returned.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)
*/
LIB_EXTERN  int LibStructural_getStoichiometryMatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount);
/*! \brief Returns the fully reordered stoichiometry matrix (row and column reordered stoichiometry matrix)

\param outMatrix a pointer to a double array that holds the output
\param outRows will be overwritten with the number of rows
\param outCols will be overwritten with the number of columns.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the returned matrix call ::LibStructural_freeMatrix with the outMatrix 
and outRows as parameter.
*/
LIB_EXTERN  int LibStructural_getFullyReorderedStoichiometryMatrix(double** *outMatrix, int* outRows, int *outCols); 
/*! \brief Returns the reordered stoichiometry matrix (row reordered stoichiometry matrix, columns are not reordered!)

\param outMatrix a pointer to a double array that holds the output
\param outRows will be overwritten with the number of rows
\param outCols will be overwritten with the number of columns.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the returned matrix call ::LibStructural_freeMatrix with the outMatrix 
and outRows as parameter.
*/
LIB_EXTERN  int LibStructural_getReorderedStoichiometryMatrix(double** *outMatrix, int* outRows, int *outCols); 
/*! \brief Returns the row and column labels for the fully reordered stoichiometry matrix (row and column reordered stoichiometry matrix)

\param outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
\param outRowCount after the call this variable will hold the number of row labels 
returned.
\param outColLabels a pointer to a string array where the column labels will be allocated
and written.
\param outColCount after the call this variable will hold the number of column labels
returned.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)
*/
LIB_EXTERN  int LibStructural_getFullyReorderedStoichiometryMatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount);

/*! \brief Returns the row and column labels for the reordered stoichiometry matrix (row reordered stoichiometry matrix)

\param outRowLabels a pointer to a string array where the row labels will be allocated 
and written.
\param outRowCount after the call this variable will hold the number of row labels 
returned.
\param outColLabels a pointer to a string array where the column labels will be allocated
and written.
\param outColCount after the call this variable will hold the number of column labels
returned.

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks To free the string arrays (outRowLabels and outColLabels) call 
::LibStructural_freeMatrix with the string array and its corresponding length 
(outRowCount or outColCount)
*/
LIB_EXTERN  int LibStructural_getReorderedStoichiometryMatrixLabels(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount);

/*! \brief Returns the reordered list of molecular species. 
\param outArray pointer to string array that will be allocated and filled with the species Ids
\param outLength the number of species

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

\remarks free outArray using ::LibStructural_freeMatrix with the outLength parameter
*/
LIB_EXTERN  int LibStructural_getReorderedSpeciesIds(char** *outArray, int *outLength); 


/*! \brief Returns the unordered list of species Ids
\param outArray pointer to string array that will be allocated and filled with the species Ids
\param outLength the number of species
\remarks free outArray using ::LibStructural_freeMatrix with the outLength parameter

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

*/
LIB_EXTERN  int LibStructural_getSpeciesIds(char** *outArray, int *outLength); 

/*! \brief Returns the reordered list of reactions Ids. 
\param outArray pointer to string array that will be allocated and filled with the reordered reaction Ids
\param outLength the number of species
\remarks free outArray using ::LibStructural_freeMatrix with the outLength parameter

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

*/
LIB_EXTERN  int LibStructural_getReorderedReactionIds(char** *outArray, int *outLength); 

/*! \brief Returns the list of independent species ids. 
\param outArray pointer to string array that will be allocated and filled with the independent species Ids
\param outLength the number of independent species
\remarks free outArray using ::LibStructural_freeMatrix with the outLength parameter

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

*/
LIB_EXTERN  int LibStructural_getIndependentSpeciesIds(char** *outArray, int *outLength); 

/*! \brief Returns the list of dependent species Ids. 
\param outArray pointer to string array that will be allocated and filled with the dependent species Ids
\param outLength the number of dependent species
\remarks free outArray using ::LibStructural_freeMatrix with the outLength parameter
\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.


*/
LIB_EXTERN  int LibStructural_getDependentSpeciesIds(char** *outArray, int *outLength); 

/*! \brief Returns the list of independent reaction ids. 
\param outArray pointer to string array that will be allocated and filled with the independent reaction Ids
\param outLength the number of independent reaction
\remarks free outArray using ::LibStructural_freeMatrix with the outLength parameter

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

*/
LIB_EXTERN  int LibStructural_getIndependentReactionIds(char** *outArray, int *outLength); 

/*! \brief Returns the list of dependent reaction Ids. 
\param outArray pointer to string array that will be allocated and filled with the dependent reaction Ids
\param outLength the number of dependent reactions
\remarks free outArray using ::LibStructural_freeMatrix with the outLength parameter
\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.


*/
LIB_EXTERN  int LibStructural_getDependentReactionIds(char** *outArray, int *outLength); 

/*! \brief Returns the list of unordered Reactions. 
Returns the original list of reactions in the same order as when it was loaded.
\param outArray pointer to string array that will be allocated and filled with the reaction Ids
\param outLength the number of reactions
\remarks free outArray using ::LibStructural_freeMatrix with the outLength parameter
\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

*/
LIB_EXTERN  int LibStructural_getReactionIds(char** *outArray, int *outLength); 

/*! \brief Returns algebraic expressions for the conservation laws.
\param outArray pointer to string array that will be allocated and filled
\param outLength the number of conservation laws
\remarks free outArray using ::LibStructural_freeMatrix with the outLength parameter
\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

*/
LIB_EXTERN  int LibStructural_getConservedLaws(char** *outArray, int *outLength); 

/*! \brief Returns the number of conservation laws.
\return the number of conservation laws
*/
LIB_EXTERN  int LibStructural_getNumConservedSums(); 
/*! \brief Returns values for conservation laws using the current initial conditions 

\param outArray will be allocated and filled with a double vector of all conserved sums
\param outLength is the number of conserved sums
\remarks free outArray using ::LibStructural_freeVector

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

*/
LIB_EXTERN int LibStructural_getConservedSums(double* *outArray, int *outLength);

/*! \brief Returns the initial conditions used in the model.
\param outVariableNames a string vector of all species Ids
\param outValues a double vector of corresponding initial conditions
\param outLength number of elements in outVariableNames and outValues (number of species)
*/
LIB_EXTERN  int LibStructural_getInitialConditions(char** *outVariableNames, double* *outValues, int *outLength); 

/*! \brief Validates structural matrices.

Calling this method will run the internal test suite against the structural 
matrices those tests include:\n

\li Test 1 : Gamma*N = 0 (Zero matrix)
\li Test 2 : Rank(N) using SVD (5) is same as m0 (5)
\li Test 3 : Rank(NR) using SVD (5) is same as m0 (5)
\li Test 4 : Rank(NR) using QR (5) is same as m0 (5)
\li Test 5 : L0 obtained with QR matches Q21*inv(Q11)
\li Test 6 : N*K = 0 (Zero matrix)

\param outResults an integer vector, each element represents the result for one
of the above tests (the 0th element representing the test result for test1), 
if the test passed the value is 1 and 0 otherwise. 
\param outLength number of tests

\remarks free outResults using ::LibStructural_freeVector

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

*/
LIB_EXTERN  int  LibStructural_validateStructuralMatrices(int* *outResults, int* outLength); 
/*! \brief Return Details about validation tests.
\param outMessage a pointer to a string where status information of the analysis 
will be returned. 
\param nLength the length of the message.
\remarks free outMessage using ::LibStructural_freeVector

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

*/
LIB_EXTERN  int LibStructural_getTestDetails(char* *outMessage, int *nLength); 

/*! \brief Returns the name of the model. 

Returns the name of the model if SBML model has Name-tag, otherwise it returns the 
SBML id. If only a stoichiometry matrix was loaded 'untitled' will be returned.

\param outMessage a pointer to a string where status information of the analysis 
will be returned. 
\param nLength the length of the message.
\remarks free outMessage using ::LibStructural_freeVector

\return The return value will be zero (0) when successful, and negative (-1) in case
no stoichiometry matrix was loaded beforehand or none of the analysis methods has 
been called yet.

*/
LIB_EXTERN  int LibStructural_getModelName(char* *outMessage, int *nLength); 

//! Returns the total number of species.
LIB_EXTERN  int LibStructural_getNumSpecies(); 

//! Returns the number of independent species.
LIB_EXTERN  int LibStructural_getNumIndSpecies(); 

//! Returns the number of dependent species.
LIB_EXTERN  int LibStructural_getNumDepSpecies(); 
//! Returns the total number of reactions.
LIB_EXTERN  int LibStructural_getNumReactions(); 
//! Returns the number of independent reactions.
LIB_EXTERN  int LibStructural_getNumIndReactions(); 
//! Returns the number of dependent reactions.
LIB_EXTERN  int LibStructural_getNumDepReactions(); 
//! Returns the rank of the stoichiometry matrix.
LIB_EXTERN  int LibStructural_getRank(); 
//! Returns the percentage of nonzero values in the stoichiometry matrix
LIB_EXTERN  double LibStructural_getNmatrixSparsity(); 

/*! \brief Set user specified tolerance

This function sets the tolerance used by the library to determine what value 
is considered as zero. Any value with absolute value smaller than this tolerance is considered as zero 
and will be neglected. 

\param dTolerance Sets the tolerance used by the library to determine a  value close to zero
*/
LIB_EXTERN  void LibStructural_setTolerance(const double dTolerance); 

/*! \brief Get user specified tolerance

This function gets the tolerance used by the library to determine what value 
is considered as zero. Any value with absolute value smaller than this tolerance is considered as zero 
and will be neglected. 

\return the tolerance used by the library to determine a  value close to zero
*/
LIB_EXTERN  double LibStructural_getTolerance(); 


//! Frees a vector previously allocated by this library.
LIB_EXTERN void LibStructural_freeVector(void* vector);

//! Frees a matrix previously allocated by this library.
LIB_EXTERN void LibStructural_freeMatrix(void** matrix, int numRows);

//LIB_EXTERN  int LibStructural_getNthReorderedSpeciesId(int n,char* *outMessage, int *nLength); 
//LIB_EXTERN  int LibStructural_getNthIndependentSpeciesId(int n,char* *outMessage, int *nLength); 
//LIB_EXTERN  int LibStructural_getNthDependentSpeciesId(int n,char* *outMessage, int *nLength); 
//LIB_EXTERN  int LibStructural_getNthReactionId(int n,char* *outMessage, int *nLength); 
//LIB_EXTERN  int LibStructural_getNthConservedEntity(int n,char* *outMessage, int *nLength); 
//LIB_EXTERN double LibStructural_getNthConservedSum(int n);

END_C_DECLS;
#endif

#endif


/*! \mainpage Structural Analysis Library

\par
This document describes the application programming interface (API) of LibLA and LibStructural  an open source (BSD) library for computing structural characteristics of cellular networks. 
\par
LibLA is a linear algebra library derives much of its functionality from the standard CLAPACK library with additional linear algebra functions not directly supported by CLAPACK. The libStructural library supports a range of methods for the structural analysis of cellular networks (derived either from SBML or stoichiometry matrices) and utilizes LibLA for some of its internal computations. 
\par Installing
To make the Structural Analysis Library easily accessible we have ]\d binary installers for Windows as wel as OS X (version 10.4 and above). 
We also habe a source distribution, complete with Visual Studio, XCode, Scons and Qt project files that allow to build the library on Windows, Linux and OS X. For detailed instructions on how to build the library see the file INSTALL included with the source distribution.
\par Dependencies
These libraries depend on two third-party libraries, LAPACK and libSBML.  Both are provided with the binary installation where necessary.
\par
This work was supported by a grant from the NIH (1R01GM0819070-01).


\author  Frank T. Bergmann (fbergman@u.washington.edu)
\author	 Herbert M. Sauro
\author	 Ravishankar Rao Vallabhajosyula (developed a previous version of the sructural analysis code)		

\par License
\par
Copyright (c) 2008, Frank T Bergmann and Herbert M Sauro\n
All rights reserved.

\par
Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

\li Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

\li Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

\li Neither the name of University of Washington nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

\par
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


*/
//-----------------------------------------------------------

//\par
//This document describes two 
//
//all classes in the LIB_STRUCTURAL and LIB_LA namespace. 
//Among others the LIB_STRUCTURAL::LibStructural class, that performs structural analysis on SBML models, 
//or even just (labeled) stoichiometry matrices. 
//
//\par
//Furthermore you will find a utility class, LIB_LA::LibLA, wrapping commonly used LAPACK 
//functionality such as eigenvalue computations, or the inverse of matrices.
//
//\par
//The remaining classes represent utility classes for support of complex numbers and the 
//structured return of LU and QR matrix decompositions.
//
//\par
//For more information about this topic, please see our reference publication at XXXXXXX or
//
//\par
//Vallabhajosyula RR, Chickarmane V, Sauro HM.
//Conservation analysis of large biochemical networks. 
//Bioinformatics 2005 Nov 29
//http://bioinformatics.oxfordjournals.org/cgi/content/abstract/bti800v1
//
//\par
//An updated version of this library will be posted on http://sys-bio.org
