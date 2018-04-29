#include <cassert>

#include "BitPatternTreeNode.h"
#include "ReversibleTreeNode.h"

int globalErrorCode;
char *errorCodeStrings[] = {
	"",
	"",
	"Error allocating memory for reversible tree indices",
	"Error loading network file",
	"Maximum number of reactions supported is 448",
	"Null pointer from malloc, increase MAX_PATHWAY_POOL_SIZE",
	"Maximum number of metabolites has been exceded"
};


// Total number of reactions in the network
int gefm_numReactions;
//Total number of metabolites in the network
int gefm_numMetabolites;
// Boolean flag for external metabolites (true if a metabolite is external)
vector<bool> gefm_externalMetabolites;
// Boolean flag for reversible reactions (true if a reaction is reversible)
vector<bool> gefm_reversible;
// Number of metabolites to be processed 
int gefm_numMetabolitesRemaining;
//Reaction names
vector<string> gefm_reactions;
// Metabolites names
vector<string> gefm_metabolites;
// Maximum cardinality for EFM computed from rank condition
int gefm_maxCardinality;

// Array for reversible tree indices
int* gefm_reversibleTreeIndices;

// Reversible reaction pairs
void* gefm_reversiblePairs;
// Number of reversible reaction pairs
int gefm_reversiblePairCount;

// Maximum number of Bitpattern Tree Nodes
int gefm_maxPoolSizeBPT;
// Pointer array of Bitpattern Tree Nodes
void* gefm_poolBPT;
// Number of Bitpattern Tree Nodes
int gefm_poolSizeBPT;

// Maximum number of Reversible Tree Nodes
int gefm_maxPoolSizeRev;
// Pointer array of Reversible Tree Nodes
void* gefm_poolRev;
// Number of Reversible Tree Nodes
int gefm_poolSizeRev;

// Lookup table to compute cardinality of bit vectors
int gefm_BIT_COUNT_LOOKUP[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};

/**
 * Initializes Bitpattern Tree Node pool
 * @param size Size of Bitpattern Tree Node pool
 */
void initBPTNodePool(int size) {
	gefm_maxPoolSizeBPT = MAX_BPT_TREE_NODES;
	gefm_poolBPT = malloc(gefm_maxPoolSizeBPT * size);
    assert(gefm_poolBPT != NULL);
    gefm_poolSizeBPT = 0;
}

/**
 * Free memory resources allocated for Bitpattern Tree Node pool
 */
void freeBPTNodePool() {
   free(gefm_poolBPT);
   gefm_poolBPT = NULL;
}

/**
 * Clears the Bitpattern Tree Node pool
 */
void clearBPTNodePool() {
	gefm_poolSizeBPT = 0;
}

/**
 * Returns pointer to next free Bitpattern Tree Node in the pool
 * @param size Node size in bytes
 * @return Pointer to the node
 */
void* nextBPTNode(int size) {
	gefm_poolSizeBPT++;
    assert(gefm_poolSizeBPT < gefm_maxPoolSizeBPT);
    return ((char*)gefm_poolBPT) + ((gefm_poolSizeBPT - 1) * size);
}

/**
 * Initializes Reversible Tree Node pool
 * @param size Size of Reversible Tree Node pool
 */
void initRevNodePool(int size) {
	gefm_maxPoolSizeRev = MAX_REVERSIBLE_TREE_NODES;
    gefm_poolRev = malloc(gefm_maxPoolSizeRev * size);
    assert(gefm_poolRev != NULL);
    gefm_poolSizeRev = 0;
}

/**
 * Free memory resources allocated for Reversible Tree Node pool
 */
void freeRevNodePool() {
   free(gefm_poolRev);
   gefm_poolRev = NULL;
}

/**
 * Clears the Reversible Tree Node pool
 */
void clearRevNodePool() {
	gefm_poolSizeRev = 0;
}

/**
 * Returns pointer to next free Reversible Tree Node in the pool
 * @param size Node size in bytes
 * @return Pointer to the node
 */
void* nextRevNode(int size) {
	gefm_poolSizeRev++;
   assert(gefm_poolSizeRev < gefm_maxPoolSizeRev);
   return ((char*)gefm_poolRev) + ((gefm_poolSizeRev - 1) * size);
}
