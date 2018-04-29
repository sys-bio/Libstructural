#ifndef REVERSIBLETREENODE_H
#define	REVERSIBLETREENODE_H

#include "PathwayPool.h"

using namespace std;

#define MAX_REVERSIBLE_TREE_NODES 20000

/**
 * Initializes Reversible Tree Node pool
 * @param size Size of Reversible Tree Node pool
 */
void initRevNodePool(int size);
/**
 * Free memory resources allocated for Reversible Tree Node pool
 */
void freeRevNodePool();
/**
 * Returns pointer to next free Reversible Tree Node in the pool
 * @param size Node size in bytes
 * @return Pointer to the node
 */
void* nextRevNode(int size);
/**
 * Clears the Reversible Tree Node pool
 */
void clearRevNodePool();

template <class BitVector>
class ReversibleTreeNode {
	public:

	   ReversibleTreeNode() {
	   }

	   void init(PathwayPool<BitVector>& list, int s, int e, BitVector& bitsCannotBeUsed, BitVector& bUsed) {
		  start = s;
		  bit = -1;
		  end = e;
		  node0 = node1 = NULL;
		  bitsUsed = bUsed;
		  //Check if the all the pathways in this partition are valid
		  if (!isValidPathway<BitVector>(bitsUsed)) {
			 end = start;
			 return;
		  }
		  bit = getSplitBit(bitsCannotBeUsed, list);
		  //Check if further partitioning is required
		  if (bit < 0) {
			 checkAllUnusedBits(bitsCannotBeUsed, list);
			 return;
		  }
		  //Split the data
		  BitVector updatedBitsCannotBeUsed(bitsCannotBeUsed);
		  updatedBitsCannotBeUsed.setBit(bit, true);
		  int middle = partition(list);
		  BitVector bUsed0(bUsed);
		  node0 = (ReversibleTreeNode*) nextRevNode(sizeof (ReversibleTreeNode<BitVector>));
		  node0->init(list, start, middle, updatedBitsCannotBeUsed, bUsed0);
		  bUsed.setBit(bit, true);
		  node1 = (ReversibleTreeNode*) nextRevNode(sizeof (ReversibleTreeNode<BitVector>));
		  node1->init(list, middle, end, updatedBitsCannotBeUsed, bUsed);
	   }

	   int size() {
		  return end - start;
	   }

	   bool isLeaf() {
		  return bit < 0;
	   }

	   ReversibleTreeNode* getNode0() {
		  return node0;
	   }

	   ReversibleTreeNode* getNode1() {
		  return node1;
	   }

	   BitVector& getBitsUsed() {
		  return bitsUsed;
	   }

	   int getStart() {
		  return start;
	   }

	   int getEnd() {
		  return end;
	   }


	private:
	   //Starting and ending index of pathways in the list
	   int start, end;
	   //Children nodes if current pathway is not a leaf
	   //node0 represents the subtree in which the reaction used as split point is inactive
	   //node1 represents the subtree in which the reaction used as split point is active
	   ReversibleTreeNode* node0;
	   ReversibleTreeNode* node1;
	   //Reaction used as split point. It is negative for leaf nodes.
	   int bit;
	   //Reactions that have been used for splitting the tree so far from root of the tree to this node
	   BitVector bitsUsed;

	   void checkAllUnusedBits(BitVector& bitsCannotBeUsed, PathwayPool<BitVector>& pathways) {
		  for (int r = 0; r < gefm_numReactions; r++) {
			 if (!bitsCannotBeUsed[r]) {
				if (pathways[start].reactionBitData[r]) {
				   bitsUsed.setBit(r, true);
				}
			 }
		  }
	   }
	   //Finds reaction that will be used to split pathways at current node

	   int getSplitBit(BitVector& bitsCannotBeUsed, PathwayPool<BitVector>& pathways) {
		  int bit = -1;
		  int count;
		  int minCount = size();
		  for (int r = 0; r < gefm_numReactions; r++) {
			 if (bitsCannotBeUsed[r]) {
				continue;
			 }
			 count = 0;
			 for (int p = start; p < end; p++) {
				if (pathways[p].reactionBitData[r]) {
				   count++;
				} else {
				   count--;
				}
			 }
			 if (count < 0) {
				count = -count;
			 }
			 if (count < minCount) {
				minCount = count;
				bit = r;
			 }
		  }
		  return bit;
	   }
	   //Sorts pathways such that pathways in which the split reaction is inactive appears before the pathways in which the split reaction is active

	   int partition(PathwayPool<BitVector>& pathways) {
		  int p0, p1;
		  Pathway<BitVector> p;
		  for (p0 = start, p1 = end - 1; p0 <= p1; p0++, p1--) {
			 //Pathway p0 has bit set
			 if (pathways[p0].reactionBitData[bit]) {
				//Pathway p1 has bit set
				if (pathways[p1].reactionBitData[bit]) {
				   //Do not skip p0
				   p0--;
				}//Pathway p1 has bit reset
				else {
				   p = pathways[p0];
				   pathways[p0] = pathways[p1];
				   pathways[p1] = p;
				}
			 }//Pathway p0 has bit reset
			 else {
				//Pathway p1 has bit reset
				if (!(pathways[p1].reactionBitData[bit])) {
				   //Do not skip p1
				   p1++;
				}
			 }
		  }
		  return p0;
	   }
};

#endif	/* REVERSIBLETREENODE_H */

