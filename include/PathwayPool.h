#ifndef PATHWAYPOOL_H
#define	PATHWAYPOOL_H

#include <cassert>
#include "Pathway.h"
#include "gEFMUtils.h"

#define MAX_PATHWAY_POOL_SIZE 20000

extern int globalErrorCode;

template <class BitVector>
class PathwayPool {
public:

	PathwayPool() {
		int sizep = sizeof(Pathway<BitVector>);
		pathways = (Pathway<BitVector>*) malloc(MAX_PATHWAY_POOL_SIZE * sizep);
		if (pathways == NULL) {
			globalErrorCode = -5;
			throw errorCodeStrings[-5];
		}
		//assert(pathways != NULL);
		nCount = 0;
	}

	~PathwayPool() {
		if (pathways) {
			free(pathways);
			pathways = NULL;
		}
	}

	unsigned int size() {
		return nCount;
	}

	Pathway<BitVector>& operator[] (const unsigned int nIndex) {
		assert(nIndex >= 0 && nIndex < nCount);
		return pathways[nIndex];
	}

	void operator<<(Pathway<BitVector>& pathway) {
		if ((nCount + 1) > MAX_PATHWAY_POOL_SIZE) {
			globalErrorCode = -5;
			throw errorCodeStrings[-5];
		}
		pathways[nCount] = pathway;
		nCount++;
	}

	Pathway<BitVector>* operator ++() {
		if((nCount + 1) > MAX_PATHWAY_POOL_SIZE) {
			globalErrorCode = -5;
			throw errorCodeStrings[-5];
		}
		return &pathways[nCount++];
	}

	void operator --() {
		assert(nCount > 0);
		nCount--;
	}

	void remove(const unsigned int nIndex) {
		assert(nCount > 0);
		nCount--;
		pathways[nIndex] = pathways[nCount];
	}

	void swap(int i, int j) {
		Pathway<BitVector> p = pathways[i];
		pathways[i] = pathways[j];
		pathways[j] = p;
	}

	Pathway<BitVector>* pathways;
	unsigned int nCount;
};

#endif	/* PATHWAYPOOL_H */
