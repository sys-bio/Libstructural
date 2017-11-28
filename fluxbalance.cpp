#include "fluxbalance.h"


#include <string>
#include <string.h>	// memset & co
#include <vector>

#include <lp_lib.h>

#include "libstructural.h"
#include "matrix.h"
#include "util.h"

#include "constraint.h"
#include "objective.h"
#include "lpresult.h"

using namespace LIB_LA;
using namespace LIB_STRUCTURAL;
using namespace std;

FluxBalance::FluxBalance(void) : OutputFilename(NULL)
{

}


FluxBalance::~FluxBalance(void)
{
}

FluxBalance::FluxBalance(std::string &sbmlContent) : OutputFilename(NULL)
{
	loadSBML(sbmlContent);
}

void FluxBalance::loadSBML(std::string &sbmlContent)
{
	// load into struct analysis tool
	loadSBMLContentIntoStructAnalysis(sbmlContent);

}

void FluxBalance::loadSBMLFromFile(std::string &fileName)
{
	// load into struct analysis tool
	LibStructural::getInstance()->loadSBMLFromFile(fileName);
	LibStructural::getInstance()->analyzeWithQR();
	ReactionNames =  LibStructural::getInstance()->getReactions();
	Stoichiometry = LibStructural::getInstance()->getStoichiometryMatrix();
}


void FluxBalance::loadStoichiometry(LIB_LA::DoubleMatrix &matrix, std::vector<std::string> &fluxNames)
{
	LibStructural::getInstance()->loadStoichiometryMatrix(matrix);
	LibStructural::getInstance()->loadReactionNames(fluxNames);
	LibStructural::getInstance()->analyzeWithQR();
	ReactionNames =  LibStructural::getInstance()->getReactions();
	Stoichiometry = LibStructural::getInstance()->getStoichiometryMatrix();
}

void FluxBalance::loadSBMLContentIntoStructAnalysis(std::string &sbmlContent)
{
	LibStructural::getInstance()->loadSBMLFromString(sbmlContent);
	LibStructural::getInstance()->analyzeWithQR();
	ReactionNames =  LibStructural::getInstance()->getReactions();
	Stoichiometry = LibStructural::getInstance()->getStoichiometryMatrix();
}

LPResult *FluxBalance::solve()
{
	// pass matrices to lp solve
	int numRows = Stoichiometry->numRows();
	int numColumns = Stoichiometry->numCols();

	// get a new lp instance
	lprec* lp = make_lp(0, numColumns);

	if (OutputFilename)	set_outputfile(lp, OutputFilename);

	double *numArray = SetupLPsolve(lp, numRows, numColumns);	

	set_maxim(lp);

	::solve(lp);

	double* numArray2 = new double[get_Ncolumns(lp)];

	get_variables(lp, numArray2);
	delete_lp(lp);

	vector<double> values; 
	for (int i = 0; i < numColumns; i++)
	{
		values.push_back(numArray2[i]);
	}

	free(numArray); free(numArray2);

	return new LPResult(ReactionNames, values);
}

LPResult *FluxBalance::minimize()
{
	// pass matrices to lp solve
	int numRows = Stoichiometry->numRows();
	int numColumns = Stoichiometry->numCols();

	// get a new lp instance
	lprec* lp = make_lp(0, numColumns);

	double *numArray = SetupLPsolve(lp, numRows, numColumns);	

	set_minim(lp);

	::solve(lp);

	double* numArray2 = new double[get_Ncolumns(lp)];

	get_variables(lp, numArray2);
	delete_lp(lp);

	vector<double> values; 
	for (int i = 0; i < numColumns; i++)
	{
		values.push_back(numArray2[i]);
	}

	free(numArray); free(numArray2);

	return new LPResult(ReactionNames, values);
}

void FluxBalance::writeToFile(std::string &fileName, ExportFormats format)
{
	// pass matrices to lp solve
	int numRows = Stoichiometry->numRows();
	int numColumns = Stoichiometry->numCols();

	// get a new lp instance
	lprec* lp = make_lp(0, numColumns);

	double *numArray = SetupLPsolve(lp, numRows, numColumns);	
	set_maxim(lp);

	switch(format)
	{
	case LP:
		write_lp(lp, strdup(fileName.c_str()));
		break;
	case MPS:
		write_mps(lp, strdup(fileName.c_str()));
		break;
	case FREE_MPS:
		write_freemps(lp, strdup(fileName.c_str()));
		break;
	}

	delete_lp(lp);
	free(numArray);

}

LPResult *FluxBalance::maximize()
{
	return solve();
}


int FluxBalance::getIndex(std::string &id)
{
	for (unsigned int i = 0; i < ReactionNames.size(); i++)
	{
		if (ReactionNames[i]==id) return i;
	}
	return -1;
}


void FluxBalance::clearConstraints() 
{
	Constraints.clear();
}
void FluxBalance::addConstraint(std::string &id, int operation, double value)
{
	Constraints.push_back(Constraint(id, operation, value));
}
void FluxBalance::setConstraints(std::vector< Constraint > constraint)
{
	Constraints.assign(constraint.begin(), constraint.end());
}

void FluxBalance::clearObjectives() 
{
	Objectives.clear();
}
void FluxBalance::addObjective(std::string &id, double value)
{
	Objectives.push_back(Objective(id, value));
}
void FluxBalance::setObjectives(std::vector< Objective > objectives)
{
	Objectives.assign(objectives.begin(), objectives.end());
}

FluxBalance* FluxBalance::getInstance()
{
	if (_Instance == NULL)
		_Instance = new FluxBalance();
	return _Instance;
}

int FluxBalance_loadSBML(const char* sbmlContent)
{
	string sbml(sbmlContent);
	FluxBalance::getInstance()->loadSBML(sbml);
	return 0;
}


int FluxBalance_loadStoichiometry(const double** matrix, const int numRows, const int numCols, const char** fluxNames)
{
	DoubleMatrix stoichiometry (matrix, numRows, numCols );
	vector< string > oNames;
	for (int i = 0; i < numCols; i++)
	{
		string sId(fluxNames[i]);
		oNames.push_back(sId);
	}
	FluxBalance::getInstance()->loadStoichiometry(stoichiometry, oNames);
	return 0;
}


int FluxBalance_clearConstraints()
{
	FluxBalance::getInstance()->clearConstraints();
	return 0;
}

int FluxBalance_addConstraint(const char* id, const int operation, const double value)
{
	string sId(id);
	FluxBalance::getInstance()->addConstraint(sId, operation, value);
	return 0;
}

int FluxBalance_setConstraints(const char** ids, const int* operations, const double* values, const int length)
{
	vector<Constraint> constraints; 
	for (int i = 0; i < length; i++)
	{
		string sId(ids[i]);
		constraints.push_back(Constraint(sId, operations[i],values[i]));
	}
	FluxBalance::getInstance()->setConstraints(constraints);

	return 0;
}

int FluxBalance_clearObjectives()
{
	FluxBalance::getInstance()->clearObjectives();
	return 0;
}

int FluxBalance_addObjective(const char* id, const double value)
{
	string sId(id);
	FluxBalance::getInstance()->addObjective(sId, value);
	return 0;
}

int FluxBalance_setObjectives(const char** ids, const double* values, const int length)
{
	vector<Objective> objectives; 
	for (int i = 0; i < length; i++)
	{
		string sId(ids[i]);
		objectives.push_back(Objective(sId, values[i]));
	}
	FluxBalance::getInstance()->setObjectives(objectives);
	return 0;
}

int FluxBalance_solve(char* **outNames, double* *outValues, int *outLength)
{
	LPResult *result = FluxBalance::getInstance()->solve();

	Util::CopyStringVector(result->Ids, *outNames, *outLength);
	Util::CopyDoubleVector(result->Values, *outValues, *outLength);

	return 0;
}

void FluxBalance_writeToLPFile(const char* fileName)
{
	string sFilename(fileName);
	FluxBalance::getInstance()->writeToFile(sFilename, LIB_STRUCTURAL::LP);
}

void FluxBalance_writeToMPSFile(const char* fileName)
{
	string sFilename(fileName);
	FluxBalance::getInstance()->writeToFile(sFilename, LIB_STRUCTURAL::MPS);
}

void FluxBalance_writeToFreeMPSFile(const char* fileName)
{
	string sFilename(fileName);
	FluxBalance::getInstance()->writeToFile(sFilename, LIB_STRUCTURAL::FREE_MPS);
}

int FluxBalance_maximize(char* **outNames, double* *outValues, int *outLength)
{
	LPResult *result = FluxBalance::getInstance()->maximize();

	Util::CopyStringVector(result->Ids, *outNames, *outLength);
	Util::CopyDoubleVector(result->Values, *outValues, *outLength);

	return 0;
}

int FluxBalance_minimize(char* **outNames, double* *outValues, int *outLength)
{
	LPResult *result = FluxBalance::getInstance()->minimize();

	Util::CopyStringVector(result->Ids, *outNames, *outLength);
	Util::CopyDoubleVector(result->Values, *outValues, *outLength);

	return 0;
}


FluxBalance* FluxBalance::_Instance = NULL;

double* FluxBalance::SetupLPsolve(void *lpBoxed, int numRows, int numColumns)
{
	lprec *lp = (lprec*) lpBoxed;
	// set reaction names
	for (int i = 0; i < numColumns; i++)
		set_col_name(lp, i + 1, strdup(ReactionNames[i].c_str()));
	// set up constraints from stoichiometry
	double *numArray = (double*)malloc(sizeof(double) * (numColumns + 1));
	memset(numArray, 0, sizeof(double) * (numColumns + 1));
	for (int i = 0; i < numRows; i++)
	{
		for (int j = 0; j < numColumns; j++)
			numArray[j + 1] = (*Stoichiometry)(i, j);
		add_constraint(lp, numArray, EQ, 0.0);
	}
	// set up custom constraints
	vector<Constraint>::iterator iter;
	for (iter = Constraints.begin(); iter != Constraints.end(); iter++)
	{
		Constraint currentConstraint = *iter;
		int index = getIndex(currentConstraint.Id);
		if (index == -1)
			continue;
		// oups ... probably should fail reaction not found
		memset(numArray, 0, sizeof(double) * (numColumns + 1));
		numArray[index + 1] = 1.0;
		add_constraint(lp, numArray, currentConstraint.Operator, currentConstraint.Value);
	}
	memset(numArray, 0, sizeof(double) * (numColumns + 1));
	// set up objective function
	vector<Objective>::iterator objectiveIter;
	for (objectiveIter = Objectives.begin(); objectiveIter != Objectives.end(); objectiveIter++)
	{
		Objective currentObjective = *objectiveIter;
		int index = getIndex(currentObjective.Id);
		if (index == -1)
			continue;
		// oups ... probably should fail reaction not found
		numArray[index + 1] = currentObjective.Value;
	}

	set_obj_fn(lp, numArray);
	return numArray;
}
