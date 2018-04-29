#ifndef BITPATTERNTREENODE_H
#define	BITPATTERNTREENODE_H

#include <cassert>
#include "Pathway.h"
#include "BitVector.h"

//Maximum number of pathways in leaf node
#define MAX_PATHWAYS  10
#define MAX_BPT_TREE_NODES 20000

using namespace std;

/**
 * Initializes Bitpattern Tree Node pool
 * @param size Size of Bitpattern Tree Node pool
 */
void initBPTNodePool(int size);
/**
 * Free memory resources allocated for Bitpattern Tree Node pool
 */
void freeBPTNodePool();
/**
 * Returns pointer to next free Bitpattern Tree Node in the pool
 * @param size Node size in bytes
 * @return Pointer to the node
 */
void* nextBPTNode(int size);
/**
 * Clears the Bitpattern Tree Node pool
 */
void clearBPTNodePool();

template <class BitVector>
class BitPatternTreeNode {
	public:
	   //List containing pathways
	   Pathway<BitVector>* pathways[MAX_PATHWAYS + 1];

	   BitPatternTreeNode() {
		  init();
	   }

	   void init() {
		  node0 = NULL;
		  node1 = NULL;
		  bit = -1;
		  pathwayCount = 0;
		  label.setAllBits();
	   }

	   ~BitPatternTreeNode() {
	   }

	   //Adds pathway

	   void addPathway(Pathway<BitVector>* p, bool* bitsUsed) {
		  //Check if node is leaf
		  if (bit < 0) {
			 assert(pathwayCount <= MAX_PATHWAYS);
			 pathways[pathwayCount++] = p;
			 //Split if the number of pathways exceeds
			 if (pathwayCount > MAX_PATHWAYS) {
				split(bitsUsed);
			 }
			 label.bitwiseAnd(p->reactionBitData);
		  }//If node is not leaf
		  else {
			 bitsUsed[bit] = true;
			 //If the bit is one store in node 1
			 if (p->reactionBitData[bit]) {
				node1->addPathway(p, bitsUsed);
			 } else {
				node0->addPathway(p, bitsUsed);
			 }
			 label.bitwiseAnd(p->reactionBitData);
		  }
	   }


	   //Check if pathway is super set

	   bool isSuperSet(BitVector& rbd) {
		  if (rbd.notAndEqualsNot(label)) {
			 return false;
		  }
		  //Iterate through all the pathways in leaf node
		  if (bit < 0) {
			 bool subset, superset;
			 for (unsigned int i = 0; i < pathwayCount; i++) {
				//            rbd.setOperations((pathways[i])->reactionBitData, subset, superset);
				superset = rbd.isSuperSetOf((pathways[i])->reactionBitData);
				subset = (pathways[i])->reactionBitData.isSuperSetOf(rbd);
				//If set is p ignore it
				if (subset) {
				   continue;
				}
				//Check if p is superset
				if (superset) {
				   return true;
				}
			 }
			 return false;
		  } else {
			 //Select proper child node
			 if (rbd[bit]) {
				if (node1->isSuperSet(rbd)) {
				   return true;
				}
			 }
			 return node0->isSuperSet(rbd);
		  }
	   }
	   //Check if pathway is super set

	   bool isSuperSet(BitVector& rbd, BitVector& ref) {
		  if (rbd.notAndEqualsNot(label)) {
			 return false;
		  }
		  //Iterate through all the pathways in leaf node
		  if (bit < 0) {
			 for (unsigned int i = 0; i < pathwayCount; i++) {
				BitVector pth = (pathways[i])->reactionBitData;
				//If set is ref ignore it
				if (pth == ref) {
				   continue;
				}
				//Check if p is superset
				if (rbd.isSuperSetOf(pth)) {
				   return true;
				}
			 }
			 return false;
		  } else {
			 //Select proper child node
			 if (rbd[bit]) {
				if (node1->isSuperSet(rbd, ref)) {
				   return true;
				}
			 }
			 return node0->isSuperSet(rbd, ref);
		  }
	   }

	   //Check if pathway is super set

	   bool isSuperSet(Pathway<BitVector>* p) {
		  return isSuperSet(p->reactionBitData);
	   }

	   bool isSuperSet(Pathway<BitVector>* p, Pathway<BitVector>* ref) {
		  return isSuperSet(p->reactionBitData, ref->reactionBitData);
	   }

	   BitPatternTreeNode* getNode0() {
		  return node0;
	   }

	   BitPatternTreeNode* getNode1() {
		  return node1;
	   }

	   bool isLeaf() {
		  return bit < 0;
	   }
	private:
	   //Node storing zero set
	   BitPatternTreeNode* node0;
	   //Node storing one set
	   BitPatternTreeNode* node1;
	   //Bit position used for decision. It is negative for leaf nodes.
	   int bit;
	   //Label for the node
	   BitVector label;
	   //Size of pathway vector
	   unsigned int pathwayCount;

	   //Split current node

	   void split(bool* bitsUsed) {
		  int splitCount [MAX_REACTION_COUNT];
		  //Select the unused bit that has max number of ones and zeros
		  for (int i = 0; i < gefm_numReactions; i++) {
			 splitCount[i] = 0;
			 if (bitsUsed[i]) {
				continue;
			 }
			 for (unsigned int j = 0; j < pathwayCount; j++) {
				if ((pathways[j])->reactionBitData[i]) {
				   splitCount[i]++;
				} else {
				   splitCount[i]--;
				}
			 }
			 if (splitCount[i] < 0) {
				splitCount[i] = -splitCount[i];
			 }
		  }
		  int min = MAX_PATHWAYS;
		  for (int i = 0; i < gefm_numReactions; i++) {
			 if (bitsUsed[i]) {
				continue;
			 }
			 if (min > splitCount[i]) {
				min = splitCount[i];
				bit = i;
			 }
		  }
		  if (bit < 0) {
			 return;
		  }
		  node0 = (BitPatternTreeNode<BitVector>*) nextBPTNode(sizeof (BitPatternTreeNode<BitVector>));
		  node0->init();
		  node1 = (BitPatternTreeNode<BitVector>*) nextBPTNode(sizeof (BitPatternTreeNode<BitVector>));
		  node1->init();
		  //Copy pathways to new leaf nodes
		  for (unsigned int i = 0; i < pathwayCount; i++) {
			 if (pathways[i]->reactionBitData[bit]) {
				node1->pathways[node1->pathwayCount++] = pathways[i];
				node1->label.bitwiseAnd((pathways[i])->reactionBitData);
			 } else {
				node0->pathways[node0->pathwayCount++] = pathways[i];
				node0->label.bitwiseAnd((pathways[i])->reactionBitData);
			 }
		  }
		  pathwayCount = 0;
	   }
};

#endif	/* BITPATTERNTREENODE_H */

