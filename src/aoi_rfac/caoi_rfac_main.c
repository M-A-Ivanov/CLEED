/*********************************************************************
ZZ/03.03.08
  file contains functions:

  main (03.03.08)
     Main program for CAOI_RFAC_MAIN program 
     (calls crfac program for each angle of incidence)

Changes:

*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "caoi_rfac.h"
#include "gh_stddef.h"
#include "proghelp.h"

int main(int argc, char *argv[])

{

  int i;
  int i_arg;
  int length;
  int iaux;
  char linebuffer[STRSIZE];
  char string[STRSIZE];
  FILE *fp;
  FILE *fpp;
  char res_file[STRSIZE];                 /* input/output files */
  char ctr_file[STRSIZE];
  char rfac_type[STRSIZE];
  float m_rfac_shift_range;
  float p_rfac_shift_range;
  float rfac_shift_step;
  float rfac;
  float faux, shift;
  float enrange;
  float averfac;
  float averenr;
  float avershift;
  float sum;
  float sterror;
  float sum1;


/*********************************************************************
 set default values 
*********************************************************************/
  m_rfac_shift_range = -10.;
  p_rfac_shift_range =  10.;
  rfac_shift_step = 0.5;
  strncpy(rfac_type,"rp", STRSIZE);
  strncpy(res_file,"---", STRSIZE);
  strncpy(ctr_file,"---", STRSIZE);
  iaux = 0;
/*********************************************************************
  Decode arguments:

    -c <ctr_file> - control file 

    -t <res_file> - (output file) IV output.
*********************************************************************/

  if (argc < 2) 
  {
      #ifdef ERROR
      fprintf(STDERR,"*** error (caoi_rfac_main): syntax error\n");
      usage(STDERR);
      #endif
      exit(1);
  }
  
  for (i_arg = 1; i_arg < argc; i_arg++)
  {
    if(*argv[i_arg] != '-')
    {
      #ifdef ERROR
      fprintf(STDERR,"*** error (caoi_rfac_main):\tsyntax error:\n");
      fprintf(STDERR,"\tusage: \tcaoi_rfac -t <res_file> -c <ctr_file>");
      #endif
      exit(1);
    }
    else
    {

      /* Read parameter input file */
      if(strncmp(argv[i_arg], "-t", 2) == 0 ||
         strcmp(argv[i_arg], "--theory") == 0 )
      {
        i_arg++;
        printf("\n%s\n",argv[i_arg]);
        strncpy(res_file, argv[i_arg], STRSIZE);
        printf("\n%s\n",res_file);
      } /* -t */

      /* Read parameter input file */
      if(strncmp(argv[i_arg], "-c", 2) == 0 ||
         strcmp(argv[i_arg], "--control") == 0 )
      {
        i_arg++;
        printf("\n%s\n",argv[i_arg]);
        strncpy(ctr_file, argv[i_arg], STRSIZE);
        printf("\n%s\n",ctr_file);
      } /* -i */

      /* Read R factor type*/
      if(strncmp(argv[i_arg], "-r", 2) == 0 ||
         strcmp(argv[i_arg], "--rfactor") == 0 )
      {
        i_arg++;
        printf("\n%s\n",argv[i_arg]);
        strncpy(rfac_type, argv[i_arg], STRSIZE);
        printf("\n%s\n",rfac_type);
      } /* -r */


      /* Read initial shift, final shift, step of a shift*/
      if(strncmp(argv[i_arg], "-s", 2) == 0 ||
         strcmp(argv[i_arg], "--shift") == 0 )
      {
        i_arg++;
	
        sscanf(argv[i_arg],"%f,%f,%f", &m_rfac_shift_range, &p_rfac_shift_range, &rfac_shift_step);
	
      } /* -s */
      
      /* help */
      if ((strcmp(argv[i_arg], "-h") == 0) || 
          (strcmp(argv[i_arg], "--help") == 0))
      {
        usage(stdout);
        exit(0);
      }
      
      /* version info */
      if ((strcmp(argv[i_arg], "-V") == 0) ||
           (strcmp(argv[i_arg], "--version") == 0))
      {
        info();
        exit(0);
      }

    }  /* else */
    
  }  /* for i_arg */

/*****************************************************************
  Check arguments:
  - check existence of ctr_file.
  - check existence of res_file.
******************************************************************/

  if(strncmp(res_file, "---", 3) == 0)
  {
    #ifdef ERROR
    fprintf(STDERR,
        "*** error (caoi_rfac_main): no .res file (option -t) specified\n");
    #endif
    exit(1);
  }

  if(strncmp(ctr_file, "---", 3) == 0)
  {
    #ifdef ERROR
    fprintf(STDERR,
        "*** error (caoi_rfac_main): no .ctr file (option -t) specified\n");
    #endif
    exit(1);
  }


/***********************************************************************
 Read sa value from the .bul file 
************************************************************************/

  length = strlen(ctr_file) - 4;        /* assume 3 char file extension */
  strncpy(proj_name,ctr_file,length);
  strncpy(string,ctr_file,length);
  sprintf(string+length,".bul");


  if ((fp = fopen(string,"r")) == NULL)
  {
    #ifdef ERROR
    fprintf(STDERR,
      "*** error (caoi_rfac_main): could not open output file \"%s\"\n",
      string);
    #endif
    exit(1);
  }

  while (fgets(linebuffer,STRSIZE,fp))
  {
    if (!strncasecmp(linebuffer,"sa:",3)) 
    {
      iaux = sscanf(linebuffer+3," %d",&sa);
    }
  }  /*while*/

  fclose(fp);

/************ Makes .ctr file for each angle **********/

  strncpy(string,ctr_file,length);
  sprintf(string+length,".ctr");
  ctrinp(string);

/********** Calls rfac progam sa times ********************/


  for (i=0;i<sa;i++)
  {

    strncpy(string,proj_name,STRSIZE);
    sprintf(string+length,"ia_%d",i+1);

    sprintf(linebuffer,
         "\"%s\" -t \"%s.res\" -c \"%s.ctr\" -r \"%s\" -s %.2f,%.2f,%.2f > \"%s.dum\"",
         getenv("CAOI_RFAC"),
         string,                  /* project name for the. file */
         string,                  /* project name for control file */
         rfac_type,                 /*type of R factor*/
         m_rfac_shift_range,          /*initial shift*/
         p_rfac_shift_range,            /*final shift*/
         rfac_shift_step,             /*step of a shift*/
         string);                 /* project name for output file */


    if (system (linebuffer)) 
    {
      fprintf(STDERR,
        "*** error (caoi_rfac_main): system call: '%s' failed\n", linebuffer);
    }

  } /*for i*/

/***************Read R factor value from .dum file*******************/

  sum1=0;
  avershift=0;
  averfac=0;
  sum=0;

  for (i=0;i<sa;i++)
  {
    strncpy(string,proj_name,STRSIZE);
    sprintf(string+length,"ia_%d.dum",i+1);

    if ((fpp = fopen(string,"r")) == NULL)
    {
      #ifdef ERROR
      fprintf(STDERR,
        "*** error (caoi_rfac_main): could not open output file \"%s\"\n",
        string);
      #endif
      exit(1);
    }

    while (fgets(linebuffer,STRSIZE,fpp)!= NULL)
    {
      iaux = sscanf(linebuffer, 
                    "%f %f %f %f", &rfac, &faux, &shift , &enrange);
    }  /*while*/


    fprintf(STDERR, "\nR%d = %f\n", i+1, rfac);
    avershift = avershift + shift;
    averfac=averfac+rfac*enrange;
    sum = sum + enrange;
    sum1 = sum1 + (1/faux)*(1/faux);
    fclose(fpp);

  } /*for*/


  avershift = (avershift/sa);
  averfac = (averfac/sum);
  averenr = (sum);
  sterror = sqrtf(1/sum1);
  fprintf(STDERR, "<R> = %f\n", averfac);
  fprintf(STDERR, 
          "\n%f %f %f %f     #     Rp  RR  shift range\n", 
          averfac, sterror, avershift, averenr);

/************** Open proj_name.dum file and write the average R factor **********************/
/*******************************************************************************************
  strncpy(string,proj_name,STRSIZE);
  sprintf(string+length,".dum");

 if ((fp = fopen(string,"w")) == NULL)
    {
#ifdef ERROR
      fprintf(STDERR,
      "*** error (caoi_rfac_main): could not open output file \"%s\"\n",
      string);
#endif
     exit(1);
    }
 fprintf(fp, "<R> = %f\n", averfac);
 fprintf(fp, "\n%f %f %f %f     #     Rp  RR  shift range\n", averfac, sterror, avershift, averenr);

************************************************************************************************/
  printf("\n%f %f %f %f     #     Rp  RR  shift range\n", 
            averfac, sterror, avershift, averenr);
  exit(0);
  
  iaux = iaux * 1; /* remove warning */

  return 0;
  
} /*main*/

