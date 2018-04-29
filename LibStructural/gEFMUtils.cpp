#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <exception>
#include <stdexcept>
#include <sstream>

using namespace std;

void *gEFMsafeCalloc (std::string msg, size_t numberOfElements, size_t elementSize) {

	void *ret;

	if ((ret = calloc (numberOfElements, elementSize)) == NULL)
		throw runtime_error ("Memory Error, failed to allocate in: " + msg);

	return ret;
}
