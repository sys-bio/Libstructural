// This example shows how to load and solve a stoichiometry matrix into the 
// flux balance analysis class. 
#include <iostream>
#include <libstructural.h>
#include <fluxbalance.h>
#include <matrix.h>

#include <vector>
#include <string>

using namespace std;
using namespace LIB_STRUCTURAL;
using namespace LIB_LA;



// Generated for file: Jana_WolfGlycolysis.xml

static double STOICHIOMETRY[11][11] = { { 0.0, -2.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0, -1.0, 0.0}, // ATP
{ 0.0, 0.0, 2.0, -1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, // glyceraldehyde_3_phosphate
{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, -1.0, -1.0, 0.0, 0.0}, // Acetyladehyde
{ 0.0, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0}, // NAD
{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0}, // External_acetaldehyde
{ 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0}, // pyruvate
{ 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, // Glucose
{ 0.0, 0.0, 0.0, 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0}, // glycerate_3_phosphate
{ 0.0, 1.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, // fructose_1_6_bisphosphate
{ 0.0, 0.0, 0.0, -1.0, 1.0, 0.0, 0.0, -1.0, 0.0, 0.0, 0.0}, // NADH
{ 0.0, 2.0, 0.0, 0.0, -1.0, -1.0, 0.0, 0.0, 0.0, 1.0, 0.0}}; // ADP

void InitializeValues(DoubleMatrix* &oMatrix, 
					  vector<string> &speciesNames, 
					  vector<double> &initialConcentrations, 
					  vector<string> &reactionNames);



int main(int argc, char*argv[])
{

	// get the instance of the flux balance library	
	FluxBalance* instance = FluxBalance::getInstance();
	
	vector<string> speciesNames; vector<string> reactionNames;      
	vector<double> initialConentrations; DoubleMatrix* oMatrix;
	
	// some function that produces the stoichiometry matrix
	// and labels
	InitializeValues(oMatrix, speciesNames, initialConentrations, reactionNames);
	
	// load an SBML model
	instance->loadStoichiometry(*oMatrix, reactionNames);
	
	// set up some constraints
	// for this example we want J0 = 10, and J7 = 4;
	string sVar1("J0"); instance->addConstraint(sVar1, FB_OPERATION_EQ, 10 );
	string sVar2("J7"); instance->addConstraint(sVar2, FB_OPERATION_EQ, 4 );	
	
	// set up objective function
	// for this example we want J8 = 5;
	string obj("J8"); instance->addObjective(obj, 5);
	
	// minimize it ... 
	LPResult *result = instance->minimize();
	
	// print the result
	
	for (int i = 0	; i < result->getNumVariables(); i++) {
		cout << result->getNthId(i) << " = " << result->getNthValue(i) << endl;
	}
	return 0;
}

void InitializeValues(DoubleMatrix* &oMatrix, 
					  vector<string> &speciesNames, 
					  vector<double> &initialConcentrations, 
					  vector<string> &reactionNames)
{
	
	// build stoichiometry matrix (this should be easier!!!)
	double *row0 = STOICHIOMETRY[0],
	*row1 = STOICHIOMETRY[1],
	*row2 = STOICHIOMETRY[2],
	*row3 = STOICHIOMETRY[3],
	*row4 = STOICHIOMETRY[4],
	*row5 = STOICHIOMETRY[5],
	*row6 = STOICHIOMETRY[6],
	*row7 = STOICHIOMETRY[7],
	*row8 = STOICHIOMETRY[8],
	*row9 = STOICHIOMETRY[9],
	*row10 = STOICHIOMETRY[10];
	
	const double* oMatrixRAW[] = {row0, row1, row2, row3, row4, row5, row6, row7, row8, row9, row10};
	
	oMatrix = new DoubleMatrix(oMatrixRAW, 11, 11);
	
	speciesNames.push_back("ATP");
	speciesNames.push_back("glyceraldehyde_3_phosphate");
	speciesNames.push_back("Acetyladehyde");
	speciesNames.push_back("NAD");
	speciesNames.push_back("External_acetaldehyde");
	speciesNames.push_back("pyruvate");
	speciesNames.push_back("Glucose");
	speciesNames.push_back("glycerate_3_phosphate");
	speciesNames.push_back("fructose_1_6_bisphosphate");
	speciesNames.push_back("NADH");
	speciesNames.push_back("ADP");
	
	initialConcentrations.push_back(3.0);
	initialConcentrations.push_back(0.0);
	initialConcentrations.push_back(0.0);
	initialConcentrations.push_back(0.5);
	initialConcentrations.push_back(0.0);
	initialConcentrations.push_back(0.0);
	initialConcentrations.push_back(0.0);
	initialConcentrations.push_back(0.0);
	initialConcentrations.push_back(0.0);
	initialConcentrations.push_back(0.5);
	initialConcentrations.push_back(1.0);
	
	reactionNames.push_back("J0");
	reactionNames.push_back("J1");
	reactionNames.push_back("J2");
	reactionNames.push_back("J3");
	reactionNames.push_back("J4");
	reactionNames.push_back("J5");
	reactionNames.push_back("J6");
	reactionNames.push_back("J7");
	reactionNames.push_back("J8");
	reactionNames.push_back("J9");
	reactionNames.push_back("J10");
}


// Running the program yields:
//
//
//Model name:  '' - run #1    
//Objective:   Minimize(R0)
//
//SUBMITTED
//Model size:       13 constraints,      11 variables,           32 non-zeros.
//Sets:                                   0 GUB,                  0 SOS.
//
//Using DUAL simplex for phase 1 and PRIMAL simplex for phase 2.
//The primal and dual simplex pricing strategy set to 'Devex'.
//
//
//Optimal solution                  40 after         11 iter.
//
//Excellent numeric accuracy ||*|| = 0
//
//MEMO: lp_solve version 5.5.0.14 for 32 bit OS, with 64 bit REAL variables.
//In the total iteration count 11, 0 (0.0%) were bound flips.
//There were 0 refactorizations, 0 triggered by time and 0 by density.
//... on average 11.0 major pivots per refactorization.
//The largest [LUSOL v2.2.1.0] fact(B) had 14 NZ entries, 1.0x largest basis.
//The constraint matrix inf-norm is 2, with a dynamic range of 2.
//Time to load data was 0.000 seconds, presolve used 0.001 seconds,
//... 0.001 seconds in simplex solver, in total 0.002 seconds.
//J0 = 10
//J1 = 10
//J2 = 10
//J3 = 8
//J4 = 12
//J5 = 12
//J6 = 12
//J7 = 4
//J8 = 8
//J9 = 4
//J10 = 8
