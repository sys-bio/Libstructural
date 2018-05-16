


vector<vector<double> > execute (Network *network, bool noRevDup);
vector<vector<double> > execute (const char* file, Network *network, bool noRevDup);
vector<vector<double> > computeElementaryModes (Network *network, bool noRevDup);

double **convertEFMVectorsToArray (vector<vector<double> > efm);

void freeResources ();
void computeMaxCardinality (Network *network);