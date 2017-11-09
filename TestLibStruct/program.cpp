#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <libstructural.h>
#include <matrix.h>

using namespace std;
using namespace LIB_STRUCTURAL;
using namespace LIB_LA;

void PrintHeader(string header)
{
	cout << endl;
	cout << "-----------------------------------------------------------------------------" << endl;
	cout << "--- " << header << endl;
	cout << "-----------------------------------------------------------------------------" << endl;
}

void PrintVector(vector<string> vec) 
{
	vector<string>::iterator it;
	for (it = vec.begin(); it != vec.end(); it++)
		cout << *it << "\t";
	cout << endl;

}

void PrintLabeledMatrix (DoubleMatrix &oMatrix, 
						 vector<string> &rowLabels, 
						 vector<string> &colLabels)
{
	try
	{

		if (oMatrix.numCols() == 0 || oMatrix.numRows() == 0)
		{
			cout << "(empty)" << endl;
			return;
		}

		// return if we don't have the right dimensions
		if (oMatrix.numCols() != colLabels.size() 
			|| oMatrix.numRows() != rowLabels.size())
		{
			cout << "(dimensions of matrices and labels don't match)" << endl;
			return;
		}

		// otherwise print 
		vector<string>::iterator it;
		cout << "\t";
		// print column labels
		for (it = colLabels.begin(); it != colLabels.end(); it++)
			cout << *it << "\t";
		cout << endl;

		for (unsigned int i = 0; i < oMatrix.numRows(); i++)
		{
			cout << rowLabels[i] << "\t";
			for (unsigned int j = 0; j < oMatrix.numCols(); j++)
				cout << oMatrix(i,j) << "\t";
			cout << endl;
		}
	}
	catch(...)
	{
		cout << "error while printing ." << endl;
	}

}

void PrintFullyReorderedStoichiometry(LibStructural &instance)
{
	PrintHeader("Fully Reordered Stoichiometry Matrix");
	DoubleMatrix *oMatrix = instance.getFullyReorderedStoichiometryMatrix();
	vector<string> sRows; vector<string> sCols;
	instance.getFullyReorderedStoichiometryMatrixLabels(sRows, sCols);
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}

void PrintReorderedStoichiometry(LibStructural &instance)
{
	PrintHeader("Reordered Stoichiometry Matrix");
	DoubleMatrix *oMatrix = instance.getReorderedStoichiometryMatrix();
	vector<string> sRows; vector<string> sCols;
	instance.getReorderedStoichiometryMatrixLabels(sRows, sCols);
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}

void PrintStoichiometry(LibStructural &instance)
{
	PrintHeader("Stoichiometry Matrix");
	DoubleMatrix *oMatrix = instance.getStoichiometryMatrix();
	vector<string> sRows; vector<string> sCols;
	instance.getStoichiometryMatrixLabels(sRows, sCols);
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}

void PrintKMatrix(LibStructural &instance)
{
	PrintHeader("K Matrix");
	DoubleMatrix *oMatrix = instance.getKMatrix();
	vector<string> sRows; vector<string> sCols;
	instance.getKMatrixLabels(sRows, sCols);
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}
void PrintK0Matrix(LibStructural &instance)
{
	PrintHeader("K0 Matrix");
	DoubleMatrix *oMatrix = instance.getK0Matrix();
	vector<string> sRows; vector<string> sCols;
	instance.getK0MatrixLabels(sRows, sCols);
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}
void PrintLinkMatrix(LibStructural &instance)
{
	PrintHeader("L Matrix");
	DoubleMatrix *oMatrix = instance.getLinkMatrix();
	vector<string> sRows; vector<string> sCols;
	instance.getLinkMatrixLabels(sRows, sCols);
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}
void PrintL0Matrix(LibStructural &instance)
{
	PrintHeader("L0 Matrix");
	DoubleMatrix *oMatrix = instance.getL0Matrix();
	vector<string> sRows; vector<string> sCols;
	instance.getL0MatrixLabels(sRows, sCols);
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}

void PrintColumnReorderedNrMatrix(LibStructural &instance)
{
	PrintHeader("Column Reordered Nr Matrix");
	vector<string> sRows; vector<string> sCols;
	DoubleMatrix *oMatrix = instance.getColumnReorderedNrMatrix();
	instance.getColumnReorderedNrMatrixLabels(sRows, sCols);
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}

void PrintNrMatrix(LibStructural &instance)
{
	PrintHeader("Nr Matrix");
	DoubleMatrix *oMatrix = instance.getNrMatrix();
	vector<string> sRows; vector<string> sCols;
	instance.getNrMatrixLabels(sRows, sCols);
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}

void PrintNICMatrix(LibStructural &instance)
{
	PrintHeader("NIC Matrix");
	DoubleMatrix *oMatrix = instance.getNICMatrix();
	vector<string> sRows; vector<string> sCols;
	instance.getNICMatrixLabels(sRows, sCols);
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}

void PrintNDCMatrix(LibStructural &instance)
{
	PrintHeader("NDC Matrix");
	DoubleMatrix *oMatrix = instance.getNDCMatrix();
	vector<string> sRows; vector<string> sCols;
	instance.getNDCMatrixLabels(sRows, sCols);
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}

void PrintN0Matrix(LibStructural &instance)
{
	PrintHeader("N0 Matrix");
	DoubleMatrix *oMatrix = instance.getN0Matrix();
	vector<string> sRows; vector<string> sCols;
	instance.getN0MatrixLabels(sRows, sCols);
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}

void PrintGammaMatrix(LibStructural &instance)
{
	PrintHeader("Gamma Matrix");
	DoubleMatrix *oMatrix = instance.getGammaMatrix();
	vector<string> sRows; vector<string> sCols;
	instance.getGammaMatrixLabels(sRows, sCols);
	PrintLabeledMatrix(*oMatrix, sRows, sCols); cout << endl << endl;
}

void PrintGammaMatrixGJ(LibStructural &instance)
{
	PrintHeader("Gamma Matrix (GJ)");
	DoubleMatrix *stoichiometry = instance.getReorderedStoichiometryMatrix();	
	vector<string> sRows; vector<string> dummy;
	instance.getReorderedStoichiometryMatrixLabels(sRows, dummy);
    
	DoubleMatrix *oMatrix = instance.findPositiveGammaMatrix(*stoichiometry, sRows);
	vector<string> sCols; 
	for (int i = 0; i < (int)oMatrix ->numRows(); i++)
	{
		stringstream stemp; stemp << i;
		sCols.push_back( stemp.str() );
	}
	PrintLabeledMatrix(*oMatrix, sCols, sRows); cout << endl << endl;
}


// print a couple of matrices
void PrintMatrices(LibStructural &instance)
{
	PrintStoichiometry(instance);		
	PrintReorderedStoichiometry(instance);		
	PrintColumnReorderedNrMatrix(instance);
	PrintN0Matrix(instance);
	PrintFullyReorderedStoichiometry(instance);		
	PrintKMatrix(instance);		
	PrintK0Matrix(instance);		
	PrintLinkMatrix(instance);		
	PrintL0Matrix(instance);
	PrintNrMatrix(instance);
	PrintNICMatrix(instance);
	PrintNDCMatrix(instance);
	PrintGammaMatrix(instance);
	PrintGammaMatrixGJ(instance);
}

void PrintVectors(LibStructural &instance)
{
	PrintHeader("All Species");
	PrintVector(instance.getSpecies());

	PrintHeader("Independent Species");
	PrintVector(instance.getIndependentSpecies());

	PrintHeader("Independent Species Names");
	PrintVector(instance.getIndependentSpeciesNamesList());

	PrintHeader("Dependent Species");
	PrintVector(instance.getDependentSpecies());

	PrintHeader("Dependent Species Names");
	PrintVector(instance.getDependentSpeciesNamesList());

	PrintHeader("All Reactions");
	PrintVector(instance.getReactions());

	PrintHeader("All Independent Reactions");
	PrintVector(instance.getIndependentReactionIds());

	PrintHeader("All Dependent Reactions");
	PrintVector(instance.getDependentReactionIds());

	PrintHeader("Conservation Laws");
	PrintVector(instance.getConservedLaws());

	PrintHeader("Reordered Species");
	PrintVector(instance.getReorderedSpecies());

	PrintHeader("Reordered Species Names");
	PrintVector(instance.getReorderedSpeciesNamesList());

	PrintHeader("Reordered Reactions");
	PrintVector(instance.getReorderedReactions());

	PrintHeader("Reaction Names");
	PrintVector(instance.getReactionsNamesList());

}

void PrintAllForFile(LibStructural &instance, string fileName)
{
	try
	{
		instance.loadSBMLFromFile(fileName);
		cout << instance.analyzeWithQR() << endl;

		PrintVectors(instance);
		PrintMatrices(instance);

		//instance.loadSBMLFromFile(fileName);

		//cout << instance.analyzeWithLU() << endl;

		//PrintVectors(instance);
		//PrintMatrices(instance);
	}	
	catch(...)
	{
		cout << "failure in PrintAllForFile: " << fileName << endl;	
	}

}

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "Need one fileName, SBML file" << endl;
		return -1;
	}


	LibStructural* instance = LibStructural::getInstance();

	PrintAllForFile(*instance, argv[1]);	

	if (argc > 2)
		PrintAllForFile(*instance, argv[2]);	
	if (argc > 3)
		PrintAllForFile(*instance, argv[3]);	
	if (argc > 4)
		PrintAllForFile(*instance, argv[4]);		

}

