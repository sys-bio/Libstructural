
#ifndef PATHWAY_H
#define PATHWAY_H

#define MAX_METABOLITES 400
#include "BitVector.h"

template <class BitVector>
class Pathway {
	private:
	   // Parent pathways that were combined to generate this pathway
	   Pathway* parent1;
	   Pathway* parent2;
	public:
	   // Represents active reactions in the pathway
	   BitVector reactionBitData;
	   // Metabolite coefficients for the pathways
	   double metaboliteCoefficients[MAX_METABOLITES];
	   int metaboliteCount;

	   Pathway() {
		  parent1 = NULL;
		  parent2 = NULL;
		  metaboliteCount = 0;
	   }

	   Pathway(int reaction) {
		  parent1 = NULL;
		  parent2 = NULL;
		  metaboliteCount = gefm_numMetabolites;
		  for (int i = 0; i <metaboliteCount; i++) {
			  metaboliteCoefficients[i] = 0;
		  }
		  reactionBitData.setBit(reaction, true);
		  
	   }

	   ~Pathway() {
	   }

	   void setParents(Pathway* p1, Pathway* p2) {
		  parent1 = p1;
		  parent2 = p2;
		  metaboliteCount = 0;
		  reactionBitData = p1->reactionBitData;
		  reactionBitData.bitwiseOr(p2->reactionBitData);
	   }

	   void setMetaboliteCoefficient(int m, double c) {
		   metaboliteCoefficients[m] = c;
	   }

	   void updateMetaboliteCoefficients(int m) {
		  metaboliteCount = gefm_numMetabolitesRemaining;
		  double scale = -parent1->metaboliteCoefficients[m] / parent2->metaboliteCoefficients[m];
		  // This is done to keep co-efficients greater than 1
		  if (scale >= 1) {
			 for (int i = 0; i < metaboliteCount; i++) {
				 metaboliteCoefficients[i] = parent1->metaboliteCoefficients[i] + scale * parent2->metaboliteCoefficients[i];
			 }
		  } else {
			 scale = 1 / scale;
			 for (int i = 0; i < metaboliteCount; i++) {
				 metaboliteCoefficients[i] = scale * parent1->metaboliteCoefficients[i] + parent2->metaboliteCoefficients[i];
			 }
		  }
		  parent1 = parent2 = NULL;
	   }

	   bool isInput(int m) {
		  return (metaboliteCoefficients[m] < NEG_ZERO);
	   }

	   bool isOutput(int m) {
		  return (metaboliteCoefficients[m] > ZERO);
	   }

	   bool isSupersetOf(Pathway& p) {
		  return reactionBitData.isSuperSetOf(p.reactionBitData);
	   }

	   int getCardinality() {
		  return reactionBitData.getCardinality();
	   }
};


#endif	/* PATHWAY_H */
