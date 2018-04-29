#ifndef BITPATTERNTREE_H
#define	BITPATTERNTREE_H

#include "BitPatternTreeNode.h"

template <class BitVector>
class BitPatternTree {
public:
   BitPatternTreeNode<BitVector>* root;

   BitPatternTree() {
   }

   void init() {
      root = (BitPatternTreeNode<BitVector>*) nextBPTNode(sizeof (BitPatternTreeNode<BitVector>));
      root->init();
   }

   ~BitPatternTree() {
   }

   //Adds pathway to root node

   void addPathway(Pathway<BitVector>* p) {
      for (int i = 0; i < gefm_numReactions; i++) {
         bitsUsed[i] = false;
      }
      root->addPathway(p, bitsUsed);
   }

   //Checks if pathway is superset

   bool isSuperSet(BitVector& rbd) {
      return root->isSuperSet(rbd);
   }

   bool isSuperSet(BitVector& rbd, BitVector& ref) {
      return root->isSuperSet(rbd, ref);
   }

   bool isSuperSet(Pathway<BitVector>* p) {
      return root->isSuperSet(p);
   }

   bool isSuperSet(Pathway<BitVector>* p, Pathway<BitVector>* ref) {
      return root->isSuperSet(p, ref);
   }
private:
   bool bitsUsed [MAX_REACTION_COUNT];
};

#endif	/* BITPATTERNTREE_H */

