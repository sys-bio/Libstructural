/*******************************************************************************
* libMetatool4_3 --  Refactored Metatool4_3                                    *
*                                                                              *
* Author:  Herbert, Sauro                                                      *
*                                                                              *
* Purpose:  Refactored Metatool4_3 to behave as a resuable library             *
*                                                                              *
* Usage:                                                                       *
*      See documentation for details of the API.                               *
********************************************************************************/

/* Original header in Metatool4_3. Note that the compilatoion instuctions are no longer appicable  */
/* ************************************************************************************ */
/* METATOOL: TO ANALYZE METABOLIC NETWORKS ******************************************** */
/* WRITTEN BY THOMAS PFEIFFER, extensive worked by F. Moldenhauer ********************* */
/* COMPILE WITH GCC, Microsoft C 6.0 or Borland C 5.0 ********************************* */
/* 5/2000 **************************************************************************** */

/* The original code appears to be in the public domain. It was decided to keep the same
access rights in this version. */

/* We hereby state that this code is in the public domain 2017 */

/* Modifications to the original Metatool4_3: */

/* Date: December, 2017 */
/* This version uses long integers for all computations. On a 64-bit machine this is 64 bits in size */
/* calloc and realloc replaced with safe equivalents */
/* All runtime errots are now handled via exceptions */
/* Added allocation routines for vectors, matrices and string lists*/
/* The code could do with some reformating but we'll keep as is for now */
/* A lot of vector and matrix indexing is handled by explisit pointer arthmetic */
/* It would probably be better is this were changed at some tim ein the future. */
/* How to use: */
/* 	mt_initialize (stoichiometryMatrix, reversibilityList);
	// Compute elementary mode
	elm = mt_elementaryModes ();
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <exception>
#include <stdexcept>
#include <sstream>

#include "libMetaToolInt4_3.h"

using namespace std;

struct mt_vector *mt_branch = NULL;

mt_vector *lib_speciesStatus;
mt_vector *lib_reversibilityInformation;

struct mt_internal {
	mt_mat *stoichiometryMatrix;
	mt_vector *reversiblilityList;
};

struct mt_internal *mt_internalDataStructure;

void mt_initialize (mt_mat *stoichiometryMatrix,  mt_vector *reversiblilityList) {

	mt_internalDataStructure = (struct mt_internal *) calloc (1, sizeof (struct mt_internal));
	mt_internalDataStructure->stoichiometryMatrix = stoichiometryMatrix;
	mt_internalDataStructure->reversiblilityList = reversiblilityList;
}

void mt_destroy () {
	free (mt_internalDataStructure);
}



// ---------------------------------------------------------------
// Safe memory allocation routines

void *safeCalloc (std::string msg, size_t numberOfElements, size_t elementSize) {

	void *ret;

	if ((ret = calloc (numberOfElements, elementSize)) == NULL)
		throw std::runtime_error ("Memory Error, failed to allocate in: " + msg);

	return ret;
}


void *safeRealloc (std::string msg, void *ptr, size_t newSize) {

	void *ret;

	if ((ret = realloc (ptr, newSize)) == NULL) {
		throw std::runtime_error ("Memory Error, failed to realloc in: " + msg);
	}
	return ret;
}

// ---------------------------------------------------------------
// String Array Support

mt_stringArray *mt_createStringArray (int n) {
	
	struct mt_stringArray *ar;

	ar = (struct mt_stringArray *) calloc (1, sizeof (struct mt_stringArray));
	ar->len = 0;
	ar->strArray = (char **)calloc (n, sizeof (char *));
	return ar;
}


void mt_addString (struct mt_stringArray *ar, char *str, int index) {

	ar->strArray[index] = (char *) malloc (strlen (str) + 1);
	strcpy (ar->strArray[index], str);
	ar->len = ar->len + 1; 
}

void mt_freeStringArray (struct mt_stringArray *ar) {
	for (int i = 0; i < ar->len; i++)
		free (ar->strArray[i]);
	free (ar);
}

// ---------------------------------------------------------------
// Vector support

struct mt_vector *mt_createVector (int r) {

	struct mt_vector *v;

	v = (struct mt_vector*) malloc (sizeof(struct mt_vector));
	v->row = r;
	v->head = (long*) malloc (v->row*sizeof(long));
	return v;
}

void mt_setVectorItem (struct mt_vector *vector, int index, long value) {
	vector->head[index] = value;
}


int mt_freeVector (struct mt_vector *v)
{
	free (v->head);
	free (v);
	return 0;
}

int mt_vectorOutput (struct mt_vector *v, FILE *savefile)
{
	int i;
	for (i = 0; i<(v->row); i++) 
		fprintf (savefile, "%2d ", v->head[i]);
	fprintf (savefile, "\n"); 
	return 0;
}


// ---------------------------------------------------------------
// Matrix support

struct mt_mat *mt_createMatrix (int r, int c) {

	struct mt_mat *m = NULL;
	int i;

	m = (struct mt_mat*) safeCalloc ("mt_createMatrix", 1, sizeof(struct mt_mat));
	m->row = r;
	m->col = c;

	m->head = (long**) calloc (m->row, sizeof(long*));
	for (i = 0; i < m->row; i++)
		m->head[i] = (long*) calloc (m->col, sizeof(long));
	return m;
}


void mt_setMatrixItem (struct mt_mat *m, int row, int col, long value) {
	m->head[row][col] = value;
}


int mt_getMatrixItem (struct mt_mat *m, int row, int col, long *value) {
	if ((row < 0) || (row >= m->row) || (col < 0) || (col >= m->col)) {
		return -1;
    }
	*value = m->head[row][col];
	return 0;
}

int mt_matrixOutput (FILE *saveFile, struct mt_mat *m)
{
	int i, ii, zero_count, zero_lines = 0;

	// first count zero_lines the correct matrix dimension FM
	for (i = 0; i<(m->row); i++)
	{
		for (zero_count = ii = 0; ii<(m->col); ii++)
		if (*(*((m->head) + i) + ii) == 0) zero_count++;
		if (zero_count == m->col)
			zero_lines++;
	}

	if (!(m->row - zero_lines)) return m->row - zero_lines;
	fprintf (saveFile, " \n matrix dimension r%d x c%d\n", m->row - zero_lines, m->col);
	for (i = 0; i<(m->row); i++)
	{
		// do not fprintf if a line contains only zeros
		for (zero_count = ii = 0; ii<(m->col); ii++)
		if (*(*((m->head) + i) + ii) == 0) zero_count++;
		if (zero_count == m->col) continue;

		for (ii = 0; ii<(m->col); ii++)
			fprintf (saveFile, "%2d ", *(*((m->head) + i) + ii));
		fprintf (saveFile, "\n");
	}
	fflush (saveFile);  // return 0; 
	return m->row - zero_lines;
} // fmatoutput


int mt_freeMatrix (struct mt_mat *m)
{
	for (int i = 0; i < m->row; i++)
		free (m->head[i]);
	free (m->head); 
	free (m); 
	return 0;
}

// Matrix multiply
struct mt_mat *mt_mult (struct mt_mat *m1, struct mt_mat *m2) {
	struct mt_mat *mm; 
	int i, j, k; 
	long sum;

	try {
		if (m1->col != m2->row)
			throw std::runtime_error ("error in mt_mult: mat dimensions are incompatible");

		mm = mt_createMatrix (m1->row, m2->col);

		for (i = 0; i < mm->row; i++)
		{
			for (k = 0; k < mm->col; k++)
			{
				sum = 0;
				for (j = 0; j < m1->col; j++)
					sum += m1->head[i][j] * m2->head[j][k];
				mm->head[i][k] = sum;
			}
		}
		return mm;
	}
	catch (...)
	{
		throw std::runtime_error ("Unknown error in mt_mult");
	}
}


struct mt_mat *mt_transpose (struct mt_mat *m)
{
	struct mt_mat *tm; 

	tm = mt_createMatrix (m->col, m->row);
	for (int i = 0; i < m->col; i++)
	{
		for (int j = 0; j < m->row; j++)
			tm->head[i][j] = m->head[j][i];
	}
	return tm;
}


struct mt_mat *mt_addi (struct mt_mat *m)
{
	struct mt_mat *mi;

	mi = mt_createMatrix (m->row, m->col + m->row);
	
	for (int i = 0; i < m->row; i++)
	{
		for (int j = 0; j < mi->col; j++)
		{
			if (j < m->col)
				mi->head[i][j] = m->head[i][j];
			else if (j == m->col + i)
				mi->head[i][j] = 1;
			else
				mi->head[i][j] = 0;
		}
	}
	return mi;
}


// --------------------------------------------------------------------------------------

struct mt_encoding *mt_insert (struct mt_encoding *t1, int r, int k, char t[TXT])
{
	struct mt_encoding *x;
	int i;
	x = (struct mt_encoding*) safeCalloc ("mt_insert", 1, sizeof (struct mt_encoding)); 
	x->key = k; x->rev = r;
	for (i = 0; i < TXT; i++) x->txt[i] = t[i];
	x->next = t1->next;
	t1->next = x;
	return x;
}


struct mt_encoding *mt_addReactions (mt_stringArray *reversible, mt_stringArray *irreversible) {

	int i = 0;
	int nr, ni;

	struct mt_encoding *enzlist, *eac;

	nr = reversible->len;
	ni = irreversible->len;

	enzlist = (struct mt_encoding*) safeCalloc ("mt_addreactions", 1, sizeof(struct mt_encoding)); 
	enzlist->next = (struct mt_encoding*) safeCalloc ("mt_addreactions", 1, sizeof(struct mt_encoding));      
	enzlist->key = 0;
	enzlist->next->next = enzlist->next; enzlist->next->key = 0;
	eac = enzlist;

	for (i = 0; i < nr; i++)
		eac = mt_insert (eac, 0, (eac->key) + 1, reversible->strArray[i]);

	for (i = 0; i < ni; i++)
		eac = mt_insert (eac, 1, (eac->key) + 1, irreversible->strArray[i]);

	return enzlist;
}

struct mt_encoding *mt_addMetabolitesInternal (mt_stringArray *floating, mt_stringArray *boundary) {

	struct mt_encoding *metlist, *eac, *mz;
	int sf, bf;

	sf = floating->len;
	bf = boundary->len;

	metlist = (struct mt_encoding*) safeCalloc ("mt_addMetaboloitesInternal", 1, sizeof(struct mt_encoding)); 
	mz = (struct mt_encoding*) safeCalloc ("mt_addMetaboloitesInternal", 1, sizeof(struct mt_encoding));

	metlist->next = mz; metlist->key = 0;
	mz->next = mz; mz->key = 0; eac = metlist;

	for (int i = 0; i < sf; i++)
		eac = mt_insert (eac, 0, (eac->key) + 1, floating->strArray[i]);

	for (int i = 0; i < bf; i++)
		eac = mt_insert (eac, 1, (eac->key) + 1, boundary->strArray[i]);

	return metlist;
}


// --------------------------------------------------------------------------------------


/* function biggest common divisor */
int mt_ggt (int u, int v)
{
	int t;
	u = abs (u); v = abs (v);
	if (u*v == 0) return (u + v);
	while (u>0) { if (u<v) { t = u; u = v; v = t; }  u = u%v; }
	return v;
} // ggt


void mt_ggt_matrix (struct mt_mat *v)
{
	// divide each row of the new tableau v by the greatest common denominator of that row
	int denominator;
	int c, r, abs1, y_n;
	//*->*/    printf("v r%d x c%d", v->row, v->col);   print_mat( v );
	for (r = 0; r < v->row; r++)
	{
		denominator = 1; y_n = 1;
		for (c = 0; c < v->col; c++)
		{
			abs1 = abs (*(*(v->head + r) + c));
			if (abs1 == 1)
			{
				denominator = 1; break;
			}
			else if (abs1)
			{
				if (y_n == 1)denominator = abs1; // this command is for initialisation of denominator 
				else denominator = mt_ggt (denominator, abs1);
				y_n = 0;
			}
		}
		if (denominator > 1)
		for (c = 0; c < v->col; c++)
		if (*(*(v->head + r) + c))
			*(*(v->head + r) + c) /= denominator;
		// NEXT_ROW:;
	}
}


int mt_numerical_array (double k2, double hi, double k1, double hu,
	const int abs1, const int sign1, const int sign2,
	const char*loop)
{
	// control the numerical overflow of calculation of tablaux
	double result;
	if (abs1)
		result = sign1*fabs (k2)* hi + sign2*fabs (k1)* hu;
	else
		result = sign1*k2*hi + sign2*k1*hu;

	if (result > LONG_MAX || result < LONG_MIN)
	{
		ostringstream msg;
		msg << "The intermediate result " << result << " exceeds the allowed integer range (+/- " << LONG_MAX << ")";
		throw std::runtime_error (msg.str());
	}
	return 0;
} // numerical_array



int mt_control_modi (struct mt_mat *m, FILE *savefile, struct mt_encoding *enzlist)
{
	int r1, r2, n = 0, k, kk, not_zero_r1, not_zero_r2, same;
	struct mt_encoding *enznames;

	for (r1 = 0; r1<m->row - 1; r1++)
	{
		for (not_zero_r1 = k = 0; k<m->col; k++)
		if (*(*((m->head) + r1) + k)) not_zero_r1++;
		// fprintf(savefile, "%d\t%d\n", r1+1, not_zero_r1);

		for (r2 = r1 + 1; r2<m->row; r2++)
		{
			/*not_zero_r1=*/not_zero_r2 = same = 0;
			for (k = 0; k<m->col; k++)
			{
				// if(*(*((m->head)+r1)+k)) not_zero_r1++;  // line is moved 04.02.2002
				if (*(*((m->head) + r2) + k)) not_zero_r2++;
				if (*(*((m->head) + r1) + k) && *(*((m->head) + r2) + k)) same++;
			}
			if ((same == not_zero_r1 || same == not_zero_r2) && same && not_zero_r1 && not_zero_r2)
			{
				ostringstream msg;
				if (not_zero_r2 > not_zero_r1) {
					msg << "\nElementary mode " << r2 + 1 << "(" << not_zero_r2 << ") contains elementary mode " << r1 + 1 << "(" << not_zero_r1 << ")";
					throw std::runtime_error (msg.str ());
				}
				else if (not_zero_r2 == not_zero_r1) {
					msg << "Elementary mode " << r2 + 1 << "(" << not_zero_r2 << ") and " << r1 + 1 << "(" << not_zero_r1 << ") are identical.";
					throw std::runtime_error (msg.str ());
				}
				else {
					msg << "Elementary mode " << r1 + 1 << "(" << not_zero_r1 << ") contains elementary mode " << r2 + 1 << "(" << not_zero_r2 << ").";
					throw std::runtime_error (msg.str ());
				}
				// Commented out by HMS Dec 2017
				//if (not_zero_r2 > not_zero_r1)     fprintf (savefile, "\nElementary mode %d (%d) contains elementary mode %d (%d).", r2 + 1, not_zero_r2, r1 + 1, not_zero_r1);
				//else if (not_zero_r2 == not_zero_r1) fprintf (savefile, "\nElementary mode %d (%d) and %d (%d) are identical.", r1 + 1, not_zero_r1, r2 + 1, not_zero_r2);
				//else                              fprintf (savefile, "\nElementary mode %d (%d) contains elementary mode %d (%d).", r1 + 1, not_zero_r1, r2 + 1, not_zero_r2);
				n = 1;
			}
		}
	}

	/* count number of reactions in elementary modes **********
	for( not_zero_r1=k=0; k<m->col; k++)
	if(*(*((m->head)+r1)+k)) not_zero_r1++;
	fprintf(savefile, "%d\t%d\n", r1+1, not_zero_r1); */

	if (n) { printf ("\n"); fprintf (savefile, "\n"); }
	// control which enzymes do not occur in elementary modes
	enznames = enzlist;
	for (kk = k = 0, r1 = 0; r1<m->col; r1++)
	{
		enznames = enznames->next;
		for (r2 = 0; r2<m->row; r2++)
		{
			if (*(*((m->head) + r2) + r1)) { k++; break; }
		}
		if (r2 == m->row)
		if (!(kk++)) fprintf (savefile, "\n%s\t", enznames->txt);
		else   fprintf (savefile, "%s\t", enznames->txt);
	}
	if (k != m->col)
	if (m->col - k > 1) fprintf (savefile, "\n%d enzymes are not involved in reactions.\n", m->col - k);
	else             fprintf (savefile, "\n%d enzyme is not involved in reactions.\n", m->col - k);
	return 0;
} // control_modi


struct mt_mat *mt_cutcol (struct mt_mat *m, int cc)
{
	struct mt_mat *mc; 
	int i, ii;

	mc = mt_createMatrix (m->row, m->col - cc);
	if (cc > m->col) {
		throw std::runtime_error ("ERROR IN FUNCTION CUTCOL: CC>M->ROW!\n");
	}
	for (i = 0; i < mc->row; i++)
	{
		for (ii = 0; ii < mc->col; ii++)
			mc->head[i][ii] = m->head[i][ii + cc];
	}
	return mc;
}

// Output routines
void mt_fout_branches (FILE *fout, struct mt_encoding *metlist)
{
	int i;
	struct mt_encoding *help;
	help = metlist->next;
	fprintf (fout, "\n");
	fprintf (fout, "-> Branch metabolites are : ");
	fprintf (fout, "\n%-20s\tcons\tbuilt\treactions", "met");
	for (i = 0; i<mt_branch->row; i++)
	{
		if (*(mt_branch->head + i))
			fprintf (fout, "\n%-20s\t%d\t%d\t%d", help->txt, help->consumed, help->built, help->reactions);
		help = help->next;
	}
	fprintf (fout, "\n");
	help = metlist->next;
	fprintf (fout, "\n-> No branch metabolites are : ");
	fprintf (fout, "\n%-20s\tcons\tbuilt\treactions", "met");
	for (i = 0; i<mt_branch->row; i++)
	{
		if (!*(mt_branch->head + i))
			fprintf (fout, "\n%-20s\t%d\t%d\t%d", help->txt, help->consumed, help->built, help->reactions);
		help = help->next;
	}
	fprintf (fout, "\n");
} // fout_branches 


// file output for conservation relation equations
void crel_equation_output (FILE* fout, struct mt_mat* crel, struct mt_encoding *metlist)
{
	struct mt_encoding* ml;
	int col, row, k, cr = 0;

	// look if all of the rows are set only with zero
	for (row = 0; row<crel->row; row++)
	{
		ml = metlist;
		for (k = col = 0; col<crel->col; col++)
		{
			if (*(*(crel->head + row) + col))
			{
				if (k++)
					cr = 1;
				else
					cr = 1;
			}
			if (cr)break;
			ml = ml->next;
		}
		if (cr)break;
	} // then skip the following loop

	if (cr)
	for (row = 0; row<crel->row; row++)
	{
		fprintf (fout, "%2d :\t", row + 1);
		ml = metlist;
		for (k = col = 0; col<crel->col; col++)
		{
			if (*(*(crel->head + row) + col))
			{
				if (k++)
				{
					if (crel->head[row][col] == 1)
						fprintf (fout, " + %s", ml->next->txt);
					else if (crel->head[row][col] == -1)
						fprintf (fout, " - %s", ml->next->txt);
					else if (crel->head[row][col] > 1)
						fprintf (fout, " + %d %s", crel->head[row][col], ml->next->txt);
					else if (crel->head[row][col] < -1)
						fprintf (fout, " - %d %s", (-1)*(crel->head[row][col]), ml->next->txt);
				}
				else
				{
					if (crel->head[row][col] == 1)
						fprintf (fout, "%s", ml->next->txt);
					else if (crel->head[row][col] == -1)
						fprintf (fout, "-%s", ml->next->txt);
					else if (abs (crel->head[row][col]) != 1)
						fprintf (fout, "%d %s", crel->head[row][col], ml->next->txt);
				}
			}
			ml = ml->next;
		}
		if (k) fprintf (fout, " = const\n");
	}
} // crel_equation_output



// ----------------------------------------------------------------------------


/* 2.4 complex mat operations ************** */
void mt_switch_if_all_rows_are_minus_into_plus (struct mt_mat *help)
{
	int i, j, count_negatives, count_positives;
	for (i = 0; i < help->row; i++) {
		count_negatives = 0; count_positives = 0;
		for (j = 0; j < help->col; j++)
		    if (*(*(help->head + i) + j) < 0)
			    count_negatives++;
		    else if (*(*(help->head + i) + j) > 0)
		    {
			   count_positives++;
			   break;
		    }
		if (!count_positives && count_negatives)
		    for (j = 0; j < help->col; j++)
		         if (*(*(help->head + i) + j)) *(*(help->head + i) + j) *= -1;
	}
}

int stack_down (struct mt_mat *m, int r1)
{
	// matrix h2 overwrite the line r1 with the following line and so on
	int i, j, col;
	for (i = r1; i<m->row - 1; i++)
	{
		for (j = i + 1, col = 0; col<m->col; col++)
			*(*(m->head + i) + col) = *(*(m->head + j) + col);
	}
	// do not free( *(m->head+j) ); that memory is freed in modes with the variable r01 at line 1590
	m->row--;
	return 0;
} // stack_down


int stack_down_rev (long *rev1, int r1, int row)
{
	// vector rev1 stack_down
	int i, j;
	// realloc ???
	for (i = r1; i<row - 1; i++)
	{
		for (j = i + 1; j<row; j++)
			*(rev1 + i) = *(rev1 + j);
	}
	// do not free( *(m->head+j) ); that memory is freed in modes with the variable r01 at line 1590
	return 0;
} // stack_down_rev


int mt_control_condition7 (struct mt_mat *m, int ii, long *rev1, int *r01) // Version 4.3 02.10.02
{
	int r1, r2, c, zero_r1, same12, z = 0;
	// struct vector hv; 
	// int zero_r2;

	if (m->row>1)
	for (r1 = 0; r1<m->row - 1; r1++)
	{
		for (zero_r1 = 0, c = ii; c<m->col; c++)
		if (!*(*(m->head + r1) + c)) zero_r1++;
		for (r2 = r1 + 1; r2<m->row; r2++)
		{
			/*zero_r2=*/same12 = 0;
			for (c = ii; c<m->col; c++)
			{
				// if(!*(*(m->head+r2)+c)) zero_r2++;
				if (!*(*(m->head + r1) + c) && !*(*(m->head + r2) + c)) same12++;
			}
			if (zero_r1 == same12)
			{
				//*->*/  printf("\nr1=%d\tr2=%d\tm->row=%d\t\trev1=%d\trev2=%d", r1, r2, m->row, *(rev1+r1), *(rev1+r2));
				//HMS printf ("Delete line %d. (%d)\n", r1, ++z);
				//*->*/  hv.head=*(m->head+r1); hv.row=m->col; vectoroutput(&hv); 
				//*->*/  hv.head=*(m->head+r2); hv.row=m->col; vectoroutput(&hv); printf("\n"); getch();
				stack_down (m, r1); // since row r2 contains more zeros than row r1, hence row r1 has to be deleted
				stack_down_rev (rev1, r1, m->row + 1);
				//HMS printf ("Condition 7: line %d deleted.\n", r1);
				r1--; break;
			}
		}
	}
	(*r01) = m->row;
	return 0;
} // control_condition7


struct mt_vector *mt_subrev (struct mt_mat *m, struct mt_vector *v)
{
	int i, ii;
	struct mt_vector *r;

	r = (struct mt_vector*) safeCalloc ("mt_subrev", 1, sizeof(struct mt_vector));  
	r->row = m->row; r->head = (long*) safeCalloc ("mt_subrev", r->row, sizeof(long));  
	for (i = 0; i<m->row; i++)
	{
		*(r->head + i) = 0;
		for (ii = 0; ii<m->col; ii++) 
			if ((*(*(m->head + i) + ii)) && (*(v->head + ii))) { 
				*(r->head + i) = 1; 
				break;
			}
	}
	return r;
}

// Remove the botton section of nex which includes the external metabolites. 
// Returns the stoichiometry matrix, rows only have internal (floating) metabolites
struct mt_mat *getStoichiometryMatrix (struct mt_mat *nex, struct mt_vector *met)
	// *mex, struct vector *ex)
{
	struct mt_mat *m;
	int i, k = 0, j;

	m = (struct mt_mat*) safeCalloc ("getStoichiometryMatrix", 1, sizeof(struct mt_mat)); 
	m->row = 0; m->col = nex->col;
	for (i = 0; i < met->row; i++)
	if (!*(met->head + i))
		m->row++;  // Counts floats
	else;
	if (m->row > 0)
	{
		m->head = (long**) calloc (m->row, sizeof(long*)); 
	}
	else
	{
		throw std::runtime_error ("The system comprises only external metabolites");
	}

	for (i = 0; i < met->row; i++)
	  if (!*(met->head + i))
	  {
		/* allocated by FM */
		//*(m->head + k) = (int*) calloc (m->col, sizeof(int));  //addressed (*(m->head + i), "*(m->head+i) not allocated");
		  long *x = (long*) safeCalloc ("getStoichiometryMatrix", m->col, sizeof(long));
		  m->head[k] = x; //addressed (*(m->head + i), "*(m->head+i) not allocated");
	      for (j = 0; j < m->col; j++)
			*(*(m->head + k) + j) = *(*(nex->head + i) + j);
		k++;
	  }
	return m;
}


struct mt_mat *mt_simplify (struct mt_mat *m)
{
	struct mt_mat *mc;
	struct mt_vector *vf;
	int i, ii, k;

	vf = (struct mt_vector*) safeCalloc ("mt_simplify", 1, sizeof(struct mt_vector)); 
	vf->row = m->row; vf->head = (long*) safeCalloc ("mt_simplify", vf->row, sizeof(long)); 

	mc = (struct mt_mat*) safeCalloc ("mt_simplify", 1, sizeof(struct mt_mat)); 
	mc->row = m->row; mc->col = m->col;

	{
		if (mt_branch)
		{
			mt_branch = (struct mt_vector*) safeCalloc ("mt_simplify", 1, sizeof(struct mt_vector)); 
			mt_branch->row = m->row; mt_branch->head = (long*) safeCalloc ("mt_simplify", mt_branch->row, sizeof(long)); 
			for (i = 0; i<mt_branch->row; i++) *(mt_branch->head + i) = 1;
		}
	}

	for (i = 0; i<m->row; i++)
	{
		*(vf->head + i) = 0;
		for (ii = 0; ii<m->col; ii++)
			*(vf->head + i) = mt_ggt (*(vf->head + i), *(*(m->head + i) + ii));
		if (*(vf->head + i) == 0)  // at this line are shown the not branch metabolites
		{
			if (mt_branch) *(mt_branch->head + i) = 0; // write into the global variable
			mc->row--;
		}
	}
	if (mt_branch)
	{
		//HMS printf ("\nBranches "); 
		//HMS for (i = 0; i<mt_branch->row; i++) printf ("%d", *(mt_branch->head + i)); printf ("\n");
	}

	if (!mc->row) // Microsoft C++ 6.0 allocates any pointer if(mc->row==0 but no correct one)
	{
		mc->head = (long**) safeCalloc ("mt_simplify", m->row, sizeof(long*)); 
		// return a copy of *m
		for (i = 0; i<m->row; i++)
		{
			*(mc->head + i) = (long*) safeCalloc ("mt_simplify", m->col, sizeof(long));
			for (ii = 0; ii<m->col; ii++)
				*(*(mc->head + i) + ii) = *(*(m->head + i) + ii);
		}
		mc->row = m->row; mc->col = mc->col;
	}
	else
	{
		mc->head = (long**) safeCalloc ("mt_simplify", mc->row, sizeof(long*));
		k = 0;
		for (i = 0; i<vf->row; i++) if (*(vf->head + i) != 0)
		{
			*(mc->head + k) = (long*) safeCalloc ("mt_simplify", mc->col, sizeof(long)); 
			for (ii = 0; ii<mc->col; ii++)
				*(*(mc->head + k) + ii) = *(*(m->head + i) + ii) / (*(vf->head + i));
			k++;
		}
	}
	mt_freeVector (vf);
	return mc;
} // simplify


/* Kernel */
struct mt_mat *mt_kernel (struct mt_mat *m)
{
	struct mt_mat *help, *k; int i, u, ii, uu, f1, f2;

	k = mt_transpose (m); help = mt_addi (k); mt_freeMatrix (k);

	for (ii = 0; ii<m->row; ii++)
	{
		for (i = 0; i<help->row; i++)
		if (*(*(help->head + i) + ii) != 0) break;
		for (u = i + 1; u<help->row; u++)
		{
			if (*(*(help->head + u) + ii) != 0)
			{
				f1 = (*(*(help->head + u) + ii)) / mt_ggt (*(*(help->head + u) + ii), *(*(help->head + i) + ii));
				f2 = (*(*(help->head + i) + ii)) / mt_ggt (*(*(help->head + u) + ii), *(*(help->head + i) + ii));
				for (uu = 0; uu<help->col; uu++)
					*(*(help->head + u) + uu) = f2*(*(*(help->head + u) + uu)) - f1*(*(*(help->head + i) + uu));
			}
		}
		if (i<help->row) for (uu = 0; uu<help->col; uu++) *(*(help->head + i) + uu) = 0;
	}
	k = mt_cutcol (help, m->row);
	mt_freeMatrix (help);
	help = mt_simplify (k);
	mt_freeMatrix (k);
	mt_switch_if_all_rows_are_minus_into_plus (help);
	return help;
} // Ke


/* Subsets */
struct mt_mat *mt_subset (struct mt_mat *m, struct mt_vector *v, int *wrong_subset) {
	struct mt_mat *help, *k; int i, ii, sign1, f1, u;

	k = mt_transpose (m);
	help = mt_addi (k);
	mt_freeMatrix (k);

	for (i = 0; i < help->row; i++)
	{
		f1 = 0; sign1 = 0;
		for (ii = 0; ii < m->row; ii++) if (*(*(help->head + i) + ii) != 0)
		{
			f1 = mt_ggt (f1, abs (*(*(help->head + i) + ii)));
			if (sign1 == 0) sign1 = *(*(help->head + i) + ii) / abs (*(*(help->head + i) + ii));
		}
		if (f1 == 0) for (ii = 0; ii < help->col; ii++) *(*(help->head + i) + ii) = 0;
		else
		{
			for (ii = 0; ii < m->row; ii++) *(*(help->head + i) + ii) = *(*(help->head + i) + ii)*sign1 / f1;
			*(*(help->head + i) + i + m->row) = f1*sign1;
		}
	}
	for (i = 0; i < help->row; i++) for (u = i + 1; u < help->row; u++)
	{
		f1 = 0;
		for (ii = 0; ii < m->row; ii++) if ((*(*(help->head + i) + ii)) != (*(*(help->head + u) + ii))) f1 = 1;
		if (f1 == 0)
		{
			for (ii = m->row; ii < help->col; ii++) *(*(help->head + i) + ii) += *(*(help->head + u) + ii);
			for (ii = 0; ii < help->col; ii++) *(*(help->head + u) + ii) = 0;
		}
	}

	k = mt_cutcol (help, m->row); mt_freeMatrix (help);
	help = mt_simplify (k); mt_freeMatrix (k);
	for (i = 0; i < help->row; i++)						/* revstatetest */
	{
		f1 = 0;
		for (ii = 0; ii < help->col; ii++) if ((*(v->head + ii)) && (*(*(help->head + i) + ii) < 0)) { f1 = 1; break; }
		if (f1 == 1) for (ii = 0; ii < help->col; ii++) *(*(help->head + i) + ii) *= -1;
		f1 = 0;
		for (ii = 0; ii < help->col; ii++) if ((*(v->head + ii)) && (*(*(help->head + i) + ii) < 0)) { f1 = 1; break; }
		if (f1 == 1)
			*wrong_subset = i; // for (ii=0;ii<help->col;ii++) *(*(help->head+i)+ii)=0;
	}
	mt_switch_if_all_rows_are_minus_into_plus (help);
	return (help);
}//subset


/* basis */
struct mt_mat *mt_basis (struct mt_mat *m, struct mt_vector *v)
{
	struct mt_mat *help, *k;
	long **h1, **h2 = NULL, abs_hi, abs_hu, ggt_Erg;
	long *rev1 = NULL, *rev2 = NULL;
	int i, u, tu, ii, uu, test1;
	int r1, r2, c, counter;
	int f1, k1, k2;

	k = mt_transpose (m); help = mt_addi (k); mt_freeMatrix (k);
	r1 = (help->row); c = help->col; counter = m->row;
	rev1 = (long*) safeCalloc ("mt_basis", v->row, sizeof(long));  //addressed (rev1, "rev1 not allocated", v->row);
	for (i = 0; i<v->row; i++) *(rev1 + i) = *(v->head + i);

	h1 = help->head;
	for (ii = 0; ii<counter; ii++)
	{
		h2 = (long**) safeCalloc ("mt_basis", 1, sizeof(long*));
		rev2 = (long*) safeCalloc ("mt_basis", 1, sizeof(long));  
		r2 = 0;
		f1 = 0;
		for (i = 0; i<r1; i++)
		if ((*(*(h1 + i) + ii)) && (!(*(rev1 + i))))
		{
			f1 = 1; break;
		}
		// f1=0;
		if (f1) /* reversible row */
		{
			// r2=0;
			h2 = (long**) safeRealloc ("mt_basis", h2, (r2 + 1)*sizeof(long*));
			{*(h2 + r2) = (long*) safeCalloc ("mt_basis", c, sizeof(long));    
			}
			rev2 = (long*) safeRealloc ("mt_basis", rev2, (r2 + 1)*sizeof(long));
			for (u = 0; u<r1; u++)
			{
				if (*(*(h1 + u) + ii) == 0)   // transfer "zero line"
				{
					*(rev2 + r2) = *(rev1 + u);
					for (uu = 0; uu<c; uu++) *(*(h2 + r2) + uu) = *(*(h1 + u) + uu);
					r2++;
					h2 = (long**) safeRealloc ("mt_basis", h2, (r2 + 1)*sizeof(long*)); 
					{*(h2 + r2) = (long*) safeCalloc ("mt_basis", c, sizeof(long));    
					}
					rev2 = (long*) safeRealloc ("mt_basis", rev2, (r2 + 1)*sizeof(long)); 
					// if(r2>r02) {printf("error1"); getch(); exit(1);} 
				}
			}
			for (u = i + 1; u<r1; u++)
			{
				if ((*(*(h1 + u) + ii)) && (!(*(rev1 + u)))) /* rev rev combinations */
				{
					*(rev2 + r2) = 0; k1 = *(*(h1 + i) + ii); k2 = *(*(h1 + u) + ii);
					abs_hi = (*(*(h1 + i) + ii) >= 0 ? *(*(h1 + i) + ii) : -*(*(h1 + i) + ii)); // FM
					abs_hu = (*(*(h1 + u) + ii) >= 0 ? *(*(h1 + u) + ii) : -*(*(h1 + u) + ii)); // FM
					ggt_Erg = mt_ggt (abs_hi, abs_hu);  // FM
					k1 /= ggt_Erg; k2 /= ggt_Erg;    // FM
					for (uu = 0; uu<c; uu++)
					{
						mt_numerical_array (k2, *(*(h1 + i) + uu), k1, *(*(h1 + u) + uu), 0, 1, -1, "loop 1");
						*(*(h2 + r2) + uu) = k2**(*(h1 + i) + uu) - k1**(*(h1 + u) + uu);
					}
					r2++;
					h2 = (long**) safeRealloc ("mt_basis", h2, (r2 + 1)*sizeof(long*)); 
					{*(h2 + r2) = (long*) safeCalloc ("mt_basis", c, sizeof(long));    
					}
					rev2 = (long*) safeRealloc ("mt_basis", rev2, (r2 + 1)*sizeof(long)); 
					// if(r2>r02) {printf("error2"); getch(); exit(1);} 
				}
			}
			for (u = 0; u<r1; u++)
			{
				if (*(*(h1 + u) + ii) && (*(rev1 + u))) /* rev irrev combinations */
				{
					*(rev2 + r2) = 1; k1 = *(*(h1 + i) + ii); k2 = *(*(h1 + u) + ii);
					abs_hi = (*(*(h1 + i) + ii) >= 0 ? *(*(h1 + i) + ii) : -*(*(h1 + i) + ii)); // FM
					abs_hu = (*(*(h1 + u) + ii) >= 0 ? *(*(h1 + u) + ii) : -*(*(h1 + u) + ii)); // FM
					ggt_Erg = mt_ggt (abs_hi, abs_hu);  // FM
					k1 /= ggt_Erg; k2 /= ggt_Erg;    // FM
					if (k1*k2<0)
					for (uu = 0; uu<c; uu++)
					{
						mt_numerical_array (k2, *(*(h1 + i) + uu), k1, *(*(h1 + u) + uu), 1, 1, 1, "loop 2");
						*(*(h2 + r2) + uu) = abs (k2)* *(*(h1 + i) + uu) + abs (k1)* *(*(h1 + u) + uu);
					}
					if (k1*k2>0)
					for (uu = 0; uu<c; uu++)
					{
						mt_numerical_array (k2, *(*(h1 + i) + uu), k1, *(*(h1 + u) + uu), 1, -1, 1, "loop 3");
						*(*(h2 + r2) + uu) = -abs (k2)**(*(h1 + i) + uu) + abs (k1)**(*(h1 + u) + uu);
					}
					r2++;
					h2 = (long**) safeRealloc ("mt_basis", h2, (r2 + 1)*sizeof(long*)); 
					{*(h2 + r2) = (long*) safeCalloc ("mt_basis", c, sizeof(long));    
					}
					rev2 = (long*) safeRealloc ("mt_basis", rev2, (r2 + 1)*sizeof(long));
					// if(r2>r02) {printf("error3"); getch(); exit(1); } 
				}
			}
		}
		else /* no reversible row */
		{
			h2 = (long**) safeRealloc ("mt_basis", h2, (r2 + 1)*sizeof(long*)); 
			{*(h2 + r2) = (long*) safeCalloc ("mt_basis", c, sizeof(long));
			}
			rev2 = (long*) safeRealloc ("mt_basis", rev2, (r2 + 1)*sizeof(long)); 

			// r2=0;
			for (u = 0; u<r1; u++)
			{
				if (*(*(h1 + u) + ii) == 0)  // transfer "zero line
				{
					*(rev2 + r2) = *(rev1 + u);
					for (uu = 0; uu<c; uu++) *(*(h2 + r2) + uu) = *(*(h1 + u) + uu);
					r2++;
					h2 = (long**) safeRealloc ("mt_basis", h2, (r2 + 1)*sizeof(long*)); 
					{*(h2 + r2) = (long*) safeCalloc ("mt_basis", c, sizeof(long));    
					}
					rev2 = (long*) safeRealloc ("mt_basis", rev2, (r2 + 1)*sizeof(long)); 
					// if(r2>r02) {printf("error4"); getch(); exit(1); }
				}
			}
			for (i = 0; i<r1; i++)
			{
				if (*(*(h1 + i) + ii)>0)
				{
					for (u = 0; u<r1; u++)
					{
						if (*(*(h1 + u) + ii)<0)
						{
							*(rev2 + r2) = 1; k1 = *(*(h1 + i) + ii); k2 = *(*(h1 + u) + ii);
							abs_hi = (*(*(h1 + i) + ii) >= 0 ? *(*(h1 + i) + ii) : -*(*(h1 + i) + ii)); // FM
							abs_hu = (*(*(h1 + u) + ii) >= 0 ? *(*(h1 + u) + ii) : -*(*(h1 + u) + ii)); // FM
							ggt_Erg = mt_ggt (abs_hi, abs_hu);  // FM
							k1 /= ggt_Erg; k2 /= ggt_Erg;    // FM
							for (uu = 0; uu<c; uu++)
							{
								mt_numerical_array (k2, *(*(h1 + i) + uu), k1, *(*(h1 + u) + uu), 1, 1, 1, "loop 4");
								*(*(h2 + r2) + uu) = abs (k2)**(*(h1 + i) + uu) + abs (k1)**(*(h1 + u) + uu);
							}
							test1 = 1;
							for (tu = 0; tu<r2; tu++)
							{
								test1 = 0;
								for (uu = counter; uu<c; uu++)
								{
									if ((*(*(h2 + r2) + uu) == 0) && (*(*(h2 + tu) + uu) != 0)) { test1 = 1; break; }
								}
								if (test1 == 0) break;
							}
							if (test1 == 1)
							{
								r2++;
								h2 = (long**) safeRealloc ("mt_basis", h2, (r2 + 1)*sizeof(long*)); 
								{*(h2 + r2) = (long*)calloc (c, sizeof(long));   
								}
								rev2 = (long*) safeRealloc ("mt_basis", rev2, (r2 + 1)*sizeof(long)); 
								// if(r2>r02) {printf("error5"); getch(); exit(1);} 
							}
						} // for u
					}
				} // if
			} // for i
		} // else
		// printf("release %d\n", r1-1);
		for (i = 0; i<r1; i++) free (*(h1 + i)); free (h1); free (rev1);
		h1 = h2; rev1 = rev2; r1 = r2; //* cf. modes !!!! -> */ r2=r02;
		help->head = h1; help->row = r1; help->col = c;
		for (i = r1; i <= r2; i++)
			free (*(help->head + i));
		r2 = r1;
		mt_ggt_matrix (help);

		ostringstream msg;
		msg << "result convex tab " << ii << " row " << r1;
		throw std::runtime_error (msg.str());

		// free( *(h2+r2));
		if (!help->row) break;
	}
	free (rev2);   // otherwise memory leak
	help->head = h2; help->row = r2; help->col = c;

	//for(i=r2; i>r1; i--) free(help->head+i); 
	if (help->row)
	{
		k = mt_cutcol (help, counter);
		mt_freeMatrix (help);
		help = mt_simplify (k); mt_freeMatrix (k);
	}
	mt_switch_if_all_rows_are_minus_into_plus (help);
	return help;
} // basis


/* modes */
struct mt_mat *mt_modes (struct mt_mat *m, struct mt_vector *v)
{
	struct mt_mat *help, *k;                       /* help matrices */
	long **h1, **h2, abs_hi, abs_hu, ggt_Erg;    /* pointer to current and nex tab */
	long *rev1, *rev2 = NULL;                           /* corresponding revesibilities */
	int i, u, ii, uu, tu;                           /* counter */
	int r1, r2, c, counter, k1, k2, test1, r01, ok_4 = 1, r02;

	k = mt_transpose (m); help = mt_addi (k); mt_freeMatrix (k);
	h1 = help->head;
	r1 = (help->row); c = help->col; counter = m->row;
	r01 = r2 = r1;
	rev1 = (long*) safeCalloc ("1: mt_modes", v->row, sizeof(long));   
	for (i = 0; i<v->row; i++) *(rev1 + i) = *(v->head + i);
	//HMS printf ("first alloc %d\n", r1);
	for (ii = 0; ii<counter; ii++)
	{
		//HMS printf ("allocate ");
		rev2 = (long*) safeCalloc ("2: mt_modes", 1, sizeof(long));   
		h2 = (long**) safeCalloc ("3: mt_modes", 1, sizeof(long*));   
		r2 = 0;
		for (i = 0; i<r1; i++)
		if (!(*(*(h1 + i) + ii))) /* taking zero rows to the nex tab */
		{
			{ h2 = (long**) safeRealloc ("4: mt_modes", h2, (r2 + 1)*sizeof(long*)); }   
			{
			*(h2 + r2) = (long*) safeCalloc ("5: mt_modes", c, sizeof(long));       
		}
			rev2 = (long*) safeRealloc ("6: mt_modes", rev2, (r2 + 1)*sizeof(long));  
			*(rev2 + r2) = *(rev1 + i);
			for (uu = 0; uu<c; uu++) *(*(h2 + r2) + uu) = *(*(h1 + i) + uu);
			++r2;
			//HMS printf ("%d ", ++r2);
		}
		//else;
		r02 = r2;
		// ok_4=1;
		for (i = 0; i<r1; i++)
		if ((*(*(h1 + i) + ii)) && (!(*(rev1 + i)))) /* reversible combinations */
		{
			for (u = i + 1; u<r1; u++)
			if ((*(*(h1 + u) + ii)) && (!(*(rev1 + u))))
			{
				/* if(ok_4) */ while (r02 <= r2) /* 1 */
				{
					h2 = (long**) safeRealloc ("7: mt_modes", h2, (r2 + 1)*sizeof(long*)); 
					*(h2 + r2) = (long*) safeCalloc ("8: mt_modes", c, sizeof(long));   
					rev2 = (long*)safeRealloc ("9; mt_modes", rev2, (r2 + 1) * sizeof (long));
					r02++;
				}
				*(rev2 + r2) = 0; k1 = *(*(h1 + i) + ii); k2 = *(*(h1 + u) + ii);
				abs_hi = (*(*(h1 + i) + ii) >= 0 ? *(*(h1 + i) + ii) : -*(*(h1 + i) + ii)); // FM
				abs_hu = (*(*(h1 + u) + ii) >= 0 ? *(*(h1 + u) + ii) : -*(*(h1 + u) + ii)); // FM
				ggt_Erg = mt_ggt (abs_hi, abs_hu);  // FM
				k1 /= ggt_Erg; k2 /= ggt_Erg;    // FM
				for (uu = 0; uu<c; uu++)
				{
					mt_numerical_array (k2, *(*(h1 + i) + uu), k1, *(*(h1 + u) + uu), 0, 1, -1, "loop 8");
					*(*(h2 + r2) + uu) = k2**(*(h1 + i) + uu) - k1**(*(h1 + u) + uu);
				}

				test1 = 1;
				for (tu = 0; tu<r2; tu++)
				{
					test1 = 0;
					for (uu = counter; uu<c; uu++)
					{
						if ((*(*(h2 + r2) + uu) == 0) && (*(*(h2 + tu) + uu) != 0)) { test1 = 1; break; }
					}
					if (test1 == 0) break; // wenn alle r2==0 auch an der Position von tu zu finden sind, dann schmeiß die neue r2 raus
				}
				if (test1 == 1){
					// control_condition007( h2, &r2, rev2, c );
					++r2;
					//printf ("%d ", ++r2); /* 1 */ // die neue NullMenge ist keine Schnittmenge der Nullen zweier bisheriger Zeilen
					// ok_4=1;
				}
				//else;//ok_4=0;
			} // for u
		}  // for i

		for (i = 0; i<r1; i++)
		if ((*(*(h1 + i) + ii)) && (!*(rev1 + i))) /* rev-irrev combinations */
		{
			for (u = 0; u<r1; u++)
			if ((*(*(h1 + u) + ii)) && (*(rev1 + u)))
			{
				/* if( ok_4 ) */ while (r02 <= r2) /* 22 */
				{
					h2 = (long**) safeRealloc ("10: mt_modes", h2, (r2 + 1)*sizeof(long*)); 
					*(h2 + r2) = (long*) safeCalloc ("11: mt_modes", c, sizeof(long));    
					rev2 = (long*) safeRealloc ("12: mt_modes", rev2, (r2 + 1)*sizeof(long));   
					r02++;
				}
				*(rev2 + r2) = 1; k1 = (*(*(h1 + i) + ii)); k2 = (*(*(h1 + u) + ii));
				abs_hi = (*(*(h1 + i) + ii) >= 0 ? *(*(h1 + i) + ii) : -*(*(h1 + i) + ii)); // FM
				abs_hu = (*(*(h1 + u) + ii) >= 0 ? *(*(h1 + u) + ii) : -*(*(h1 + u) + ii)); // FM
				ggt_Erg = mt_ggt (abs_hi, abs_hu);  // FM
				k1 /= ggt_Erg; k2 /= ggt_Erg;    // FM
				if (k1*k2<0)
				for (uu = 0; uu<c; uu++)
				{
					mt_numerical_array (k2, *(*(h1 + i) + uu), k1, *(*(h1 + u) + uu), 1, 1, 1, "loop 5");
					*(*(h2 + r2) + uu) = abs (k2)**(*(h1 + i) + uu) + abs (k1)**(*(h1 + u) + uu);
				}
				if (k1*k2>0) // ooo
				for (uu = 0; uu<c; uu++)
				{
					mt_numerical_array (k2, *(*(h1 + i) + uu), k1, *(*(h1 + u) + uu), 1, -1, 1, "loop 6");
					*(*(h2 + r2) + uu) = -abs (k2)**(*(h1 + i) + uu) + abs (k1)**(*(h1 + u) + uu);
				}
				test1 = 1;
				for (tu = 0; tu<r2; tu++)
				{
					test1 = 0;
					for (uu = counter; uu<c; uu++)
					{
						if ((*(*(h2 + r2) + uu) == 0) && (*(*(h2 + tu) + uu) != 0)) { test1 = 1; break; }
					}
					if (test1 == 0) break;
				}
				if (test1 == 1){
					// control_condition007( h2, &r2, rev2, c );
					++r2;
					//HMS printf ("%d ", ++r2); /* 2 */
					// ok_4=1;
				}
				//else; // ok_4=0;
			} // for u
		} // for i
		for (i = 0; i<r1; i++)
		if ((*(*(h1 + i) + ii)>0) && (*(rev1 + i))) /* irrev combinations */
		{
			for (u = 0; u<r1; u++)
			if ((*(*(h1 + u) + ii)<0) && (*(rev1 + u)))
			{
				/* if(ok_4) */ while (r02 <= r2) /* 333 */
				{
					h2 = (long**) safeRealloc ("13: mt_modes", h2, (r2 + 1) * sizeof (long*));
					*(h2 + r2) = (long*) safeCalloc ("14: mt_modes", c, sizeof (long));
					rev2 = (long*) safeRealloc ("15: mt_modes", rev2, (r2 + 1) * sizeof (long));
					r02++;
				}
				*(rev2 + r2) = 1; k1 = abs (*(*(h1 + i) + ii)); k2 = abs (*(*(h1 + u) + ii));
				abs_hi = (*(*(h1 + i) + ii) >= 0 ? *(*(h1 + i) + ii) : -*(*(h1 + i) + ii)); // FM
				abs_hu = (*(*(h1 + u) + ii) >= 0 ? *(*(h1 + u) + ii) : -*(*(h1 + u) + ii)); // FM
				ggt_Erg = mt_ggt (abs_hi, abs_hu);  // FM
				k1 /= ggt_Erg; k2 /= ggt_Erg;    // FM
				for (uu = 0; uu<c; uu++)
				{
					mt_numerical_array (k2, *(*(h1 + i) + uu), k1, *(*(h1 + u) + uu), 0, 1, 1, "loop 7");
					*(*(h2 + r2) + uu) = k2**(*(h1 + i) + uu) + k1**(*(h1 + u) + uu);
				}
				test1 = 1;
				for (tu = 0; tu<r2; tu++)
				{
					test1 = 0;
					for (uu = counter; uu<c; uu++)
					{
						if ((*(*(h2 + r2) + uu) == 0) && (*(*(h2 + tu) + uu) != 0)) { test1 = 1; break; }
					}
					if (test1 == 0) break;
				}
				if (test1 == 1){
					// control_condition007( h2, &r2, rev2, c );
					++r2;
					//HMS printf ("%d ", ++r2); /* 3 */
					// ok_4=1;
				}
				//else; // ok_4=0;
			} // for u
		} // for i
		//HMS printf("release %d\t", r01-1);
		for (i = 0; i<r01; i++) free (*(h1 + i)); free (h1); free (rev1);
		h1 = h2; rev1 = rev2; r1 =/*r2/ */(r2 <= r02 ? r2 : r02); r01 = r02; // r2; // r02
		//HMS printf ("\nresult modes tab %d row %d\n", ii, r1);
		help->head = h1;   help->row = r1; help->col = c;
		// control_condition04(help, ii); // for debugging
		// control_condition7( help, ii, rev1, &r1 ); // r1=help->row; /* FM 20.09.2000 condition (7) in www2.bioinf.mdc-berlin.de/metabolic/metatool/algorithm.pdf */
		r2 = r1;
		mt_ggt_matrix (help);
		// print_mat(help);
		if (!help->row) break;
	} // for ii
	mt_control_condition7 (help, 0, rev1, &r1); // function moved into this source code line on 01.10.2002
	free (rev2); // otherwise memory leak FM (detected using the unix software purify)
	while (r02>r1)
	{
		free (*(h1 + (r02 - 1))); r02--;
	}
	help->head = h1; help->row = r1; help->col = c;
	mt_ggt_matrix (help);
	if (help->row)
	{
		k = mt_cutcol (help, counter);
		mt_freeMatrix (help);
		help = mt_simplify (k);
		mt_freeMatrix (k);
		// print_mat(help);
	}
	mt_switch_if_all_rows_are_minus_into_plus (help);
	return help;
} // modes



int mt_overallOutput (struct mt_mat *m, struct mt_encoding *list, FILE *savefile)
{
	int i, t, j, zero_count, zero_lines = 0;
	long **mat;
	struct mt_encoding *acel;

	mat = m->head;

	fprintf (savefile, "\n overall reaction\n");
	// count zerolines
	for (i = 0; i<m->row; i++)
	{
		for (zero_count = j = 0; j<m->col; j++)
		{
			if (!(*(*(m->head + i) + j))) zero_count++;
		}
		if (zero_count == m->col)
		{
			zero_lines++;
		}
	}
	if (m->row == zero_lines)
		fprintf (savefile, " - not found -\n");
	else
	for (i = 0; i<m->row; i++)
	{
		for (zero_count = j = 0; j<m->col; j++)
		{
			if (!(*(*(m->head + i) + j))) zero_count++;
		}
		if (zero_count == m->col)
		{/* skip zerolines */ zero_lines++;    fprintf (savefile, "\n %d:\tno net transfomation of external metabolites", i + 1); continue;
		}
		fprintf (savefile, "\n %d:\t", i + 1/*-zero_lines*/);
		acel = list->next;
		t = 0;
		while (acel != acel->next)
		{
			if (*(*(mat + i) + (acel->key - 1))<0)
			{
				if (t == 1) fprintf (savefile, " + ");
				if (*(*(mat + i) + (acel->key - 1)) == -1) fprintf (savefile, "%s", acel->txt);
				else fprintf (savefile, "%d %s", -*(*(mat + i) + (acel->key - 1)), acel->txt);
				t = 1;
			}
			acel = acel->next;
		};
		fprintf (savefile, " = ");
		t = 0;
		acel = list->next;
		while (acel != acel->next)
		{
			if (*(*(mat + i) + (acel->key - 1))>0)
			{
				if (t == 1) fprintf (savefile, " + ");
				if (*(*(mat + i) + (acel->key - 1)) == 1) fprintf (savefile, "%s", acel->txt);
				else fprintf (savefile, "%d %s", *(*(mat + i) + (acel->key - 1)), acel->txt);
				t = 1;
			}
			acel = acel->next;
		};
	}
	fprintf (savefile, "\n"); fflush (savefile);
	return 0;
} // foveralloutput


// ------------------------------------------------------------------------
// Main Exported Analysis API

struct mt_mat *mt_elementaryModes () {
	struct mt_mat  *nred; 
	struct mt_mat  *vsub;	  /* subsets */
	struct mt_mat  *vkernel;  /* subsets */
	struct mt_vector  *redrev;
	struct mt_mat   *reducedModes;
	struct mt_mat   *modes;
	struct mt_mat  *help;
	int wrong_subset;

	// This code is taken from the original metatool4_3 
	vkernel = mt_kernel (mt_internalDataStructure->stoichiometryMatrix);
	vsub = mt_subset (vkernel, mt_internalDataStructure->reversiblilityList, &wrong_subset);
	nred = mt_transpose (vsub);
	help = mt_mult (mt_internalDataStructure->stoichiometryMatrix, nred);
	mt_freeMatrix (nred);
	nred = mt_simplify (help);
	redrev = mt_subrev (vsub, mt_internalDataStructure->reversiblilityList);

	reducedModes = mt_modes (nred, redrev);
	modes = mt_mult (reducedModes, vsub);

	mt_freeMatrix (vkernel);
	mt_freeMatrix (vsub);
	mt_freeMatrix (nred);
	mt_freeMatrix (help);
	mt_freeVector (redrev);

	return modes;
}


struct mt_encoding *mt_addMetabolites (mt_stringArray *floating, mt_stringArray *boundary) {
	return mt_addMetabolitesInternal (floating, boundary);
}

void mt_loadSpeciesStatus (struct mt_vector *speciesStatus) {
	lib_speciesStatus = speciesStatus;
}

struct mt_mat *mt_loadFullStoichiometryMatrix (struct mt_mat *fullStoichiometryMatrix) {
	return getStoichiometryMatrix (fullStoichiometryMatrix, lib_speciesStatus);
}

void mt_addReversibilityInformation (struct mt_vector *reversibilityInformation) {
	lib_reversibilityInformation = reversibilityInformation;
}