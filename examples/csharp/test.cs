using System;
using structuralCS;

public class Test
{
	static int Main(string[] args)
	{
		Console.WriteLine("LibStructural Test Program");
		string LINE = new string('-', Console.WindowWidth);		
		Console.WriteLine(LINE);
		
		LibStructural test = new LibStructural();
		
		if (args.Length == 0) return 0;
		
		Console.WriteLine( test.loadSBMLFromFile(args[0]) );
		Console.WriteLine( test.getTestDetails() );
		
		return 0;
	}
}