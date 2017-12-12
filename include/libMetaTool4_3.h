/*******************************************************************************
* libMetatool4_3 --  Refactored Metatool4_3                                    *
*                                                                              *
* Author:  Herbert, Sauro                                                      *
*                                                                              *
* Purpose:  Refactored Metatool4_3 to behave as a resuable library             *
*                                                                              *
* Usage:                                                                       *
*      See documentation for details of teh API.                               *
*      Simple example provided below:                                          *
********************************************************************************/

/* Original header in Metatool4_3. Note that the compilatoion instuctions are no longer appicable  */
/* ************************************************************************************ */
/* METATOOL: TO ANALYZE METABOLIC NETWORKS ******************************************** */
/* WRITTEN BY THOMAS PFEIFFER, extensive worked by F. Moldenhauer ********************* */
/* COMPILE WITH GCC, Microsoft C 6.0 or Borland C 5.0 ********************************* */
/* 5/2000 ***************************************************************************** */


#ifndef __LIBMETATOOL
#define __LIBMETATOOL

#define mt_FLOATING_SPECIES 0
#define mt_BOUNDARY_SPECIES 1

#define mt_REVERSIBLE 0
#define mt_IRREVERSIBLE 1

#define TXT 100   /* From original metatoool, will remove eventually */

struct mt_encoding { int rev; int key; int consumed; int built; int reactions; char txt[TXT]; struct mt_encoding *next; };

struct mt_vector { 
	  int row; 
	  int *head; 
};

extern mt_vector *lib_speciesStatus;
extern mt_vector *lib_reversibilityInformation;

struct mt_mat { int row; int col; int **head; };

struct mt_stringArray { int len; char **strArray; };

extern mt_stringArray *mt_createStringArray (int n);
extern void mt_addString (mt_stringArray *strArray, char *str, int index);
extern void mt_freeStringArray (struct mt_stringArray *ar);

extern struct mt_vector *mt_createVector (int r);
extern int mt_freeVector (struct mt_vector *v);
extern int mt_getMatrixItem (struct mt_mat *m, int row, int col, int *value);
extern void mt_setVectorItem (struct mt_vector *vector, int index, int value);
extern int mt_vectorOutput (struct mt_vector *v, FILE *savefile);

extern struct mt_mat *mt_createMatrix (int r, int c);
extern int mt_freeMatrix (struct mt_mat *m);
extern void mt_setMatrixItem (struct mt_mat *m, int row, int col, int value);
extern int mt_matrixOutput (FILE *saveFile, struct mt_mat *m);

extern struct mt_mat *mt_transpose (struct mt_mat *m);
extern struct mt_mat *mt_mult (struct mt_mat *m1, struct mt_mat *m2);
extern struct mt_mat *mt_addi (struct mt_mat *m);

extern struct mt_encoding *mt_addReactions (mt_stringArray *reversible, mt_stringArray *irreversible);
extern struct mt_encoding *mt_addMetabolitesInternal (mt_stringArray *floating, mt_stringArray *boundary);

extern int mt_ggt (int u, int v);

extern struct mt_mat *getStoichiometryMatrix (struct mt_mat *nex, struct mt_vector *met);
extern struct mt_vector *mt_subrev (struct mt_mat *m, struct mt_vector *v);
extern struct mt_mat *mt_cutcol (struct mt_mat *m, int cc);
extern struct mt_mat *mt_simplify (struct mt_mat *m);
extern struct mt_mat *mt_kernel (struct mt_mat *m);
extern int mt_switch_if_all_row_is_minus_into_plus (struct mt_mat *help);
extern struct mt_mat *mt_subset (struct mt_mat *m, struct mt_vector *v, int *wrong_subset);
extern struct mt_mat *mt_modes (struct mt_mat *m, struct mt_vector *v);
extern struct mt_mat *mt_basis (struct mt_mat *m, struct mt_vector *v);
extern int mt_control_modi (struct mt_mat *m, FILE *savefile, struct mt_encoding *enzlist);

extern int mt_overallOutput (struct mt_mat *m, struct mt_encoding *list, FILE *savefile);
extern void mt_fout_branches (FILE *fout, struct mt_encoding *metlist);
extern void crel_equation_output (FILE* fout, struct mt_mat* crel, struct mt_encoding *metlist);
extern struct mt_vector *mt_branch;

extern void mt_initialize (mt_mat *stoichiometryMatrix, /*struct mt_encoding *metlist, struct mt_encoding *enzlist,*/ mt_vector *rev);
extern void mt_destroy ();
extern struct mt_mat *mt_elementaryModes ();

extern struct mt_encoding *mt_addMetabolites (mt_stringArray *floating, mt_stringArray *boundary);
extern void    mt_loadSpeciesStatus (struct mt_vector *speciesStatus);
extern struct mt_mat *mt_loadFullStoichiometryMatrix (struct mt_mat *fullStoichiometryMatrix);
extern void    mt_addReversibilityInformation (struct mt_vector *reversibilityInformation);


#endif
