/* METATOOL 4.3 // 25 October 2002 */
// this version is able to read stoichiometric coefficients in the two forms:
// e.g. 2/3 or 0.333 
// and as always 3.0 or 3
// and /* and */ or // can be employed to set parts of the metatool-input file in comments

/* ************************************************************************************ */
/* METATOOL: TO ANALYZE METABOLIC NETWORKS ******************************************** */
/* WRITTEN BY THOMAS PFEIFFER, extensive worked by F. Moldenhauer ********************* */
/* COMPILE WITH GCC, Microsoft C 6.0 or Borland C 5.0********************************** */
/* 5/2000 ***************************************************************************** */

/* ######################## */
/* fix by HMS to generate unreduced elemenary modes. */
/* Changed lines marked by HMS######### */
/* 26.06.2000, change in the elementary modes matrix section, */
/* gives out the unreduced matrix instead of reduced subset matrix  */ 

/************************************************************/
/* Minor change to run under linux (removed || !defined sun */
/* and removed _getch(); so no input needed under windows ****/
/* Brett Olivier 20040106 (BGOLI###) ************************/
/************************************************************/


/* INCLUDED FILES ********************************************************************* */
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <float.h>
#include <time.h>

#ifdef __APPLE__
#include <unistd.h>
#endif

#ifdef _WIN32
#include <direct.h>
#endif

#ifdef sun
 #include <floatingpoint.h>
#endif

#if (defined DOS || defined _DOS || defined WIN32 || defined _WIN32)
  #include <io.h>
  #include <conio.h>
  #include <malloc.h>
  #include<sys\types.h>  // for time measurment
  #include<sys\timeb.h>
#else 
  #define _getch() getchar()
#endif

#ifndef _MAX_PATH
 #define _MAX_PATH 256
#endif
#ifdef sun
 #define powl pow
 #define floorl floor
 #define ceill ceil
 #define fabsl fabs
#endif

#ifdef sun
  #define long_double double
#else 
  #define long_double double
#endif
using namespace std;
inline void addressed(void *p, char *s, int a )
{
 if(p==NULL)
 {
  /*printf("\nNot enough memory: %s\nProgramm prematurely finished.\n", s );
  _getch();*/ exit(1);
 }
 if(!a)
 {
  /*printf("\namount for allocation is zero : %s\nProgramm prematurely finished.\n", s );
  _getch(); */exit(2);
 }
}

// round value to the number of dezimal positions
long_double rund ( long_double value, int dezimale )
{
 long_double unten, oben, rest;
 return value;  // wegen Dezimalzahleingabe nicht mehr mцglich 19.03.2002
 value *= powl(10, dezimale);
 unten = floorl( value );
 oben  = ceill( value );
 rest = value - unten;

 if( rest == 0.5 )
   // if( ((int)unten%2) ) return(oben/powl(10, dezimale));
   if( floorl(unten) == (floorl(unten/2.0) + floorl(unten/2.0)) ) return(oben/powl(10, dezimale));
   else return( unten/powl(10, dezimale) );
 if( rest > 0.5 ) return ( oben/powl(10, dezimale) );
 if( rest < 0.5 ) return ( unten/powl(10, dezimale) );
 return -1.0;
} // rund

/* MAKROS ***************************************************************************** */
#define TXT 200

/* STRUCT DECLARATIONS **************************************************************** */
struct enc    {int rev; int key; int consumed; int built; int reactions; char ri[1001]; char txt[TXT]; struct enc *next;};  
struct vector {int row; long_double *head;};
struct mat    {int row; int col; long_double **head;};

/* DECLARATION of FUNCTIONS *********************************************************** */
int  compstr (int m, char t1[100], char t2[100]);
long_double ggt (long_double u, long_double v);
void ggt_matrix (struct mat *v);
void read_comment( FILE *fp_in );
int  detect_double_declared_metabolites( struct enc *metlist );
int  control_modi (struct mat *m, FILE *savefile, struct enc *enzlist);
int  control_condition7 (struct mat *m, int ii, long_double *rev1, int *r01);

// global variables that it is possible to scan metabolites and enzymes containing whitespaces
struct vector *branch; /* contains the number of intern metabolites at branch points */
char **metabolites; int* mets_cumul; int met_counter;
char **enzyme_names; int enzyme_counter;
/* FUNCTIONS ************************************************************************** */
/* ************************************************************************************ */
/* 1. for struct enc ****************************************************************** */
/* ************************************************************************************ */
/* 1.1 insert element ************************* */
struct enc *ins (struct enc *t1, int r, int k, char t[TXT])
{
 struct enc *x;
 int i;
  x= (struct enc*) calloc (1, sizeof (struct enc)); addressed( x, "x not allocated", 1);
  x->key=k; x->rev=r;
  for (i=0;i<TXT;i++) x->txt[i]=t[i];
  x->next=t1->next;
  t1->next=x;
 return x;
} // ins

int my_fscanf_enz_names(FILE *s, char* tx);
/* 1.2 function get enzlist ******************* */
struct enc *getenzlist (FILE *s)
{
 struct enc *enzlist, *eac; // , *ez; // FM function is corrected at 15.02.2002
 char tx[TXT];

 enzlist=(struct enc*)calloc(1, sizeof(struct enc )); addressed( enzlist, "enzlist not allocated", 1);
 enzlist->next=(struct enc*)calloc(1, sizeof(struct enc)); addressed(enzlist->next, "enzlist->next not allocated", 1);
 enzlist->key=0;
 enzlist->next->next=enzlist->next; enzlist->next->key=0;
 eac=enzlist;

 read_comment(s);
 fscanf(s,"%s",tx);

  	   if ((tx[0]!='-')&&(tx[1]!='E')) 
        printf ("ERROR IN FILE!");
  	   while (!feof(s))
    	  {
    	  my_fscanf_enz_names(s, tx); // fscanf(s,"%s",tx); 
          if (tx[0]=='-') break;
    	  eac=ins(eac,0,(eac->key)+1,tx);
    	  }	
  	   while (!feof(s))
    	  {
    	  my_fscanf_enz_names(s, tx); // fscanf(s,"%s",tx); 
         if (tx[0]=='-') break;
    	  eac=ins(eac,1,(eac->key)+1,tx);
    	  }
  	   return enzlist;
} // getenzlist

// my own strrev which does not exsist under UNIX
char* strrev (char* tx)
{
  int len, i;
  char help[TXT];
  len=strlen(tx)-1;
  for(i=0; i<=len; i++)
    help[i]=tx[len-i];
  help[i]='\x0';
  for(i=0; i<=len; i++)
    tx[i]=help[i];
  return tx;
} // strrev

void mets_which_differ( struct enc *metlist, int mrow )
{
  int i, j, yn;
  struct enc *help;
  for( i=0; i<met_counter; i++ )
  {
   help = metlist->next; yn=1;
   for( j=0; j<mrow; j++ )
   { 
    if( strcmp(*(metabolites+i), help->txt)) yn=1;
    else {yn=0; break;}
    help=help->next;
   }
   if( yn ) 
   {
     printf("\n->%s<- not found in -METINT -METEXT declarations", *(metabolites+i));
     if     ( (char)**(metabolites+i) == '.' && isdigit((*(metabolites+i))[1])) printf(" (Check the stoichiometrical coefficient. A leading zero must not be left out.)");
     else if( (char)**(metabolites+i) == '.'                                  ) printf(" (Check the stoichiometrical coefficient.)");  // FM 30.07.2002
   }
  }

} // mets_which_differ

void fout_reactions (FILE *fout, struct enc *enzlist)
{
  struct enc *help;
  help = enzlist->next;
  fprintf(fout, "\n");
  do
  {
    fprintf( fout, "%s ", help->txt);
    help=help->next;
  }while( help->next != help);

  fprintf(fout, "\n"); fflush(fout);
} // fout_reactions

void fout_branches (FILE *fout, struct enc *metlist)
{
  int i;
  struct enc *help;
  help = metlist->next;
  fprintf(fout, "\n");
  fprintf(fout, "-> Branch metabolites are : ");
  fprintf(fout, "\n%-20s\tcons\tbuilt\treactions", "met");
  for( i=0; i<branch->row; i++ )
  {
    if( *(branch->head+i) )
      fprintf( fout, "\n%-20s\t%d\t%d\t%d\t%s", help->txt, help->consumed, help->built, help->reactions, help->ri); // fprintf( fout, "%s ", help->txt);
    help=help->next;
  }
  fprintf(fout, "\n");
  help = metlist->next;
  fprintf(fout, "\n-> No branch metabolites are : ");
  fprintf(fout, "\n%-20s\tcons\tbuilt\treactions", "met");
  for( i=0; i<branch->row; i++ )
  {
    if( !*(branch->head+i) )
      fprintf( fout, "\n%-20s\t%d\t%d\t%d\t%s", help->txt, help->consumed, help->built, help->reactions, help->ri); // fprintf( fout, "%s ", help->txt);
    help=help->next;
  }
  fprintf(fout, "\n");
} // fout_branches 

// format formatstring for double numbers
int formatting( char *format, long_double **ptab, int rn, int cn )
{
  #ifdef sun
    char ns[1250]={"11.11"};
    long_double hd=0;
    char *pchar=NULL;
  #endif
  char *number_string=NULL;
  char string[1250]={"222.222"};
  int i, j, max=0;
  long_double num=0;
  int dec=0, sign=0, ndig = 7;

  /* a negative number */
  for( i=0; i<rn; i++ )
  {
    for( j=0; j<cn; j++ )
    {
     if(*((*(ptab+i))+j) == (int)-0.0 ) *((*(ptab+i))+j)=(int)0.0; // for deleting minus sign
     num = (long_double) (*(*(ptab+i)+j));
#ifdef sun
    fconvert(num, 0, &dec, &sign, ns ); // &ns[0] );
    // printf("\nrn%d cn%d num=%lf ns = %s dec = %d sign=%d", rn, cn, num, ns, dec, sign); _getch();
#else
    
     number_string = fcvt((double/*FM 18.09.2000*/)num, ndig, &dec, &sign);
#endif
     max = ( ((dec+sign) > max) ? (dec+sign) : max );
    }
  }
  strcpy( format, "%" );
  // itoa(max, string, 10 ); // itoa is not available at UNIX machines
#ifdef sun
  hd = (int)max;
  pchar = gconvert(hd, 24, 1, string );
  strcpy( string, pchar );
#else
   #ifdef _WIN32
  _gcvt(max, 24, string);
   #else
  gcvt(max, 24, string);
   #endif
#endif

  strcat( format, string );
  // strcat( format, "0lf " ); // No full stop for Microsoft C !!!! Only god knows why !!!
  // printf("Formatstring = %s", format ); // i=getchar();
  format[strlen(format)-1]='\x0';
  return 0;
} // formatting

int longer_name_found(FILE *s, char* static_tx0);
// compare tx-strings with those of *(metabolites+i),
// concatenate the strings while there is an agreement, otherwise exit(1)
int my_fscanf_met_names(FILE *s, char* tx)
{
 long int fposition; int i, trial=1; char tx1[TXT], tx0[TXT]; static char static_tx0[TXT];

 fposition=ftell(s);
 *tx='\x0'; fscanf(s,"%s",tx1);
 strcpy(tx0, tx1);
 do{
   if( strlen(tx)+strlen(tx1)>=TXT && trial==1 ) 
   {fseek(s, fposition, SEEK_SET); strcpy(tx, static_tx0); trial=2; fscanf(s,"%s",tx0);strcat(tx," ");strcat(tx, tx0);}
   else if( strlen(tx)+strlen(tx1)>=TXT && trial==2 ) 
   {/*printf("\nMetabolite -> %s <- not found in the stoichiometric equations!\n\nProgram prematurely terminated.\nPlease press ENTER", tx0); _getch();*/ exit(3);}
   else if(!strlen(tx))
    strcat(tx, tx1);
   else 
    {strcat(tx," "); strcat(tx, tx1);}
   for(i=0; i<met_counter; i++)
   { if(!strcmp(*(metabolites+i), tx)) /* if found */ 
     {strcpy(static_tx0, tx); 
        if(!strcmp(static_tx0, tx0))
          { if(longer_name_found(s, static_tx0)) strcpy(tx, static_tx0); return 0;}
        else 
          { if(longer_name_found(s, static_tx0)) strcpy(tx, static_tx0); return 1;}
     }
   }
   if (tx[0]=='-')
        return 0;
   fscanf(s,"%s",tx1);
 }while(1);
} // my_fscanf_met_names

int longer_name_found(FILE *s, char* static_tx0)
{
 long int fposition; int i, trial=1; char tx1[TXT], tx0[TXT];
 char tx[TXT]={""}; 

 fposition=ftell(s);
 fscanf(s,"%s",tx1);
 strcpy(tx, static_tx0);
 strcpy(tx0, tx1);
 do{
   if( strlen(tx)+strlen(tx1)>=TXT && trial==1 ) 
   {fseek(s, fposition, SEEK_SET); strcpy(tx, static_tx0); trial=2; fscanf(s,"%s",tx0);strcat(tx," ");strcat(tx, tx0);}
   else if( strlen(tx)+strlen(tx1)>=TXT && trial==2 )
   {fseek(s, fposition, SEEK_SET); return 0; /* printf("\nMetabolite -> %s <- not found in the stoichiometric equations!\n\nProgram prematurely finished.\nPlease press ENTER", tx0);_getch();  fclose(s); exit(1);*/}
   else if(!strlen(tx))
    strcat(tx, tx1);
   else 
    {strcat(tx," "); strcat(tx, tx1);}
   for(i=0; i<met_counter; i++)
   { if(!strcmp(*(metabolites+i), tx)) /* if found */ 
     {strcpy(static_tx0, tx);
        if(!strcmp(static_tx0, tx0))
          { return 0; }
        else 
          return 1;
     } else;
   }
   if (tx[0]=='-')
        return 0;
   fscanf(s,"%s",tx1);
 }while(1);
} // longer_name_found

int my_fscanf_enz_names(FILE *s, char* tx)
{
 long int fposition; int i, trial=1; char tx1[TXT], tx0[TXT]; static char static_tx0[TXT];

 fposition=ftell(s);
 *tx='\x0'; fscanf(s,"%s",tx1);
 strcpy(tx0, tx1);
 do{
   if( strlen(tx)+strlen(tx1)>=TXT && trial==1 ) 
   {fseek(s, fposition, SEEK_SET); strcpy(tx, static_tx0); trial=2; fscanf(s,"%s",tx0);strcat(tx," ");strcat(tx, tx0);}
   else if( strlen(tx)+strlen(tx1)>=TXT && trial==2 )
   {/*printf("\nEnzyme name ->%s<- not found in the stoichiometric equations!\n\nProgram prematurely finished.\nPlease press ENTER", tx0); _getch(); */exit(4);}
   else if(!strlen(tx))
    strcat(tx, tx1);
   else 
    {strcat(tx," "); strcat(tx, tx1);}
   for(i=0; i<enzyme_counter; i++)
   { if(!strcmp(*(enzyme_names+i), tx)) /* if found */ 
     {strcpy(static_tx0, tx); return 0;} else;}
   if (tx[0]=='-') return 0;
   fscanf(s,"%s",tx1);
 }while(1);
} // my_fscanf_enz_names

/* 1.3 function get metlist ******************* */
struct enc *getmetlist (FILE *s)
{
 struct enc *metlist, *eac, *mz;
 char tx[TXT];

 metlist=(struct enc*)calloc(1, sizeof( struct enc )); addressed( metlist, "metlist not allocated", 1);
 mz=     (struct enc*)calloc(1, sizeof( struct enc )); addressed( mz,      "mz not allocated", 1     );
      
 metlist->next=mz; metlist->key=0;
 mz->next=mz; mz->key=0; eac=metlist;
 while (!feof(s))
 {
  my_fscanf_met_names(s, tx);  //fscanf(s,"%s",tx);
  if (tx[0]=='-') break;
  eac=ins(eac,0,(eac->key)+1,tx);
  eac->rev=0; // notice metabolite as intern
 }
 while (!feof(s))
 {
  my_fscanf_met_names(s, tx);// fscanf(s,"%s",tx);
  if (tx[0]=='-') break;
  eac=ins(eac,1,(eac->key)+1,tx);
  eac->rev=1;
 }
 detect_double_declared_metabolites( metlist );
 return metlist;
} // getmetlist

/* ************************************************************************************ */
/* 2. functions for struct vector and struct mat *********************************** */
/* ************************************************************************************ */
/* 2.1 output ********************************* */
int vectoroutput (struct vector *v)
{
 int i;
 printf ("\n");
#ifndef sun
 for (i=0; i<v->row; i++) printf ("%3lg",*(v->head+i));
#else 
 for (i=0; i<v->row; i++) printf ("%3lg",*(v->head+i));
#endif
 // printf ("\n"); 
 return 0;
} // vectoroutput 

void print_mat(struct mat *m)
{
 int i, j;
 printf("\n");
 for( i=0; i<m->row; i++)
 {  for( j=0; j<m->col; j++)
    #ifdef sun
     printf("%2g", *(*((m->head)+i)+j));
    #else
     printf("%2lg", *(*((m->head)+i)+j));
    #endif
   printf("\n");
 }
} //print_mat

int null_mat(struct mat *m)
{
 int i, j, k=0;
 for( i=0; i<m->row; i++)
  for( j=0; j<m->col; j++)
   if( *(*((m->head)+i)+j))k++;
 return k;
}//null_mat

int save_frequency_of_metabolites(char *f1, char *f2, struct enc *metlist)
{
   FILE *fp_out; int i, j, k, l, max, summe=0;
   char fname[500];
   struct enc mlist;

   strcpy(fname, f2);
   i=strlen(fname); 
   do{ --i; }
     while(fname[i]!='\\' && fname[i]!='/' && i);
   strcpy(&fname[i], "\\mets.txt");
   fp_out = fopen(fname, "w");
   if(!fp_out) {/*printf("fp_out ->%s<- ", fname); perror(""); _getch(); */exit(5);}
   
   fprintf(fp_out, "%s\n", f1);
   for(j=0; j<met_counter; j++ )
   {
     for(max=i=0; i<met_counter; i++ ) 
       if(*(mets_cumul+i) > max) { max=*(mets_cumul+i); k=i;}
     fprintf(fp_out, "%3d ", *(mets_cumul+k));
     for(mlist=*metlist->next, l=0; l<met_counter; l++)
     { // give intern or extern
       if(!strcmp(mlist.txt, *(metabolites+k)))
       {
        if(mlist.rev)
         {fprintf(fp_out, "external ");}
        else
         {fprintf(fp_out, "int      ");}
       }
       mlist=*mlist.next;
     }
     fprintf(fp_out, "%s\n", *(metabolites+k)); // the name of metabolites
     summe+=*(mets_cumul+k);
     *(mets_cumul+k)=0;
   }
   fprintf(fp_out, "%d x %d\n", met_counter, summe);
   fprintf(fp_out, "\n%d enzymes :\n", enzyme_counter);
   for(j=0; j<enzyme_counter; j++ )
     fprintf(fp_out, "%s\n", *(enzyme_names+j));
   fclose(fp_out);
   return 0;
} // save_frequency_of_metabolites

int save_frequency_of_edges_and_nodes(FILE *fout, int *edges_nodes, int edges);
int save_frequency_of_metabolites_in_output(FILE *fout, struct enc *metlist)
{
 int i, j, k, l, max, max0, summe=0;
 int *edges_nodes;
 struct enc mlist;

 for(max0=i=0; i<met_counter; i++ ) 
   if(*(mets_cumul+i) > max0) { max0=*(mets_cumul+i);}
 edges_nodes = (int*) calloc((max0+1), sizeof(int)); addressed(edges_nodes, "edges_nodes not allocated.", max0+1);

 fprintf(fout, "\n");
 for(j=0; j<met_counter; j++ )
 {
  for(max=i=0; i<met_counter; i++ ) 
    if(*(mets_cumul+i) > max) { max=*(mets_cumul+i); k=i;}
  // count frequency of edges
  ( *( edges_nodes + *(mets_cumul+k)) )++;
  fprintf(fout, "%3d ", *(mets_cumul+k));
  for(mlist=*metlist->next, l=0; l<met_counter; l++)
  { // give intern or extern
    if(!strcmp(mlist.txt, *(metabolites+k)))
    {
     if(mlist.rev)
       {fprintf(fout, "external ");}
     else
       {fprintf(fout, "int      ");}
    }
    mlist=*mlist.next;
  }
  fprintf(fout, "%s\n", *(metabolites+k)); // the name of metabolites
  summe+=*(mets_cumul+k);
  *(mets_cumul+k)=0;
 }
 fprintf(fout, "%3d metabolites, %3d is the summarized frequency\n", met_counter, summe);
 save_frequency_of_edges_and_nodes(fout, edges_nodes, max0 );
 free(edges_nodes);
 return 0;
} // save_frequency_of_metabolites_in_output


// Calculation Correlation coefficient ##############################################
// Sinificancelevels for Correlationscoefficients r
// 			(FG = n-2 )
// Pareys Studientexte Tab. 23, S.170
double Table_r_2sides[][4] = {

       {  5,  .754, .875, .951,},
       {  6,  .707, .834, .925,},
       {  7,  .666, .798, .898,},
       {  8,  .632, .765, .872,},
       {  9,  .602, .735, .847,},
       {  10, .576, .708, .823,},
   
       {  11, .553, .684, .801,},
       {  12, .532, .661, .780,},
       {  13, .514, .641, .760,},
       {  14, .497, .623, .742,},
       {  15, .482, .606, .725,},
       {  16, .468, .590, .708,},
       {  17, .456, .575, .693,},
       {  18, .444, .561, .679,},
       {  19, .433, .549, .665,},
       {  20, .423, .537, .652,},
				
       {  21, .413, .526, .640,},
       {  22, .404, .515, .629,},
       {  23, .396, .505, .618,},
       {  24, .388, .496, .607,},
       {  25, .381, .487, .597,},
       {  26, .374, .478, .588,},
       {  27, .367, .470, .579,},
       {  28, .361, .463, .570,},
       {  29, .355, .456, .562,},
       {  30, .349, .449, .554,},
				
       {  35, .325, .418, .519,},
       {  40, .304, .393, .490,},
       {  50, .273, .354, .443,},
       {  60, .250, .325, .408,},
       {  70, .232, .302, .380,},
       {  80, .217, .283, .357,},
       {  90, .205, .267, .338,},
       { 100, .195, .254, .321,},
       { 120, .178, .232, .294,},
				
       { 150, .159, .208, .263,},
       { 200, .138, .181, .230,},
       { 250, .124, .162, .206,},
       { 300, .113, .148, .188,},
       { 350, .105, .137, .175,},
       { 400, .098, .128, .164,},
       { 500, .088, .115, .146,},
       { 700, .074, .097, .124,},
       {1000, .062, .081, .104,},
       {1500, .050, .066, .085,},
       {2000, .044, .058, .073 }
};
#define  ZWEISEITIG		2
#define  _2komma5_Prozent	2.5
#define  _5_Prozent		5


//ЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫЫ
//   class   class  class   class  class   class  class   class  class
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

class Variance {  // calculation of the statistical variance of a feature
  public:
   double *x, *y;
   int n;
   double Sx();
   double Sy();
   double Sxx();
   double Syy();
   double Sxy();
   double SYY();
   Variance( double *a, double *b, int c=1 ){ x=a; y=b; n=c; }
   Variance( double *a, int c ){ x=a; n=c; }
}; // class Variance

class Linear_Correlation : public Variance{
  public:
    double Linear_Correlationskoeff_r, BestimmtheitsMass_B; // Bestimmtheit=coefficient of determination
    Linear_Correlation( double *a, double *b, int c, double r=-2 );
    double r();
    int   reference_r_2sides();
};

Linear_Correlation::Linear_Correlation( double *a, double *b, int c, double r ):Variance(a, b, c )
{ Linear_Correlationskoeff_r=r; };

// F U N C T I O N D E F I N I T I O N S #############################################
#include<iostream>
#include<float.h>
int Linear_Correlation::reference_r_2sides()
{
  int i, j=0, signf=0;
  extern double Table_r_2sides[46][4];

  for(i=0; i<46; i++ )
    if ( Table_r_2sides[i][0] >= n-2  ) break;
  cout << "\n" << "FG\t5%\t1%\t0,1%\n" << endl;
  for ( j=0; j<4; j++ )
  {
    cout << Table_r_2sides[i][j] << "\t";
    if( fabs(Linear_Correlationskoeff_r) > Table_r_2sides[i][j] ) signf++;
  }
  cout << "\n" << endl;
  switch(signf)
  {
    case 1:  cout << "The dependency is significant (p<5%)."   << endl; break;
    case 2:  cout << "The dependency is significant (p<1%)."   << endl; break;
    case 3:  cout << "The dependency is significant (p<0.1%)." << endl; break;
    default: cout << "The dependency is not significant. (p>5%)"       << endl;
  }
  return signf;
};

double Linear_Correlation::r(){
   double quotient = Sxx() * Syy();
   if( quotient )
   {
     Linear_Correlationskoeff_r = Sxy() / sqrt( quotient );
     BestimmtheitsMass_B     = pow (Sxy(),2.0) / quotient;
     return( Linear_Correlationskoeff_r );
   }
   else
   { BestimmtheitsMass_B = -2 ; return( -2 ); }
};

double Variance::Sxx()
{
    // sum xэ - ( (sum x)э/n )
    int i=0; double s_xq=0, sx=0;
    while( i<n ){
      s_xq += x[i] * x[i];
      sx   += x[i];
      i++;
    };
    if( n )
      return( (double)(s_xq - ((sx*sx)/(double)n)));
    else
      return(FLT_MAX);
}
double Variance::Sx()
{
    // sum x
    int i=0;
    double Sx=0; 
    while( i<n ){
      Sx += x[i++];
    };
    return( Sx );
}
double Variance::Sy()
{
    // sum y
    int i=0;
    double Sy=0; 
    while( i<n ){
      Sy += y[i++];
    };
    return( Sy );
}
double Variance::Syy()
{
    // sum yэ - ( (sum y)э/n )
    int i=0; double s_yq=0, sy_q=0;
    while( i<n ){
      s_yq += y[i] * y[i];
      sy_q += y[i];
      i++;
    };
    return( s_yq - ( (sy_q*sy_q)/double(n)));
};
double Variance::Sxy()
{
    // sum x*y - ( (sum x * sum y)/n )
    int i=0; double s_xy=0, s_x=0, s_y=0;
    while( i<n ){
      s_xy += x[i] * y[i];
      s_x  += x[i];
      s_y  += y[i];
      i++;
    };
    return( s_xy - ( (s_x * s_y) / (double)n));
};
double Variance::SYY()
{
    double s_xy=0, sxx, syy;
    sxx = Sxx();   syy = Syy();   s_xy = Sxy();
    if( sxx )
     return( syy -  (s_xy * s_xy) / sxx );
    else
     return( FLT_MAX );
};
// Correlation end ##########################################################################



class Multiple_Linear_Regression
{
  public:
  int matinv (double  *matptr,int dim, int cols);
  int mlr    (double *x, int Xrows, int Xcols, double *y, int bkonst, double *b);
  int matmult(double *matptr1, double *matptr2, double *result_ptr,
	          int rows1, int cols1, int cols2);
};

int Multiple_Linear_Regression::matinv(double  *matptr,int dim, int cols)
{	int  *ix,  *iy,  *iq;
	double max, d, t;
	int i, j, k, l, is, iz, correct=1;

  ix=(int  *)calloc(dim+1, sizeof(int)); addressed(ix, "ix not allocated", dim+1);
  iy=(int  *)calloc(dim+1, sizeof(int)); addressed(iy, "iy not allocated", dim+1);
  iq=(int  *)calloc(dim+1, sizeof(int)); addressed(iq, "iq not allocated", dim+1);


  for(j=0;j<dim;j++)                   /* Ermittlung Index gr”бtes Element */
  { max=0;
    if(j==10)
	   printf(" ");
    for(i=0;i<dim;i++)
	 {  if(*(iq+i) != 1)
		for(k=0;k<dim;k++)
			if((*(iq+k) != 1)&&(max <= fabs(*(matptr+i*cols+k))))
			 {  is=k;   iz=i;    max = fabs(*(matptr+i*cols+k));  }
    }

    *(iq+is) = *(iq+is) + 1;

    if(iz != is)                         /*  PIVOTIERUNG   */
	  for(i=0;i<dim;i++)
	    {                   t = *(matptr+iz*cols+i);
		  *(matptr+iz*cols+i) = *(matptr+is*cols+i);
            *(matptr+is*cols+i) = t;    }


    *(ix+j) = iz;    *(iy+j) = is;

    d  = *(matptr+is*cols+is);     *(matptr+is*cols+is) = 1;
    for(i=0;i<dim;i++)     *(matptr+is*cols+i) /= d;

    /*  reduction of all of the other rows */
    for(l=0;l<dim;l++)
    { if(l != is)    {  t = *(matptr+l*cols+is);  *(matptr+l*cols+is) = 0;
				   for(i=0;i<dim;i++)
					 *(matptr+l*cols+i) -= *(matptr+is*cols+i) * t;
    }   		      }
 }     /***********  END    J - LOOP   *************/

 if(correct)             /*****  re-exchange *********/
 {  for(j=0;j<dim;j++)
    {  i = dim - 1 - j;
	  if(*(ix+i) != *(iy+i))
	  {  iz = *(ix+i); is = *(iy+i);
		for(l=0;l<dim;l++)    {              t     = *(matptr+l*cols+iz);
						    *(matptr+l*cols+iz) = *(matptr+l*cols+is);
						    *(matptr+l*cols+is) =    t    ;
 }  }  }                        }
 free(ix); free(iy); free(iq);
 return(correct);
} // matinv

int Multiple_Linear_Regression::matmult(double *matptr1, double *matptr2, double *result_ptr,
	   int rows1, int cols1, int cols2)
{ int i,j,k;

	 i=0;
	 while(i<rows1)
	 {    j=0;
		 while(j<cols2)
		 {    *(result_ptr+i*cols2+j)=0;
			 k=0;
			 while(k<cols1)
			 { *(result_ptr+i*cols2+j) +=
			    (*(matptr1+i*cols1+k)) * (*(matptr2+k*cols2+j));
			   k++;
			 } j++;
		 } i++;
	 }
	 return(1);
} // matmult

// multivariate lineare Regression
// parameter bkonst>0 says, that the  regression is not 
// forced through the origin of the coordinate system
// x has to be given in columns, x' in a row

int Multiple_Linear_Regression::mlr( double *x, int Xrows, int Xcols, double *y, int bkonst, double *b ) 
{
 double *xx,
        *xtransp,
        *xinv,
        *xinv_mal_xtransp;
 int    i, j, K=0;

  // bkonst=1;
  if( bkonst != 0 ) K=1;
  xtransp = (double *) calloc( (Xcols+K)*Xrows, sizeof(double));          addressed(xtransp, "xtransp not allocated", (Xcols+K)*Xrows);
  xinv =    (double *) calloc( (Xcols+K)*(Xcols+K), sizeof(double));      addressed(xinv, "xinv not allocated", (Xcols+K)*Xrows);
  xinv_mal_xtransp = (double *) calloc( (Xcols+K)*Xrows, sizeof(double)); addressed(xinv_mal_xtransp , "xinv_mal_xtransp  not allocated", (Xcols+K)*Xrows);
  xx =      (double *) calloc( (Xcols+K)*Xrows, sizeof(double));          addressed(xx, "xx not allocated", (Xcols+K)*Xrows);

  for( i=0; i<Xrows; i++ )
   for( j=0; j<Xcols; j++ )
     *(xx+i*(Xcols+K)+j)=*(x+i*Xcols+j);
  // the following commmand line is necessary for calculation of the constant b in the regression equation
  if( K )
    for( i=0; i<Xrows; i++ ) *(xx+i*(Xcols+K)+Xcols)=1;

  for( i=0; i<Xrows; i++ )
    for( j=0; j<(Xcols+K); j++ )
      *(xtransp + j*Xrows + i) = *(xx + i*(Xcols+K) + j);

  matmult( xtransp, xx, xinv, Xcols+K, Xrows, Xcols+K );
  matinv ( xinv, Xcols+K, Xcols+K );
  matmult( xinv, xtransp, xinv_mal_xtransp, Xcols+K, Xcols+K, Xrows );
  matmult( xinv_mal_xtransp, y, b, Xcols+K, Xrows, 1 );

  printf("\n");for( i=0; i<Xcols+K; i++ ) printf("b%d = %f\t", i+1, *(b+i)); printf("\n");

  free(xx); free(xtransp); free(xinv); free(xinv_mal_xtransp);
  return 0;
} // mlr

int save_frequency_of_edges_and_nodes(FILE *fout, int *edges_nodes, int edges )
{
 int i;
 double *x, *y, *b;
 int    Xrows=0, Xcols=1;
 Multiple_Linear_Regression regression;

 x = (double*) calloc(edges, sizeof(double)); addressed(x, "x in mlr not allocated", edges); 
 y = (double*) calloc(edges, sizeof(double)); addressed(y, "y in mlr not allocated", edges); 
 b = (double*) calloc(edges, sizeof(double)); addressed(b, "b in mlr not allocated", edges); 

 fprintf(fout, "\nedges\tfrequency of nodes\n");
 for(i=1; i<=edges; i++ )
 {
  if(edges_nodes[i])
  { 
    y[Xrows] = log10(*(edges_nodes+i)); x[Xrows]= log10(i); Xrows++;
    fprintf(fout, "%3d\t%3d\n", i, *(edges_nodes+i));
  }
 }

 // multiple linear regression and correlation
 if(Xrows>1)
 {
  regression.mlr(x, Xrows, Xcols, y, 1, b );
  fprintf(fout, "freq_of_nodes = %.4lf * edges^(%+.4lf)\n", exp(log(10)*b[1]), b[0]);
  Linear_Correlation R ( x, y, Xrows );
  if(Xrows>2)
  {
    cout << "\nLinear correlation coefficient r = " << R.r() << "\t(PEARSON)" << endl; 
    fprintf(fout, "Linear correlation coefficient r = %lf\n", R.Linear_Correlationskoeff_r);
    switch(R.reference_r_2sides())
    {
     case 1:  fprintf(fout, "The dependency is significant (p<0.05).\n");  break;
     case 2:  fprintf(fout, "The dependency is significant (p<0.01).\n");   break;
     case 3:  fprintf(fout, "The dependency is significant (p<0.001).\n");  break;
     default: fprintf(fout, "The dependency is not significant (p>0.05).\n");
    }
  }
  else
  { 
    cout << "\nOnly one data couple exists for the connectivities, correlation coefficient is not calculable.\n" << "\r" << endl;
    fprintf(fout, "correlation coefficient not calculable.\n" );
  }
 }
 else 
  fprintf(fout, "A regression equation of edges and nodes is senseless.\n", b[0], b[1]);
 fflush(fout);
 free(x);
 free(y); 
 free(b);
 return 0;
} // save_frequency_of_edges_and_nodes


int fvectoroutput (struct ::vector *v, FILE *savefile)
{
 int i;
 for (i=0; i<(v->row); i++)
 {
   if(fabsl(*(v->head+i))>1.0E-6)
    fprintf(savefile,"%2g ", (double)*((v->head)+i));
   else fprintf(savefile, " 0 ");
 }
 fprintf (savefile,"\n"); return 0;
}

int fmatoutput (struct mat *m, FILE *savefile)
{
 int i,ii, zero_count, zero_lines=0;
 char format[101] = {" "};
 double min_coeff;

 // first count zero_lines for a correct matrix dimension FM
 for (i=0; i<(m->row); i++) 
 {
  for (min_coeff=DBL_MAX, zero_count=ii=0; ii<(m->col); ii++) 
  {
   if( *(*((m->head)+i)+ii)==0.0) zero_count++;
   if( *(*((m->head)+i)+ii))  min_coeff = (fabs(*(*((m->head)+i)+ii))< min_coeff ? fabs(*(*((m->head)+i)+ii)) : min_coeff );
  }
  if( zero_count==m->col) zero_lines++;
  // normalize_row
  if(min_coeff!=1.0 && min_coeff!=DBL_MAX) for(ii=0; ii<m->col; ii++) if(*(*(m->head+i)+ii))(*(*(m->head+i)+ii))/=min_coeff;
 }

 // to format
 formatting( format, m->head, m->row, m->col );
#ifdef sun
   strcat(format, "g ");
#else   
   strcat(format, "lg ");
#endif
 if(!(m->row-zero_lines)) return m->row-zero_lines;
 fprintf (savefile," \n matrix dimension r%d x c%d\n",m->row-zero_lines, m->col);
 for (i=0; i<(m->row); i++) 
 {
  // do not fprintf if a line contains only zeros
  for (zero_count=ii=0; ii<(m->col); ii++) 
    if( *(*((m->head)+i)+ii)==0.0) zero_count++;
  if( zero_count==m->col) continue;
 
  for (ii=0; ii<(m->col); ii++) 
    // fprintf(savefile,"%2.0Lf ",*(*((m->head)+i)+ii));
    fprintf(savefile, format, (long_double) *(*((m->head)+i)+ii));
  fprintf(savefile,"\n");
 }
 return m->row-zero_lines; 
} // fmatoutput

int fenzymeoutput (struct mat *m, struct enc *list, FILE *savefile)
{
 int i,r, zero_lines=0, yn=0, multiple_membership=0;
 long_double **mat, **h;
 unsigned long fpos;
 struct enc *acel;
 char format[101];
 static int si=0; int not_zero_r1, k;

 h=(long_double**)calloc( 1, sizeof(long_double));
 mat=m->head;
 fprintf (savefile,"\n enzymes\n");

 if(si>2) fprintf(savefile, "# in () indicates # of enzymes used by the elementary mode");
 for (i=0;i<m->row;i++)
 {
   if(si>2)
    for( not_zero_r1=k=0; k<m->col; k++)
     if(*(*((m->head)+i)+k)) not_zero_r1++;

   fpos=ftell(savefile);
   if(si>2) fprintf (savefile,"\n %d: (%2d) ",i+1, not_zero_r1 );
   else     fprintf (savefile,"\n %d:\t",i+1 /*-zero_lines*/ );


   acel=list->next; r=0;
   
   while (acel!=acel->next)
   {
    if (*(*(mat+i)+(acel->key-1))!=0) 
	{
	 fprintf(savefile, " " );
     if(fabsl(*(*(mat+i)+(acel->key-1)))==1 && strstr(acel->txt, " "))        fprintf(savefile, "(");  // metabolites with spaces to fasten together
     
     if (*(*(mat+i)+(acel->key-1))==1) 
       fprintf(savefile,"%s",acel->txt);
     else if (*(*(mat+i)+(acel->key-1))==-1) 
       fprintf(savefile,"-%s",acel->txt);
	 else 
     {  // fprintf(savefile," (%Lf %s)",*(*(mat+i)+(acel->key-1)),acel->txt);
        // to format
        fprintf(savefile,"("); *h=(*(mat+i)+(acel->key-1)); 
        formatting( format, h, 1, 1 );
        #ifdef sun
         strcat(format, "g ");  // strcpy( format, "%3g ");
        #else   
         strcat(format, "lg "); // strcpy( format, "%Lg");
        #endif
        fprintf(savefile, format, *(*(mat+i)+(acel->key-1)));
        fprintf(savefile,"%s)", acel->txt);
     }
     if( fabsl(*(*(mat+i)+(acel->key-1)))==1 && strstr( acel->txt, " ") )        fprintf(savefile,")");
     if ((acel->rev)==1) r=1;
     yn=1;
	}
    acel=acel->next;
   };
   if(yn==1)
   {if (r==1) fprintf (savefile," irreversible");
    else fprintf (savefile," reversible");
   }else
    { ++zero_lines; fseek( savefile, fpos, SEEK_SET); }
   yn=0;
 }
 if( m->row == zero_lines ) fprintf(savefile, " - not found -\n");
 fprintf (savefile,"\n");
 free(h);
 if(si>2) if(multiple_membership>m->row) 
     fprintf(savefile, "\nAt least one elementary mode belongs to more than one block");
 si++;
 return i;
} // fenzymeoutput

int foveralloutput (struct mat *m, struct enc *list, FILE *savefile)
{
 int i,t, j, zero_count, zero_lines=0;
 long_double **mat, **h, min_coeff;
 struct enc *acel;
 char format[101];
 h=(long_double**)calloc(1, sizeof(long_double*));
 mat=m->head;
 //*->*/    printf("m r%d x c%d", m->row, m->col);   print_mat( m  ); _getch();
 fprintf(savefile,"\n overall reaction\n");
 // count zerolines
 for (i=0;i<m->row;i++)
 {
   for(zero_count=j=0; j<m->col; j++)
   {
     if(!(*(*(m->head+i)+j))) zero_count++;
   }
   if(zero_count==m->col)
     { zero_lines++; }
 }
 if( m->row == zero_lines ) 
   fprintf(savefile, " - not found -\n");
 else
 for (i=0;i<m->row;i++)
 {
  for(min_coeff=DBL_MAX, zero_count=j=0; j<m->col; j++)
  {
    if(!(*(*(m->head+i)+j))) zero_count++;
    if((*(*(m->head+i)+j)))   min_coeff = (fabs(*(*(m->head+i)+j))< min_coeff ? fabs(*(*(m->head+i)+j)) : min_coeff );
  }
  if(zero_count==m->col)
   {/* skip zerolines */ zero_lines++; fprintf (savefile,"\n %d:\tno net transfomation of external metabolites",i+1); continue; }
  fprintf (savefile,"\n %d:\t",i+1/*-zero_lines*/); fflush(savefile);

  // normalize_row
  if(min_coeff!=1.0 && min_coeff!=DBL_MAX)
   for(j=0; j<m->col; j++)  if(*(*(m->head+i)+j)) (*(*(m->head+i)+j))/=min_coeff; else;
  
  acel=list->next;
  t=0;	
  while (acel!=acel->next)
  {
   if (*(*(mat+i)+(acel->key-1))<0) 
   {			
    if (t==1) fprintf (savefile," + ");
    if (*(*(mat+i)+(acel->key-1))==-1)
       fprintf (savefile,"%s",acel->txt);
    else
    { // fprintf(savefile,"%Lf %s",-*(*(mat+i)+(acel->key-1)),acel->txt);
      // to format
      *h=(*(mat+i)+(acel->key-1)); 
        (**h)=-(**h);
      formatting( format, h, 1, 1 );
      #ifdef sun
        strcat(format, "g ");  
      #else   
        strcat(format, "lg "); 
      #endif
      fprintf(savefile, format, *(*(mat+i)+(acel->key-1)));
      // fprintf(savefile, "%lg ", (double) *(*(mat+i)+(acel->key-1)));
      fprintf(savefile,"%s", acel->txt); 
        (**h)=-(**h);
      fflush(savefile);
    }
    t=1;
   }
   acel=acel->next;
  };
  fprintf (savefile," = ");
  t=0;
  acel=list->next;
  while (acel!=acel->next)
  {
   if (*(*(mat+i)+(acel->key-1))>0) 
   {
    if (t==1) fprintf (savefile," + ");
    if (*(*(mat+i)+(acel->key-1))==1) 
       {fprintf (savefile,"%s",acel->txt);fflush(savefile);}
    else
    { // fprintf(savefile,"%Lf %s",*(*(mat+i)+(acel->key-1)),acel->txt);
      *h=(*(mat+i)+(acel->key-1)); 
        formatting( format, h, 1, 1 );
      #ifdef sun
       strcat( format, "g ");
      #else   
       strcat( format, "lg ");
      #endif
      fprintf(savefile, format, (long_double)*(*(mat+i)+(acel->key-1)));
      fprintf(savefile,"%s",acel->txt);
      fflush(savefile);
    }
    t=1;
   }
   acel=acel->next;
  };
 }
 // if( m->row == zero_lines ) fprintf(savefile, " - not found -\n");
 // if(zero_lines) 
 //   fprintf(savefile, "\n\nThe remaining overall reactions perform no net transformation of external metabolites.");
 fprintf (savefile,"\n");fflush(savefile);
 free(h);
 return 0;
} // foveralloutput 

struct ::vector *getrev (struct enc *list)
{
 int i=0;
    struct enc *acel; struct ::vector *v;
 acel=list->next;
 while (acel!=acel->next) {acel=acel->next;i++;}
    v=(struct ::vector*)calloc(1, sizeof(struct ::vector)); v->row=i;   addressed( v, "v not allocated", 1);
 v->head=(long_double*)calloc(v->row, sizeof(long_double));      addressed( v->head, "v->head not allocated", v->row);
 acel=list->next; i=0;
 while (acel!=acel->next) {*(v->head+(i++))=acel->rev; acel=acel->next;} 
 return v;
} //struct vector *getrev 

int scan_metabolites( char*tx, char *end )
{
  static int first=1;
  char help[TXT];
  int len, i, y_n=0;
  if(!met_counter)first=1;
  
  len=strlen(tx)-3;
  i=0; while(isspace(tx[i++])); 
  strcpy(help, &tx[i-1]); help[len-i+1]='\x0';
  strcpy(tx, help);
  if( !strcmp( end, " : ") ) return 0;
  if( first )
  { metabolites = (char**)calloc(1, sizeof(char*)); addressed(metabolites, "metabolites", 1 ); 
    mets_cumul  = (int*)  calloc(1, sizeof(int*));  addressed(mets_cumul, "mets_cumul", 1 ); }
  for(i=0; i<met_counter; i++ )
   if( !first && !strcmp(*(metabolites+i), help )) 
   { y_n=1; (*(mets_cumul+i))++; break;}
  if(!y_n)
  {
   metabolites = (char**)realloc(metabolites, (met_counter+1)*sizeof(char*)); addressed(metabolites, "realloc metabolites", (met_counter+1)); 
   mets_cumul  = (int*)  realloc(mets_cumul,  (met_counter+1)*sizeof(char*)); addressed(mets_cumul, "realloc mets_cumul", (met_counter+1)); *(mets_cumul+met_counter) = 1;
   *(metabolites+met_counter) = (char*)calloc(len+1, sizeof(char)); addressed(*(metabolites+met_counter), "*(metabolites+met_counter)", len+1);
   // copy the name of the metabolite if it is not included into the global array
   strcpy(*(metabolites+met_counter), help);
   met_counter++;
  }
  first=0;
  return 0;
} //scan_metabolites

int read_unit(FILE *fp_in, char *tx) // read syntactical units of the stoichiometrical equations
{
 int c, c1, i=0, len;
 char end[4];
 long int f_pos;

 do{
   do{
     c = fgetc(fp_in);
     if(c==EOF && tx[i-1]!='.') 
          return -1;
     c1 = fgetc(fp_in); 
     if(c1!=EOF)
        fseek(fp_in, (long)-1, SEEK_CUR);
     if(isspace(c1)&&isspace(c)){continue;}
     if(c1==':'&&isspace(c)||c1=='.'&&isspace(c)||c1=='='&&isspace(c)||c1=='+'&&isspace(c)) 
       f_pos=ftell(fp_in)-1;
    
     if(c=='\t' ||  c=='\n' || c==EOF)c=' ';
     if(strlen(tx)>1)
       if(tx[strlen(tx)-1]=='.' && isspace(c) ) c=' ';

     if((i>1&&isspace(tx[i-1]))&&isspace(c))
      printf("miau");  // this command is nonsens 30.11.2001 FM
     else
      {tx[i] = (char)c; tx[++i]='\x0';}
     if(i>2) 
       if(isspace(tx[i-3]) &&
          (tx[i-2]==':'||tx[i-2]=='+'||tx[i-2]=='='||tx[i-2]=='.' ) &&
          isspace(tx[i-1]))
          break; 
   }while(1);
   
   len=strlen(tx); 
#ifdef _WIN32
     tx=_strrev(tx);
#else
     tx=strrev(tx);
#endif
   while(isspace(tx[len-1]))tx[--len]='\x0';
#ifdef _WIN32
     tx=_strrev(tx);
#else
     tx=strrev(tx);
#endif
   if(isspace(tx[0])) tx[0]=' ';
   if(i==2)
    {
     if( !strcmp(tx, ": ") || !strcmp(tx, "+ ") ||
         !strcmp(tx, "= ") || !strcmp(tx, ". ") ) 
         { return 0; }
    }
   if(i>2)
   {
    {
     if( !strcmp(tx, ": ") || !strcmp(tx, "+ ") ||
         !strcmp(tx, "= ") || !strcmp(tx, ". ") ) 
         { return 0; }
    }
    if(strlen(tx)==3)strcpy(end, tx);else strcpy(end, &tx[strlen(tx)-3]);
    if( !strcmp(end, " : ") || !strcmp(end, " + ") ||
        !strcmp(end, " = ") || !strcmp(end, " . "))  
        { scan_metabolites(tx, end);
          if(!strcmp(end, " : ")||!strcmp(end, " + ")||!strcmp(end, " = ")||!strcmp(end, " . "))
            fseek(fp_in,f_pos, SEEK_SET);
          break;
        }
   } 
 } while(1);
 return 0;
}// read_unit */

int read_stoichiometric_coefficient(FILE *s, char* tx, double *rs)// int *rs)
{
 int c, i=0, len, y_n, digits, decimal_dot, fraction_stroke, nominator, denominator;
 long int f_position;
 char *end;
 
 digits=0; fraction_stroke=0; decimal_dot=0;
 f_position = ftell(s);
 do{
   c = fgetc(s);
   if( isdigit(c) ) digits++;
   if( c == '/' ) fraction_stroke = i;
   if( c == '.' ) decimal_dot     = i;
   tx[i++] = (char)c; tx[i]='\x0';
   if(i>2)
   {
    end = &tx[i-2];
      if( !strcmp(end, "+ ") ) break;
    if( !strcmp(end, "= ") ) break; 
    if( !strcmp(end, ". ") && digits<i-1) break;
   }
 }while( !isspace(tx[i-1]) );
 
 y_n=0; len = strlen(tx)-1;
 for( i=0; i<len; i++ )
 {  if( !isdigit(tx[i]) && (digits!=len-1 && !strcmp(tx, "."))) {y_n=1; break;}else;
    if( !isdigit(tx[i]) && (digits==len-1 && strcmp(tx, "/"))) {y_n=1; break;}else;
 }
 if(y_n && fraction_stroke )
   {
     fseek(s, -1, SEEK_CUR); 
     nominator = atoi(tx); denominator= atoi(&tx[fraction_stroke+1]);
     *rs=(double) nominator / (double) denominator;
   }
 else if(y_n && decimal_dot || digits == len) // oo 
   { fseek(s, -1, SEEK_CUR); *rs=(double) atof(tx); /*atoi(tx);*/  }
 else 
   { fseek(s, f_position, SEEK_SET); *rs=1.0;}
 return 0;
} //read_stoichiometric_coefficient

struct mat *getnex (FILE *s, struct enc *metlist, struct enc *enzlist) 
{
 char tx[TXT]={""};
 struct enc *el, *ml, *acel;
 struct mat *m;
 int i, metfl, con, rev; double rs=0.0;
 int nmet, nreact, acrow, acol;
 long_double **sub;
    
 // unfortunately the metabolites have to be read once more  out of the stochiometrical equations
 if( met_counter )
 {for(i=0; i<met_counter; i++)
   free(*(metabolites+i)); 
  free(metabolites); met_counter=0;
  free(mets_cumul);
 }
 nmet=0;nreact=0;
 acel=metlist->next;
 while (acel!=acel->next) {acel=acel->next;nmet++;}
 acel=enzlist->next;
 while (acel!=acel->next) {acel=acel->next;nreact++;}
 sub=(long_double**)calloc(nmet, sizeof(long_double*));  addressed( sub, "sub not allocated", nmet);
 for (acrow=0;acrow<nmet;acrow++)
 {*(sub+acrow)=(long_double*)calloc(nreact, sizeof(long_double)); addressed( *(sub+acrow), "*(sub+acrow) not allocated", nreact);}
 metfl=0; 
 while (!feof(s))	
 {
   el=enzlist; ml=metlist;
   if( read_unit(s, tx) == -1 ) break; 
   if      (tx[0]=='.') {metfl= 0;  if( read_unit(s, tx)==-1 /*fscanf(s,"%s",tx)==EOF*/ ) break;            }
   else if (tx[0]==':') {metfl= 1; read_stoichiometric_coefficient(s,tx,&rs); read_unit(s, tx); }  
   else if (tx[0]=='=') {metfl=-1; read_stoichiometric_coefficient(s,tx,&rs); read_unit(s, tx); }
   else if (tx[0]=='+') {          read_stoichiometric_coefficient(s,tx,&rs); read_unit(s, tx); }
   if(!metfl)
   {
    con=1; acrow=0;
    while (el!=el->next) 
    {
     el=el->next;   
     if(compstr(100,tx,el->txt)) 
     {con=0;rev=el->rev;break;}
    }
    if (con) 
    {/*printf ("ERROR: UNKNOWN ENZYME: %s\n",tx); printf("press ENTER key -> "); _getch(); printf("\r                   \r");*/} 
    else 
      acol=(el->key)-1;   
   }
   else
   {
    con=1;
    while (ml!=ml->next) 
    {
     ml=ml->next;
     if (compstr(100,tx,ml->txt)) 
     {con=0; break;}
    }
    if (con) 
      {/*printf ("ERROR: UNKNOWN METABOLITE: %s\n",tx); printf("press ENTER key -> "); _getch(); printf("\r                   \r");*/}
    else acrow=(ml->key)-1; 
    *(*(sub+acrow)+acol)=-rs*metfl;
    // for irreversible reactions: only consumed or only built
    if(metfl>0 ) // if rev==1 the reaction is irreversible
       { ml->consumed++; }
    else       
       { ml->built++;    }
    ml->reactions++; if(rev) strcat(ml->ri, "i"); else strcat(ml->ri, "r");
    if(strlen(ml->ri)>1000) {/*printf("\nEnlarge the array size of the struct enc ri.\nProgram prematurely finished.\nPlease press ENTER key "); _getch(); */exit(6);}
   }   
   tx[0]='\x0';
 }; // end while()
 m=(struct mat*)calloc(1, sizeof(struct mat));   addressed( m, "m not allocated", 1);
 m->row=nmet; 
 m->col=nreact; 
 m->head=sub;
 return m;
} //struct mat *getnex 

int get_enzyme_names(char *enz_name)
{
 int len, i;
 len=strlen(enz_name);
 if(!enzyme_counter)
   enzyme_names = (char**)calloc(1, sizeof(char*)); addressed(enzyme_names, "enzyme_names", 1 );

 enzyme_names = (char**)realloc(enzyme_names , (enzyme_counter+1)*sizeof(char*)); addressed(enzyme_names , "realloc enzym_names", (enzyme_counter+1));
 *(enzyme_names+enzyme_counter) = (char*)calloc(len+1, sizeof(char)); addressed(*(enzyme_names+enzyme_counter), "*(enzyme_names+enzyme_counter)", len+1);
 strcpy(*(enzyme_names+enzyme_counter), enz_name);
 enzyme_counter++;
  // test if there is a double defined reaction
 for( i=0; i<enzyme_counter-1; i++ )
   if(!strcmp(*(enzyme_names+i), enz_name))
   { /*printf("The enzyme reaction ->%s<- is more than once defined.", enz_name ); 
     printf("\nPlease delete the extra defined reaction in the input file.\n\nProgram prematurly finished. Please press ENTER key ");
     _getch(); */exit(7); 
   }
 return 0;
} // get_enzyme_names

int get_metabolites_from_equations (FILE *s) 
{
 char tx[TXT];
 double /*int*/ rs=0;static int a;

 do{ fgets(tx, TXT-1, s);
 }while(strcmp(tx, "-CAT\n"));
 fgetc(s); // read only NEWLINE
 
 fseek(s, -1, SEEK_CUR);
 while (!feof(s))	
 {                     a++; 
   if      (read_unit(s,tx)==-1) { break; } if(a==1)
                                            {get_enzyme_names(tx); printf("\nreading reaction %s\n", tx);}
   if      (tx[0]=='.') { if(read_unit(s, tx)==-1) break;
                          get_enzyme_names(tx); printf("reading reaction %s\n", tx); }
   else if (tx[0]==':') { read_stoichiometric_coefficient(s,tx,&rs);
                          read_unit(s, tx); }  
   else if (tx[0]=='=') { read_stoichiometric_coefficient(s,tx,&rs);
                          read_unit(s, tx); }
   else if (tx[0]=='+') { read_stoichiometric_coefficient(s,tx,&rs);
                          read_unit(s, tx); }
   tx[0]='\x0';
 }; // end while()
 rewind(s);
 return 0;
} //get_metabolites_from_equations

struct mat *cutnex (struct mat *nex, struct ::vector *met )
                             // *mex, struct vector *ex)
{
 struct mat *m;
 int i,k=0, j;
 m=(struct mat*)calloc(1, sizeof(struct mat)); addressed( m, "m not allocated", 1);
 m->row=0; m->col=nex->col;
 for (i=0; i<met->row; i++) if (!*(met->head+i)) m->row++; else;
 if(m->row)
 { m->head=(long_double**)calloc( m->row, sizeof(long_double*)); addressed( m->head, "m->head not allocated", m->row); }
 else
 { /*printf("\nTheThe system comprises only external metabolites.\n"); _getch(); printf("\r                  \r");*/ exit(8); }
 for (i=0;i<met->row;i++)
 if (!*(met->head+i))
 {
   /* allocated by FM */
   *(m->head+k)=(long_double*)calloc(m->col, sizeof(long_double));  addressed( *(m->head+k), "*(m->head+i) not allocated", m->col);
   for( j=0; j<m->col; j++ ) *(*(m->head+k)+j)=*(*(nex->head+i)+j); k++;
 }
 return m;
}

/* free ************************************** */
int freevector (struct ::vector *v)
{free (v->head); free (v); return 0;}
int freemat (struct mat *m)
{
 int i;
 for (i=0; i<m->row; i++) 
 free (*(m->head+i));
 free (m->head); free (m); return 0;
}
int free_met_list(struct enc *metlist)
{
 int i=0, j;
 struct enc *a;
 a=metlist;
  while( a->next != a->next->next )
  {
   a= a->next;
   i++;
  }
 for( ++i; i>0; i-- )
 {
  a=metlist;
   for( j=0; j<i-1; j++ )
    a = a->next;
  free( a->next );
 }
 free(a);
 return 0;
} // free_met_list

int print_met_list(struct enc *metlist)
{
 int i=0;
 // FILE *fp_out;
 struct enc *a;
 // fp_out = fopen("d:\\quelltxt\\metatool\\v4\\mets.txt","a"); 
 // if(!fp_out){perror("file error: "); printf("list.txt"); exit(1);}
 a=metlist;
  while( a->next != a->next->next )
  {
   a= a->next;
   printf("%3d %s\n", i+1, a->txt );
   // fprintf(fp_out, "%3d %s\n", i+1, a->txt );
   i++;
  }
 // fclose(fp_out);
 return 0;
} // print_met_list

int additional_em(char* fname, struct mat *cb, struct mat *em)
{
  FILE *fp;
  int con, elem, ok, col, help, *zero_lines, zero_count, zl=0;
  int c, r, re, cc, ee, k=0;
  
  zero_lines=(int*)calloc(em->row, sizeof(int)); addressed(zero_lines, "zero_lines in additional", em->row);
  fp=fopen(fname, "a+");
  if(fp==NULL) {/*printf("\n%s ", fname); perror("File error"); _getch(); */exit(9);}
  
    // control if cb not found in em
  for(r=0; r<cb->row; r++)
  {
   for( re=0; re<em->row; re++ )
   {
    for(cc=ee=ok=c=0; c<cb->col; c++)
    {  
     if( *(*(cb->head+r)+c) && *(*(em->head+re)+c) ) ok++;
     if( *(*(cb->head+r)+c) ) cc++;
     if( *(*(em->head+re)+c) ) ee++;
    }
    if( ok==cc && ok==ee ) break;
   }
   if(!(ok==cc && ok==ee)) { fprintf(fp, "\nConvex mode %3d not found in elementary modes.", r+1 ); 
                              printf(    "Convex mode %3d not found in elementary modes.\n", r+1); k=1;}
  }
  if(k) fprintf(fp, "\n");

  fprintf(fp, "\nThe elementary mode");
  for(con=0; con<cb->row; con++)
  {
    for(elem=0; elem<em->row; elem++)
    {
     for(zero_count=ok=0, col=0; col<em->col; col++ )
     {  if( *(*(cb->head+con)+col) == *(*(em->head+elem)+col) ) ok++; else break;
        if( !*(*(em->head+elem)+col) ) zero_count++;
     }
     if(zero_count==em->col) 
        *(zero_lines+elem)=1;
     if(ok==em->col)
     {    for(col=0; col<em->col; col++)
            *(*(em->head+elem)+col) = 0; }
    }
  }

  for(help=0, elem=0; elem<em->row; elem++)
  {
    for(ok=0, col=0; col<em->col; col++ )
      if( *(*(em->head+elem)+col) != 0 ){ ok++; break; }
    if(ok)
       {help++;}
  }
  if(help>1) fprintf(fp, "s (%d) ", help); 
  else if( !help ) {fprintf(fp,"/s is/are equal to convex basis.\n"); fclose(fp); free(zero_lines); return 0;}
  else fprintf(fp, " ");

  for(elem=0; elem<em->row; elem++)
  {
    if(*(zero_lines+elem))zl++;
    for(ok=0, col=0; col<em->col; col++ )
      if( *(*(em->head+elem)+col) != 0 ){ ok++; break; }
    if(ok)
      fprintf(fp, "%d ", (elem+1)-zl );
  }
  if(help>1) fprintf(fp, "are "); else fprintf(fp, "is ");
  fprintf(fp, "additional to the convex basis.\n");
  fclose(fp);
  free(zero_lines);
  return 0;
} // additional_em

// the function filter_comment makes it possible to write commentaries
// at any place into the metatool input file
// it cuts out the commentaries starting with // to the end of line and all from "/*" to "*/"
FILE* filter_comment(FILE *fp_in, char **tmp_file_name)
{
   /* fname defines the template for the temporary file.  */
   char fname[] = {"fnXXXXXX"}, *unique_name;
   FILE *tmp_fp_out;
   int c0=0, c1, c2, c3, c4, schachtel=0; 
   char string[100];

   read_comment(fp_in);
#ifdef _WIN32
   unique_name = _mktemp(fname);
#else
   unique_name = mktemp(fname);
#endif
   tmp_fp_out = fopen(unique_name, "w+");
                addressed(tmp_fp_out,"temporary file not opened", 1);

   do{
      if((c1=fgetc(fp_in))!=EOF)
      {
       if(c1=='/')
       {
        if((c2=fgetc(fp_in))!=EOF)
        if(c2=='/')
        {
            do { fgets( string, 99, fp_in ); } while(string[strlen(string)-1]!='\n'); fprintf(tmp_fp_out, "\n"); 
        }
        else if(c2=='*')
        { 
          if(c1=='/' && c2=='*') schachtel++;
          if(c0=='*' && c1=='/') schachtel--;
            if(schachtel<1) {/*printf("\nThe input file contains interlocking commentaries. (To many closing marks)\nSee file %s.\n", unique_name); _getch(); */exit(10);}
          if((c3=fgetc(fp_in))==EOF) break;
          do{
            if((c4=fgetc(fp_in))==EOF) break;
            if( c3=='*' && c4=='/' ) { schachtel--; break; }
            if( c3=='/' && c4=='*' ) schachtel++; 
            if(schachtel>1) {/*printf("\nThe input file contains interlocking commentaries.\nSee file %s.\n(To many opening marks)\n", unique_name); _getch(); */exit(11);}
            c3=c4;
          }while(1); 
        }
       }
       else
       {
        if(c1=='\n' )
         fprintf(tmp_fp_out, "\n");
        else
         fputc(c1, tmp_fp_out);
        fflush(tmp_fp_out);
       }
       c0=c1;
      }
      else break;
   }while(1);
   rewind(tmp_fp_out);   
   *tmp_file_name = (char*) calloc (strlen(unique_name)+1, sizeof(char)); addressed( tmp_file_name, "tmp_file_name not allocated", strlen(unique_name));
   strcpy( *tmp_file_name, unique_name);
   fclose(fp_in);
   return tmp_fp_out;
} // filter_comment

void read_comment( FILE *fp_in )
{
 char*help;
 long int f_pos;
 help=(char*)calloc(1000, sizeof(char));addressed(help,"not enough memory | in read comment", 1000);
 do{
  f_pos = ftell(fp_in);
  fgets( help, 999, fp_in);
  if( !strncmp(help, "-ENZREV", 7) ) {fseek(fp_in, f_pos, SEEK_SET); break;}
  // if( !strstr(help, "#") ) {fseek(fp_in, f_pos, SEEK_SET); break;}
 }while(1);
 free(help);
}

/* 2.3 mat operations ********************+* */
  struct mat *transp (struct mat *m)
      {
      struct mat *tm; int i,ii;
      tm=(struct mat*)calloc(1, sizeof(struct mat));   addressed( tm, "tm not allocated", 1);
      tm->row=m->col; tm->col=(m->row);
      tm->head=(long_double**)calloc(tm->row, sizeof(long_double*)); addressed( tm->head, "tm->head not allocated", tm->row);
      for (i=0; i<m->col; i++) 
          {
          *(tm->head+i)=(long_double*)calloc(tm->col, sizeof(long_double));  addressed( *(tm->head+i), "*(tm->head+i) not allocated", tm->col);
          for (ii=0; ii<m->row; ii++) *(*(tm->head+i)+ii)=*(*(m->head+ii)+i);
          }
      return tm;
      }
  struct mat *addi (struct mat *m) // add the identity matrix to m
      {
      struct mat *mi; int i,ii;
      mi=(struct mat*)calloc(1, sizeof(struct mat)); addressed( mi, "mi not allocated", 1);
      mi->row=m->row; mi->col=m->col+m->row;
      mi->head=(long_double**)calloc(mi->row, sizeof(long_double*)); addressed( mi->head, "mi->head not allocated", mi->row);
      for (i=0; i<m->row; i++)
          {
          *(mi->head+i)=(long_double*)calloc(mi->col, sizeof(long_double)); addressed( *(mi->head+i), "*(mi->head+i) not allocated", mi->col);
          for (ii=0; ii<mi->col; ii++)
             {
             if (ii<m->col) *(*(mi->head+i)+ii)=*(*(m->head+i)+ii);
             else if (ii==m->col+i) *(*(mi->head+i)+ii)=1;
             else *(*(mi->head+i)+ii)=0;
             }
          }
      return mi;
      }

struct mat *cutcol (struct mat *m, int cc)
{
 struct mat *mc; int i,ii;
 mc=(struct mat*)calloc(1, sizeof(struct mat)); addressed( mc, "mc not allocated", 1);
 mc->row=m->row; mc->col=m->col-cc;
 mc->head=(long_double**)calloc(mc->row, sizeof(long_double*)); addressed( mc->head, "mc->head not allocated", mc->row);
 if(cc>m->col) {/*printf("ERROR IN FUNCTION CUTCOL: CC>M->ROW!\n"); _getch(); */exit(12); /* return NULL; */}
 for (i=0; i<mc->row; i++)
 {
  *(mc->head+i)=(long_double*)calloc(mc->col, sizeof(long_double)); addressed( *(mc->head+i), "*(mc->head+i) not allocated", mc->col);
  for (ii=0; ii<mc->col; ii++)
    *(*(mc->head+i)+ii)=*(*(m->head+i)+ii+cc);
 }
 return mc;
}

struct mat *simplify (struct mat *m)
{
 struct mat *mc;
    struct ::vector *vf;
 int i,ii,k;

    vf=(struct ::vector*)calloc(1, sizeof(struct ::vector)); addressed( vf, "vf not allocated", 1);
 vf->row=m->row; vf->head=(long_double*)calloc(vf->row, sizeof(long_double)); addressed( vf->head, "vf->head not allocated", vf->row);
 
 mc=(struct mat*)calloc(1, sizeof(struct mat)); addressed( mc, "mc not allocated", 1);
 mc->row=m->row; mc->col=m->col;
 
 {
  if( branch )
  {  
      branch=(struct ::vector*)calloc(1, sizeof(struct ::vector)); addressed( branch, "branch not allocated", 1);
    branch->row=m->row; branch->head=(long_double*)calloc(branch->row, sizeof(long_double)); addressed( branch->head, "branch->head not allocated", branch->row);
    for(i=0; i<branch->row; i++) *(branch->head+i)=1;
  }
 }

 for (i=0; i<m->row; i++)
 {
   *(vf->head+i)=0;
   for (ii=0;ii<m->col;ii++) 
       *(vf->head+i)=ggt(*(vf->head+i),*(*(m->head+i)+ii));
   if (*(vf->head+i)==0) // at this line are shown the not branch metabolites
   {
    if(branch) *(branch->head+i)=0.0;  // write into the global variable 
    mc->row--;
   }
 }
 if( branch )
 { printf("\nBranches ");
   for(i=0; i<branch->row; i++ ) 
   #ifdef sun     
     printf("%g", *(branch->head+i)); 
   #else
     printf("%lg", *(branch->head+i));
   #endif
   printf("\n"); 
 }

 if( !mc->row ) // Microsoft C++ 6.0 allocates any pointer if(mc->row==0 but no correct one)
 {
   printf("there is no simplification ...\n");
   mc->head=(long_double**)calloc(m->row, sizeof(long_double*)); addressed( mc->head, "mc->head not allocated 1", m->row);
   // return a copy of *m
   for (i=0; i<m->row; i++)
   {
    *(mc->head+i)=(long_double*)calloc(m->col, sizeof(long_double)); addressed( *(mc->head+i), "*(mc->head+i) not allocated", m->col);
    for (ii=0;ii<m->col;ii++) 
      *(*(mc->head+i)+ii)=*(*(m->head+i)+ii);
   }
   mc->row=m->row; mc->col=mc->col;
 }
 else 
 {
   mc->head=(long_double**)calloc(mc->row, sizeof(long_double*)); addressed( mc->head, "mc->head not allocated", mc->row); k=0;
   for (i=0; i<vf->row; i++) if (*(vf->head+i)!=0) 
   {
    *(mc->head+k)=(long_double*)calloc(mc->col, sizeof(long_double)); addressed( *(mc->head+k), "*(mc->head+k) not allocated", mc->col);
    for (ii=0;ii<mc->col;ii++) 
      *(*(mc->head+k)+ii)=*(*(m->head+i)+ii)/(*(vf->head+i));
    k++;
   }
 }
 freevector(vf);
 return mc;
} // simplify
      
struct mat *mult (struct mat *m1, struct mat *m2)
{
 struct mat *mm; int i,j,k; long_double sum;
 if (m1->col!=m2->row) printf("error in mult: mat dimensions are inkompatible");
 mm=(struct mat*)calloc(1, sizeof(struct mat));  addressed( mm, "mm not allocated", 1);
 mm->row=m1->row; mm->col=m2->col;
 mm->head=(long_double**)calloc(mm->row, sizeof(long_double*));  addressed( mm->head, "mm->head not allocated", mm->row);
 for (i=0; i<mm->row; i++)
 {
  *(mm->head+i)=(long_double*)calloc(mm->col, sizeof(long_double));  addressed( *(mm->head+i), "*(mm->head+i) not allocated", mm->col);
  for (k=0; k<mm->col; k++)
  {
   sum=0.0;
   for (j=0; j<m1->col; j++) 
     {if(*(*(m1->head+i)+j) && *(*(m2->head+j)+k))
     sum+=((long_double)*(*(m1->head+i)+j)*((long_double)*(*(m2->head+j)+k)));
      if(fabs(sum)<1E-12) sum=0.0; } // FM 02.04.2002 Fehler unklar; rund geht nicht; Differenzen numerisch nicht immer Null
   *(*(mm->head+i)+k)=sum;
  }
 }
 return mm;
}
/* 2.4 complex mat operations ************** */
int switch_if_all_rows_are_minus_into_plus(struct mat *help)
{
  int i, j, count_negatives, count_positives;
  for(i=0; i<help->row; i++ )
  {
    count_negatives=0; count_positives=0;
    for(j=0; j<help->col; j++ )
      if     ( *(*(help->head+i)+j) < 0 ) 
        count_negatives++;
      else if( *(*(help->head+i)+j) > 0 )
        { count_positives++; break;}
    if( !count_positives && count_negatives )
    for(j=0; j<help->col; j++ )
      if(*(*(help->head+i)+j)) *(*(help->head+i)+j)*=-1.0;
  }
 return 0;
} // switch_if_all_rows_are_minus_into_plus
/* Kernel */
struct mat *kernel (struct mat *m)
{
 struct mat *help, *k; int i,u,ii,uu; long_double f1,f2;
       
 k=transp(m);
 help=addi(k);
 freemat(k);
 
 for (ii=0; ii<m->row; ii++)
 {
  for (i=0; i<help->row; i++) 
   if (*(*(help->head+i)+ii)!=0) break;
  for (u=i+1; u<help->row; u++) 
  {
   if (*(*(help->head+u)+ii)!=0)
   {
    f1=(*(*(help->head+u)+ii))/ggt(*(*(help->head+u)+ii),*(*(help->head+i)+ii));
    f2=(*(*(help->head+i)+ii))/ggt(*(*(help->head+u)+ii),*(*(help->head+i)+ii));
    for (uu=0;uu<help->col;uu++) 
      *(*(help->head+u)+uu)=f2*(*(*(help->head+u)+uu))-f1*(*(*(help->head+i)+uu));
   }
  }
  if (i<help->row) for (uu=0;uu<help->col;uu++) *(*(help->head+i)+uu)=0;
 }
 k=cutcol(help,m->row); 
 freemat(help); 
 help=simplify(k);
 freemat(k); 
 switch_if_all_rows_are_minus_into_plus(help);
 return help;
} // Kernel

int delete_corresponding_row_in_rev(struct ::vector *a, int wrong_subset)
{
 int j;
 
 if(wrong_subset<a->row-1)
   for( j=0; j<a->row; j++ )
     *(a->head+j) = *(a->head+j+1);
 a->row--;
 return 0;
} //delete_corresponding_row_in_rev
      
/* Subsets */
struct mat *subset (struct mat *m, struct ::vector *v, int **wrong_subset)
{
 struct mat *help, *k; int i,ii,u; 
 long_double f1,sign1;
 k=transp(m); help=addi(k); freemat(k);
     
 for (i=0;i<help->row;i++) 
 {
	f1=0; sign1=0;  
	for (ii=0;ii<m->row;ii++)
     if (*(*(help->head+i)+ii)!=0)
	{
	 f1=ggt(f1,*(*(help->head+i)+ii));
	 if (sign1==0) 
       sign1=*(*(help->head+i)+ii)/fabsl(*(*(help->head+i)+ii));
    }
	if (f1==0) 
      for (ii=0;ii<help->col;ii++) 
        *(*(help->head+i)+ii)=0;
	else 
	{
	 for (ii=0;ii<m->row;ii++) 
         *(*(help->head+i)+ii)=*(*(help->head+i)+ii)*sign1/f1;
	 *(*(help->head+i)+i+m->row)=f1*sign1; 
	}
 }	
 //*->*/    printf("help r%d x c%d", help->row, help->col);   print_mat( help ); _getch();

 for (i=0;i<help->row;i++) 
 for (u=i+1;u<help->row;u++)
	{
	 f1=0;
		for (ii=0;ii<m->row;ii++) 
        if ((*(*(help->head+i)+ii))!=(*(*(help->head+u)+ii)))
         f1=1;
		if (f1==0)
		{
			for (ii=m->row;ii<help->col;ii++) *(*(help->head+i)+ii)+=*(*(help->head+u)+ii);
			for (ii=0;ii<help->col;ii++) *(*(help->head+u)+ii)=0;
		}
 }

 k=cutcol(help,m->row); freemat(help);
 help=simplify(k); freemat(k);
 *wrong_subset = (int*) calloc(help->row, sizeof(int)); addressed(wrong_subset, "wrong_subset not enough memory", help->row);
 //*->*/ print_mat(help);
 for (i=0;i<help->row;i++)						/* revstatetest */
	{
		f1=0;
		for (ii=0;ii<help->col;ii++) 
          if ((*(v->head+ii))&&(*(*(help->head+i)+ii)<0))
          {f1=1;break;}
		if (f1==1) 
         for (ii=0;ii<help->col;ii++) 
           *(*(help->head+i)+ii)*=-1;
		f1=0;
		for (ii=0;ii<help->col;ii++) 
           if ((*(v->head+ii))&&(*(*(help->head+i)+ii)<0))
           {f1=1;break;}
		if (f1==1) 
        { *(*wrong_subset+i)=1;  /* for (ii=0;ii<help->col;ii++) *(*(help->head+i)+ii)=0;*/ }
 }	
 switch_if_all_rows_are_minus_into_plus(help);
 return (help);
} // subset

struct ::vector *subrev (struct mat *m, struct ::vector *v)
{
 int i,ii;
    struct ::vector *r;
    r=(struct ::vector*)calloc(1, sizeof(struct ::vector));  addressed( r, "r not allocated", 1);
 // stutz
 r->row=m->row; r->head=(long_double*)calloc(r->row, sizeof(long_double));  addressed( r->head, "r->head not allocated", r->row);
 for (i=0;i<m->row;i++) 
 {
  *(r->head+i)=0;
  for (ii=0;ii<m->col;ii++) if ((*(*(m->head+i)+ii))&&(*(v->head+ii))) {*(r->head+i)=1; break;}
 }
 return r;
}

inline int numerical_array( long_double k2, long_double hi, long_double k1, long_double hu, 
                     const int abs1, const int sign1, const int sign2, const char*loop )
{
 // control the numerical overflow of calculation of tablaux
 long double result;
  if( abs1 )
  { result = sign1*fabsl(k2)*(long double)hi + sign2*fabsl(k1)*(long double)hu;}
  else
    result = sign1*k2*(long double)hi + sign2*k1*(long double)hu;
  
  if((long double) result > (long double)  DBL_MAX ||
     (long double) result < (long double) -DBL_MAX )
    {/*printf("\n\nerror: the intermediate result %.0lf exceeds\nthe allowed integer range  (+- %lf), %s\
               \nProgram prematurely finished.", (double)result, DBL_MAX, loop);
     _getch(); */exit(13);}
  return 0;  
} // numerical_array
      
/* basis */
struct mat *basis (struct mat *m, struct ::vector *v)
{
  struct mat *help, *k; 
  long_double **h1, **h2=NULL, abs_hi, abs_hu, ggt_Erg;
  long_double *rev1, *rev2=NULL, k1,k2;
  int i,u,tu,ii,uu,test1;
  int f1, r1, r2, c, counter;

  k=transp(m);help=addi(k);freemat(k);
  r1=(help->row); c=help->col; counter=m->row;
  rev1=(long_double*)calloc(v->row, sizeof(long_double));  addressed( rev1, "rev1 not allocated", v->row);
  for (i=0;i<v->row;i++) {*(rev1+i)=*(v->head+i); if(fabsl(*(rev1+i))<1.0E-6) *(rev1+i)=0.0;}
  
  h1=help->head;
  for (ii=0; ii<counter; ii++)
  {
    h2  =(long_double**)calloc(1,sizeof(long_double*));  addressed( h2,   "h2 not allocated (1)", 1 );
    rev2=(long_double*) calloc(1,sizeof(long_double) );  addressed( rev2, "rev2 not allocated (2)", 1);
    r2=0;
    f1=0;
    for (i=0; i<r1; i++) 
      if ((*(*(h1+i)+ii))&&(!(*(rev1+i))))
        {f1=1;break;}
    if (f1) /* reversible row */
    {
     // r2=0; 06.03.01
     h2 = (long_double**) realloc(h2, (r2+1)*sizeof(long_double*)); addressed(h2, "h2 realloc (1)", r2+1);
     {*(h2+r2)  =(long_double*)calloc(c, sizeof(long_double));    addressed( *(h2+r2), "*(h2+r2) not allocated (6)", c); }
     rev2 = (long_double*) realloc(rev2, (r2+1)*sizeof(long_double)); addressed( rev2, "rev2 realloc (2)", (r2+1));
     for (u=0; u<r1; u++) 
     {
      if (*(*(h1+u)+ii)==0)
      {
       *(rev2+r2)=*(rev1+u);
       for (uu=0; uu<c; uu++) *(*(h2+r2)+uu)=*(*(h1+u)+uu);
       r2++;
       h2 = (long_double**) realloc(h2, (r2+1)*sizeof(long_double*)); addressed(h2, "h2 realloc (1)", r2+1);
       {*(h2+r2)  =(long_double*)calloc(c, sizeof(long_double));    addressed( *(h2+r2), "*(h2+r2) not allocated (6)", c); }
       rev2 = (long_double*) realloc(rev2, (r2+1)*sizeof(long_double)); addressed( rev2, "rev2 realloc (2)", (r2+1));
       // if(r2>r02) {printf("error1"); _getch(); exit(1);}
      }
     }
     for (u=i+1; u<r1; u++)
     {
      if ((*(*(h1+u)+ii))&&(!(*(rev1+u)))) /* rev rev combinations */
      {
       *(rev2+r2)=0.0; k1=*(*(h1+i)+ii); k2=*(*(h1+u)+ii);
       abs_hi=(*(*(h1+i)+ii)>=0 ? *(*(h1+i)+ii) : -*(*(h1+i)+ii)); // FM
       abs_hu=(*(*(h1+u)+ii)>=0 ? *(*(h1+u)+ii) : -*(*(h1+u)+ii)); // FM
       ggt_Erg=ggt(abs_hi,abs_hu);  // FM
       k1/=ggt_Erg; k2/=ggt_Erg;    // FM
       for (uu=0;uu<c;uu++) 
       {
        numerical_array(k2,*(*(h1+i)+uu),k1,*(*(h1+u)+uu), 0, 1, -1, "loop 1");
        *(*(h2+r2)+uu)=k2**(*(h1+i)+uu)-k1**(*(h1+u)+uu);
        if(fabsl(*(*(h2+r2)+uu))<1.0E-6) *(*(h2+r2)+uu)=0.0;
        else *(*(h2+r2)+uu)=rund(*(*(h2+r2)+uu), 0);
       }
       r2++;
       h2 = (long_double**) realloc(h2, (r2+1)*sizeof(long_double*)); addressed(h2, "h2 realloc (1)", r2+1);
       {*(h2+r2)  =(long_double*)calloc(c, sizeof(long_double));    addressed( *(h2+r2), "*(h2+r2) not allocated (6)", c); }
       rev2 = (long_double*) realloc(rev2, (r2+1)*sizeof(long_double)); addressed( rev2, "rev2 realloc (2)", (r2+1));
       // if(r2>r02) {printf("error2"); _getch(); exit(1);}
      }
     }
     for (u=0; u<r1; u++) 
     {
      if (*(*(h1+u)+ii)&&(*(rev1+u))) /* rev irrev combinations */
      {
       *(rev2+r2)=1.0; k1=*(*(h1+i)+ii); k2=*(*(h1+u)+ii);
       abs_hi=(*(*(h1+i)+ii)>=0 ? *(*(h1+i)+ii) : -*(*(h1+i)+ii)); // FM
       abs_hu=(*(*(h1+u)+ii)>=0 ? *(*(h1+u)+ii) : -*(*(h1+u)+ii)); // FM
       ggt_Erg=ggt(abs_hi,abs_hu);  // FM
       k1/=ggt_Erg; k2/=ggt_Erg;    // FM
       if (k1*k2<0)
       for (uu=0;uu<c;uu++)
       {
        numerical_array( k2, *(*(h1+i)+uu), k1, *(*(h1+u)+uu), 1, 1, 1, "loop 2"); 
        *(*(h2+r2)+uu)=fabsl(k2)* *(*(h1+i)+uu)+ fabsl(k1)* *(*(h1+u)+uu); 
        if(fabsl(*(*(h2+r2)+uu))<1.0E-6) *(*(h2+r2)+uu)=0.0;
        else *(*(h2+r2)+uu)=rund(*(*(h2+r2)+uu), 0);
       }
       if (k1*k2>=0) 
       for (uu=0;uu<c;uu++)
       {
        numerical_array( k2, *(*(h1+i)+uu), k1, *(*(h1+u)+uu), 1, -1, 1, "loop 3");
        *(*(h2+r2)+uu)=-fabsl(k2)**(*(h1+i)+uu)+fabsl(k1)**(*(h1+u)+uu);
        if(fabsl(*(*(h2+r2)+uu))<1.0E-6) *(*(h2+r2)+uu)=0.0;
        else *(*(h2+r2)+uu)=rund(*(*(h2+r2)+uu), 0);
       }
       r2++;
       h2 = (long_double**) realloc(h2, (r2+1)*sizeof(long_double*)); addressed(h2, "h2 realloc (1)", r2+1);
       {*(h2+r2)  =(long_double*)calloc(c, sizeof(long_double));    addressed( *(h2+r2), "*(h2+r2) not allocated (6)", c); }
       rev2 = (long_double*) realloc(rev2, (r2+1)*sizeof(long_double)); addressed( rev2, "rev2 realloc (2)", (r2+1));
       // if(r2>r02) {printf("error3"); _getch(); exit(1); }
      }
     }
    }
    else /* no reversible row */
    {
     h2 = (long_double**) realloc(h2, (r2+1)*sizeof(long_double*)); addressed(h2, "h2 realloc (1)", r2+1);
     {*(h2+r2)  =(long_double*)calloc(c, sizeof(long_double));    addressed( *(h2+r2), "*(h2+r2) not allocated (6)", c); }
     rev2 = (long_double*) realloc(rev2, (r2+1)*sizeof(long_double)); addressed( rev2, "rev2 realloc (2)", (r2+1));

     // r2=0; 06.03.01
     for (u=0; u<r1; u++)
     {
      if (*(*(h1+u)+ii)==0)
      {
       *(rev2+r2)=*(rev1+u);
       for (uu=0; uu<c; uu++) *(*(h2+r2)+uu)=*(*(h1+u)+uu);
       r2++;
       h2 = (long_double**) realloc(h2, (r2+1)*sizeof(long_double*)); addressed(h2, "h2 realloc (1)", r2+1);
       {*(h2+r2)  =(long_double*)calloc(c, sizeof(long_double));    addressed( *(h2+r2), "*(h2+r2) not allocated (6)", c); }
       rev2 = (long_double*) realloc(rev2, (r2+1)*sizeof(long_double)); addressed( rev2, "rev2 realloc (2)", (r2+1));
       // if(r2>r02) {printf("error4"); _getch(); exit(1);}
      }
     }
     for (i=0; i<r1; i++) 
     {
      if (*(*(h1+i)+ii)>0) 
      {
       for (u=0; u<r1; u++) 
       {
        if (*(*(h1+u)+ii)<0)
        {
         *(rev2+r2)=1.0; k1=*(*(h1+i)+ii); k2=*(*(h1+u)+ii);
         abs_hi=(*(*(h1+i)+ii)>=0 ? *(*(h1+i)+ii) : -*(*(h1+i)+ii)); // FM
         abs_hu=(*(*(h1+u)+ii)>=0 ? *(*(h1+u)+ii) : -*(*(h1+u)+ii)); // FM
         ggt_Erg=ggt(abs_hi,abs_hu);  // FM
         k1/=ggt_Erg; k2/=ggt_Erg;    // FM
         for (uu=0;uu<c;uu++)
         { 
          numerical_array(k2, *(*(h1+i)+uu), k1, *(*(h1+u)+uu), 1, 1, 1, "loop 4");
          *(*(h2+r2)+uu)=fabsl(k2)**(*(h1+i)+uu)+fabsl(k1)**(*(h1+u)+uu); 
          if(fabsl(*(*(h2+r2)+uu))<1.0E-6) *(*(h2+r2)+uu)=0.0;
          else *(*(h2+r2)+uu)=rund(*(*(h2+r2)+uu), 0);
         }
         test1=1;
         for (tu=0;tu<r2;tu++) 
         {
          test1=0;
          for (uu=counter;uu<c;uu++)
          {
           if ((*(*(h2+r2)+uu)==0)&&(*(*(h2+tu)+uu)!=0)) {test1=1;break;}
          }
          if (test1==0) break;
         }
         if (test1==1)
         {
           r2++; 
           h2 = (long_double**) realloc(h2, (r2+1)*sizeof(long_double*)); addressed(h2, "h2 realloc (1)", r2+1);
           {*(h2+r2)  =(long_double*)calloc(c, sizeof(long_double));    addressed( *(h2+r2), "*(h2+r2) not allocated (6)", c); }
           rev2 = (long_double*) realloc(rev2, (r2+1)*sizeof(long_double)); addressed( rev2, "rev2 realloc (2)", (r2+1));
           //if(r2>r02) {printf("error5"); _getch(); exit(1);}
         }
        }
       }
      }
     }
    }
    for(i=0;i<r1;i++) free (*(h1+i)); free (h1); free(rev1); 
    h1=h2; rev1=rev2; r1=r2; //* cf. modes !!!! -> */ r2=r02;
    help->head=h1; help->row=r1; help->col=c;
    for(i=r1; i<=r2; i++)
      free(*(help->head+i));
    ggt_matrix(help);
    printf("result tab %d row %d\n", ii,r1); // _getch();
    // free( *(h2+r2));
    if(!help->row) break;
   }
 free(rev2);   // otherwise memory leak
 help->head=h2; help->row=r2; help->col=c;
 
 //for(i=r2; i>r1; i--) free(help->head+i); 
 if(help->row) 
 { 
   k=cutcol(help,counter);
   freemat(help);
   help=simplify(k);freemat(k);
 }
 switch_if_all_rows_are_minus_into_plus(help);
 return help;
} // basis

/* modes */
struct mat *modes (struct mat *m, struct ::vector *v)
{
 struct mat *help, *k;                       /* help matrices */
 long_double **h1, **h2, abs_hi, abs_hu, ggt_Erg;  /* pointer to current and nex tab */
 long_double *rev1, *rev2=NULL,k1,k2;                           /* corresponding revesibilities */
 int i,u,ii,uu,tu;                           /* counter */
 int r1,r2,c, counter, test1, r01, ok_4=1, r02;

 k=transp(m);help=addi(k);freemat(k);
 h1=help->head;
 r1=(help->row); c=help->col; counter=m->row;
 r01=r2=r1;
 rev1=(long_double*)calloc(v->row,sizeof(long_double));   addressed( rev1, "rev1 not allocated (5.5)", v->row);
 for (i=0;i<v->row;i++) {*(rev1+i)=*(v->head+i);if(fabsl(*(rev1+i))<1.0E-6) *(rev1+i)=0.0;}
 printf("first alloc %d\n", r1 );
 for (ii=0; ii<counter; ii++)
 {
  printf("allocate ");
  rev2=(long_double*) calloc(1, sizeof(long_double) );   addressed( rev2, "rev2 not allocated (7)", 1);
  h2  =(long_double**)calloc(1, sizeof(long_double*));   addressed( h2, "h2 not allocated (6)", 1);
   r2=0;
   for (i=0; i<r1; i++)
    if (!(*(*(h1+i)+ii)))                 /* taking zero rows to the nex tab */
    {
     {h2=(long_double**)realloc(h2, (r2+1)*sizeof(long_double*)); addressed( h2, "h2 not allocated (8)", r2+1); }
     {*(h2+r2)  =(long_double*)calloc(c, sizeof(long_double));    addressed( *(h2+r2), "*(h2+r2) not allocated (6)", c); }
     rev2=(long_double*) realloc(rev2, (r2+1)*sizeof(long_double) );   addressed( rev2, "rev2+r2 not allocated (7) in realloc", r2+1);
     *(rev2+r2)=*(rev1+i);
     for(uu=0;uu<c;uu++) *(*(h2+r2)+uu)=*(*(h1+i)+uu);
     printf("%d ", ++r2);
    }else;
    r02=r2;
    ok_4=1;
    for (i=0; i<r1; i++) 
     if ((*(*(h1+i)+ii))&&(!(*(rev1+i)))) /* reversible combinations */
     {
      for (u=i+1; u<r1; u++)
       if ((*(*(h1+u)+ii))&&(!(*(rev1+u))))
       {
        if(ok_4) // FM 23.07.2001 while(r02<=r2) /* 1 */
        {h2=(long_double**)realloc(h2, (r2+1)*sizeof(long_double*)); addressed( h2, "h2 not allocated (8)", r2+1);
         *(h2+r2)  =(long_double*)calloc(c, sizeof(long_double));    addressed( *(h2+r2), "*(h2+r2) not allocated (6)", c);
         rev2=(long_double*) realloc(rev2, (r2+1)*sizeof(long_double) );   addressed( rev2, "rev2+r2 not allocated (7) in realloc", r2+1);
         r02++;
        }
        *(rev2+r2)=0.0; k1=*(*(h1+i)+ii); k2=*(*(h1+u)+ii);
        abs_hi=(*(*(h1+i)+ii)>=0 ? *(*(h1+i)+ii) : -*(*(h1+i)+ii)); // FM
        abs_hu=(*(*(h1+u)+ii)>=0 ? *(*(h1+u)+ii) : -*(*(h1+u)+ii)); // FM
        ggt_Erg=ggt(abs_hi,abs_hu);  // FM
        k1/=ggt_Erg; k2/=ggt_Erg;    // FM
        for (uu=0;uu<c;uu++)
        {  
           numerical_array( k2, *(*(h1+i)+uu), k1, *(*(h1+u)+uu), 0, 1, -1, "loop 007");
           *(*(h2+r2)+uu)=k2**(*(h1+i)+uu)-k1**(*(h1+u)+uu);
           *(*(h2+r2)+uu)=rund(*(*(h2+r2)+uu), 0);
        }

        test1=1;
        for (tu=0;tu<r2;tu++)
        {
         test1=0;
         for (uu=counter;uu<c;uu++)
         {
           // if ( (*(*(h2+tu)+uu)==0.0) && (*(*(h2+r2)+uu)!=0.0)) {test1=1;break;} // Hier ist der Fehler Sascha 26.09.2001 Seiten sind vertauscht
           if ( (*(*(h2+r2)+uu)==0.0) && (*(*(h2+tu)+uu)!=0.0)) {test1=1;break;}    // Diese Zeile ist aus meta3.5.1_int.cpp ьbernommen 26.09.2001
         }
         if (test1==0) break; // 06.03.01
        }
        if (test1==1){
         printf("%d ", ++r2); /* 1 */
         ok_4=1; // FM 23.07.2001
        }else ok_4=0;
       } // for u
     }  // for i

    for (i=0; i<r1; i++) 
     if ((*(*(h1+i)+ii))&&(!*(rev1+i))) /* rev-irrev combinations */
     {
      for (u=0; u<r1; u++) 
       if ((*(*(h1+u)+ii))&&(*(rev1+u)))
       {
         if(ok_4) // FM 23.07.2001 while(r02<=r2) /* 22 */
         {h2=(long_double**)realloc(h2, (r2+1)*sizeof(long_double*));     addressed( h2, "h2 not allocated (8)", r2+1); 
          *(h2+r2)  =(long_double*)calloc(c, sizeof(long_double));        addressed( *(h2+r2), "*(h2+r2) not allocated (6)", c);
          rev2=(long_double*) realloc(rev2, (r2+1)*sizeof(long_double) ); addressed( rev2, "rev2+r2 not allocated (7) in realloc", r2+1);
         r02++;
         } 
         *(rev2+r2)=1.0; k1=(*(*(h1+i)+ii)); k2=(*(*(h1+u)+ii));
         abs_hi=(*(*(h1+i)+ii)>=0 ? *(*(h1+i)+ii) : -*(*(h1+i)+ii)); // FM
         abs_hu=(*(*(h1+u)+ii)>=0 ? *(*(h1+u)+ii) : -*(*(h1+u)+ii)); // FM
         ggt_Erg=ggt(abs_hi,abs_hu);  // FM
         k1/=ggt_Erg; k2/=ggt_Erg;    // FM
         if (k1*k2<0.0)
           for (uu=0;uu<c;uu++)
           {
             numerical_array( k2, *(*(h1+i)+uu), k1, *(*(h1+u)+uu), 1, 1, 1, "loop5");
             *(*(h2+r2)+uu)=fabsl(k2)**(*(h1+i)+uu)+fabsl(k1)**(*(h1+u)+uu);
             *(*(h2+r2)+uu)=rund(*(*(h2+r2)+uu), 0);
           }
         if (k1*k2>0.0) // ooo
           for (uu=0;uu<c;uu++)
           {
             numerical_array( k2, *(*(h1+i)+uu), k1, *(*(h1+u)+uu), 1, -1, 1, "loop 6");
             *(*(h2+r2)+uu)=-fabsl(k2)**(*(h1+i)+uu)+fabsl(k1)**(*(h1+u)+uu);
             *(*(h2+r2)+uu)=rund(*(*(h2+r2)+uu), 0);
           }
         test1=1;
         for (tu=0;tu<r2;tu++)
         {
          test1=0;
          for (uu=counter;uu<c;uu++)
          {
           if ((*(*(h2+r2)+uu)==0.0)&& (*(*(h2+tu)+uu)!=0.0)) {test1=1;break;}
          }
          if (test1==0) break;
         }
         if (test1==1){
           printf("%d ", ++r2); /* 2 */
           ok_4=1; // FM 23.07.2001
         }else ok_4=0; // FM 23.07.2001
       } // for u
     } // for i
    for (i=0; i<r1; i++) 
     if ((*(*(h1+i)+ii)>0.0)&&(*(rev1+i))) /* irrev combinations */
     {
      for (u=0; u<r1; u++) 
       if ((*(*(h1+u)+ii)<0.0)&&(*(rev1+u)))
       {
        if(ok_4) // FM 23.07.2001 while(r02<=r2) /* 333 */
        {h2=(long_double**)realloc(h2, (r2+1)*sizeof(long_double*));     addressed( h2, "h2 not allocated (8)", r2+1);
         *(h2+r2)  =(long_double*)calloc(c, sizeof(long_double));        addressed( *(h2+r2), "*(h2+r2) not allocated (6)", c);
         rev2=(long_double*) realloc(rev2, (r2+1)*sizeof(long_double) ); addressed( rev2, "rev2+r2 not allocated (7) in realloc", r2+1);
         r02++;
        }
        *(rev2+r2)=1.0; k1=fabsl(*(*(h1+i)+ii)); k2=fabsl(*(*(h1+u)+ii));
        abs_hi=(*(*(h1+i)+ii)>=0 ? *(*(h1+i)+ii) : -*(*(h1+i)+ii)); // FM
        abs_hu=(*(*(h1+u)+ii)>=0 ? *(*(h1+u)+ii) : -*(*(h1+u)+ii)); // FM
        ggt_Erg=ggt(abs_hi,abs_hu);  // FM
        k1/=ggt_Erg; k2/=ggt_Erg;    // FM
        for (uu=0;uu<c;uu++)
        {
          numerical_array( k2, *(*(h1+i)+uu), k1, *(*(h1+u)+uu), 0, 1, 1, "loop 7");
          *(*(h2+r2)+uu)=k2**(*(h1+i)+uu)+k1**(*(h1+u)+uu);
          *(*(h2+r2)+uu)=rund(*(*(h2+r2)+uu), 0);
        }
        test1=1;
        for (tu=0;tu<r2;tu++)
        {
         test1=0;
         for (uu=counter;uu<c;uu++)
         {
          if ((*(*(h2+r2)+uu)==0.0)&&(*(*(h2+tu)+uu)!=0.0)) {test1=1;break;}
         }
         if (test1==0) // than all zeros containing in r2 too in tu; condition (4) in www2.bioinf.mdc-berlin.de/metabolic/metatool/algorithm.pdf
            break;
        }
        if (test1==1){
          printf("%d ", ++r2); /* 3 */
          ok_4=1; // FM 23.07.2001
        }else ok_4=0; // FM 23.07.2001
       } // for u
     } // for i
    // printf("release %d\t", r01);
    for(i=0;i<r01;i++) free (*(h1+i)); free (h1); free(rev1); 
    h1=h2; rev1=rev2; r1=(r2<=r02?r2:r02); r01=r02; // r02
    printf("\nresult modes tab %d row %d\n",ii,r1);
    help->head   = h1;   help->row  =r1; help->col=c;
    // control_condition7( help, ii, rev1, &r1 ); // r1=help->row; /* FM 20.09.2000 condition (7) in www2.bioinf.mdc-berlin.de/metabolic/metatool/algorithm.pdf */
    r2=r1; // FM 23.07.2001
    ggt_matrix( help );
    // print_mat(help);
    if(!help->row) break; // 06.03.01
 } // for ii
 control_condition7( help, 0, rev1, &r1 ); // function moved into this source code line on 01.10.2002 cf. condition (7) in www2.bioinf.mdc-berlin.de/metabolic/metatool/algorithm.pdf */
 free(rev2); // otherwise memory leak FM (detected using the unix software purify)
 while(r02>r1)
   {free(*(h1+(r02-1))); r02--;}
 help->head=h1; help->row=r1; help->col=c;
     ggt_matrix(help); // FM 15.06.2001
 if(help->row)
 {
   k=cutcol(help,counter); 
   freemat(help);
   help=simplify(k);
   freemat(k);
   // print_mat(help);
 }
 switch_if_all_rows_are_minus_into_plus(help);
 return help;
} // modes

/* ************************************************************************************ */        
/* 3. other functions ***************************************************************** */
/* ************************************************************************************ */
/* function compstr */
  int compstr (int m, char t1[100], char t2[100])
    	  {
    	  int i;
    	  int k=1;
    	  for (i=0; i<m; i++)
      		  {
      		  if ((t1[i]=='\0')&&(t2[i]=='\0')) break;
      		  if (t1[i]!=t2[i]) {k=0; break;}
      		  }
    	  return k;
    	  }

inline long_double ldmod( long_double u, long_double v);
/* function biggest common divisor */
long_double ggt1 (long_double u, long_double v)
{
 long_double t;

 u=fabsl(u); v=fabsl(v);
 if     (!(u*v))           return(u+v);
 else if(u==v)             return u;
 else if(u==1.0 || v==1.0) return ((long_double)1.0);
 printf("");
 while (u>0)
 {
   if (u<v)
   {t=u; u=v; v=t;}
   u = (long_double)ldmod(u,v);
 }
 return (v);
} /* ggt */

long_double ggt (long_double u, long_double v)
{
 long_double t, r;

 u=fabsl(u); v=fabsl(v);
 if     (!(u*v))           return(u+v);
 else if(u==v)             return u;
 else if(u==1.0 || v==1.0) return ((long_double)1.0);
 if (u<v)
 {t=u; u=v; v=t;}
 r=u;
 while (r>0)
 {
   u = rund(u, 0); v=rund(v, 0);
   r = (long_double)ldmod(u,v);
   u=v;  v=r;
 }
 return (u);
} /* ggt */


void ggt_matrix (struct mat *v)
{
 // divide each row of the new tableau through the greatest common denominator of this row
 long_double denominator, abs1;
 int c, r, y_n;
 //*->*/    printf("v r%d x c%d", v->row, v->col);   print_mat( v );
 // 23.07.2001 goto FINI;
 for(r=0; r<v->row; r++) 
 {
  // for(j=0; j<v->col; j++) if( *(*(v->head+r)+j) == 1 || *(*(v->head+r)+j) == -1) goto NEXT_ROW;
  denominator=1.0;y_n=1;
  for(c=0; c<v->col; c++) 
  { 
   abs1 = fabsl(*(*(v->head+r)+c));
   /**/ if(abs1==1.0)
    {denominator=1.0; break;}
   else /**/ if(abs1) 
   { if(y_n==1)denominator=abs1; // this command is for initialisation of denominator 
     else denominator=ggt(denominator, abs1);
     y_n=0;
   } 
  }
  /**/if(denominator > 1)
   for( c=0; c<v->col; c++)
    if(*(*(v->head+r)+c)) 
      *(*(v->head+r)+c)/=denominator;
  // NEXT_ROW:;
 }
 //*->*/    printf("v r%d x c%d", v->row, v->col);   print_mat( v );
 // FINI:;
} // ggt_matrix

void ggt_matrix_simple_algorithm (struct mat *v) //(long_double *v, int col )
{
 long_double denominator, min, i, abs1;
 int c, r, counter, non_zero;
 //*->*/    printf("v r%d x c%d", v->row, v->col);   print_mat( v );
 for(r=0; r<v->row; r++) 
 {
   min = LDBL_MAX;denominator=0;
   for(c=0; c<v->col; c++) 
   { 
     abs1=fabsl(*(*(v->head+r)+c));
     if(abs1) min = abs1<min? abs1 : min; else;
   }
   i=2.0;
   while(i<=min)
   { 
    for( non_zero=counter=c=0; c<v->col; c++ )
     if(*(*(v->head+r)+c) ) 
     {
       if( floorl( *(*(v->head+r)+c)/i ) == ceill( *(*(v->head+r)+c)/i )) counter++;
       non_zero++;
     }
    if(counter==non_zero) denominator=i;
    i++;
   };
  if(denominator)
   for( c=0; c<v->col; c++)
    if(*(*(v->head+r)+c)) *(*(v->head+r)+c)/=denominator;
 }
 //*->*/    printf("v r%d x c%d", v->row, v->col);   print_mat( v );
} // ggt_matrix

int delete_zero_line(struct mat *a, int zero_line)
{
 int i, j;
 
 if(zero_line<a->row-1)
  for( i=zero_line; i<a->row; i++ )
   for( j=0; j<a->col; j++ )
     *(*(a->head+i)+j) = *(*(a->head+i+1)+j);
 free( *(a->head+(a->row-1))); 
 a->row--;
 return 0;
} //delete_zero_line

inline long_double ldmod( long_double u, long_double v)
{
  
  if(u<0 || v<0 ) 
     printf("shit");
  if(u==v)  
    return 0;
  else if(u>v)  
    return (u-floorl(u/v)*v);
  else 
    // return (v-floorl(v/u)*u);
    return u;
}

// genreate file names for input and output files
int get_filein_fileout(int argn, char** files_in_out, char f[3][_MAX_PATH] )
{
 if( argn != 3 ) 
 {
   printf("\nMETATOOL [path]inputfile [path]outputfile\n\n");
   printf("\nFilenames as program parameters are not found\n\n");
   printf("\nPlease type in the source file name with metabolic reaction eqations:\n");
   printf("\n-> "); scanf("%s", f[1]);
   printf("\nand type in the file name for METATOOL output:\n");
   printf("\n-> "); scanf("%s", f[2]);
 }
 else 
 {  
   strcpy( f[1], files_in_out[1] );
   strcpy( f[2], files_in_out[2] );
 }
 if( !strcmp(f[1], f[2]) )
 {/*printf("\nThe name of input file\n%s\nis the same as the name of the output file\n%s\nProgram prematurely finished.", f[1], f[2]); _getch(); */exit(14); }
 strcpy( f[0], files_in_out[0] ); 
 return 0;
} // get_filein_fileout

int detect_double_declared_enzymes( struct enc *list )
{
  struct enc *a, *b;

  a=list;
  do{
   a=a->next;
   b=a->next;
   while( b!=b->next)
   {
     if( !strcmp(a->txt, b->txt)) 
     { 
       /*printf("\nEnzyme ->%s<- is delared twice", a->txt );
       if     ( !a->rev && !b->rev) printf(" as reversible.");
       else if(  a->rev &&  b->rev) printf(" as irreversible.");
       else                          printf(" as reversible and as irreversible.");
       printf("\nplease press ENTER"); _getch(); printf("\r                  \r");
       printf("\nProgram prematurely finished.\n"); */exit(15); // FM 01.08.2002
     }
     b=b->next;
   }
  }while(a != a->next);

  return 0;
} // detect_double_declared_enzymes

int detect_double_declared_metabolites( struct enc *metlist )
{
 int int_ext1, int_ext2, i, j, met_counter=0;
 struct enc *ml1;
 struct enc *ml2;

 ml1=metlist;  
 
 while(ml1!=ml1->next)
 { met_counter++; ml1=ml1->next;}
 
 ml1=metlist->next; int_ext1=ml1->rev;

  for(i=0; i<met_counter-1; i++)
  {
   ml2=ml1->next; int_ext2=ml2->rev;
   for(j=i+1; j<met_counter; j++ )
   {
    if(!strcmp(ml1->txt, ml2->txt))
    {
     /*printf("\nMetabolite ->%s<- is declared twice ", ml1->txt);
     if     ( !int_ext1 && !int_ext2 ) printf("as internal.");
     else if(  int_ext1 &&  int_ext2 ) printf("as external.");
     else                            printf("as internal and external.");
     printf("\nPlease change your input file.");
     printf("\nplease press ENTER"); _getch(); printf("\r                  \r");*/ 
     return 1;
    }
    ml2=ml2->next;int_ext2=ml2->rev;
   } // for j
   ml1=ml1->next; int_ext1=ml1->rev;
  } // for i
  return 0;
} // detect_double_declared_metabolites

int control_modi (struct mat *m, FILE *savefile, struct enc *enzlist)
{
 int r1, r2, k, kk, n=0, zero_lines=0, not_zero_r1=0, not_zero_r2=0, same=0;
 struct enc *enznames;

 for(r1=0; r1<m->row-1; r1++ )
 {
   for(r2=r1+1; r2<m->row; r2++)
   {
     not_zero_r1=not_zero_r2=same=0;
     for( k=0; k<m->col; k++)
     {  
       if(*(*((m->head)+r1)+k)) not_zero_r1++;
       if(*(*((m->head)+r2)+k)) not_zero_r2++;
       if(*(*((m->head)+r1)+k) && *(*((m->head)+r2)+k) ) same++;
     }
     if( (same == not_zero_r1 || same == not_zero_r2) && same && not_zero_r1 && not_zero_r2 )
     {  
       if(not_zero_r2 > not_zero_r1)     printf("\nElementary mode %d (%d) contains elementary mode %d (%d).", r2+1, not_zero_r2, r1+1, not_zero_r1 );
       else if(not_zero_r2==not_zero_r1) printf("\nElementary mode %d (%d) and %d (%d) are identical.", r2+1, not_zero_r2, r1+1, not_zero_r1 );
       else                              printf("\nElementary mode %d (%d) contains elementary mode %d (%d).", r1+1, not_zero_r1, r2+1, not_zero_r2 );

       if(not_zero_r2 > not_zero_r1)     fprintf(savefile, "\nElementary mode %d (%d) contains elementary mode %d (%d).", r2+1, not_zero_r2, r1+1, not_zero_r1 );
       else if(not_zero_r2==not_zero_r1) fprintf(savefile, "\nElementary mode %d (%d) and %d (%d) are identical.", r1+1, not_zero_r1, r2+1, not_zero_r2 );
       else                              fprintf(savefile, "\nElementary mode %d (%d) contains elementary mode %d (%d).", r1+1, not_zero_r1, r2+1, not_zero_r2 );
       n=1;
     }
   }
 }
 if(n) { printf("\n");fprintf(savefile, "\n"); }
 // control which enzymes not occur in modes
 enznames = enzlist;
 for(kk=k=0, r1=0; r1<m->col; r1++ )
 {
  enznames = enznames->next;
  for(r2=0; r2<m->row; r2++)
  {
   if(*(*((m->head)+r2)+r1)) {k++; break;}
  }
  if(r2==m->row) 
     if(!(kk++)) fprintf(savefile, "\n%s\t", enznames->txt);
     else fprintf(savefile, "%s\t", enznames->txt);
 }
 if(k!=m->col) 
  if(m->col-k > 1) fprintf(savefile, "\n%d enzymes are not involved in reactions.\n", m->col-k ); 
  else             fprintf(savefile, "\n%d enzyme is not involved in reactions.\n", m->col-k ); 
 return 0;
} // control_modi

int stack_down(struct mat *m, int r1 )
{
 // matrix h2 overwrite the line r1 with the following line and so on
 int i, j, col;
 for(i=r1; i<m->row-1; i++ )
 {
   for( j=i+1, col=0; col<m->col; col++)
       *(*(m->head+i)+col) = *(*(m->head+j)+col);
 }
 // do not free( *(m->head+j) ); that memory is freed in modes with the variable r01 at line 1590
 m->row--;
 return 0;
} // stack_down
int stack_down_rev(long_double *rev1, int r1, int row )
{
 // vector rev1 stack_down
 int i, j;
 // realloc ???
 for(i=r1; i<row-1; i++ )
 {
   for( j=i+1; j<row; j++)
       *(rev1+i) = *(rev1+j);
 }
 // do not free( *(m->head+j) ); that memory is freed in modes with the variable r01 at line 1590
 return 0;
} // stack_down_rev

int control_condition7 (struct mat *m, int ii, long_double *rev1, int *r01)
{
 int r1, r2, c, zero_r1, zero_r2, same12=0;
 // struct vector hv;
  
 if(m->row>1) // changed in comparison to meta3.5_int FM 23.07.2001
 for(r1=0; r1<m->row-1; r1++ )
 {
   for( zero_r1=0, c=ii; c<m->col; c++)
       if(!*(*(m->head+r1)+c)) zero_r1++;
   for(r2=r1+1; r2<m->row; r2++)
   {
     zero_r2=same12=0;
     for( c=ii; c<m->col; c++)
     {  
       if(!*(*(m->head+r2)+c)) zero_r2++;  // FM 04.02.2002
       if(!*(*(m->head+r1)+c) && !*(*(m->head+r2)+c) ) same12++;
     }
     if(zero_r1 == same12) // changed in comparison to meta3.5_int FM 23.07.2001
      // if( *(rev1+r1) && *(rev1+r2) )// this line must not staying!! - Changed in comparison to meta3.5_int FM 23.07.2001 // FM 04.02.2002 deletet; 04.09.2002 reanimated
      {  
       //*->*/ printf("\nr1=%d\tr2=%d\tm->row=%d\t\trev1=%d\trev2=%d", r1, r2, m->row, *(rev1+r1), *(rev1+r2));
       //*->*/ hv.head=*(m->head+r1); hv.row=m->col; vectoroutput(&hv); 
       //*->*/ hv.head=*(m->head+r2); hv.row=m->col; vectoroutput(&hv); printf("\n");
       stack_down(m, r1);
       stack_down_rev(rev1, r1, m->row+1 );
       printf("Condition 7: line %d deleted.\n", r1); 
       r1--; break;
      }
   }
 }
 (*r01)=m->row; // changed in comparison to meta3.5_int FM 23.07.2001
 return 0;
} // control_condition7

int unbalanced_internal_mets(struct mat *n, FILE *fout, struct enc *metlist)
{
 int r, c, counter, y_n=1;
 struct enc *help;

 help = metlist->next;
 for(r=0; r<n->row; r++)
 {
   counter=0;
   for(c=0; c<n->col; c++)
     if( *(*(n->head+r)+c) ) counter++;
   if( counter==1 )
   {
     if(y_n)
     {  printf(       "\nNOT BALANCED INTERNAL METABOLITES\n\n" );
        fprintf(fout, "\nNOT BALANCED INTERNAL METABOLITES\n\n" );
     } 
     y_n=0;
     printf(       "%s\n", help->txt );
     fprintf(fout, "%s\n", help->txt );
   }
   help = help->next;
 } // for r
 return 0;
} // unbalanced_internal_mets

// file output for conservation relation equations
void crel_equation_output(FILE* fout, struct mat* crel, struct enc* metlist)
{
 struct enc* ml;
 int col, row, k, cr=0;
 
  // look if all of the rows are set only with zero
 for(row=0; row<crel->row; row++)
 {
  ml=metlist;
  for( k=col=0; col<crel->col; col++ )
  {
    if( *(*(crel->head+row)+col) )
    {
      if(k++)
       cr=1;
      else
       cr=1;
    }
    if(cr)break;
    ml=ml->next;
  }
  if(cr)break;
 } // then skip the following loop

 if(cr)
 for(row=0; row<crel->row; row++)
 {
  fprintf( fout, "%2d :\t", row+1);
  ml=metlist;
  for( k=col=0; col<crel->col; col++ )
  {
    if( *(*(crel->head+row)+col) )
    {
      if(k++)
      {
       if(crel->head[row][col]==1.0)
         fprintf(fout, " + %s", ml->next->txt);
       else if(crel->head[row][col]==-1.0)
         fprintf(fout, " - %s", ml->next->txt);
       else if(crel->head[row][col] > 1.0)
         fprintf(fout, " + %lg %s", crel->head[row][col], ml->next->txt);
       else if(crel->head[row][col] < -1.0)
         fprintf(fout, " - %lg %s", (-1)*(crel->head[row][col]), ml->next->txt);
      }
      else
      {
       if( crel->head[row][col] == 1.0 )
         fprintf(fout, "%s", ml->next->txt);
       else if( crel->head[row][col] == -1.0 )
         fprintf(fout, "-%s", ml->next->txt);
       else if( fabs(crel->head[row][col]) != 1.0 )
         fprintf(fout, "%lg %s", crel->head[row][col], ml->next->txt);
      }
    }
    ml=ml->next;
  }
  if(k) fprintf(fout, " = const\n");
 }
} // crel_equation_output


// +++++++++++++++++ here starts the blockdiagonalisation (should written in an separate file)
// the source code is the same as bldi3.cpp - integration in metatool 11.02.2002
// C program BLOCDIAG for computing and block-diagonalizing the null-space 
// matrices to stoichiometry matrices of reaction systems. 
// For the underlying theory see S. Schuster and R. Schuster:
// Detecting Strictly Detailed Balanced Subnetworks In Open Chemical 
// Reaction Networks. J. Math. Chem. 6, 1991, 17-40.
// or R. Heinrich and S. Schuster: The Regulation of Cellular Systems,
// Chapman & Hall, New York 1996, ch. 3.2.2
// The program was developed by F. Moldenhauer and S. Schuster (Berlin) 
// based on a Pascal source code published in the above-mentioned
// J. Math. Chem. paper. 
// 
// Input and output of stoichiometry matrix, translation from pascal to c,
// and changing static arrays in dynamic ones were programmed by Ferdinand 
// Moldenhauer, Sept. 1999
// Use of integer variables instead of real numbers and reduction by
// greatest common denominator were introduced by Stefan Schuster, Dec. 1999

// Compile this source code with model compact (far pointers) for DOS 
// application; edit unnecessary commands for UNIX application

// changes since: 17.07.2001
// read the stoichiometric matrix containing the names of rows and names of columns
// do not forget!!! give the number of external metaboliteson the command line  !!!


/*
#define TRUE  1
#define FALSE 0

int    *c,*ca,*cb, *l, z;
int    f, h, i, j, k, m, maxx, n, q, r, rho, rank, product, prod_old;
int    *a, *b, *p;
int    exchange, null_row, null_column;
char   matrix[3]; 
*/


/* ************************************************************************************ */        
/* MAIN *****************************************************************************+* */
/* ************************************************************************************ */
int main(int argn, char *files_in_out[])
{
    FILE          *src,*fout;         	   /* input/output file */
    struct enc    *enzlist, *metlist;	   /* linked lists of metabolites and enzymes */
    struct mat    *help;			       /* matrix for internal operations */
    struct mat    *nex, *n, *nred, *tnex;  /* stoich. matrix, reduced, with ext. met */
    struct mat    *vkernel, *vsub;	   	   /* kernel, subsets */
    struct mat    *crel;			       /* conservation relations */
    struct mat    *vbasis, *vmode;		   /* basis, modes */
    struct mat    *unreduced, *unreduced1, *overall, *overall1;  // unreduced1 for comparing vbasis and elementary modes
    struct ::vector *rev, *redrev, *met;
    int i, ii, vb=1, **wrong_subset;
    char f[3][_MAX_PATH]; 
    time_t start, finish;
    time(&start);
    char          *tmp_file_name=NULL;     /* for a temporary file without commentaries */
    FILE          *fp1;                    /* to control the programmer that the metatool inputfile cannot be deletet */

	char* buffer;

	// Get the current working directory:
#ifdef _WIN32
    if ((buffer = _getcwd(NULL, 0)) == NULL)
        perror("_getcwd error");
    else
    {
        printf("%s \nLength: %d\n", buffer, strlen(buffer));
        free(buffer);
    }
#else
    if ((buffer = getcwd(NULL, 0)) == NULL)
        perror("_getcwd error");
    else
    {
        printf("%s \nLength: %d\n", buffer, strlen(buffer));
        free(buffer);
    }
#endif
    

#if (defined DOS || defined _DOS || defined WIN32 || defined _WIN32)
    char t1[128], t2[128];
    struct _timeb tstruct1, tstruct2;    
    _strtime(t1); _ftime( &tstruct1 );
    // printf( "Starting time:\t\t\t\t%s : %u\n", t1, tstruct1.millitm );
#endif
    
    branch=(struct ::vector*)0;

/* genreate file names for input and output files ****************** */
    get_filein_fileout(argn, files_in_out, f );

/* INPUT *********************************************************** */  
    fp1=src=fopen(f[1],"r"); 
	if (src==NULL) {/*perror("file error!"); printf("%s ", f[1]); _getch(); */
		return 18;}  

    src = filter_comment(src, &tmp_file_name);
    // fill in the global array char **metabolites and int met_counter;
    get_metabolites_from_equations( src );

    enzlist=getenzlist(src); 
      rev=getrev(enzlist);
      detect_double_declared_enzymes(enzlist);
    
    metlist=getmetlist(src);
      met=getrev(metlist);

    // if it is wanted a control file
    // save_frequency_of_metabolites(f[1], f[2], metlist);

    if( met_counter!=met->row)
    {/*printf("\nThere are %d metabolites in the stoichiometric equations\nand %d are declared as -METINT or -METEXT\n\nProgram prematurely terminated.", met_counter, met->row);
     mets_which_differ( metlist, met->row );
     if(met_counter<met->row) printf("\n%d metabolite(s) is/are more than one times declared.",met->row-met_counter);printf("\nPlease press ENTER"); _getch();*/ exit(16);}

/* GENERATE THE STOICHIOMETRIC MATRICES  NEX AND N ****************** */
    nex=getnex(src,metlist,enzlist); 
    // ggt_matrix(nex);
    //*->*/    printf("nex r%d x c%d", nex->row, nex->col); print_mat(nex);
    // print_met_list(metlist); // for control
    // print_met_list(enzlist); // for control
    n=cutnex(nex,met);
    fclose (src);
    fout=fopen(f[2],"w"); if(!fout){/*perror("fout-error "), printf("%s", f[2]); _getch(); */exit(17);}
    fprintf (fout,"METATOOL OUTPUT (double) Version 4.3 (25 October 2002) %s\n\n", f[0]);
    fprintf(fout, "INPUT FILE: %s\n\n", f[1]);
    fprintf (fout,"INTERNAL METABOLITES: %d\nEXTERNAL METABOLITES: %d\nREACTIONS: %d\n",n->row, nex->row-n->row, n->col);
    save_frequency_of_metabolites_in_output(fout, metlist);
    fprintf (fout,"\nSTOICHIOMETRIC MATRIX\n"); 
    fmatoutput(n,fout);
    fprintf(fout, "The following line indicates reversible (0) and irreversible reactions (1)\n" );
    fvectoroutput (rev,fout);
    fprintf(fout, "rows and columns are sorted as declared in the inputfile\n" ); fflush(fout);

/* DETECT NOT BALANCED INTERNAL METABOLITES IN N ******************* */ 
    unbalanced_internal_mets(n, fout, metlist);

/* GET KERNEL OF N **************************** */ 
    vkernel=kernel(n); 
    fprintf(fout,"\nKERNEL\n"); 
     fmatoutput(vkernel,fout);
     fprintf(fout, "%d reactions (columns) are sorted in the same order as in the ENZREV ENZIRREV section.\n", vkernel->col);
     fenzymeoutput(vkernel,enzlist,fout);
    control_modi(vkernel, fout, enzlist);
    tnex=transp(nex); 
      overall=mult(vkernel,tnex);           fflush(fout);
      foveralloutput (overall,metlist,fout);fflush(fout);
    freemat (overall);

    
/* GET SUBSETS ******************************** */
    wrong_subset=(int**)calloc(1, sizeof(int*)); addressed(wrong_subset, "wrong_subset ", 1);
    vsub=subset(vkernel,rev,wrong_subset);
    //*->*/    printf("vsub r%d x c%d", vsub->row, vsub->col); print_mat(vsub); // _getch();
    freemat(vkernel);
    redrev=subrev(vsub,rev); 
    //*->*/    printf("vsub: r%d x c%d", vsub->row, vsub->col); print_mat(vsub);
    fprintf(fout,"\nSUBSETS OF REACTIONS\n"); fmatoutput (vsub,fout);
    fprintf(fout, "%d reactions (columns) are sorted in the same order as in the ENZREV ENZIRREV section.\n", vsub->col);
    fenzymeoutput(vsub,enzlist,fout);
    overall=mult(vsub,tnex); 
    foveralloutput (overall,metlist,fout);
    freemat (overall);
    for( i=0; i<vsub->row; i++ )
    { 
      if( *(*wrong_subset+i) )
      { /*printf("\n!!! Subset %d with contradictory irreversibility constraints, please check your model.\n", i+1 );
        printf("    Enzymes of that subset are cancelled for the further calculations.\n");
        printf("\nplease press ENTER"); _getch(); printf("\r                  \r");
        fprintf(fout, "\n!!! Subset %d with contradictory irreversibility constraints, please check your model.\n", i+1 );
        fprintf(fout, "    Enzymes of that subset are cancelled for further calculations.\n");*/
        for (ii=0;ii<vsub->col;ii++) *(*(vsub->head+i)+ii)=0;
        // delete_zero_line(vsub, i );
        // delete_corresponding_row_in_rev(redrev, i);
      }
    }
    free(*wrong_subset); free(wrong_subset);
    fflush(fout);

/* REDUCTION OF THE SYSTEM ******************** */
    nred=transp(vsub); 
    help=mult(n,nred); freemat (nred); branch=(struct ::vector*)1;
    nred=simplify(help); freemat (help);
    //*->*/    printf("nred: r%d x c%d",   nred->row, nred->col);    print_mat(nred  );
    if (nred->row==0) {/*printf ("System reduced to zero (simple system)\nEach subset forms an elementary mode\n");fprintf (fout, "System reduced to zero (simple system)\nEach subset forms an elementary mode\n");_getch();*/return 0;}
    fprintf(fout,"\nREDUCED SYSTEM with %d branch point metabolites in %d reactions (columns)\n", nred->row, nred->col); fflush(fout);
    fmatoutput (nred,fout);             fflush(fout);
    fprintf(fout, "The following line indicates reversible (0) and irreversible reactions (1)\n" );
    fvectoroutput (redrev,fout);        fflush(fout);
    fout_branches (fout, metlist);
    freevector    (branch ); branch=(struct ::vector*)NULL; fflush(fout);

/* CONVEX BASIS ******************************* */
    vbasis=basis(nred,redrev);
    //*->*/    printf("vbasis: r%d x c%d", vbasis->row, vbasis->col); print_mat(vbasis);
    //*->*/    printf("nred: r%d x c%d",   nred->row, nred->col);     print_mat(nred  );
    //*->*/    printf("redrev: r%d ",      redrev->row);              vectoroutput(redrev);
    if(!vbasis->row) { fprintf(fout,"\nThere is no convex basis.\n"); goto FINISH;}
    fprintf(fout,"\nCONVEX BASIS\n");  fflush(fout);
    // count zero_rows for convex basis FM
    // {int zero_count, ii; 
    //  for (zero_count=ii=0; ii<vbasis->col; ii++) 
    //  {
    //   for (i=0; i<vbasis->row; i++) 
    //    if( *(*((vbasis->head)+i)+ii)==0.0) zero_count++;
    //   if( zero_count==vbasis->row) 
    //    fprintf(fout, "\n Please check irreversibilities.");
    //   if(zero_count) break;
    //  }
    // }
    //if( (vb=null_mat(vbasis)) )
    {
     // fmatoutput (vbasis,fout);          fflush(fout);
     //*->*/    printf("vsub: r%d x c%d", vsub->row, vsub->col); print_mat(vsub);
     unreduced=mult(vbasis,vsub);
     freemat (vbasis);     
     //*->*/    printf("unreduced: r%d x c%d", unreduced->row, unreduced->col); print_mat(unreduced);
     //*->*/    printf("tnex: r%d x c%d", tnex->row, tnex->col); print_mat(tnex);
     // instead of vbasis
     fmatoutput (unreduced, fout); fflush(fout);
     overall=mult(unreduced,tnex);
     //*->*/    printf("overall r%d x c%d", overall->row, overall->col); print_mat(overall);
     fenzymeoutput(unreduced,enzlist,fout);fflush(fout); 
     foveralloutput (overall,metlist,fout);fflush(fout);
     // freemat(unreduced); /*freemat(overall);*/ 
     fflush(fout);
    }

/* CONSERVATION RELATIONS ********************* */
    help=transp(n);
    crel=kernel(help); freemat(help); // FM
    fprintf(fout,"\nCONSERVATION RELATIONS\n");  
    if(fmatoutput (crel,fout))
       crel_equation_output(fout, crel, metlist);
    else 
       fprintf(fout, "- not found -\n");  // 04.05.2001
    freemat (crel); fflush(fout);

/* ELEMENTARY MODES *************************** */
    //*->*/    printf("nred r%d x c%d", nred->row, nred->col); print_mat(nred);
    //*->*/    printf("redrev: r%d\n", redrev->row);   vectoroutput(redrev);
    vmode=modes(nred,redrev);    
    //*->*/    printf("vmode r%d x c%d", vmode->row, vmode->col); print_mat(vmode); 
    
    fprintf(fout,"\nELEMENTARY MODES\n"); // HMS######### Removed by HMS  
    unreduced1=mult(vmode,vsub);
    // HMS############ Added by HMS to allow output of unreduced elementary modes
	fmatoutput (unreduced1, fout); fflush (fout);    
    fprintf(fout, "%d reactions (columns) are sorted in the same order as in the ENZREV ENZIRREV section.\n", unreduced1->col);
    overall1=mult(unreduced1,tnex);
    //*->*/    printf("vsub: r%d x c%d", vsub->row, vsub->col); print_mat(vsub);
    //*->*/    printf("unreduced1: r%d x c%d", unreduced1->row, unreduced1->col); print_mat(unreduced1);
    //*->*/    printf("overall r%d x c%d", overall1->row, overall1->col); print_mat(overall1);
    ggt_matrix(unreduced1);
    fenzymeoutput(unreduced1,enzlist,fout); 
    ggt_matrix(overall1);
    foveralloutput (overall1,metlist,fout); fflush(fout);
    freemat(vmode);
    control_modi(unreduced1, fout, enzlist);
    // freemat(unreduced1); /* freemat(overall1); */

/* FREE *************************************** */
    fclose(fout);
    freemat(nred);
    freevector(redrev); 
    freemat(vsub);
    freemat(tnex);
    freemat(nex);
    freemat(n);
    // free concatenation lists
    free_met_list( metlist ); free_met_list(enzlist);// FM
    freevector(rev);freevector(met);
    if( met_counter )
    {
      for(i=0; i<met_counter; i++)free(*(metabolites+i));
      free(metabolites); met_counter=0; free(mets_cumul);
    }
    if(vb)
    { 
     additional_em(f[2], unreduced, unreduced1); // overall, overall1);
     freemat(overall);
    }
    freemat(overall1);
    freemat(unreduced); freemat(unreduced1);
FINISH:;
    { // delete the temporary metatool input which contains no commentaries
      char command[101];
// #if (defined DOS || defined _DOS || defined WIN32 || defined _WIN32 || !defined sun)
#if (defined DOS || defined _DOS || defined WIN32 || defined _WIN32) // BGOLI###
      strcpy(command, "del ");
#else
      strcpy(command, "rm ");
#endif
      strcat( command, tmp_file_name );
      if(src!=fp1) system( command );
      if(tmp_file_name) free(tmp_file_name);
    }
    time(&finish);
#if (defined DOS || defined _DOS || defined WIN32 || defined _WIN32)
    _strtime(t2); _ftime( &tstruct2 );
    printf("\nStarting time was   :\t%s : %3u\n", t1, tstruct1.millitm );
    if( tstruct1.millitm > tstruct2.millitm )
    {
      printf( "The current time is :\t%s : %3u\ndiff of millisec    :\t           %3d", t2, tstruct2.millitm, (int)(1000-tstruct1.millitm+tstruct2.millitm) );
      printf( "\nProgram took %6.0f seconds and %3u millisec. ", difftime( finish, start )-1.0, (int)(1000-tstruct1.millitm+tstruct2.millitm) );
    }
    else
    {
      printf( "The current time is :\t%s : %3u\ndiff of millisec    :\t           %3d", t2, tstruct2.millitm, (int)(tstruct2.millitm-tstruct1.millitm) );
      printf( "\nProgram took %6.0f seconds and %3u millisec. ", difftime( finish, start ), (int)(tstruct2.millitm-tstruct1.millitm) );
    }
    // printf("\nProgram correctly finished. -> please press ENTER key "); _getch();
    printf("\nProgram correctly finished. "); // BGOLI###
#else
    printf("\nProgram correctly finished.\n");
    fclose(stdin); fclose(stdout); fclose(stderr);
#endif
    return 0;
} // main
