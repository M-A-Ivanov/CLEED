/***********************************************************************
GH/19.09.95
 File contains:

  sr_po(int ndim, char *bak_file, char *log_file)
 Perform a search according to POWELL'S METHOD

 Driver for routine AMOEBA (From numerical recipes)
 Modified:
 GH/23.08.95
***********************************************************************/

#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <math.h>
#include "search.h"
#include "gh_stddef.h"

/**********************************************************************/

void sr_po(int ndim, char *bak_file, char *log_file)
/***********************************************************************
  Perform a search according to POWELL'S METHOD
***********************************************************************/

{

int i_par, j_par;
int nfunc;

real rmin;
real *p,**xi;

FILE *log_stream;

/***********************************************************************
  POWELL'S METHOD
***********************************************************************/

 if( (log_stream = fopen(log_file, "a")) == NULL) { OPEN_ERROR(log_file); }
 fprintf(log_stream,"=> POWELL'S METHOD\n\n");

/***********************************************************************
  Set up initial direction set
***********************************************************************/
 i_par = 1;
 p = vector(1, ndim);
 xi = matrix(1, ndim,1, ndim);

 if(strncmp(bak_file, "---", 3) == 0)
 {
   fprintf(log_stream,"=> Set up initial direction set:\n");
   fclose(log_stream);

   for(i_par = 1; i_par <= ndim; i_par ++)
   { p[i_par] = 0.; }

   for (i_par = 1; i_par <= ndim; i_par ++)
   {
     for (j_par =1; j_par <= ndim; j_par ++)
       if(i_par == j_par) xi[i_par][j_par] = 1.;
       else               xi[i_par][j_par] = 0.;
   }
 }
 else
 {
/* Read direction set from file */
 }

/***********************************************************************
  Enter search function sr_powell
***********************************************************************/

#ifdef CONTROL
 fprintf(STDCTR,"(sr_po): Enter sr_powell #%i\n", i_par);
#endif

 fprintf(log_stream,"=> Start search (abs. tolerance = %.3e)\n", R_TOLERANCE);
/* close log file before entering the search */
 fclose(log_stream);  

 sr_powell(p, xi, ndim, R_TOLERANCE, &nfunc, &rmin, sr_evalrf);

/***********************************************************************
  Write final results to log file
***********************************************************************/

#ifdef CONTROL
 fprintf(STDCTR,"(sr_po): %d iterations in sr_powell\n", nfunc);
#endif

 if( (log_stream = fopen(log_file, "a")) == NULL) { OPEN_ERROR(log_file); }

 fprintf(log_stream,"\n=> No. of iterations in sr_powell: %3d\n", nfunc);
 fprintf(log_stream,"=> Optimum parameter set:\n");
 for (j_par = 1; j_par <= ndim; j_par++ )
   fprintf(log_stream, "%.6f ", p[j_par]);
 fprintf(log_stream,"\n");

 fprintf(log_stream,"=> Optimum function value:\n");
 fprintf(log_stream,"rmin = %.6f\n", rmin);

 fclose(log_stream);

 free_matrix(xi,1, ndim,1);
 free_vector(p,1);

}  /* end of function sr_po */

/***********************************************************************/
