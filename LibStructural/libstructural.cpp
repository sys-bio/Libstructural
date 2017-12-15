#ifdef WIN32
#pragma warning(disable: 4996)
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h>
#endif

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include "libstructural.h"
#include "libla.h"
#include "sbmlmodel.h"
#include "matrix.h"
#include "util.h"
#include "math.h"
#include "libMetaTool4_3.h"

#define SMALL_NUM           1.0E-9
#define PRINT_PRECISION		10
#define LINE			    "-----------------------------------------------------------------------------"

#define LibStruct_VERSION "1.0.0"

using namespace LIB_STRUCTURAL;
using namespace LIB_LA;
using namespace std;

char* getVersion () {
	return LibStruct_VERSION;
}


#ifndef NO_SBML
// ----------------------------------------------------------------------------------------
// string loadSBMLFromString(string)
//
// This is the main method that the users should run their models with. The method takes
// the SBML file as an input (string). This could be in SBML level 1 or SBML level 2 format.
// The users should check the validity of the SBML files before loading it into the method.
// Conservation analysis is carried out using Householder QR algorithm to generate the L0
// matrix, from which other matrices of interest can subsequently be generated. The results
// of the analysis are output as a string and are also accessible from other methods in
// the API.
// ----------------------------------------------------------------------------------------
void LibStructural::loadSBMLFromString(string sSBML)
{
    DELETE_IF_NON_NULL(_Model);
    _Model = new SBMLmodel(sSBML);
    analyzeWithQR();
}

void LibStructural::loadSBMLFromFile(string sFileName)
{
	DELETE_IF_NON_NULL(_Model);
	_Model = SBMLmodel::FromFile(sFileName);
	analyzeWithQR();
}

// Initialization method, takes SBML as input
string LibStructural::loadSBMLwithTests(string sSBML)
{
	DELETE_IF_NON_NULL(_Model);		_Model = new SBMLmodel(sSBML);

	analyzeWithQR();
	_sResultStream << endl << endl;
	_sResultStream << getTestDetails();

	return _sResultStream.str();
}



void LibStructural::InitializeFromModel(LIB_STRUCTURAL::SBMLmodel& oModel)
{
	numFloating = oModel.numFloatingSpecies();
	numReactions = oModel.numReactions();
	numBoundary = oModel.getModel()->getNumSpeciesWithBoundaryCondition();

	_sModelName = (oModel.getModel()->isSetName() ? oModel.getModel()->getName() : oModel.getModel()->getId());

	CREATE_ARRAY(spVec,int,numFloating);
	CREATE_ARRAY(colVec,int,numReactions);
	_consv_list.clear();

	for (int i = 0; i < numFloating; i++)
	{
		const Species * species = oModel.getNthFloatingSpecies(i);
		_speciesIndexList[i] = species->getId();
		_speciesNamesList[i] = species->getName();
		_speciesNamesList2[_speciesNamesList[i]] = i;
		_speciesIndexList2[_speciesIndexList[i]] = i;
		_speciesValueList[_speciesIndexList[i]] = ( species->isSetInitialConcentration() ? species->getInitialConcentration() : species->getInitialAmount());
		_consv_list.push_back(_speciesIndexList[i]);
		spVec[i] = i;
	}

	for (int i = 0; i < numReactions; i++)
	{
		const Reaction *reaction = oModel.getNthReaction(i);
		_reactionIndexList[i] = reaction->getId();
		_reactionNamesList[i] = reaction->getName();
		colVec[i] = i;
	}

	for (int i = 0; i < numBoundary; i++)
	{
		const Species * species = oModel.getNthBoundarySpecies(i);
		_bSpeciesIndexList[i] = species->getId();
		_bSpeciesIndexList2[_bSpeciesIndexList[i]] = i;
		_bSpeciesNamesList[i] = species->getName();
		_bSpeciesNamesList2[_bSpeciesIndexList[i]] = i;
		_bSpeciesValueList[_bSpeciesIndexList[i]] = ( species->isSetInitialConcentration() ? species->getInitialConcentration() : species->getInitialAmount());
	}

}

#endif


void LibStructural::FreeMatrices()
{
	// clear boundary species lists
	_bSpeciesIndexList.clear();				_bSpeciesIndexList2.clear();
	_bSpeciesNamesList.clear();				_bSpeciesNamesList2.clear();
	_bSpeciesValueList.clear();

	// clear reaction lists
	_reactionIndexList.clear();				_reactionNamesList.clear();

	// clear floating species lists
	_speciesIndexList.clear();				_speciesIndexList2.clear();
	_speciesNamesList.clear();				_speciesNamesList2.clear();
	_speciesValueList.clear();

	// delete allocated matrices
	DELETE_IF_NON_NULL(_K0);				DELETE_IF_NON_NULL(_N0);
	DELETE_IF_NON_NULL(_Nr);				DELETE_IF_NON_NULL(_L0);
	DELETE_IF_NON_NULL(_L);					DELETE_IF_NON_NULL(_K);
	DELETE_IF_NON_NULL(_NullN);				DELETE_IF_NON_NULL(_G);
	DELETE_IF_NON_NULL(_Nmat);				DELETE_IF_NON_NULL(_NmatT);
	DELETE_IF_NON_NULL(_Nmat_orig);			DELETE_IF_NON_NULL(_NmatT_orig);

	// delete allocated arrays
	DELETE_ARRAY_IF_NON_NULL(_T);			DELETE_ARRAY_IF_NON_NULL(_IC);
	DELETE_ARRAY_IF_NON_NULL(_BC);

	DELETE_ARRAY_IF_NON_NULL(spVec);		DELETE_ARRAY_IF_NON_NULL(colVec);

}

string LibStructural::GenerateResultString()
{
	stringstream oBuffer;

	oBuffer << LINE << endl << LINE << endl << "STRUCTURAL ANALYSIS MODULE : Results " << endl
		<< LINE << endl << LINE << endl;

	oBuffer << "Size of Stochiometric Matrix: " << _NumRows << " x "  << _NumCols
		<< " (Rank is  " << _NumIndependent << ")";

	if (_NumCols > 0)
	{
		oBuffer << endl << "Nonzero entries in Stochiometric Matrix: " << nz_count
			<< "  (" << _Sparsity << "% full)" << endl;

	}
	else
	{
		oBuffer << "This model has no reactions. " << endl;
	}

	oBuffer << endl << "Independent Species (" << _NumIndependent << ") :" << endl;

	for (int i = 0; i < _NumIndependent; i++)
	{
		oBuffer << _speciesIndexList[spVec[i]];
		if (i+1 < _NumIndependent) oBuffer << ", ";
	}

	oBuffer << endl << endl << "Dependent Species ";
	if ((_NumRows == _NumIndependent) || (_NumCols == 0) || (zero_nmat))
	{
		oBuffer << ": NONE" << endl << endl;
	}
	else
	{
		oBuffer << "(" << _NumDependent << ") :" << endl;

		for (int i = _NumIndependent; i < _NumRows; i++)
		{
			oBuffer << _speciesIndexList[spVec[i]];
			if (i + 1 < _NumRows) oBuffer << ", ";
		}
		oBuffer << endl << endl;
	}

	oBuffer << "L0 : ";
	if (_NumRows == _NumIndependent)
	{
		oBuffer << "There are no dependencies. L0 is an EMPTY matrix";
	}
	else if (_NumCols == 0)
	{
		oBuffer << "There are " << _NumRows << " dependencies. L0 is a " << _NumRows << "x" << _NumRows << " matrix.";
	}
	else if (zero_nmat)
	{
		oBuffer << "All " << _NumRows <<" species are independent. L is an identity matrix.";
	}
	else
	{
		oBuffer << "There " << (_NumDependent != 1 ? "are "  : "is ")
			<< _NumDependent << (_NumDependent != 1 ? " dependencies." : " dependency.")
			<< " L0 is a " << _NumDependent << "x" << _NumIndependent << " matrix.";
	}

	oBuffer << endl << endl << "Conserved Entities";


	if ((_NumCols == 0) || (zero_nmat))
	{
		oBuffer << endl;
		for (int i=0; i<_NumRows; i++)
		{
			oBuffer << (i+1) << ": " << _speciesIndexList[spVec[i]] << endl;
		}
	}
	else if (_NumRows == _NumIndependent)
	{
		oBuffer << ": NONE" << endl;
	}
	else
	{
		oBuffer << endl;
		for (int i = 0; i < _NumDependent; i++)
		{
			oBuffer << (i+1) << ": " + _consv_list[i] << endl;
		}
	}

	oBuffer << LINE << endl << LINE << endl
		<< "Originally ddeveloped by the Computational Systems Biology Group at Keck Graduate Institute " << endl
		<< "and the Saurolab at the Bioengineering Departmant at University of Washington." << endl
		<< "Contact : Herbert M Sauro (hsauro@u.washington.edu).   " << endl << endl
		<< "Current Authors: Yosef Bedaso and Kiri Choi" << endl << endl
		<< "(previous authors) Ravishankar Rao Vallabhajosyula" << endl
		<< LINE << endl << LINE << endl << endl;

	return oBuffer.str();
}

string LibStructural::getResultString () {
	return _sResultStream.str ();
}


string LibStructural::getSummary () {
	return _sResultStream.str ();
}


void LibStructural::Initialize()
{
#ifndef NO_SBML
	if (_Model != NULL)
	{
		// free used elements
		FreeMatrices();

		// read species and reactions
		InitializeFromModel(*_Model);
		// build stoichiometry matrix
		BuildStoichiometryMatrixFromModel(*_Model);

		// initialize other matrices
		InitializeFromStoichiometryMatrix(*_Nmat);
	}
	else
#endif
	{
		if (_Nmat->numCols() != _inputReactionIds.size())
		{
			_inputReactionIds.clear();
			for (unsigned int i = 0; i < _Nmat->numCols(); i++)
			{
				stringstream sTemp; sTemp << i;
				_inputReactionIds.push_back( sTemp.str() );
			}
		}
		if (_Nmat->numRows() != _inputSpeciesIds.size())
		{
			_inputSpeciesIds.clear(); _inputValues.clear();
			for (unsigned int i = 0; i < _Nmat->numRows(); i++)
			{
				stringstream sTemp; sTemp << i;
				_inputSpeciesIds.push_back( sTemp.str() );
				_inputValues.push_back ( 1.0 );
			}
		}

		DoubleMatrix oCopy(*_Nmat);

		InitializeFromStoichiometryMatrix( oCopy	,
			_inputSpeciesIds, _inputReactionIds,
			_inputValues);
	}


}

void LibStructural::InitializeFromStoichiometryMatrix(DoubleMatrix& oMatrix,
													  vector<string>& speciesNames,
													  vector<string>& reactionNames,
													  vector<double>& concentrations)
{
	// free used elements
	FreeMatrices();

	numFloating = speciesNames.size();
	numReactions = reactionNames.size();
	numBoundary = 0;

	_sModelName = "untitled";

	CREATE_ARRAY(spVec,int,numFloating);
	CREATE_ARRAY(colVec,int,numReactions);
	_consv_list.clear();

	for (int i = 0; i < numFloating; i++)
	{
		_speciesIndexList[i] = speciesNames[i];
		_speciesNamesList[i] = speciesNames[i];
		_speciesNamesList2[_speciesNamesList[i]] = i;
		_speciesIndexList2[_speciesIndexList[i]] = i;
		_speciesValueList[_speciesIndexList[i]] = concentrations[i];
		_consv_list.push_back(_speciesIndexList[i]);
		spVec[i] = i;
	}

	for (int i = 0; i < numReactions; i++)
	{
		_reactionIndexList[i] = reactionNames[i];
		_reactionNamesList[i] = reactionNames[i];
		colVec[i] = i;
	}

	// initialize other matrices
	InitializeFromStoichiometryMatrix(oMatrix);
}


void LibStructural::InitializeFromStoichiometryMatrix(DoubleMatrix& oMatrix)
{
	_NumRows = oMatrix.numRows();
	_NumCols = oMatrix.numCols();

	if (_Nmat == NULL) _Nmat = new DoubleMatrix(oMatrix);

	// number of non-zero elements
	nz_count = 0;
	for (int i=0; i<_NumRows; i++) {
		for (int j=0; j<_NumCols; j++) {
			if (fabs(oMatrix(i,j)) > _Tolerance) nz_count++;
		}
	}
	zero_nmat = (nz_count == 0);

	// get sparsity
	_Sparsity = (double) (nz_count * 100)/(_NumRows*_NumCols);

	// get transpose
	DELETE_IF_NON_NULL(_NmatT); _NmatT = oMatrix.getTranspose();

	// store copies of stoichimetry matrix and it's transpose
	DELETE_IF_NON_NULL(_Nmat_orig); _Nmat_orig = new DoubleMatrix(oMatrix);
	DELETE_IF_NON_NULL(_NmatT_orig);_NmatT_orig = new DoubleMatrix(*_NmatT);


	// If the network has reactions only between boundary species, the stoichiometry matrix will be
	// empty. This means that it is equivalent to a network without any reactions. Therefore, we need
	// to construct the conservation matrices, dependent and independent species accordingly.
	//
	if (zero_nmat)
	{
		_NumIndependent = 0;
		_NumDependent = 0;

		_N0 = new DoubleMatrix(_NumDependent, _NumCols);
		_K0 = new DoubleMatrix(_NumIndependent, _NumCols-_NumIndependent);

		_Nr = new DoubleMatrix(_NumRows, _NumCols);

		_K = new DoubleMatrix(_NumCols,_NumCols);
		_NullN = new DoubleMatrix(_NumCols,_NumCols);

		_L0 = new DoubleMatrix(_NumRows, _NumRows);
		_L = new DoubleMatrix(_NumRows, _NumRows);
		_G = new DoubleMatrix(_NumRows, _NumRows);

		for (int i = 0; i < _NumRows; i++)
		{
			(*_L0)(i,i) =-1.0;
			(*_G)(i,i) = 1.0;
		}

		for (int i = 0; i < _NumRows; i++)
		{
			for (int j = 0; j < _NumRows; j++)
			{
				(*_L)(i,j) = (*_L0) (j,i);
			}
		}

		for (int i = 0; i < _NumCols; i++)
		{
			(*_K) (i,i) =-1.0;
			(*_NullN) (i,i) =-1.0;
		}
	}

}

#ifndef NO_SBML

void  LibStructural::BuildStoichiometryMatrixFromModel(LIB_STRUCTURAL::SBMLmodel& oModel)
{
	_NumRows = numFloating;
	_NumCols = numReactions;
	DELETE_IF_NON_NULL(_Nmat); _Nmat = new DoubleMatrix(numFloating, numReactions);

	for (int i = 0; i < numReactions; i++)
	{
		const Reaction* reaction = oModel.getNthReaction(i);
		int numReactants = reaction->getNumReactants();
		int numProducts = reaction->getNumProducts();
		for (int j = 0; j < numReactants; j++)
		{
			const SpeciesReference* reference = reaction->getReactant(j);
			if (_bSpeciesIndexList2.find(reference->getSpecies()) == _bSpeciesIndexList2.end())
			{
				int row_id = _speciesIndexList2[reference->getSpecies()];
				(*_Nmat)(row_id,i) = (*_Nmat)(row_id,i) - (reference->getStoichiometry());
			}
		}

		for (int j = 0; j < numProducts; j++)
		{
			const SpeciesReference* reference = reaction->getProduct(j);
			if (_bSpeciesIndexList2.find(reference->getSpecies()) == _bSpeciesIndexList2.end())
			{
				int row_id = _speciesIndexList2[reference->getSpecies()];

				(*_Nmat)(row_id,i) = (*_Nmat)(row_id,i) + (reference->getStoichiometry());
			}
		}
	}
}

#endif


// Uses QR Decomposition for Conservation analysis
void LibStructural::analyzeWithQR()
{
	if (numFloating == 0 || numReactions == 0) {
		return;
	}

	Initialize();

	if (_NumRows == 0)
	{
		_sResultStream << "Model has no floating species.";
	}
	else if (_NumCols == 0)
	{
		_sResultStream << "Model has no Reactions.";
	}
	else
	{
		vector< DoubleMatrix*> oQRResult = LibLA::getInstance()->getQRWithPivot(*_NmatT);
		DoubleMatrix *Q = oQRResult[0];
		DoubleMatrix *R = oQRResult[1];
		DoubleMatrix *P = oQRResult[2];

		Util::gaussJordan(*R, _Tolerance);

		// The rank is obtained by looking at the number of zero rows of R, which is
		// a lower trapezoidal matrix.
		_NumIndependent = Util::findRank(*R, _Tolerance);

		_NumDependent = _NumRows - _NumIndependent;

		DoubleMatrix L0t(_NumIndependent, _NumDependent);
		for (int i = 0; i < _NumIndependent; i++)
		{

			for (int j = 0; j < _NumDependent; j++)
			{
				L0t(i,j) = (*R)(i,j+_NumIndependent);
			}
		}

		DELETE_IF_NON_NULL(_L0); _L0 = L0t.getTranspose();

		// reorder species
		for (unsigned int i = 0; i < P->numRows(); i++)
		{
			for (unsigned int j = 0; j < P->numCols(); j++)
			{
				if ((*P)(i,j) == 1)
				{
					spVec[j]=i;
					break;
				}
			}
		}

		DELETE_IF_NON_NULL(_G); _G = new DoubleMatrix(_NumDependent, _NumRows);
		for (int i = 0; i < _NumDependent; i++)
		{
			for (int j = 0; j < _NumIndependent; j++)
			{
				(*_G)(i,j) = -(*_L0)(i,j);
			}
			(*_G)(i,_NumIndependent+i) = 1.0;
		}


		reorderNmatrix();
		computeNrMatrix();
		computeN0Matrix();
		computeLinkMatrix();
		computeConservedSums();
		computeConservedEntities();
		computeK0andKMatrices();

		DELETE_IF_NON_NULL(Q); DELETE_IF_NON_NULL(R); DELETE_IF_NON_NULL(P);

		_sResultStream << GenerateResultString();
	}

}


void LibStructural::reorderNmatrix()
{
	DELETE_IF_NON_NULL(_Nmat); _Nmat = new DoubleMatrix(_NumRows, _NumCols);
	for (int i=0; i<_NumRows; i++)
	{
		for (int j=0; j<_NumCols; j++)
		{
			(*_Nmat)(i,j) = (* _NmatT_orig)(j,spVec[i]);
		}
	}
}
void LibStructural::computeNrMatrix()
{
	DELETE_IF_NON_NULL(_Nr); _Nr = new DoubleMatrix(_NumIndependent, _NumCols);

	for (int i = 0; i < _NumIndependent; i++)
	{
		for (int j = 0; j < _NumCols; j++)
		{
			(*_Nr)(i,j) = (*_NmatT_orig)(j,spVec[i]);
		}
	}
}
void LibStructural::computeN0Matrix()
{
	DELETE_IF_NON_NULL(_N0); _N0 = new DoubleMatrix(_NumDependent, _NumCols);

	for (int i=0; i<_NumDependent; i++)
	{
		for (int j=0; j<_NumCols; j++)
		{
			(*_N0)(i,j) = (*_NmatT_orig)(j,spVec[_NumIndependent+i]);
		}
	}

}
void LibStructural::computeLinkMatrix()
{
	DELETE_IF_NON_NULL(_L);		_L = new DoubleMatrix(_NumRows, _NumIndependent);


	for (int i=0; i<_NumIndependent; i++)
	{
		(*_L)(i,i) = 1.0;
	}

	for (int i=_NumIndependent; i<_NumRows; i++)
	{
		for (int j=0; j<_NumIndependent; j++)
		{
			(*_L)(i,j) = (*_L0)(i-_NumIndependent,j);
		}
	}

}
void LibStructural::computeConservedSums()
{
	CREATE_ARRAY(_IC,double,numFloating);
	for (int i=0; i<numFloating; i++)
	{
		_IC[i] = _speciesValueList[_speciesIndexList[spVec[i]]];
	}

	CREATE_ARRAY(_BC,double,numBoundary);
	for (int i=0; i<numBoundary; i++)
	{
		_BC[i] = _bSpeciesValueList[_bSpeciesIndexList[i]];
	}

	DELETE_ARRAY_IF_NON_NULL(_T);

	if ((_NumCols == 0) || (zero_nmat))
	{
		_T = new double[numFloating];
		for (int i=0; i<numFloating; i++)
		{
			_T[i] = _IC[i];
		}
	}
	else
	{
		_T = new double[_NumDependent]; memset(_T, 0, sizeof(double)*_NumDependent);

		for (int i=0; i<_NumDependent; i++)
		{
			for (int j=0; j<numFloating; j++)
			{
				if (fabs((*_G)(i,j)) > _Tolerance)
				{
					_T[i] = _T[i] + (*_G)(i,j)*_IC[j];
				}
			}
		}
	}

}
void LibStructural::computeConservedEntities()
{

	double gval; string spname;

	_consv_list.clear();

	if (_NumCols > 0)
	{
		for (int i=0; i<(_NumDependent); i++)
		{

			stringstream oBuilder;

			for (int j=0; j<numFloating; j++)
			{
				gval = (*_G)(i,j);
				if (fabs(gval) > 0.0)
				{
					spname = _speciesIndexList[spVec[j]];
					if (gval < 0)
					{
						if (fabs(gval + 1) < _Tolerance)
							oBuilder << " - " << spname;
						else
							oBuilder << " - "  << fabs(gval) << " " << spname;
					}
					if (gval > 0)
					{
						if (fabs(gval - 1) < _Tolerance)
							oBuilder << " + " << spname;
						else
							oBuilder << " + "  << fabs(gval) << " " << spname;
					}
				}
			}
			_consv_list.push_back (oBuilder.str());
		}
	}
	else
	{
		for (int i=0; i<_NumRows; i++)
		{
			_consv_list.push_back ( _speciesIndexList[spVec[i]] );
		}
	}

}
void LibStructural::computeK0andKMatrices()
{
	DoubleMatrix Nmat_h(_NumRows, _NumCols);
	for (int i = 0; i < _NumRows; i++)
	{
		for (int j = 0; j < _NumCols; j++)
		{
			Nmat_h(i,j) = (*_Nmat_orig)(spVec[i],j);
		}
	}

	DoubleMatrix *Q; DoubleMatrix *R; DoubleMatrix *P;

	if ((_NumRows == 1 )  && ( _NumCols == 1 ))
	{
		Q = new DoubleMatrix(1,1); (*Q)(0,0) = 1.0;
		R = new DoubleMatrix(1,1); (*R)(0,0) = (*_NmatT)(0,0);
		P = new DoubleMatrix(1,1); (*P)(0,0) = 1.0;
	}
	else if ((_NumRows == 1 )  && ( _NumCols > 1 ))
	{
		Q = new DoubleMatrix(1,1); (*Q)(0,0) = 1.0;
		R = new DoubleMatrix(1,_NumCols);
		P = new DoubleMatrix(_NumCols,_NumCols);
		for (int i = 0; i < _NumCols; i++)
		{
			(*R)(0,i) = Nmat_h(0,i);
			(*P)(i,i) = 1.0;
		}
	}
	else
	{
		vector< DoubleMatrix *> oResult = LibLA::getInstance()->getQRWithPivot(Nmat_h);

		Q = oResult[0]; R = oResult[1]; P = oResult[2];
	}

	//Util::gaussJordan(*R, _Tolerance);
	Util::GaussJordan(*R, _Tolerance);

	int nDependent = _NumCols-_NumIndependent;

	DELETE_IF_NON_NULL(_K0); _K0 = new DoubleMatrix(_NumIndependent, nDependent);

	for (int i=0;  i <_NumIndependent; i++)
	{
		for (int j=0; j< _NumCols-_NumIndependent ; j++)
		{
			(*_K0)(i,j) = Util::RoundToTolerance( - (*R)(i,j+_NumIndependent), _Tolerance);
		}
	}

	DELETE_IF_NON_NULL(_K);	_K = new DoubleMatrix(_NumCols, _NumCols - _NumIndependent);


	for (int i=0; i<(_NumCols - _NumIndependent); i++)
	{
		(*_K)(i,i) = 1.0;
	}
	for (int i=0; i<_NumIndependent ; i++)
	{
		for (int j=0; j<(_NumCols - _NumIndependent); j++)
		{
			(*_K)(i+(_NumCols - _NumIndependent),j) = (*_K0)(i,j);
		}
	}

	// reorder species
	for (unsigned int i = 0; i < P->numRows(); i++)
	{
		for (unsigned int j = 0; j < P->numCols(); j++)
		{
			if ((*P)(i,j) == 1)
			{
				colVec[j]=i;
				break;
			}
		}
	}

	DELETE_IF_NON_NULL(_NullN);	_NullN = new DoubleMatrix(*_K);

	DELETE_IF_NON_NULL(Q); DELETE_IF_NON_NULL(R); DELETE_IF_NON_NULL(P);
}



//Uses LU Decomposition for Conservation analysis
void LibStructural::analyzeWithLU()
{
	if (numFloating == 0 || numReactions == 0) {
		return;
	}

	LU_Result * oLUResult = NULL;

	Initialize();

	if (_NumRows == 0)
	{
		_sResultStream << "Model has no floating species.";
	}
	else if (_NumCols == 0)
	{
		_sResultStream << "Model has no Reactions.";
	}
	else
	{
		LU_Result * oLUResult = LibLA::getInstance()->getLU(*_NmatT);
		DoubleMatrix* L = oLUResult->L;
		DoubleMatrix* U = oLUResult->U;
		IntMatrix* P = oLUResult->P;

		// nInfo is zero if there are no singular values on Umat pivot positions
		// if there are zeros, the columns of NmatT have to be permuted.
		// First we check if nInfo is < 0 (illegal value) or if it is > 0 (this
		// means a zero has been encountered on the diagonal while computing LU
		// factorization). nInfo = 0 implies a successful exit. So we have to
		// to swap the cols only if nInfo > 0
		int nInfo = oLUResult->nInfo;

		if (nInfo < 0 )
		{
			throw new ApplicationException("Exception in analyzeWithLU()", "Illegal Value encountered while performing LU Factorization");
		}
		else if (nInfo > 0)
		{
			// swap columns;
			int z_pivot = nInfo-1;
			//int nz_pivot = nInfo;
			unsigned int pvt_id, col1, col2, col1_next;
			col1 = z_pivot;
			while (col1 < U->numRows())
			{
				col2 = col1 + 1;
				col1_next = col2;
				while (col2 < U->numRows())
				{
					pvt_id = z_pivot;
					if (fabs((*U)(col2,col2)) < _Tolerance) { // then the pivot at U[i][i] is a zero
						col2++;
						continue;
					}
					// here we have found a nonzero pivot - so swap it with col1

					_NmatT->swapCols(col1,col2);
					U->swapCols(col1,col2);
					int tmp = spVec[col1];
					spVec[col1] = spVec[col2];
					spVec[col2] = tmp;
					break;
				}
				col1 = col1_next;
			}
			DELETE_IF_NON_NULL(oLUResult);

			oLUResult = LibLA::getInstance()->getLU(*_NmatT);
			L = oLUResult->L;
			U = oLUResult->U;
			P = oLUResult->P;

		}

		Util::gaussJordan(*U, _Tolerance);


		// The rank is obtained by looking at the number of zero rows of R, which is
		// a lower trapezoidal matrix.
		_NumIndependent = Util::findRank(*U, _Tolerance);

		_NumDependent = _NumRows - _NumIndependent;

		DoubleMatrix L0t(_NumIndependent, _NumDependent);
		for (int i = 0; i < _NumIndependent; i++)
		{

			for (int j = 0; j < _NumDependent; j++)
			{
				L0t(i,j) = (*U)(i,j+_NumIndependent);
			}
		}

		_L0 =  L0t.getTranspose();

		DELETE_IF_NON_NULL(_G);	_G = new DoubleMatrix(_NumDependent, _NumRows);
		for (int i = 0; i < _NumDependent; i++)
		{
			for (int j = 0; j < _NumIndependent; j++)
			{
				(*_G)(i,j) = -(*_L0)(i,j);
			}
			(*_G)(i,_NumIndependent+i) = 1.0;
		}


		reorderNmatrix();
		computeNrMatrix();
		computeN0Matrix();
		computeLinkMatrix();
		computeConservedSums();
		computeConservedEntities();
		computeK0andKMatrices();

		_sResultStream << GenerateResultString();
	}

	DELETE_IF_NON_NULL(oLUResult);
}

// Uses LU Decomposition for Conservation analysis
void LibStructural::analyzeWithLUandRunTests()
{
	if (numFloating == 0 || numReactions == 0) {
		return;
	}
	analyzeWithLU();
	_sResultStream << endl << endl;
	_sResultStream << getTestDetails();
}


// Uses fully pivoted LU Decomposition for Conservation analysis
void LibStructural::analyzeWithFullyPivotedLU()
{
	LU_Result * oLUResult = NULL;

	Initialize();

	if (_NumRows == 0)
	{
		_sResultStream << "Model has no floating species.";
	}
	else if (_NumCols == 0)
	{
		_sResultStream << "Model has no Reactions.";
	}
	else
	{
		if (zero_nmat)
		{
			_sResultStream << "Model has empty stoiciometry matrix.";
		}
		else
		{
			oLUResult = LibLA::getInstance()->getLUwithFullPivoting(*_NmatT);
			DoubleMatrix* L = oLUResult->L;
			DoubleMatrix* U = oLUResult->U;
			IntMatrix* P = oLUResult->P;
			IntMatrix* Q = oLUResult->Q;

			// nInfo is zero if there are no singular values on Umat pivot positions
			// if there are zeros, the columns of NmatT have to be permuted.
			// First we check if nInfo is < 0 (illegal value) or if it is > 0 (this
			// means a zero has been encountered on the diagonal while computing LU
			// factorization). nInfo = 0 implies a successful exit. So we have to
			// to swap the cols only if nInfo > 0
			int nInfo = oLUResult->nInfo;

			if (nInfo < 0 )
			{
				throw new ApplicationException("Exception in analyzeWithLU()", "Illegal Value encountered while performing LU Factorization");
			}
			else if (nInfo > 0)
			{
				// swap columns;
				int z_pivot = nInfo-1;
				//int nz_pivot = nInfo;
				unsigned int pvt_id, col1, col2, col1_next;
				col1 = z_pivot;
				while (col1 < U->numRows())
				{
					col2 = col1 + 1;
					col1_next = col2;
					while (col2 < U->numRows())
					{
						pvt_id = z_pivot;
						if (fabs((*U)(col2,col2)) < _Tolerance) { // then the pivot at U[i][i] is a zero
							col2++;
							continue;
						}
						// here we have found a nonzero pivot - so swap it with col1
						_NmatT->swapCols(col1,col2);
						U->swapCols(col1,col2);
						int tmp = spVec[col1];
						spVec[col1] = spVec[col2];
						spVec[col2] = tmp;
						break;
					}
					col1 = col1_next;
				}
				DELETE_IF_NON_NULL(oLUResult);

				oLUResult = LibLA::getInstance()->getLUwithFullPivoting(*_NmatT);
				L = oLUResult->L;
				U = oLUResult->U;
				P = oLUResult->P;
				Q = oLUResult->Q;

			}
			Util::gaussJordan(*U, _Tolerance);

			// The rank is obtained by looking at the number of zero rows of R, which is
			// a lower trapezoidal matrix.
			_NumIndependent = Util::findRank(*U, _Tolerance);

			_NumDependent = _NumRows - _NumIndependent;

			DoubleMatrix L0t(_NumIndependent, _NumDependent);
			for (int i = 0; i < _NumIndependent; i++)
			{

				for (int j = 0; j < _NumDependent; j++)
				{
					L0t(i,j) = (*U)(i,j+_NumIndependent);
				}
			}

			DELETE_IF_NON_NULL(_L0); _L0 = L0t.getTranspose();

			int count = 0;
			for (unsigned int i=0; i<Q->numRows(); i++) {
				for (unsigned int j=0; j<Q->numCols(); j++) {
					if ((*Q)(i,j) == 1) {
						if ((int)j < _NumRows) {
							spVec[count] = j;
							count = count + 1;
							break;
						}
					}
				}
			}

			DELETE_IF_NON_NULL(_G); _G = new DoubleMatrix(_NumDependent, _NumRows);
			for (int i = 0; i < _NumDependent; i++)
			{
				for (int j = 0; j < _NumIndependent; j++)
				{
					(*_G)(i,j) = -(*_L0)(i,j);
				}
				(*_G)(i,_NumIndependent+i) = 1.0;
			}


			reorderNmatrix();
			computeNrMatrix();
			computeN0Matrix();
			computeLinkMatrix();
			computeConservedSums();
			computeConservedEntities();
			computeK0andKMatrices();

		}

		DELETE_IF_NON_NULL(oLUResult);

		_sResultStream << GenerateResultString();
	}
}


// Uses fully pivoted LU Decomposition for Conservation analysis
void LibStructural::analyzeWithFullyPivotedLUwithTests()
{
	_sResultStream << endl << endl;
	_sResultStream << getTestDetails();

}


int LibStructural_getSummary (char* *outMessage, int *nLength)
{
	try
	{
		*outMessage = strdup (LibStructural::getInstance()->getResultString().c_str ());
		*nLength = strlen (*outMessage);
		return 0;
	}
	catch (...)
	{
		return -1;
	}
}


// Returns L0 Matrix
LibStructural::DoubleMatrix* LibStructural::getL0Matrix()
{
	if ( (_NumRows == _NumIndependent) || (_NumRows == 0) || _L0 == NULL)
	{
		return new DoubleMatrix();
	}
	else if (_NumCols == 0 || zero_nmat)
	{
		return new DoubleMatrix(*_L0);
	}
	else
	{
		DoubleMatrix* oMatrix = new DoubleMatrix(_NumRows - _NumIndependent, _NumIndependent);
		for (int i = 0; i < _NumRows - _NumIndependent; i++)
		{
			for (int j = 0; j < _NumIndependent; j++)
			{
				(*oMatrix)(i,j) = (*_L0)(i,j);
			}
		}
		return oMatrix;
	}
}


void LibStructural::getL0MatrixIds(vector< string > &oRows, vector< string > &oCols )
{
	if (numFloating == 0 || numReactions == 0) {
		return;
	}

	oRows = getDependentSpeciesIds();
	oCols = getIndependentSpeciesIds();
}

// Returns Nr Matrix
LibStructural::DoubleMatrix* LibStructural::getNrMatrix()
{
	return _Nr;
}

LibStructural::DoubleMatrix* LibStructural::getFullyReorderedNrMatrix()
{
	DoubleMatrix* NFullReordered = getFullyReorderedStoichiometryMatrix();
	DoubleMatrix* _NrNew = new DoubleMatrix(_NumIndependent, _NumCols);

	for (int i = 0; i < _NumIndependent; i++)
	{
		for (int j = 0; j < _NumCols; j++)
		{
			(*_NrNew)(i, j) = (*NFullReordered)(spVec[i], j);
		}
	}
	DELETE_IF_NON_NULL (NFullReordered);
	return _NrNew;
}

LibStructural::DoubleMatrix* LibStructural::getFullyReorderedN0StoichiometryMatrix()
{
	if (numFloating == 0 || numReactions == 0) {
		return new DoubleMatrix(0,0);
	}

	DoubleMatrix* NFullReordered = getFullyReorderedStoichiometryMatrix();

	int N_rowLen = NFullReordered->numRows();
	int Nr_rowLen = _Nr->numRows();
	int N0_rowLen = N_rowLen - Nr_rowLen;

	if (N0_rowLen == 0)
	   return new DoubleMatrix(0, 0);

	DoubleMatrix* _N0New = new DoubleMatrix(N0_rowLen, _NumCols);

	for (int i = Nr_rowLen; i < N_rowLen; i++)
	{
		for (int j = 0; j < _NumCols; j++)
		{
			(*_N0New)(i-Nr_rowLen, j) = (*NFullReordered)(spVec[i], j);
		}
	}
	DELETE_IF_NON_NULL(NFullReordered);
	return _N0New;
}


void LibStructural::getNrMatrixIds(vector< string > &oRows, vector< string > &oCols )
{
	if (numFloating == 0 || numReactions == 0) {
		return;
	}
	
	oRows = getIndependentSpeciesIds();
	oCols = getReactionsIds();
}


// Returns N0 Matrix
LibStructural::DoubleMatrix* LibStructural::getN0Matrix()
{
	return _N0;
}

void LibStructural::getN0MatrixIds(vector< string > &oRows, vector< string > &oCols )
{
	if (numFloating == 0 || numReactions == 0) {
		return;
	}
	oRows = getDependentSpeciesIds();
	oCols = getReactionsIds();
}

// Returns L, the Link Matrix
LibStructural::DoubleMatrix* LibStructural::getLinkMatrix()
{
	if (numFloating == 0 || numReactions == 0) {
		return new DoubleMatrix(0,0);
	}
	return _L;
}

void LibStructural::getLinkMatrixIds(vector< string > &oRows, vector< string > &oCols )
{
	if (numFloating == 0 || numReactions == 0) {
		return;
	}
	oRows = getReorderedSpeciesIds();
	oCols = getIndependentSpeciesIds();
}

// Returns K0
LibStructural::DoubleMatrix* LibStructural::getK0Matrix()
{
	if (numFloating == 0 || numReactions == 0) {
		return new DoubleMatrix(0,0);
	}
	return _K0;
}

void LibStructural::getK0MatrixIds(vector< string > &oRows, vector< string > &oCols )
{
	if (numFloating == 0 || numReactions == 0) {
		return;
	}
	vector<string> oReactionLables = getReorderedReactionsIds();
	DoubleMatrix *k0 = getK0Matrix();


	int nDependent = k0->numCols();
	int nIndependent = k0->numRows();

	for (int i = 0; i < nDependent; i++)
	{
		oCols.push_back(oReactionLables[nIndependent + i]);
	}


	for (int i = 0; i < nIndependent; i++)
	{
		oRows.push_back(oReactionLables[i]);
	}

}

// Returns Nullspace of NK = 0
LibStructural::DoubleMatrix* LibStructural::getKMatrix()
{
	return _K;
}

void LibStructural::getKMatrixIds(vector< string > &oRows, vector< string > &oCols )
{
	if (numFloating == 0 || numReactions == 0) {
		return;
	}
	vector<string> oReactionLables = getReorderedReactionsIds();
	DoubleMatrix *k0 = getK0Matrix();


	int nDependent = k0->numCols();
	int nIndependent = k0->numRows();

	for (int i = 0; i < nDependent; i++)
	{
		oCols.push_back(oReactionLables[nIndependent + i]);
		oRows.push_back(oReactionLables[nIndependent + i]);
	}


	for (int i = 0; i < nIndependent; i++)
	{
		oRows.push_back(oReactionLables[i]);
	}

}

vector< string > LibStructural::getReorderedReactionsIds()
{
	vector< string >	oResult;
	for (int i = 0; i < numReactions; i++)
	{
		oResult.push_back(_reactionIndexList[colVec[i]]);
	}
	return oResult;
}


//Returns the reordered list of species
vector< string > LibStructural::getReorderedSpeciesIds()
{
	vector< string >	oResult;
	for (int i = 0; i < numFloating; i++)
	{
		oResult.push_back(_speciesIndexList[spVec[i]]);
	}
	return oResult;
}

////Returns the list of species
//vector< string > LibStructural::getSpeciesIds()
//{
//	vector< string >	oResult;
//	for (int i = 0; i < numFloating; i++)
//	{
//		oResult.push_back(_speciesIndexList[i]);
//	}
//	return oResult;
//}

//Returns the actual names of the reordered species
// vector< string > LibStructural::getReorderedSpeciesNamesList()
// {
// 	vector< string >	oResult;
// 	for (int i = 0; i < numFloating; i++)
// 	{
// 		oResult.push_back(_speciesNamesList[spVec[i]]);
// 	}
// 	return oResult;
// }

// Returns the list of independent species
vector< string > LibStructural::getIndependentSpeciesIds()
{
	vector< string >	oResult;


	if (numFloating == 0)
		return oResult;
	else if (numReactions == 0 || zero_nmat)
	{
		return getReorderedSpeciesIds();
	}
	else
	{
		for (int i=0; i<_NumIndependent; i++)
		{
			oResult.push_back(_speciesIndexList[spVec[i]]);
		}
	}

	return oResult;
}


//! Returns the list of independent reactions
vector< string > LibStructural::getIndependentReactionIds()
{
	vector <string> result;
	if (numFloating == 0 || numReactions == 0) {
		return result;
	}

	int nDependent = _K0->numCols();
	int nIndependent = _Nr->numCols() - nDependent;

	for (int j = 0; j < nIndependent; j++)
	{
		result.push_back(_reactionIndexList[colVec[j]]);
	}
	return result;

}


//! Returns the list of dependent reactions
vector< string > LibStructural::getDependentReactionIds()
{
	vector<string> result;

	if (numFloating == 0 || numReactions == 0) {
		return result;
	}

	int nDependent = _K0->numCols();
	int nIndependent = _Nr->numCols() - nDependent;
	for (int j = 0; j < nDependent; j++)
	{
		result.push_back(_reactionIndexList[colVec[j + nIndependent]]);
	}
	return result;

}

//Returns the actual names of the independent species
// vector< string > LibStructural::getIndependentSpeciesNamesList()
// {
// 	vector< string >	oResult;
//
// 	if (numFloating == 0)
// 		return oResult;
// 	else if (numReactions == 0 || zero_nmat)
// 	{
// 		return getReorderedSpeciesNamesList();
// 	}
// 	else
// 	{
// 		for (int i=0; i<_NumIndependent; i++)
// 		{
// 			oResult.push_back(_speciesNamesList[spVec[i]]);
// 		}
// 	}
//
// 	return oResult;
// }


// Returns the list of dependent species
vector< string > LibStructural::getDependentSpeciesIds()
{
	vector< string >	oResult;

	if (numFloating == 0 || numReactions == 0 || zero_nmat || _NumRows == _NumIndependent)
		return oResult;

	for (int i = 0; i < _NumDependent; i++)
	{
		oResult.push_back( _speciesIndexList[spVec[_NumIndependent+i]] );
	}


	return oResult;
}

//Returns the actual names of the dependent species
// vector< string > LibStructural::getDependentSpeciesNamesList()
// {
// 	vector< string >	oResult;
//
// 	if (numFloating == 0 || numReactions == 0 || zero_nmat || _NumRows == _NumIndependent)
// 		return oResult;
//
// 	for (int i = 0; i < _NumDependent; i++)
// 	{
// 		oResult.push_back( _speciesNamesList[spVec[_NumIndependent+i]] );
// 	}
//
//
// 	return oResult;
// }

// Returns Initial Conditions used in the model
vector< pair <string, double> > LibStructural::getInitialConditions()
{
	vector< pair <string, double> > oResult;
	for (int i = 0; i < _NumRows; i++)
	{
		oResult.push_back( pair< string, double> (_speciesIndexList[spVec[i]], _IC[i]));
	}
	return oResult;
}

// Returns the list of Reactions
vector< string > LibStructural::getReactionsIds()
{
	vector< string > oResult;
	for (int i = 0; i < numReactions; i++)
	{
		oResult.push_back( _reactionIndexList[i] );
	}
	return oResult;
}

// //Returns actual names of the Reactions
// vector< string > LibStructural::getReactionsNamesList()
// {
// 	vector< string > oResult;
// 	for (int i = 0; i < numReactions; i++)
// 	{
// 		oResult.push_back( _reactionNamesList[i] );
// 	}
// 	return oResult;
// }

// Returns the list of Species
vector< string > LibStructural::getSpeciesIds()
{
	vector< string > oResult;
	for (int i = 0; i < numFloating; i++)
	{
		oResult.push_back(_speciesIndexList[i]);
	}
	return oResult;
}

//Returns actual names of the Species
// vector< string > LibStructural::getSpeciesNamesList()
// {
// 	vector< string > oResult;
// 	for (int i = 0; i < numFloating; i++)
// 	{
// 		oResult.push_back(_speciesNamesList[i]);
// 	}
// 	return oResult;
// }

// Returns Gamma, the conservation law array
LibStructural::DoubleMatrix* LibStructural::getGammaMatrix()
{
	if (numFloating == 0 || numReactions == 0) {
		return new DoubleMatrix(0,0);
	}
	return _G;
}


LibStructural::DoubleMatrix* LibStructural::findPositiveGammaMatrix(DoubleMatrix &stoichiometry,
																	std::vector< std::string> &rowLabels)
{
	DoubleMatrix *current = getGammaMatrixGJ(stoichiometry);
	if (Util::isPositive(*current, _Tolerance)) return current;
	DELETE_IF_NON_NULL(current);

	// first get a vector which we use to hold the current permutation
	vector<int> currentPermutation;
	for (unsigned int i = 0; i < stoichiometry.numRows(); i++)
	{
		currentPermutation.push_back(i);
	}

	sort(currentPermutation.begin(), currentPermutation.end());

	vector<int> lastPermutation (currentPermutation.begin(), currentPermutation.end());
#ifdef DEBUG
	int nCount = 0;
#endif
	while ( next_permutation(currentPermutation.begin(), currentPermutation.end()) )
	{
#ifdef DEBUG
		nCount ++;
#endif
		DoubleMatrix tempStoichiometry(stoichiometry);
		vector<int> tempPermutation (currentPermutation.begin(), currentPermutation.end());

		vector<string> tempRowIds(rowLabels.begin(), rowLabels.end());

		for (unsigned int i = 0; i < stoichiometry.numRows(); i++)
		{
			int j = tempPermutation[i];
			if (i == j ) continue;

			// find position
			int pos = -1;
			for (unsigned int k = i; k < tempPermutation.size(); k++)
			{
				if (k == i)
				{
					pos = k;
				}
			}

			tempStoichiometry.swapRows(tempPermutation[j], tempPermutation[pos]);

			swap(tempRowIds[ tempPermutation[j] ], tempRowIds[ tempPermutation[pos] ]);
			swap(tempPermutation[j], tempPermutation[pos]);

		}

		current = getGammaMatrixGJ(tempStoichiometry);
		if (Util::isPositive(*current, _Tolerance))
		{
			rowLabels.assign(tempRowIds.begin(), tempRowIds.end());
			return current;
		}
		DELETE_IF_NON_NULL(current);

	}
#ifdef DEBUG
	cout << "went through all " << nCount << " permutations without finding a positive gamma matrix" << endl;
#endif
	return NULL;
}

LibStructural::DoubleMatrix* LibStructural::getGammaMatrixGJ(DoubleMatrix &stoichiometry)
{
	int numCols = (int)stoichiometry.numCols();
	int numRows = (int)stoichiometry.numRows();
	DoubleMatrix workMatrix (numRows, numCols + numRows);

	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
			workMatrix(i,j) = stoichiometry(i,j);
		workMatrix(i, i + numCols) = 1.0;
	}

	Util::GaussJordan(workMatrix, _Tolerance);

	DoubleMatrix tempMatrix (numRows, numCols);
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numCols; j++)
			tempMatrix(i,j) = workMatrix(i,j);
	}

	int rank = Util::findRank(tempMatrix, _Tolerance);

	DoubleMatrix* result = new DoubleMatrix(numRows - rank, numRows);
	for (int i = 0; i < numRows - rank; i++)
	{
		for (int j = 0; j < numRows; j++)
		{
			(*result)(i,j) = workMatrix(i + rank,j+numCols);
		}
	}


	return result;
}

void LibStructural::getGammaMatrixIds(vector< string > &oRows, vector< string > &oCols )
{
	DoubleMatrix *G = getGammaMatrix();

	for (unsigned int i = 0; i < G->numRows(); i++)
	{
		stringstream stream; stream << i;
		oRows.push_back(stream.str());
	}

	oCols = getReorderedSpeciesIds();

}

//  Returns algebraic expressions for conserved cycles
vector< string > LibStructural::getConservedLaws()
{
	vector <string > oResult;
	if (_NumRows == 0 || _NumRows == _NumIndependent)
	{
		return oResult;
	}
	else if (numReactions == 0)
	{
		for (int i = 0; i < _NumRows; i++)
		{
			oResult.push_back(_consv_list[i]);
		}
	}
	else
	{
		for (int i = 0; i < _NumRows-_NumIndependent; i++)
		{
			oResult.push_back(_consv_list[i]);
		}
	}
	return oResult;
}

// Returns values for conserved cycles using Initial conditions
vector< double > LibStructural::getConservedSums()
{

	vector< double > oResult;

	if (_NumCols == 0 || zero_nmat)
	{
		computeConservedSums();
		for (int i = 0; i < _NumRows; i++)
		{
			oResult.push_back(_T[i]);
		}
	}
	else
	{
		for (int i = 0; i < _NumRows - _NumIndependent; i++)
		{
			oResult.push_back( _T[i] );
		}
	}

	return oResult;

}


// Returns the original stoichiometry matrix
LibStructural::DoubleMatrix* LibStructural::getStoichiometryMatrix()
{
	return _Nmat_orig;
}

void LibStructural::getStoichiometryMatrixIds(vector< string > &oRows, vector< string > &oCols )
{
	oRows = getSpeciesIds();
	oCols = getReactionsIds();
}


// Returns reordered stoichiometry matrix
LibStructural::DoubleMatrix* LibStructural::getReorderedStoichiometryMatrix()
{
	return _Nmat;
}

void LibStructural::getReorderedStoichiometryMatrixIds(vector< string > &oRows, vector< string > &oCols )
{
	oRows = getReorderedSpeciesIds();
	oCols = getReactionsIds();
}


bool LibStructural::testConservationLaw_1()
{
	bool bTest1 = true;
	if (_G == NULL || _Nmat == NULL) return false;
	DoubleMatrix* Zmat = Util::matMult((_NumRows-_NumIndependent), _NumRows, *_G, *_Nmat, _NumCols);
	for (int i = 0; i < _NumRows - _NumIndependent; i++)
	{
		for (int j = 0; j < _NumCols; j++)
		{
			if (fabs((*Zmat)(i,j)) > _Tolerance)
			{
				delete Zmat;
				return false;
			}
		}
	}
	delete Zmat;
	return bTest1;
}

bool LibStructural::testConservationLaw_2()
{
	if (_Nmat_orig == NULL) return false;
	vector <double> singularVals = LibLA::getInstance()->getSingularValsBySVD(*_Nmat_orig);
	_SvdRankNmat = min(_NumRows, _NumCols);
	for (unsigned int i=0; i<singularVals.size(); i++)
	{
		if (fabs(singularVals[i]) < _Tolerance) _SvdRankNmat--;
	}
	if (_SvdRankNmat != _NumIndependent) return false;
	return true;
}

bool LibStructural::testConservationLaw_3()
{
	if (_Nr == NULL) return false;
	vector <double> singularVals = LibLA::getInstance()->getSingularValsBySVD(*_Nr);
	_SvdRankNr = _NumIndependent;
	for (unsigned int i=0; i<singularVals.size(); i++)
	{
		if (fabs(singularVals[i]) < _Tolerance) _SvdRankNr--;
	}
	if (_SvdRankNr < _NumIndependent) return false;
	return true;
}

bool LibStructural::testConservationLaw_4()
{
	if (_Nmat == NULL) return false;
	vector < DoubleMatrix* > oResult = LibLA::getInstance()->getQRWithPivot(*_Nmat);

	DoubleMatrix* Q = oResult[0];
	DoubleMatrix* R = oResult[1];
	DoubleMatrix* P = oResult[2];

	DoubleMatrix* Q11 = Util::getSubMatrix(Q->numRows(), Q->numCols(), _NumIndependent, _NumIndependent, 0, 0, *Q);

	vector < Complex > q11Eigenvalues = LibLA::getInstance()->getEigenValues(*Q11);

	_QrRankNmat = 0;
	double absval = 0.0;
	for (unsigned int i=0; i<q11Eigenvalues.size(); i++)
	{
		absval = sqrt( (q11Eigenvalues[i].Real)*(q11Eigenvalues[i].Real) + (q11Eigenvalues[i].Imag)*(q11Eigenvalues[i].Imag) );
		if (absval > _Tolerance) _QrRankNmat++;
	}

	bool test4 = (_QrRankNmat == _NumIndependent);

	DELETE_IF_NON_NULL(Q); DELETE_IF_NON_NULL(R); DELETE_IF_NON_NULL(P); DELETE_IF_NON_NULL(Q11);

	return test4;
}

bool LibStructural::testConservationLaw_5()
{
	if (_Nmat == NULL || _L0 == NULL) return false;
	vector < DoubleMatrix* > oResult = LibLA::getInstance()->getQRWithPivot(*_Nmat);

	DoubleMatrix* Q = oResult[0];
	DoubleMatrix* R = oResult[1];
	DoubleMatrix* P = oResult[2];

	DoubleMatrix* Q11 = Util::getSubMatrix(Q->numRows(), Q->numCols(), _NumIndependent, _NumIndependent, 0, 0, *Q);
	DoubleMatrix* Q21 = Util::getSubMatrix(Q->numRows(), Q->numCols(), Q->numRows() - _NumIndependent, _NumIndependent, _NumIndependent, 0, *Q);

	DoubleMatrix* Q11inv = NULL;

	if (Q11->numRows() * Q11->numCols() == 0)
	{
		Q11inv = new DoubleMatrix(0,0);
	}
	else
	{
	try { Q11inv = LibLA::getInstance()->inverse(*Q11); } catch (...) {}

	if (Q11inv == NULL)
	{
		delete Q; delete R; delete P; delete Q11; delete Q21;
		return false;
	}
	}

	DoubleMatrix* L0x = Util::matMult((Q->numRows() - _NumIndependent), _NumIndependent, *Q21, *Q11inv, Q11inv->numCols());

	bool test5 = true;
	double val = 0.0;
	for (unsigned int i=0; i<(Q->numRows() - _NumIndependent); i++)
	{
		for (int j=0; j<_NumIndependent; j++)
		{
			val = (*L0x)(i,j) - (*_L0)(i,j);
			if (fabs(val) > _Tolerance)
			{
				test5 = false;
			}
		}
	}

	delete Q; delete R; delete P; delete Q11; delete Q21; delete Q11inv; delete L0x;
	return test5;
}

// Returns the NIC Matrix (partition of linearly independent columns of Nr)
DoubleMatrix* LibStructural::getNICMatrix()
{
	if (_Nr == NULL || _K0 == NULL) return NULL;
	int nDependent = _K0->numCols();
	int nIndependent = _Nr->numCols() - nDependent;
	DoubleMatrix *oCopy  = new DoubleMatrix(_Nr->numRows(), nIndependent);

	for (unsigned int i = 0; i < _Nr->numRows(); i++)
	{
		for (int j = 0; j < nIndependent; j++)
		{
			(*oCopy)(i,j) = (*_Nr)(i, colVec[j]);
		}
	}

	return oCopy;

}

void LibStructural::getNICMatrixIds(vector< string > &oRows, vector< string > &oCols )
{
	if (numFloating == 0 || numReactions == 0) {
		return;
	}
	oRows = getIndependentSpeciesIds(); //getReorderedSpeciesIds();
	int nDependent = _K0->numCols();
	int nIndependent = _Nr->numCols() - nDependent;

	for (int j = 0; j < nIndependent; j++)
	{
		oCols.push_back(_reactionIndexList[colVec[j]]);
	}

}


// Returns the NDC Matrix (partition of linearly dependent columns of Nr)
DoubleMatrix* LibStructural::getNDCMatrix()
{
	if (_Nr == NULL || _K0 == NULL) return NULL;
	int nDependent = _K0->numCols();
	int nIndependent = _Nr->numCols() - nDependent;
	DoubleMatrix *oCopy  = new DoubleMatrix(_Nr->numRows(), nDependent);

	for (unsigned int i = 0; i < _Nr->numRows(); i++)
	{
		for (int j = 0; j < nDependent; j++)
		{
			(*oCopy)(i,j) = (*_Nr)(i, colVec[j + nIndependent]);
		}
	}

	return oCopy;

}

void LibStructural::getNDCMatrixIds(vector< string > &oRows, vector< string > &oCols )
{
	if (numFloating == 0 || numReactions == 0) {
		return;
	}
	oRows = getIndependentSpeciesIds(); //getReorderedSpeciesIds();
	int nDependent = _K0->numCols();
	int nIndependent = _Nr->numCols() - nDependent;
	for (int j = 0; j < nDependent; j++)
	{
		oCols.push_back(_reactionIndexList[colVec[j + nIndependent]]);
	}
}



void LibStructural::getColumnReorderedNrMatrixIds(vector< string > &oRows, vector< string > &oCols )
{
	
	if (numFloating == 0 || numReactions == 0) {
		return;
	}
	oRows = getIndependentSpeciesIds();   //getReorderedSpeciesIds();

	int nDependent = _K0->numCols();
	int nIndependent = _Nr->numCols() - nDependent;

	for (int j = 0; j < nDependent; j++)
	{
		oCols.push_back(_reactionIndexList[colVec[j + nIndependent]]);
	}
	for (int j = 0; j < nIndependent; j++)
	{
		oCols.push_back(_reactionIndexList[colVec[j]]);
	}


}


DoubleMatrix* LibStructural::getColumnReorderedNrMatrix()
{
	if (_Nr == NULL || _K0 == NULL) return NULL;
	DoubleMatrix *oCopy  = new DoubleMatrix(_Nr->numRows(), _Nr->numCols());
	int nDependent = _K0->numCols();
	int nIndependent = _Nr->numCols() - nDependent;

	for (unsigned int i = 0; i < _Nr->numRows(); i++)
	{
		for (int j = 0; j < nDependent; j++)
		{
			(*oCopy)(i,j) = (*_Nr)(i, colVec[j + nIndependent]);
		}
		for (int j = 0; j < nIndependent; j++)
		{
			(*oCopy)(i,j + nDependent) = (*_Nr)(i, colVec[j]);
		}
	}

	return oCopy;

}


DoubleMatrix* LibStructural::getFullyReorderedStoichiometryMatrix()
{
	if (numFloating == 0 || numReactions == 0) {
		return new DoubleMatrix(0, 0);
	}

	try
	{
		// get Column reordered Matrix
		DoubleMatrix* oTemp = getColumnReorderedNrMatrix();

		// then the result matrix will be the combined NR and N0 matrix
		DoubleMatrix* oResult = new DoubleMatrix(oTemp->numRows() + _N0->numRows(), oTemp->numCols());

		int nDependent = _K0->numCols();
		int nIndependent = _Nr->numCols() - nDependent;

		for (unsigned int i = 0; i < oTemp->numRows(); i++)
		{
			for (unsigned int j = 0; j < oTemp->numCols(); j++)
			{
				(*oResult)(i,j) = (*oTemp)(i,j);
			}
		}

		// now fill the last rows with reordered N0;
		for (unsigned int i = 0; i < _N0->numRows(); i++)
		{
			for (int j = 0; j < nDependent; j++)
			{
				(*oResult)(i+oTemp->numRows(),j) = (*_N0)(i, colVec[j + nIndependent]);
			}
			for (int j = 0; j < nIndependent; j++)
			{
				(*oResult)(i+oTemp->numRows(),j + nDependent) = (*_N0)(i, colVec[j]);
			}
		}
		delete oTemp;
		return oResult;
	}
	catch(...)
	{
	}
	return NULL;
}

//! Returns Labels for the fully  Reordered stoichiometry Matrix
/*!
\param oRows a string vector that will be overwritten to hold the row labels
\param oCols a string vector that will be overwritten to hold the column labels.
*/
void LibStructural::getFullyReorderedStoichiometryMatrixIds(vector< string > &oRows, vector< string > &oCols )
{
	if (numFloating == 0 || numReactions == 0) {
		return;
	}
	getColumnReorderedNrMatrixIds(oRows, oCols);
	vector<string> dependent =  getDependentSpeciesIds();

	vector<string>::iterator it;

	for( it = dependent.begin(); it != dependent.end(); it++ )
		oRows.push_back(*it);

}


bool LibStructural::testConservationLaw_6()
{
	bool bTest1 = true;
	if (_K0 == NULL || _NmatT == NULL) return false;

	DoubleMatrix* oCopy = getColumnReorderedNrMatrix();
	DoubleMatrix* Zmat = Util::matMult(*oCopy, *_K);

	for (unsigned int i = 0; i < Zmat->numRows(); i++)
	{
		for (unsigned int j = 0; j < Zmat->numCols(); j++)
		{
			if (fabs((*Zmat)(i,j)) > _Tolerance)
			{
				delete Zmat; delete oCopy;
				return false;
			}
		}
	}
	delete Zmat;delete oCopy;
	return bTest1;
}

// Tests if conservation laws are correct
vector< string > LibStructural::validateStructuralMatrices()
{
	vector < string > oResult;

	if (testConservationLaw_1()) oResult.push_back("Pass");
	else oResult.push_back("Fail");

	if (testConservationLaw_2()) oResult.push_back("Pass");
	else oResult.push_back("Fail");

	if (testConservationLaw_3()) oResult.push_back("Pass");
	else oResult.push_back("Fail");

	if (testConservationLaw_4()) oResult.push_back("Pass");
	else oResult.push_back("Fail");

	if (testConservationLaw_5()) oResult.push_back("Pass");
	else oResult.push_back("Fail");

	if (testConservationLaw_6()) oResult.push_back("Pass");
	else oResult.push_back("Fail");

	return oResult;

}

// Return Details about conservation tests
string LibStructural::getTestDetails()
{
	stringstream oBuffer;

	vector < string > testResults = validateStructuralMatrices();

	oBuffer << "Testing Validity of Conservation Laws." << endl << endl;
	if (testResults[0] == "Pass")
		oBuffer << "Passed Test 1 : Gamma*N = 0 (Zero matrix)" << endl;
	else
		oBuffer << "Failed Test 1 : Gamma*N != 0 (Zero matrix)" << endl;

	if (testResults[1] == "Pass")
		oBuffer << "Passed Test 2 : Rank(N) using SVD (" << _SvdRankNmat << ") is same as m0 (" << _NumIndependent << ")" << endl;
	else
		oBuffer << "Failed Test 2 : Rank(N) using SVD (" << _SvdRankNmat << ") is different from m0 (" << _NumIndependent << ")" << endl;

	if (testResults[2] == "Pass")
		oBuffer << "Passed Test 3 : Rank(NR) using SVD (" << _SvdRankNr << ") is same as m0 (" << _NumIndependent << ")" << endl;
	else
		oBuffer << "Failed Test 3 : Rank(NR) using SVD (" << _SvdRankNr << ") is different from m0 (" << _NumIndependent << ")" << endl;

	if (testResults[3] == "Pass")
		oBuffer << "Passed Test 4 : Rank(NR) using QR (" << _QrRankNmat << ") is same as m0 (" << _NumIndependent << ")" << endl;
	else
		oBuffer << "Failed Test 4 : Rank(NR) using QR (" << _QrRankNmat << ") is different from m0 (" << _NumIndependent << ")" << endl;

	if (testResults[4] == "Pass")
		oBuffer << "Passed Test 5 : L0 obtained with QR matches Q21*inv(Q11)" << endl;
	else
		oBuffer << "Failed Test 5 : L0 obtained with QR is different from Q21*inv(Q11)" << endl;

	if (testResults[5] == "Pass")
		oBuffer << "Passed Test 6 : N*K = 0 (Zero matrix)" << endl;
	else
		oBuffer << "Failed Test 6 : N*K != 0 (Zero matrix)" << endl;

	return oBuffer.str();
}

// Returns the name of the model
string LibStructural::getModelName()
{
	return _sModelName;
}

// Returns the total number of species
int LibStructural::getNumSpecies()
{
	return numFloating;
}

// Returns the number of independent species
int LibStructural::getNumIndSpecies()
{
	return _NumIndependent;
}

// Returns the number of dependent species
int LibStructural::getNumDepSpecies()
{
	
	return _NumDependent;
}

// Returns the total number of reactions
int LibStructural::getNumReactions()
{
	return numReactions;
}

// Returns the number of independent reactions
int LibStructural::getNumIndReactions()
{
	if (numFloating == 0 || numReactions == 0) {
		return 0;
	}
	return _Nr->numCols() - _K0->numCols();
}

// Returns the number of dependent reactions
int LibStructural::getNumDepReactions()
{
	if (numFloating == 0 || numReactions == 0) {
		return 0;
	}
	return _K0->numCols();
}

// Returns rank of stoichiometry matrix
int LibStructural::getRank()
{
	return _NumIndependent;
}

// Returns the number of nonzero values in Stoichiometry matrix
double LibStructural::getNmatrixSparsity()
{
	if ( (_NumRows == 0 ) || (_NumCols == 0) ) _Sparsity = 0.0;
	return _Sparsity;
}


// Returns a matrix of elementary modes
DoubleMatrix* LibStructural::getElementaryModes () {
	
	struct mt_mat *stoichiometryMatrix;
	struct mt_mat *elm;
	struct mt_vector *reversibilityList;
	DoubleMatrix *oResult;
	long value;

	if (_NumRows == 0)
	{
		_sResultStream << "Model has no floating species.";
		throw  std::runtime_error ("Model has no floating species.");
	}
	else if (_NumCols == 0)
	{
		_sResultStream << "Model has no Reactions.";
		throw  std::runtime_error ("Model has no reactions.");
	}
	else
	{
		reversibilityList = mt_createVector (numReactions);
		for (int i = 0; i < numReactions; i++) {
			const Reaction* reaction = _Model->getNthReaction (i);
			if (reaction->getReversible ()) {
				mt_setVectorItem (reversibilityList, i, mt_REVERSIBLE);
			}
			else {
				mt_setVectorItem (reversibilityList, i, mt_IRREVERSIBLE);
			}
		}

		stoichiometryMatrix = mt_createMatrix (numFloating, numReactions);
		for (int i = 0; i < numFloating; i++) {
			for (int j = 0; j < numReactions; j++) {
				value = (*_Nmat_orig)(i, j);
				mt_setMatrixItem (stoichiometryMatrix, i, j, value);
			}
		}
	
		// Initialize Metatool
		mt_initialize (stoichiometryMatrix, reversibilityList);
	    // Compute elementary mode
		elm = mt_elementaryModes ();

		// Destroy Metatool
		mt_destroy ();
		mt_freeMatrix (stoichiometryMatrix);
		mt_freeVector (reversibilityList);
	}

	oResult = new DoubleMatrix (elm->row, elm->col);
	for (int i = 0; i < elm->row; i++) {
		for (int j = 0; j < elm->col; j++) {
			mt_getMatrixItem (elm, i, j, &value);
			(*oResult) (i, j) = value;
		}
	}
	mt_freeMatrix (elm);

	return oResult;
}


//Set user specified tolerance
void LibStructural::setTolerance(double dTolerance)
{
	_Tolerance = dTolerance;
}

LibStructural* LibStructural::getInstance()
{
	if (_Instance == NULL)
		_Instance = new LibStructural();
	return _Instance;
}


// load a new stoichiometry matrix and reset current loaded model
void LibStructural::loadStoichiometryMatrix (DoubleMatrix& oMatrix)
{
#ifndef NO_SBML
	DELETE_IF_NON_NULL(_Model);
#endif
	FreeMatrices();
	_inputReactionIds.clear();
	_inputSpeciesIds.clear();
	_inputValues.clear();
	DELETE_IF_NON_NULL(_Nmat);
	_Nmat = new DoubleMatrix(oMatrix);

	// Add default reaction names to matrix
	for (int i = 0; i < oMatrix.numCols(); i++) {
		std::string sTemp; sTemp = "_J" + std::to_string (i);
		_inputReactionIds.push_back (sTemp);
	}

	// Add default species names to matrix
	for (int i = 0; i < oMatrix.numRows(); i++) {
	    std::string sTemp; sTemp = "S" + std::to_string (i);
		_inputSpeciesIds.push_back (sTemp);
		_inputValues.push_back (1.0);
	}

	DoubleMatrix oCopy (*_Nmat);

	InitializeFromStoichiometryMatrix (oCopy,
		_inputSpeciesIds, _inputReactionIds,
		_inputValues);

	analyzeWithQR ();
}

// load reaction Ids and initial values
void LibStructural::loadReactionIdsWithValues(vector< string > &reactionIds, vector<double> &reactionValues)
{
	_inputReactionIds.assign(reactionIds.begin(), reactionIds.end());
	_inputValues.assign(reactionValues.begin(), reactionValues.end());
}

// load species names and initial values
void LibStructural::loadSpeciesIdsWithValues ( vector< string > &speciesIds, vector<double> &speciesValues)
{
	_inputSpeciesIds.assign(speciesIds.begin(), speciesIds.end());
	_inputValues.assign(speciesValues.begin(), speciesValues.end());
}

// load reaction names
void LibStructural::loadReactionIds ( vector< string > &reactionNames)
{
	_inputReactionIds.assign(reactionNames.begin(), reactionNames.end());
}

//// load reaction names
//void LibStructural::loadSpeciesNames(vector< string > &speciesNames)
//{
//	_inputSpeciesNames.assign(speciesNames.begin(), speciesNames.end());
//}



/*   Other Miscellaneous functions   */
// -----------------------------------------------------------------------

LibStructural::DoubleMatrix* LibStructural::rref_FB(DoubleMatrix &oMatrix, double tolerance)
{
	int _NumCols = (int)oMatrix.numCols();
	int _NumRows = (int)oMatrix.numRows();

	DoubleMatrix* oResult = new DoubleMatrix(_NumRows, _NumCols);

	for (int i = 0; i < _NumRows; i++)
		for (int j = 0; j < _NumCols; j++)
		{
			(*oResult)(i, j) = (oMatrix)(i, j);
		}

    std::vector <int> rowPivots;
    std::vector <int> colPivots;

	if (tolerance < 0)
	   Util::GaussJordan(*oResult, fabs (tolerance));
	else
	   Util::FullyPivotedGaussJordan(*oResult, fabs (tolerance), rowPivots, colPivots);
	return oResult;
}


void cleanUpMatrix (DoubleMatrix *oMatrix, double tolerance) {
	// Removes all numbers close to zero, i.e between -tol and +tol
	int ri = oMatrix->numRows(); int ci = oMatrix->numCols();
	for (int i = 0; i < ri; i++)
		for (int j = 0; j < ci; j++)
			if (fabs((*oMatrix)(i, j)) < tolerance)
				(*oMatrix)(i, j) = 0;
}


void exchangeRows (DoubleMatrix *oMatrix, int r1, int r2) {

	int nCols = oMatrix->numCols();
	double t = 0;
	for (int i = 0; i < nCols; i++) {
		t = (*oMatrix)(r1, i);
		(*oMatrix)(r1, i) = (*oMatrix)(r2, i);
		(*oMatrix)(r2, i) = t;
	}
}

LibStructural::DoubleMatrix* LibStructural::rref(DoubleMatrix &oMatrix, double tolerance)
{
	int nRows = (int)oMatrix.numRows();
	int nCols = (int)oMatrix.numCols();
	int currentRow; double factor;

	DoubleMatrix* oEchelon = new DoubleMatrix(nRows, nCols);

	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
		{
			(*oEchelon)(i, j) = (oMatrix)(i, j);
		}

	int Arow = 0; int Acol = 0;
	while ((Arow < nRows) && (Acol < nCols)) {
		// locate a nonzero column
		if (fabs((*oEchelon)(Arow, Acol)) < tolerance) {
			// If the entry is zero work our way down the matrix
			// looking for a nonzero entry, when found, swap it for Arow
			currentRow = Arow;
			do {
				// next row
				currentRow++;
				// Have we reached the end of the rows but we've still got columns left to scan?
				if ((currentRow >= nRows) && (Acol <= nCols)) {
					// reset row counter back to where it was and try next column
					currentRow = Arow; Acol++;
				}

				// If we've scanned the whole matrix, then lets get out...
				if (currentRow >= nRows) {
					cleanUpMatrix(oEchelon, tolerance);
					return oEchelon;
				}
			} while (fabs((*oEchelon)(currentRow, Acol)) < tolerance);

			// We've found a nonzero row entry so swap it with 'Arow' which did have a zero as its entry
			oEchelon->swapRows(Arow, currentRow);
			//exchangeRows(oEchelon, Arow, currentRow);
		}
		// Arow now holds the row of interest }
		factor = 1.0 / (*oEchelon)(Arow, Acol);
		// reduce all the entries along the column by the factor
		for (int i = Acol; i < nCols; i++)
			(*oEchelon)(Arow, i) = (*oEchelon)(Arow, i) * factor;

		// now eliminate all entries above and below Arow, this generates the reduced form
		for (int i = 0; i < nRows; i++) {
			// miss out Arow itself
			if ((i != Arow) && (fabs((*oEchelon)(i, Acol)) > tolerance)) {
				factor = (*oEchelon)(i, Acol);
				// work your way along the column doing the same operation
				for (int j = Acol; j < nCols; j++) {
			   	    (*oEchelon)(i, j) = (*oEchelon)(i, j) - factor * (*oEchelon)(Arow, j);
			    }
			}
		}

		Arow++; Acol++;
	}
	cleanUpMatrix (oEchelon, tolerance);
	return oEchelon;
}



LibStructural::DoubleMatrix* LibStructural::getEigenVectors(DoubleMatrix &oMatrix)
{
	ComplexMatrix* oCm = LibLA::getInstance()->getEigenVectors(oMatrix);

	DoubleMatrix* oResult = new DoubleMatrix(oCm->size()/2, oCm->size()*2);

	int colPair = 0;
	for (int i = 0; i < oCm->numRows(); i++)
		for (int j = 0; j < oCm->numCols(); j++) {
			(*oResult)(i, colPair) = (*oCm)(i, j).Real;
			(*oResult)(i, colPair+1) = (*oCm)(i, j).Imag;
			colPair = colPair + 2;
		}

	return oResult;
}


double LibStructural::getConditionNumber (DoubleMatrix &oMatrix)
{
	vector <double> singularVals = LibLA::getInstance()->getSingularValsBySVD(oMatrix);

	double largest = -999999.0;
	double smallest = 9999999.0;

	for (int i = 0; i < singularVals.size(); i++) {
		if (singularVals[i] > largest)
			largest = singularVals[i];
		if (singularVals[i] < smallest)
			smallest = singularVals[i];
	}

	if (smallest == 0)
		return 1000000;
	return largest / smallest;
}


LibStructural::DoubleMatrix* LibStructural::getEigenValues (DoubleMatrix &oMatrix)
{
	int _NumCols = (int)oMatrix.numCols();
	int _NumRows = (int)oMatrix.numRows();

	DoubleMatrix* oResult = new DoubleMatrix(_NumRows, _NumCols);

	vector < Complex > eigenvalues = LibLA::getInstance()->getEigenValues(oMatrix);

	for (unsigned int i = 0; i<eigenvalues.size(); i++)
	{
		(*oResult)(i, 0) = eigenvalues[i].Real;
		(*oResult)(i, 1) = eigenvalues[i].Imag;
	}

	return oResult;
}

LibStructural::DoubleMatrix* LibStructural::getLeftNullSpace(DoubleMatrix &oMatrix) {
	return LibLA::getInstance()->getLeftNullSpace(oMatrix);
}

LibStructural::DoubleMatrix* LibStructural::getRightNullSpace(DoubleMatrix &oMatrix) {
	return LibLA::getInstance()->getRightNullSpace(oMatrix);
}


int LibStructural::getRank(DoubleMatrix &oMatrix) {
	return LibLA::getInstance()->getRank(oMatrix);
}


double LibStructural::getRCond(DoubleMatrix &oMatrix) {
	return LibLA::getInstance()->getRCond(oMatrix);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// load a new stoichiometry matrix and reset current loaded model
LIB_EXTERN  int LibStructural_loadStoichiometryMatrix (double ** inMatrix, const int nRows, const int nCols)
{
	DoubleMatrix oMatrix(inMatrix, nRows, nCols);
	LibStructural::getInstance()->loadStoichiometryMatrix( oMatrix );
	return 0;
}

// load species names and initial values
LIB_EXTERN  int LibStructural_loadSpeciesIdsWithValues ( const char** speciesIds, const double* speciesValues, const int nLength)
{
	vector< string > oNames; vector< double> oValues;
	for (int i = 0; i < nLength; i++)
	{
		oNames.push_back(string(speciesIds[i]));
		oValues.push_back(speciesValues[i]);
	}
	LibStructural::getInstance()->loadSpeciesIdsWithValues(oNames, oValues);
	return 0;
}

// Load reaction names
LIB_EXTERN  int LibStructural_loadReactionIds ( const char** reactionIds, const int nLength)
{
	vector< string > oNames;
	for (int i = 0; i < nLength; i++)
	{
		oNames.push_back(string(reactionIds[i]));
	}
	LibStructural::getInstance()->loadReactionIds(oNames);
	return 0;
}

// load species names
//LIB_EXTERN  int LibStructural_loadSpeciesIds(const char** speciesIds, const int nLength)
//{
//	vector< string > oNames;
//	for (int i = 0; i < nLength; i++)
//	{
//		oNames.push_back(string(speciesIds[i]));
//	}
//	LibStructural::getInstance()->loadSpeciesIds(oNames);
//	return 0;
//}


#ifndef NO_SBML

// Initialization method, takes SBML as input
LIB_EXTERN  int LibStructural_loadSBMLFromString(const char* sSBML)
{
	try
	{
		LibStructural::getInstance()->loadSBMLFromString(string(sSBML));
		return 0;
	}
	catch (...)
	{
		return -1;
	}
}

LIB_EXTERN  int LibStructural_loadSBMLFromFile(const char* sFileName)
{
	try
	{
		LibStructural::getInstance()->loadSBMLFromFile(string(sFileName));
		return 0;
	}
	catch (...)
	{
		return -1;
	}
}


// Initialization method, takes SBML as input
LIB_EXTERN  int LibStructural_loadSBMLwithTests(const char* sSBML, char* *oResult, int *nLength)
{
	try
	{
		string sbmlString(sSBML);
		string sResult = LibStructural::getInstance()->loadSBMLwithTests(sbmlString);
		(*oResult) = strdup(sResult.c_str());
		(*nLength) = strlen(*oResult);
		return 0;
	}
	catch(...)
	{
		return -1;
	}
}

#endif

// Uses QR factorization for Conservation analysis
LIB_EXTERN  int LibStructural_analyzeWithQR(char* *outMessage, int *nLength)
{
	LibStructural::getInstance ()->analyzeWithQR ();
	*outMessage = strdup (LibStructural::getInstance ()->getResultString().c_str ());
	*nLength = strlen(*outMessage);
	return 0;
}

// Uses LU Decomposition for Conservation analysis
LIB_EXTERN  int LibStructural_analyzeWithLU(char* *outMessage, int *nLength)
{
	LibStructural::getInstance ()->analyzeWithLU ();
	*outMessage = strdup(LibStructural::getInstance ()->getResultString ().c_str ());
	*nLength = strlen(*outMessage);
	return 0;
}

// Uses LU Decomposition for Conservation analysis
LIB_EXTERN  int LibStructural_analyzeWithLUandRunTests(char* *outMessage, int *nLength)
{
	LibStructural::getInstance ()->analyzeWithLUandRunTests ();
	*outMessage = strdup(LibStructural::getInstance ()->getResultString ().c_str ());
	*nLength = strlen(*outMessage);
	return 0;
}

// Uses fully pivoted LU Decomposition for Conservation analysis
LIB_EXTERN  int LibStructural_analyzeWithFullyPivotedLU(char* *outMessage, int *nLength)
{
	LibStructural::getInstance ()->analyzeWithFullyPivotedLU ();
	*outMessage = strdup(LibStructural::getInstance ()->getResultString ().c_str ());
	*nLength = strlen(*outMessage);
	return 0;
}

// Uses fully pivoted LU Decomposition for Conservation analysis
LIB_EXTERN  int LibStructural_analyzeWithFullyPivotedLUwithTests(char* *outMessage, int *nLength)
{
	LibStructural::getInstance ()->analyzeWithFullyPivotedLUwithTests ();
	*outMessage = strdup(LibStructural::getInstance ()->getResultString ().c_str ());
	*nLength = strlen(*outMessage);
	return 0;
}

// Returns L0 Matrix
LIB_EXTERN  int LibStructural_getL0Matrix(double** *outMatrix, int* outRows, int *outCols)
{
	DoubleMatrix *oTemp = LibStructural::getInstance()->getL0Matrix();
	Util::CopyMatrix(*oTemp, *outMatrix, *outRows, *outCols);
	delete oTemp;
	return 0;
}

// Returns Nr Matrix
LIB_EXTERN  int LibStructural_getNrMatrix(double** *outMatrix, int* outRows, int *outCols)
{
	DoubleMatrix* oMatrix = LibStructural::getInstance()->getNrMatrix();
	if (oMatrix == NULL)
		return -1;
	Util::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
	return 0;
}


// Returns N0 Matrix
LIB_EXTERN  int LibStructural_getN0Matrix(double** *outMatrix, int* outRows, int *outCols)
{
	DoubleMatrix* oMatrix = LibStructural::getInstance()->getN0Matrix();
	if (oMatrix == NULL)
		return -1;
	Util::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
	return 0;
}

// Returns L, the Link Matrix
LIB_EXTERN  int LibStructural_getLinkMatrix(double** *outMatrix, int* outRows, int *outCols)
{
	DoubleMatrix* oMatrix = LibStructural::getInstance()->getLinkMatrix();
	if (oMatrix == NULL)
		return -1;
	Util::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
	return 0;
}

// Returns K0
LIB_EXTERN  int LibStructural_getK0Matrix(double** *outMatrix, int* outRows, int *outCols)
{
	DoubleMatrix* oMatrix = LibStructural::getInstance()->getK0Matrix();
	if (oMatrix == NULL)
		return -1;
	Util::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
	return 0;
}

// Returns K Matrix
LIB_EXTERN  int LibStructural_getKMatrix(double** *outMatrix, int* outRows, int *outCols)
{
	DoubleMatrix* oMatrix = LibStructural::getInstance()->getKMatrix();
	if (oMatrix == NULL)
		return -1;
	Util::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
	return 0;
}

////Returns the reordered list of species
//LIB_EXTERN  int LibStructural_getNthReorderedSpeciesId(int n,char* *outMessage, int *nLength)
//{
//	outMessage = strdup(LibStructural::getInstance()->getReorderedSpeciesIds()[n].c_str());
//	nLength = strlen(outMessage);
//	return nLength;
//}

////Returns the list of independent species
//LIB_EXTERN  int LibStructural_getNthIndependentSpeciesId(int n,char* *outMessage, int *nLength)
//{
//	outMessage = strdup(LibStructural::getInstance()->getIndependentSpeciesIds()[n].c_str());
//	nLength = strlen(outMessage);
//	return nLength;
//}

////Returns the list of dependent species
//LIB_EXTERN  int LibStructural_getNthDependentSpeciesId(int n,char* *outMessage, int *nLength)
//{
//	outMessage = strdup(LibStructural::getInstance()->getDependentSpeciesIds()[n].c_str());
//	nLength = strlen(outMessage);
//	return nLength;
//}

////Returns the list of Reactions
//LIB_EXTERN  int LibStructural_getNthReactionId(int n,char* *outMessage, int *nLength)
//{
//	outMessage = strdup(LibStructural::getInstance()->getReactionsIds()[n].c_str());
//	nLength = strlen(outMessage);
//	return nLength;
//}


// Returns Gamma, the conservation law array
LIB_EXTERN  int LibStructural_getGammaMatrix(double** *outMatrix, int* outRows, int *outCols)
{
	DoubleMatrix* oMatrix = LibStructural::getInstance()->getGammaMatrix();
	if (oMatrix == NULL)
		return -1;
	Util::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
	return 0;
}

LIB_EXTERN int LibStructural_getGammaMatrixGJ(double** inMatrix, int numRows, int numCols,
										  double** *outMatrix, int *outRows, int *outCols)
{
	DoubleMatrix oMatrix(inMatrix, numRows, numCols);
	DoubleMatrix *oResult = LibStructural::getInstance()->getGammaMatrixGJ( oMatrix );

	Util::CopyMatrix(*oResult, *outMatrix, *outRows, *outCols); delete oResult;

	return 0;
}

LIB_EXTERN int LibStructural_findPositiveGammaMatrix(double** inMatrix, int numRows, int numCols,
													 const char** inRowLabels,
													 double** *outMatrix, int *outRows, int *outCols,
													 char** *outRowLabels, int *outRowCount)
{
	DoubleMatrix oMatrix(inMatrix, numRows, numCols);


	vector< string > rowNames;
	for (int i = 0; i < numRows; i++)
	{
		rowNames.push_back(string(inRowLabels[i]));
	}

	DoubleMatrix *oResult = LibStructural::getInstance()->findPositiveGammaMatrix( oMatrix, rowNames );
	if (oResult == NULL)
		return -1;
	Util::CopyMatrix(*oResult, *outMatrix, *outRows, *outCols); delete oResult;
	Util::CopyStringVector(rowNames, *outRowLabels, *outRowCount);

	return 0;
}


//// Returns algebraic expressions for conserved cycles
//LIB_EXTERN  int LibStructural_getNthConservedEntity(int n,char* *outMessage, int *nLength)
//{
//	outMessage = strdup(LibStructural::getInstance()->getConservedLaws()[n].c_str());
//	nLength = strlen(outMessage);
//	return nLength;
//}

// Returns values for conserved cycles using Initial conditions
LIB_EXTERN  int LibStructural_getNumConservedSums()
{
	return (int) LibStructural::getInstance()->getConservedSums().size();
}


//LIB_EXTERN double LibStructural_getNthConservedSum(int n)
//{
//	return LibStructural::getInstance()->getConservedSums()[n];
//}

// Returns Initial Conditions used in the model
///LIB_EXTERN  int vector< pair <string, double> > LibStructural_getInitialConditions();

// Returns the original stoichiometry matrix
LIB_EXTERN  int LibStructural_getStoichiometryMatrix(double** *outMatrix, int* outRows, int *outCols)
{
	DoubleMatrix* oMatrix = LibStructural::getInstance()->getStoichiometryMatrix();
	if (oMatrix == NULL)
		return -1;
	Util::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
	return 0;
}

// Returns reordered stoichiometry matrix
LIB_EXTERN  int LibStructural_getReorderedStoichiometryMatrix(double** *outMatrix, int* outRows, int *outCols)
{
	DoubleMatrix* oMatrix = LibStructural::getInstance()->getReorderedStoichiometryMatrix();
	if (oMatrix == NULL)
		return -1;
	Util::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
	return 0;
}

// Tests if conservation laws are correct
LIB_EXTERN  int  LibStructural_validateStructuralMatrices(int* *outResults, int* outLength)
{
	vector< string > oResult = LibStructural::getInstance()->validateStructuralMatrices();

	*outResults = (int*) malloc(sizeof(int)*oResult.size()); memset(*outResults, 0, sizeof(int)*oResult.size());
	*outLength = oResult.size();

	for (int i = 0; i < *outLength; i++)
	{
		(*outResults)[i] = (int) (oResult[i]=="Pass");
	}
	return 0;
}

// Return Details about conservation tests
LIB_EXTERN  int LibStructural_getTestDetails(char* *outMessage, int *nLength)
{
	*outMessage = strdup(LibStructural::getInstance()->getTestDetails().c_str());
	*nLength = strlen(*outMessage);
	return 0;

}

//Returns the name of the model
LIB_EXTERN  int LibStructural_getModelName(char* *outMessage, int *nLength)
{
	*outMessage = strdup(LibStructural::getInstance()->getModelName().c_str());
	*nLength = strlen(*outMessage);
	return 0;

}

// Returns the total number of species
LIB_EXTERN  int LibStructural_getNumSpecies()
{
	return LibStructural::getInstance()->getNumSpecies();
}

// Returns the number of independent species
LIB_EXTERN  int LibStructural_getNumIndSpecies()
{
	return LibStructural::getInstance()->getNumIndSpecies();
}

// Returns the number of dependent species
LIB_EXTERN  int LibStructural_getNumDepSpecies()
{
	return LibStructural::getInstance()->getNumDepSpecies();
}

// Returns the total number of reactions
LIB_EXTERN  int LibStructural_getNumReactions()
{
	return LibStructural::getInstance()->getNumReactions();
}

// Returns the number of independent reactions
LIB_EXTERN  int LibStructural_getNumIndReactions()
{
	return LibStructural::getInstance()->getNumIndReactions();
}
// Returns the number of dependent reactions
LIB_EXTERN  int LibStructural_getNumDepReactions()
{
	return LibStructural::getInstance()->getNumDepReactions();
}



// Returns rank of stoichiometry matrix
LIB_EXTERN  int LibStructural_getRank()
{
	return LibStructural::getInstance()->getRank();
}

// Returns the number of nonzero values in Stoichiometry matrix
LIB_EXTERN  double LibStructural_getNmatrixSparsity()
{
	return LibStructural::getInstance()->getNmatrixSparsity();
}


LIB_EXTERN  int LibStructural_getElementaryModes (double** *outMatrix, int* outRows, int *outCols) {
	try
	{
		DoubleMatrix *oTemp = LibStructural::getInstance ()->getElementaryModes ();
		Util::CopyMatrix (*oTemp, *outMatrix, *outRows, *outCols);
		delete oTemp;
		return 0;
	}
	catch (...)
	{
		return -1;
	}
}

// Set user specified tolerance
LIB_EXTERN  void LibStructural_setTolerance(double dTolerance)
{
	LibStructural::getInstance()->setTolerance(dTolerance);
}

LIB_EXTERN void LibStructural_freeVector(void* vector)
{
	if (vector) free(vector);
}

LIB_EXTERN void LibStructural_freeMatrix(void** matrix, int numRows)
{
	for (int i = 0; i < numRows; i++)
	{
		if (matrix[i]) free(matrix[i]);
	}
	free(matrix);
}


LIB_EXTERN int LibStructural_getConservedSums(double* *outArray, int *outLength)
{
	vector<double> oSums = LibStructural::getInstance()->getConservedSums();
	Util::CopyDoubleVector(oSums, *outArray, *outLength);
	return 0;

}
LIB_EXTERN  int LibStructural_getConservedLaws(char** *outArray, int *outLength)
{
	vector<string> oValues = LibStructural::getInstance()->getConservedLaws();
	Util::CopyStringVector(oValues, *outArray, *outLength);
	return 0;
}
LIB_EXTERN  int LibStructural_getReactionsIds(char** *outArray, int *outLength)
{
	vector<string> oValues = LibStructural::getInstance()->getReactionsIds();
	Util::CopyStringVector(oValues, *outArray, *outLength);
	return 0;
}
LIB_EXTERN  int LibStructural_getDependentSpeciesIds(char** *outArray, int *outLength)
{
	vector<string> oValues = LibStructural::getInstance()->getDependentSpeciesIds();
	Util::CopyStringVector(oValues, *outArray, *outLength);
	return 0;
}
LIB_EXTERN  int LibStructural_getIndependentSpeciesIds(char** *outArray, int *outLength)
{
	vector<string> oValues = LibStructural::getInstance()->getIndependentSpeciesIds();
	Util::CopyStringVector(oValues, *outArray, *outLength);
	return 0;
}

LIB_EXTERN  int LibStructural_getDependentReactionIds(char** *outArray, int *outLength)
{
	vector<string> oValues = LibStructural::getInstance()->getDependentReactionIds();
	Util::CopyStringVector(oValues, *outArray, *outLength);
	return 0;
}
LIB_EXTERN  int LibStructural_getIndependentReactionIds(char** *outArray, int *outLength)
{
	vector<string> oValues = LibStructural::getInstance()->getIndependentReactionIds();
	Util::CopyStringVector(oValues, *outArray, *outLength);
	return 0;
}

LIB_EXTERN  int LibStructural_getReorderedReactionIds(char** *outArray, int *outLength)
{
	vector<string> oValues = LibStructural::getInstance()->getReorderedReactionsIds();
	Util::CopyStringVector(oValues, *outArray, *outLength);
	return 0;
}


LIB_EXTERN  int LibStructural_getSpeciesIds(char** *outArray, int *outLength)
{
	vector<string> oValues = LibStructural::getInstance()->getSpeciesIds();
	Util::CopyStringVector(oValues, *outArray, *outLength);
	return 0;
}


LIB_EXTERN  int LibStructural_getReorderedSpeciesIds(char** *outArray, int *outLength)
{
	vector<string> oValues = LibStructural::getInstance()->getReorderedSpeciesIds();
	Util::CopyStringVector(oValues, *outArray, *outLength);
	return 0;
}

LIB_EXTERN  int LibStructural_getInitialConditions(char** *outVariableNames, double* *outValues, int *outLength)
{
	vector< pair < string, double> > oInitialConditions = LibStructural::getInstance()->getInitialConditions();

	*outLength = oInitialConditions.size();

	*outVariableNames = (char**)malloc(sizeof(char*)**outLength); memset(*outVariableNames, 0, sizeof(char*)**outLength);
	*outValues = (double*) malloc(sizeof(double)**outLength); memset(*outValues, 0, sizeof(double)**outLength);

	for (int i = 0; i < *outLength; i++)
	{
		pair<string,double> oTemp = oInitialConditions[i];
		(*outVariableNames)[i] = strdup(oTemp.first.c_str());
		(*outValues)[i] = oTemp.second;
	}

	return 0;
}

LIB_EXTERN  int LibStructural_getL0MatrixIds(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
	LibStructural_getDependentSpeciesIds(outRowLabels, outRowCount);
	LibStructural_getIndependentSpeciesIds(outColLabels, outColCount);

	return 0;
}

LIB_EXTERN  int LibStructural_getNrMatrixIds(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
	LibStructural_getIndependentSpeciesIds(outRowLabels, outRowCount);
	LibStructural_getReactionsIds(outColLabels, outColCount);

	return 0;
}

LIB_EXTERN  int LibStructural_getColumnReorderedNrMatrixIds(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
	vector<string> oRows; vector<string> oCols;
	LibStructural::getInstance()->getColumnReorderedNrMatrixIds(oRows, oCols);

	Util::CopyStringVector(oRows, *outRowLabels, *outRowCount);
	Util::CopyStringVector(oCols, *outColLabels, *outColCount);

	return 0;
}

LIB_EXTERN  int LibStructural_getColumnReorderedNrMatrix(double** *outMatrix, int* outRows, int *outCols)
{
	DoubleMatrix* oMatrix = LibStructural::getInstance()->getColumnReorderedNrMatrix();
	if (oMatrix == NULL)
		return -1;
	Util::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
	delete oMatrix;
	return 0;
}

// Returns the NIC Matrix (partition of linearly independent columns of Nr)
LIB_EXTERN  int LibStructural_getNICMatrix(double** *outMatrix, int* outRows, int *outCols)
{
	DoubleMatrix* oMatrix = LibStructural::getInstance()->getNICMatrix();
	if (oMatrix == NULL)
		return -1;
	Util::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
	delete oMatrix;
	return 0;
}

// Returns the NDC Matrix (partition of linearly dependent columns of Nr)
LIB_EXTERN  int LibStructural_getNDCMatrix(double** *outMatrix, int* outRows, int *outCols)
{
	DoubleMatrix* oMatrix = LibStructural::getInstance()->getNDCMatrix();
	if (oMatrix == NULL)
		return -1;
	Util::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
	delete oMatrix;
	return 0;
}

LIB_EXTERN  int LibStructural_getNICMatrixIds(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
	vector<string> oRows; vector<string> oCols;
	LibStructural::getInstance()->getNICMatrixIds(oRows, oCols);

	Util::CopyStringVector(oRows, *outRowLabels, *outRowCount);
	Util::CopyStringVector(oCols, *outColLabels, *outColCount);

	return 0;
}


LIB_EXTERN  int LibStructural_getNDCMatrixIds(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
	vector<string> oRows; vector<string> oCols;
	LibStructural::getInstance()->getNDCMatrixIds(oRows, oCols);

	Util::CopyStringVector(oRows, *outRowLabels, *outRowCount);
	Util::CopyStringVector(oCols, *outColLabels, *outColCount);

	return 0;
}

LIB_EXTERN  int LibStructural_getN0MatrixIds(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
	LibStructural_getDependentSpeciesIds(outRowLabels, outRowCount);
	LibStructural_getReactionsIds(outColLabels, outColCount);
	return 0;
}

LIB_EXTERN  int LibStructural_getLinkMatrixIds(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
	LibStructural_getReorderedSpeciesIds(outRowLabels, outRowCount);
	LibStructural_getIndependentSpeciesIds(outColLabels, outColCount);

	return 0;
}

LIB_EXTERN  int LibStructural_getK0MatrixIds(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
	LibStructural* instance = LibStructural::getInstance();

	vector<string> oReactionLables = instance->getReorderedReactionsIds();
	DoubleMatrix *k0 = instance->getK0Matrix();


	int nDependent = k0->numCols();
	int nIndependent = k0->numRows();

	*outRowCount = nIndependent;
	*outColCount = nDependent;

	*outRowLabels = (char**) malloc(sizeof(char*)**outRowCount); memset(*outRowLabels, 0, sizeof(char*)**outRowCount);
	*outColLabels = (char**) malloc(sizeof(char*)**outColCount); memset(*outColLabels, 0, sizeof(char*)**outColCount);



	for (int i = 0; i < nDependent; i++)
	{
		(*outColLabels)[i] = strdup(oReactionLables[nIndependent + i].c_str());
	}


	for (int i = 0; i < nIndependent; i++)
	{
		(*outRowLabels)[i] = strdup(oReactionLables[i].c_str());
	}

	return 0;
}

LIB_EXTERN  int LibStructural_getKMatrixIds(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
	LibStructural* instance = LibStructural::getInstance();

	vector<string> oReactionLables = instance->getReorderedReactionsIds();
	DoubleMatrix *k = instance->getKMatrix();


	int nDependent = k->numCols();
	int nIndependent = k->numRows() - nDependent;

	*outRowCount = k->numRows();
	*outColCount = nDependent;

	*outRowLabels = (char**) malloc(sizeof(char*)**outRowCount); memset(*outRowLabels, 0, sizeof(char*)**outRowCount);
	*outColLabels = (char**) malloc(sizeof(char*)**outColCount); memset(*outColLabels, 0, sizeof(char*)**outColCount);



	for (int i = 0; i < nDependent; i++)
	{
		(*outColLabels)[i] = strdup(oReactionLables[nIndependent + i].c_str());
		(*outRowLabels)[i] = strdup(oReactionLables[nIndependent + i].c_str());
	}


	for (int i = 0; i < nIndependent; i++)
	{
		(*outRowLabels)[i+nDependent] = strdup(oReactionLables[i].c_str());
	}

	return 0;
}

LIB_EXTERN  int LibStructural_getGammaMatrixIds(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
	
	LibStructural_getReorderedSpeciesIds(outColLabels, outColCount);
	DoubleMatrix *G = LibStructural::getInstance()->getGammaMatrix();

	*outRowCount = G->numRows();
	*outRowLabels = (char**) malloc(sizeof(char*)**outRowCount); memset(*outRowLabels, 0, sizeof(char*)**outRowCount);
	for (int i = 0; i < *outRowCount; i++)
	{
		stringstream stream; stream << i;
		(*outRowLabels)[i] = strdup(stream.str().c_str());
	}
	return 0;
}

LIB_EXTERN  int LibStructural_getStoichiometryMatrixIds(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
	LibStructural_getSpeciesIds(outRowLabels, outRowCount);
	LibStructural_getReactionsIds(outColLabels, outColCount);
	return 0;
}

LIB_EXTERN  int LibStructural_getFullyReorderedStoichiometryMatrixIds(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
	vector<string> oRows; vector<string> oCols;
	LibStructural::getInstance()->getFullyReorderedStoichiometryMatrixIds(oRows, oCols);

	Util::CopyStringVector(oRows, *outRowLabels, *outRowCount);
	Util::CopyStringVector(oCols, *outColLabels, *outColCount);

	return 0;
}
LIB_EXTERN  int LibStructural_getReorderedStoichiometryMatrixIds(char** *outRowLabels, int *outRowCount, char** *outColLabels, int *outColCount)
{
	LibStructural_getReorderedSpeciesIds(outRowLabels, outRowCount);
	LibStructural_getReactionsIds(outColLabels, outColCount);
	return 0;
}

LIB_EXTERN  int LibStructural_getFullyReorderedStoichiometryMatrix(double** *outMatrix, int* outRows, int *outCols)
{
	try
	{
	DoubleMatrix* oMatrix = LibStructural::getInstance()->getFullyReorderedStoichiometryMatrix();
	if (oMatrix == NULL)
		return -1;
	Util::CopyMatrix(*oMatrix, *outMatrix, *outRows, *outCols);
	delete oMatrix;
	}
	catch(...)
	{
		return -1;
	}
	return 0;
}


LIB_EXTERN  double LibStructural_getTolerance()
{
	return LibStructural::getInstance()->getTolerance();
}


LibStructural* LibStructural::_Instance = NULL;
