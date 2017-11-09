
public class Program {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		// need to load the swig generated wrapper first ... this should be in 
		// fancy try catch blocks ... 
		System.loadLibrary("structuralj");
		
		// this is the instance doing all the work
		LibStructural struct = new LibStructural();
		
		// test 1, load a SBML file and print the labeled fully reordered stoichiometry
		// matrix		
		loadSBMLandPrintFullyReorderedStoichiometryMatrix(struct, "/Applications/SBW-2.7.8/SBML Models/BorisEJB.xml");
		
		// test 2, load a stoichiometry matrix, and analyze this one.
		double[][] matrix = new double[][] { 
				new double[] {1.0,              -1.0,               0.0,              0.0},
                new double[] {0.0,               1.0,              -1.0,               0.0},
                new double[] {0.0,               0.0,               1.0,              -1.0}
		};
		String[] speciesNames = new String[] { "S2", "S3", "S4" };
		double[] speciesConcentrations = new double[] { 1.0, 0.0, 1.0 };
		String[] reactionNames = new String[] {"J1", "J2", "J3", "J4"};
		loadAndAnalyzeStoichiometryMatrix(struct, matrix, speciesNames, speciesConcentrations, reactionNames);
				
	}

	public static void loadAndAnalyzeStoichiometryMatrix(LibStructural struct,
			double[][] matrix,
			String[] speciesNames,
			double[] speciesConcentrations,
			String[] reactionNames) {
		
		struct.loadStoichiometryMatrix(createDoubleMatrixFromJavaMatrix(matrix));
		struct.loadSpecies(createStringVectorFromJavaStringArray(speciesNames),
				createDoubleVectorFromJavaDoubleArray(speciesConcentrations));
		struct.loadReactionNames(createStringVectorFromJavaStringArray(reactionNames));
		
		String result = struct.analyzeWithQR();
		System.out.println(result);
		System.out.println(struct.getTestDetails());
		
		printFullyReorderedStoichiometryMatrix(struct);
	}

	public static StringVector createStringVectorFromJavaStringArray(
			String[] vector) {
		if (vector == null) return new StringVector();
		int length = vector.length;
		if (length == 0) return new StringVector();
		StringVector result = new StringVector(length);
		for (int i = 0; i < length; i++) {
			result.set(i, vector[i]);
		}
		return result;
	}
	
	public static DoubleVector createDoubleVectorFromJavaDoubleArray(
			double[] vector) {
		if (vector == null) return new DoubleVector();
		int length = vector.length;
		if (length == 0) return new DoubleVector();
		DoubleVector result = new DoubleVector(length);
		for (int i = 0; i < length; i++) {
			result.set(i, vector[i]);
		}
		return result;
	}

	public static DoubleMatrix createDoubleMatrixFromJavaMatrix(
			double[][] matrix) {
		if (matrix == null) return new DoubleMatrix();
		int numRows = matrix.length;
		if (numRows == 0) return new DoubleMatrix();
		
		int numCols = matrix[0].length;
		
		DoubleMatrix doubleMatrix = new DoubleMatrix(numRows, numCols);
		for (int i = 0; i < numRows; i++) {
			for (int j = 0; j < numCols; j++) {
				doubleMatrix.set(i, j, matrix[i][j]);
			}
		}
		return doubleMatrix;
	}

	public static void loadSBMLandPrintFullyReorderedStoichiometryMatrix(
			LibStructural struct, String fileName) {
		String sResult = struct.loadSBMLFromFile(fileName);
		System.out.println(sResult);
		System.out.println(struct.getTestDetails());
		
		printFullyReorderedStoichiometryMatrix(struct);
	}

	public static void printFullyReorderedStoichiometryMatrix(
			LibStructural struct) {
		StringVector rowLabels = new StringVector();
		StringVector colLabels = new StringVector();		
		System.out.println();
		System.out.println("Fully Reordered Stoichiometry matrix");
		struct.getFullyReorderedStoichiometryMatrixLabels(rowLabels, colLabels);		
		printLabeledMatrix(struct.getFullyReorderedStoichiometryMatrix(), rowLabels,colLabels);
		System.out.println();
	}

	public static void printLabeledMatrix(
			DoubleMatrix matrix,
			StringVector rowLabels, StringVector colLabels) {		
		
		long cols = matrix.numCols();
		long rows = matrix.numRows();
		
		System.out.print("\t");
		for (int j = 0; j < cols; j++) {
			System.out.print(colLabels.get(j) + "\t");
			
		}
		System.out.println();
		
		for (int i = 0; i < rows; i++) {
			System.out.print(rowLabels.get(i) + "\t");
			for (int j = 0; j < cols; j++) {
				System.out.print(matrix.get(i,j) + "\t");
			}
			System.out.println();
		}
		
	}

	public static void printMatrix(
			DoubleMatrix matrix) {		
		long cols = matrix.numCols();
		long rows = matrix.numRows();
		
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				System.out.print(matrix.get(i,j) + "\t");
			}
			System.out.println();
		}
		
	}

}
