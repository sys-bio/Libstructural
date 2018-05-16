
#include <stdlib.h>

#include "gEFMUtils.h"
#include "Network.h"

// This is the C compatible API

// Create a network object
Network *gefm_createNetwork ();

// Free a network object
int gefm_deleteNetwork (Network *network);

// Add a species to the model with given name
// Is the species is a boundary species, set isBoundary to true
int gefm_addSpecies (Network *network, const char * name, bool isBoundarySpecies);

// Add a reaction to the model with given name
// Is the reaction is reversible, set isReversible to true
int gefm_addReactionName (Network *network, const char *name, bool isReversible);

// Set a stoichiometry coefficient in the stoichiometry matrix
int gefm_setStoichiometry (Network *network, int iSpecies, int iReaction, double stoich);

// Create the space to store the stochiometry matrix
// This must be called before adding reactions
int gefm_createStoichiometryMatrix (Network *network);

// Once everything is set up, call this function to split the reversible reactions
int gefm_splitReversibleReactions (Network *network);

// Get the elementary modes. numberOfModes (rows) and numberOfReactions (columns) are the dimensions 
// of the array. You must use numberOfReactions as the number of columns
double **gefm_getElementaryModes (Network *network, int *numberOfModes, int *numberOfReactions, bool noRevDup);
