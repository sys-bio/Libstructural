#ifndef GLOBALDATA_H
#define	GLOBALDATA_H

#include "Network.h"
#include "BitVector.h"

#define ZERO 1e-5
#define NEG_ZERO -ZERO

#define mCoeff(pathway, metabolite) (metaboliteCoefficients[(pathway * numMetabolites) + metabolite])
#define isPathwayInput(coeff) (coeff < NEG_ZERO)
#define isPathwayOutput(coeff) (coeff > ZERO)


// Network
// extern Network network;
// Total number of reactions in the network
extern int gefm_numReactions;
// Total number of metabolites in the network
extern int gefm_numMetabolites;
// Boolean flag for external metabolites (true if a metabolite is external)
extern vector<bool> gefm_externalMetabolites;
// Boolean flag for reversible reactions (true if a reaction is reversible)
extern vector<bool> gefm_reversible;
// Number of metabolites to be processed 
extern int gefm_numMetabolitesRemaining;
// Reaction names
extern vector<string> gefm_reactions;
// Metabolites names
extern vector<string> gefm_metabolites;
// Maximum cardinality for EFM computed from rank condition
extern int gefm_maxCardinality;


// Array for reversible tree indices
extern int* gefm_reversibleTreeIndices;
// Reversible reaction pairs
extern void* gefm_reversiblePairs;
// Number of reversible reaction pairs
extern int gefm_reversiblePairCount;

//extern int gefm_Max_Metabolites;


/**
 * Checks if the given pathway is valid and does not contain 2-futile cycles.
 * @param pahtway Pathway to be tested.
 * @return true if pathway is valid, false otherwise.
 */
template <class BitVector>
bool isValidPathway(BitVector& pathway) {
   BitVector* revPairs = (BitVector*)gefm_reversiblePairs;
   for (int i = 0; i < gefm_reversiblePairCount; i++) {
      if (pathway.isSuperSetOf(revPairs[i])) {
         return false;
      }
   }
   return true;
}

#endif	/* GLOBALDATA_H */
