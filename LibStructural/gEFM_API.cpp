
#include <stdio.h>
#include <stdlib.h>

#include "gEFMUtils.h"
#include "Network.h"
#include "computeEFM.h"

// This is the C compatible API



Network *gefm_createNetwork () {
	try {
		return new Network ();
	}
	catch (...) {
		return NULL;
	}
}


	int gefm_deleteNetwork (Network *network) {
		try {
			delete network;
			return 0;
		}
		catch (...) {
			return -1;
		}
	}


	int gefm_addSpecies (Network *network, const char * name, bool isBoundarySpecies) {
		try {
			network->metabolites.push_back (name);
			network->external.push_back (isBoundarySpecies);
			return 0;
		}
		catch (...) {
			return -1;
		}
	}


	int gefm_addReactionName (Network *network, const char *name, bool isReversible) {
		try {
			network->reactions.push_back (string (name));
			network->reversible.push_back (isReversible);
			return 0;
		}
		catch (...) {
			return -1;
		}
	}


	// Create space for the stoichiometrry matrix (includes external/boundary) species
	int gefm_createStoichiometryMatrix (Network *network) {

		try {
			int numSpecies = network->metabolites.size ();
			int numReactions = network->reactions.size ();

			network->s.reserve (numSpecies);
			for (int i = 0; i < numSpecies; i++) {
				network->s.push_back (vector<double> ());
				network->s[i].reserve (numReactions);
				for (int j = 0; j < numReactions; j++)
					network->s[i].push_back (0);
			}
			return 0;
		}
		catch (...) {
			return -1;
		}
	}



int gefm_setStoichiometry (Network *network, int iSpecies, int iReaction, double stoich) {

	try {
		if (iSpecies < network->metabolites.size () && iReaction < network->reactions.size ()) {
			network->s[iSpecies][iReaction] = stoich;
			return 0;
		}
		return -1;
	}
	catch (...) {
		return -1;
	}
}


int gefm_splitReversibleReactions (Network *network) {
	
	try {
		network->splitReversibleReactions ();
		return 0;
	}
	catch (...) {
		return -1;
	}
}


double **gefm_getElementaryModes (Network *network, int *numberOfModes, int *numberOfReactions, bool noRevDup) {

	*numberOfModes = 0;
	*numberOfReactions = 0;

	try {
		vector<vector<double> > efm = computeElementaryModes (network, noRevDup);
		double ** efmArray = convertEFMVectorsToArray (efm);
		if (efmArray != NULL) {
			*numberOfModes = efm.size ();
			if (numberOfModes > 0)
				*numberOfReactions = efm[0].size ();
			else
				*numberOfReactions = 0;
		}
		return efmArray;
	}
	catch (...) {
		*numberOfModes = 0;
		*numberOfReactions = 0;
		return NULL;
	}
}


