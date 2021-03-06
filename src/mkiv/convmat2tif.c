/***************************************************************************
GH/26.05.05

     Filename: convmat2tiflog.c

**************************************************************************

VJ/07.02.03 - Creation
GH/15.03.03 - 
GH/26.05.05 - introduce log
LD/16/06.14 - update to ImageMatrix

Purpose: conv_mat2tif.c converts the image matrix parameters (ImageMatrix)
into TIFF parameters (tifvalues) and applies log scaling for displaying. 
It then allocates memory for buf and copies all imagedata to buf. 
Used in function writetif (writetif.c) which is contained in functions 
such as drawbound.

Input:

ImageMatrix *mat_image

Output:
tifvalues *tif_image - must be allocated, 
       the pointer tif_image->buf must either be NULL or point to a valid
       array of characters.

Notes:
    
**************************************************************************/

#include "mkiv.h"
#include <stdlib.h>
#include <math.h>

int conv_mat2tif(ImageMatrix *mat_image, tifvalues *tif_image)
{
  unsigned long i;
  unsigned int n_size;

  unsigned short *data_shor;
  unsigned char  *data_char;

  unsigned short pix_shor, max_val, min_val;
  unsigned char  pix_char;

  unsigned char  *lut;

  float norm;
     
/*  Convert ImageMatrix into tifvalues */

  tif_image->imagewidth  = mat_image->rows;
  tif_image->imagelength = mat_image->cols;

  tif_image->bitspersample = 8;
  tif_image->compression = 1;
  tif_image->photometricinterpretation = 1;
/*
 These numbers do not seem to matter ...
*/
  tif_image->stripoffsets = 134765640;
  tif_image->rowsperstrip = 11;
  tif_image->stripbytecounts = 134765424;
/*
  tif_image->stripoffsets = 134775496;
  tif_image->rowsperstrip = 20;
  tif_image->stripbytecounts = 134775408;
*/
  tif_image->xresolution = 0;
  tif_image->yresolution = 0;
  tif_image->planarconfiguration = 1;
  tif_image->resolutionunit = 2;

/**********************************************
    tif_image->ENERGY/CURRENT are not defined.

  tif_image->ENERGY = image->ispare1;
  tif_image->CURRENT = image->ispare2;
**********************************************/

/* Allocate memory for tif_image pointer buf */

  n_size = tif_image->imagelength * tif_image->imagewidth;
  data_char = (unsigned char *) malloc(sizeof(unsigned char)*n_size);

  if(data_char == NULL)
  { 
    fprintf (stderr, "*** error (conv_mat2tif_log): Failed to allocate memory");
    exit (1);
  }

/* First: find minimum / maxiumum value in mat_image->imagedata */

  /* Allocate memory for LUT */
  lut = (unsigned char *)calloc(256, sizeof(char));
  for(i=0; i < 256; i++)
  {
    lut[i] = (unsigned char) (46.018 * log( (double) i ) );
/*
    lut[i] = (unsigned char) ( 318.75 *  i / (i + 63.75) );
    lut[i] = (unsigned char) (46.018 * log( (double) i ) );
    fprintf(stdout, "i = %d, lut = %d\n", i, lut[i]); 
*/
  }

  data_shor = (unsigned short *)mat_image->imagedata;

  max_val = 1;
  min_val = data_shor[0];

  for(i=0; i < n_size; i++)
  {
    max_val = MAX(*(data_shor + i), max_val);
    min_val = MIN(*(data_shor + i), min_val);
  }

  if(max_val > min_val) norm = 255. / (max_val - min_val);
  else norm = 255.;

  fprintf (stdout, "(conv_mat2tif_log): min/max_val = %d/%d, norm = %f\n", 
                   min_val, max_val, norm);

/* normalize all imagedata and copy to buf */

  for(i=0; i < n_size; i++) 
  {
    pix_shor = *(data_shor + i);
    pix_char = (unsigned char)( (pix_shor - min_val) * norm);
    *(data_char + i) = lut[pix_char];
  }                

  if(tif_image->buf != NULL) 
  {
    fprintf(stderr,"(conv_mat2tif_log): free buffer\n");
    free(tif_image->buf);
  }
  tif_image->buf = (char*) data_char;
  return 0;
}
