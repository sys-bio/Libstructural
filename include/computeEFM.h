


vector<vector<double> > execute (Network *network);
vector<vector<double> > execute (const char* file, Network *network);
vector<vector<double> > computeElementaryModes (Network *network);

double **convertEFMVectorsToArray (vector<vector<double> > efm);

void freeResources ();
void computeMaxCardinality (Network *network);