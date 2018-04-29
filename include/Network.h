#ifndef NETWORK_H
#define	NETWORK_H

#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>

using namespace std;
//Maximum line size
#define LINE_SIZE 102400
//Maximum token (word) size in a line
#define TOKEN_SIZE 1024

/**
 * This class represents a network after splitting the resversible reactions
 * into reversible reaction pair.
 */
class Network {
	private:

	   /**
		* Reads a single line from the given file.
		* @param file File to be read.
		* @param line Line read.
		* @param length Maximum line length.
		* @return Number of characters read.
		*/
	   int readLine(FILE* file, char* line, int length) {
		  int count, result;
		  char c;
		  for (count = 0; count < length - 1; count++) {
			 result = fscanf(file, "%c", &c);
			 if (result == EOF) {
				break;
			 }
			 if (c == '\r') {
				count--;
				continue;
			 }
			 if (c == '\n') {
				break;
			 }
			 line[count] = c;
		  }
		  line[count] = 0;
		  return count;
	   }

	   /**
		* Reads next token from the given line.
		* @param str String to be tokenized
		* @param token Next token
		* @param pointer Pointer to the next token
		*/
	   void nextToken(char* str, char* token, int& pointer) {
		  int count = 0;
		  while (str[pointer] != '\t' && str[pointer] != 0) {
			 token[count] = str[pointer];
			 count++;
			 pointer++;
		  }
		  token[count] = 0;
		  if (str[pointer] == '\t') {
			 pointer++;
		  }
	   }

	public:
	   //S matrix of the network
	   vector<vector<double> > s;
	   //Metabolite names
	   vector<std::string> metabolites;
	   //Reaction names
	   vector<string> reactions;
	   //Boolean flag for external metabolites (true if a metabolite is external)
	   vector<bool> external;
	   //Boolean flag for reversible reactions (true if a reaction is reversible)
	   vector<bool> reversible;
	   //Reversible reaction pairs
	   vector<int> reversiblePairs;

	   Network() {
	   }

	   ~Network() {
	   }

	   /**
	   * Checks if the network is valid.
	   * @return true if the network is valid, false otherwise.
	   */
	   bool validateNetwork () {
		   unsigned int reactionCount = reactions.size ();
		   unsigned int metaboliteCount = metabolites.size ();
		   bool valid = (reactionCount == reversible.size ());
		   valid = valid && (metaboliteCount == external.size ());
		   valid = valid && (metaboliteCount == s.size ());
		   for (unsigned int m = 0; valid && (m < metaboliteCount); m++) {
			   valid = (reactionCount == s[m].size ());
		   }
		   return valid;
	   }

		  /**
		* Splits reversible reactions into irreversible reaction pairs.
		*/
	   void splitReversibleReactions() {
		  for (unsigned int r = 0; r < reversible.size(); r++) {
			 if (reversible[r]) {
				string rxn(reactions[r].c_str());
				rxn.resize(rxn.size() + 2);
				rxn[rxn.size() - 2] = '_';
				rxn[rxn.size() - 1] = 'R';
				reactions.insert(reactions.begin() + r + 1, rxn);
				reversible.insert(reversible.begin() + r + 1, true);
				for (unsigned int m = 0; m < metabolites.size(); m++) {
				   s[m].insert(s[m].begin() + r + 1, (s[m][r] != 0) ? -s[m][r] : 0);
				}
				reversiblePairs.push_back(r);
				r++;
			 }
		  }
	   }

	   /**
		* Reads the network from the given file.
		* @param filename Full path of the file.
		* @return true if network is successfully loaded, false otherwise.
		*/
	   bool readNetworkFile(const char* filename) {
		  FILE* file = fopen(filename, "r");
		  if (file == NULL) {
			 fprintf(stderr, "Unable to open the following file for reading: %s\r\n", filename);
			 return false;
		  }
		  char line [LINE_SIZE];
		  char token [TOKEN_SIZE];
		  int pointer;
		  int lineNumber = 0;
		  int tokenNumber;
		  while (readLine(file, line, LINE_SIZE) > 0) {
			 pointer = 0;
			 tokenNumber = 0;
			 if (lineNumber >= 2) {
				s.push_back(vector<double>());
			 }
			 while (line[pointer] != 0) {
				nextToken(line, token, pointer);
				switch (lineNumber) {
					  //Reaction names
				   case 0:
					  if (tokenNumber >= 2) {
						 reactions.push_back(string(token));
					  }
					  break;
					  //Reaction reversibility
				   case 1:
					  if (tokenNumber >= 2) {
						  bool rev = (token[0] == '1') && (token[1] == 0);
						  reversible.push_back(rev);
						 //                     reversible.push_back(strcmp(token, "1") == 0);
					  }
					  break;
					  //Metabolites
				   default:
					  if (tokenNumber == 0) {
						 metabolites.push_back(string(token));
					  } else if (tokenNumber == 1) {
						  bool _external = (token[0] == '1') && (token[1] == 0);
						  external.push_back(_external);
						 //                     external.push_back(strcmp(token, "1") == 0);
					  } else {
						 s[lineNumber - 2].push_back(atof(token));
					  }
				}
				tokenNumber++;
			 }
			 lineNumber++;
		  }
		  fclose(file);
		  if (!validateNetwork()) {
			 fprintf(stderr, "The following file is not a valid network file : %s\r\n", filename);
			 return false;
		  }
		  splitReversibleReactions();
		  return true;
	   }
};

#endif	/* NETWORK_H */
