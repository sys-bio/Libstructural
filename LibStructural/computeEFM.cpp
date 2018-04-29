#include <stdio.h>
#include <stdlib.h>

#include "EFMGenerator.h"
#include "BitVector.h"
#include "Rank.h"
#include "gEFMUtils.h"

/**
* Computes elementary flux modes for the loaded network. The function is
* templated for BitVector used to store reactions.
*/
template<class BitVector>

vector<vector<double> > execute (Network *network) {

	vector<vector<double> > efmErr;

	gefm_reversibleTreeIndices = NULL;

	gefm_reversibleTreeIndices = (int*)malloc (MAX_PATHWAY_POOL_SIZE * sizeof (int));
	if (gefm_reversibleTreeIndices == NULL) {
		globalErrorCode = -2;
		throw errorCodeStrings[-2];

	}

	Pathway<BitVector>* pathways = (Pathway<BitVector>*) malloc (gefm_numReactions * sizeof (Pathway<BitVector>));
	for (int r = 0; r < gefm_numReactions; r++) {
		gefm_reactions.push_back (network->reactions[r]);
		pathways[r] = Pathway<BitVector> (r);
		gefm_reversible.push_back (network->reversible[r]);
		for (int m = 0; m < gefm_numMetabolites; m++) {
			pathways[r].setMetaboliteCoefficient (m, network->s[m][r]);
		}
	}

	int n = sizeof(BitVector);
	gefm_reversiblePairs = malloc (gefm_reversiblePairCount * n);
	BitVector* revPairs = (BitVector*)gefm_reversiblePairs;
	for (unsigned int r = 0; r < network->reversiblePairs.size (); r++) {
		revPairs[r] = BitVector ();
		revPairs[r].setBit (network->reversiblePairs[r], true);
		revPairs[r].setBit (network->reversiblePairs[r] + 1, true);
	}

	EFMGenerator<BitVector> efmgenerator (pathways);
	efmgenerator.genenrateEFMs ();
	vector<vector<double> > efm = efmgenerator.printEFMs (network);
	free (pathways);
	return efm;
}

/**
* Frees allocated memory resources.
*/
void freeResources () {
	if (gefm_reversibleTreeIndices != NULL) {
		free (gefm_reversibleTreeIndices);
	}
	if (gefm_reversiblePairs != NULL) {
		free (gefm_reversiblePairs);
	}
}

/**
* Computes maximum cardinality for the rank test.
*/
void computeMaxCardinality (Network *network) {
	double* matrix = (double*)malloc (sizeof (double) * gefm_numMetabolites * gefm_numReactions);
	int rows = 0;
	for (int m = 0, i = 0; m < gefm_numMetabolites; m++) {
		if (!network->external[m]) {
			rows++;
			for (int r = 0; r < gefm_numReactions; r++, i++) {
				matrix[i] = network->s[m][r];
			}
		}
	}
	gefm_maxCardinality = computeRank (matrix, gefm_numReactions, rows, NULL, NULL) + 1;
	free (matrix);
}


/**
* Computes elementary flux modes for the given network.
* @param file Network file.
*/
vector<vector<double> > execute (const char* file, Network *network) {
	vector<vector<double> > efmErr;

	if (!network->readNetworkFile (file)) {
		globalErrorCode = -3;
		throw errorCodeStrings[-3];
	}
	gefm_numReactions = network->reactions.size ();
	gefm_numMetabolitesRemaining = gefm_numMetabolites = network->metabolites.size ();
	for (int m = 0; m < gefm_numMetabolites; m++) {
		gefm_metabolites.push_back (network->metabolites[m]);
		gefm_externalMetabolites.push_back (network->external[m]);
	}
	gefm_reversiblePairCount = network->reversiblePairs.size ();
	computeMaxCardinality (network);
	if (gefm_numReactions <= 32) {
		return execute<BitVector32> (network);
	}
	else if (gefm_numReactions <= 64) {
		return execute<BitVector64> (network);
	}
	else if (gefm_numReactions <= 96) {
		return execute<BitVector96> (network);
	}
	else if (gefm_numReactions <= 128) {
		return execute<BitVector128> (network);
	}
	else if (gefm_numReactions <= 160) {
		return execute<BitVector160> (network);
	}
	else if (gefm_numReactions <= 192) {
		return execute<BitVector192> (network);
	}
	else if (gefm_numReactions <= 448) {
		return execute<BitVector448> (network);
	}
	else {
			globalErrorCode = -4;
			throw errorCodeStrings[-4];
	}
}

vector<vector<double> > computeElementaryModes (Network *network) {
	vector<vector<double> > efmErr;

	gefm_metabolites.clear();
	gefm_externalMetabolites.clear();
	gefm_reactions.clear();
	gefm_reversible.clear();

	gefm_numReactions = network->reactions.size ();
	gefm_numMetabolitesRemaining = gefm_numMetabolites = network->metabolites.size ();
	for (int m = 0; m < gefm_numMetabolites; m++) {
		gefm_metabolites.push_back (network->metabolites[m]);
		gefm_externalMetabolites.push_back (network->external[m]);
	}
	gefm_reversiblePairCount = network->reversiblePairs.size ();
	computeMaxCardinality (network);
	if (gefm_numReactions <= 32) {
		return execute<BitVector32> (network);
	}
	else if (gefm_numReactions <= 64) {
		return execute<BitVector64> (network);
	}
	else if (gefm_numReactions <= 96) {
		return execute<BitVector96> (network);
	}
	else if (gefm_numReactions <= 128) {
		return execute<BitVector128> (network);
	}
	else if (gefm_numReactions <= 160) {
		return execute<BitVector160> (network);
	}
	else if (gefm_numReactions <= 192) {
		return execute<BitVector192> (network);
	}
	else if (gefm_numReactions <= 448) {
		return execute<BitVector448> (network);
	}
	else {
		globalErrorCode = -4;
		throw errorCodeStrings[-4];
	}
}

double **convertEFMVectorsToArray (vector<vector<double> > efm) {

	try {

		if (efm.size () == 0)
			return NULL;

		int nRows = efm.size ();
		int nColumns = efm[0].size ();

	    double **efmArray = (double **) gEFMsafeCalloc("", nRows, sizeof (double *));
		for (int i = 0; i < nRows; i++)
			efmArray[i] = (double *) gEFMsafeCalloc("", nColumns, sizeof (double));

		for (int i = 0; i < nRows; i++) {
			for (int j = 0; j < nColumns; j++) {
				efmArray[i][j] = efm[i][j];
			}
		}
		return efmArray;
	}
	catch (...) {
		return NULL;
	}
}
