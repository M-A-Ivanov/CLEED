
/**************************************************************************<
    >
    >         File Name: readvar.c
    >
>**************************************************************************/

/**************************************************************************/
#include <ctype.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include "mkiv.h"
#include "variables.h"
#include "file_functs.h"
/**************************************************************************/

int readvar(int verb, char *var_path, char *param_path,
        float *cos_min, float *cos_max, float *verh, 
        float *acci, float *accb, int *distance, int *update, int *step, 
        int *trip_max, float *scale_min, float *range_min, float *s2n_good, 
        float *s2n_bad, int *ref_min, float *sec_range, float *s2n_ref, 
        float *ref_dev, float *bas_rat, float *range, struct vector *scale, 
        float *angle, float *ratio)

/****************************************************************************
  CS/30.7.93  

 Purpose:
  readvar reads some necessary data statements from the file mkiv.var
  if variables don't occur in the input file, defaults are taken instead

****************************************************************************/

{

    int i;
    char buf[STRSZ], var[STRSZ];
    FILE *fvar, *fparam;

/***************************************************************************/

    /* Preset variables, default values */
    *cos_min = .6;
    *cos_max = .9;
    *verh = 1.5;
    *acci = .9;
    *accb = .7;
    *distance = 120;
    *update = 10;
    *step = 2;
    *trip_max = 599;
    *scale_min = 3.;
    *range_min = 3.;
    *s2n_good = .9;
    *s2n_bad = .2;
    *ref_min = 10;
    *sec_range = 3.;
    *s2n_ref = 1.2;
    *range = .045;
    scale->xx = .055;
    scale->yy = .055;
    *angle = 0.;
    *ratio = .22;
    *ref_dev = 2.5;
    *bas_rat = .7;
    *bas_rat = .7;

    /* Open statement */
    fvar = fopen(var_path,"r");
    if (fvar == NULL) 
    {
        fprintf(stderr, "***error (readvar): "
                "cannot open '%s'\n", var_path);
        exit(1);
    }
    else 
        printf("\nreading input from \"%s\":\n", var_path);
     
    /* Read data */
    while ( (fgets(buf, STRSZ, fvar) != NULL) && (*buf != '.') ) 
    {
        if ( ispunct(*buf) )  continue;
        sscanf(buf, "%s", var);

        for ( i=0; i<VAR_MAX; i++)
            if ( ! strncasecmp( var, VAR[i], VAR_LEN ) ) break;

        switch (i+1) 
        {
            case  1 :  sscanf( buf, "%s %f", var, cos_min);	break;
            case  2 :  sscanf( buf, "%s %f", var, cos_max);	break;
            case  3 :  sscanf( buf, "%s %f", var, verh); break;
            case  4 :  sscanf( buf, "%s %f", var, acci); break;
            case  5 :  sscanf( buf, "%s %f", var, accb); break;
            case  6 :  sscanf( buf, "%s %d", var, distance); break;
            case  7 :  sscanf( buf, "%s %d", var, update); break;
            case  8 :  sscanf( buf, "%s %d", var, step); break;
            case  9 :  sscanf( buf, "%s %d", var, trip_max); break;
            case 10 :  sscanf( buf, "%s %f", var, scale_min); break;
            case 11 :  sscanf( buf, "%s %f", var, range_min); break;
            case 12 :  sscanf( buf, "%s %f", var, s2n_good); break;
            case 13 :  sscanf( buf, "%s %f", var, s2n_bad); break;
            case 14 :  sscanf( buf, "%s %d", var, ref_min); break;
            case 15 :  sscanf( buf, "%s %f", var, sec_range); break;
            case 16 :  sscanf( buf, "%s %f", var, s2n_ref); break;
            case 17 :  sscanf( buf, "%s %f", var, ref_dev); break;
            case 18 :  sscanf( buf, "%s %f", var, bas_rat); break;
            case 19 :  sscanf( buf, "%s %f", var, range); break;
            case 20 :  sscanf( buf, "%s %f", var, &(scale->xx)); break;
            case 21 :  sscanf( buf, "%s %f", var, &(scale->yy)); break;
            case 22 :  sscanf( buf, "%s %f", var, angle); break;
            case 23 :  sscanf( buf, "%s %f", var, ratio); break;

            default :  fprintf(stderr, "***error (readvar): "
                               "wrong argument '%s'\n", var); 
        }
    }
    fclose(fvar);

    fparam = fopen(param_path,"a");
    if (fparam == NULL) 
    {
        fprintf(stderr, "***error (readvar): "
                "unable to open '%s'", param_path);
        exit(1);
    }
    else {
        VPRINT("writing input-values to '%s' \n\n", param_path);
        fprintf(fparam,"cos_min = %.2f \n",*cos_min);
        fprintf(fparam,"cos_max = %.2f \n",*cos_max);
        fprintf(fparam,"verh = %.2f \n",*verh);
        fprintf(fparam,"acci = %.2f \n",*acci);
        fprintf(fparam,"accb = %.2f \n",*accb);
        fprintf(fparam,"distance = %d \n",*distance);
        fprintf(fparam,"update = %d \n",*update);
        fprintf(fparam,"step = %d \n",*step);
        fprintf(fparam,"trip_max = %d \n",*trip_max);
        fprintf(fparam,"scale_min = %.1f \n",*scale_min);
        fprintf(fparam,"range_min = %.1f \n",*range_min);
        fprintf(fparam,"s2n_good = %.3f \n",*s2n_good);
        fprintf(fparam,"s2n_bad = %.3f \n",*s2n_bad);
        fprintf(fparam,"ref_min = %d \n",*ref_min);
        fprintf(fparam,"sec_range = %.1f \n",*sec_range);
        fprintf(fparam,"s2n_ref = %.2f \n",*s2n_ref);
        fprintf(fparam,"ref_dev = %.1f \n",*ref_dev);
        fprintf(fparam,"bas_rat = %.2f \n",*bas_rat);
        fprintf(fparam,"range = %.3f \n",*range);
        fprintf(fparam,"scale_a = %.3f \n",scale->xx);
        fprintf(fparam,"scale_b = %.3f \n",scale->yy);
        fprintf(fparam,"angle = %.1f \n",*angle);
        fprintf(fparam,"ratio = %.2f \n",*ratio);
        fclose(fparam);
        QQ {
            char *param_content = file_content(param_path); 
            printf("%s:\n%s\n", param_path, param_content);
        }
    }
    
    return 0;
}
/***************************************************************************/
