/****************************************************************************
                        LATT.C (Georg Held 01.05.06)

CHANGES

 15.04.2006 - Creation
 27.04.2006 - include hcp, bcc, dia.
 29.04.2006 - correct format of *.xyz file 
            - allow input from file (CLEED format).
 01.05.2006 - calculate indices for lattice vectors.
 20.05.2013 - [LD] added script on line 2 of output
            - [LD] max displacement options
 02.06.2013 - [LD] --version option added
 29.04.2014 - [LD] Moved preprocessor information into header "latt.h"
 
Format of output file will be:
     <atom_name>  <x> <y> <z>
Lines beginning with '#' are interpreted as comments.

****************************************************************************/

#include "latt.h"
#include "miller_index.h"

FILE *ctr_stream;
FILE *inf_stream;

int main(int argc, char *argv[])
{

  char line_buffer[STRSZ];
  
  int n1, n2, n3;
  
  int n_bas=0, i_bas;
  size_t i_atom, j_atom;
  
  double faux_x, faux_y, faux_z, faux_len;
  
  char *bas_name = malloc(sizeof(char) * NAMSZ);
  
  double b1_len = 0., b2_len = 0., b3_len = 0.;
  double nor_len = 0.0;
  
  int ind[3][3] = { {0, 0}, {0, 0}, {0, 0} };
  miller_hkl_t miller;
  
  /* variables for print out */
  double dist_min, dist_max;
  
  int n_max, n_atoms;
  int i_pass;
  
  FILE *out_stream;
  
  lattice_t *lat = lattice_init(1);
  
  /********************************************************
   * Preset input / output streams
   ********************************************************/
  
  out_stream = stdout;
  ctr_stream = stdout;
  inf_stream = stderr;
   
  nor_len = 0.0;

  /********************************************************
   * Check command line and decode arguments
   ********************************************************/

  #if DEBUG > 0
  fprintf(stderr, "#LATT debug info\n");
  fprintf(stderr, "#TOLERANCE = %7.4f\n", TOLERANCE);
  #endif  
  
  latt_parse_args(argc, argv, lat);

  if (strcmp(lat->input_filename, "stdin") == 0)
  {
    ctr_stream = NULL;
  } 

  if (strlen(lat->output_filename) > 0)
  {  
    if (out_stream != NULL && fileno(out_stream) != fileno(stdout)) 
    {
    fclose(out_stream); 
    }
  
    if ((out_stream = fopen(lat->output_filename, "w")) == NULL)
    {
    fprintf(stderr, "*** error: "
            "open failed %s\n", lat->output_filename);
    exit(1);
    }

    sprintf(line_buffer, "%s.inf", lat->output_filename);
    if ((inf_stream = fopen(line_buffer, "w")) == NULL)
    {
    fprintf(stderr, "*** error: "
            "open failed %s\n", lat->output_filename);
    exit(1);
    }
  }
   
  fprintf(inf_stream, "# LATT: version %s\n", PROG_VERSION);
  fprintf(ctr_stream, "# LATT: version %s\n", PROG_VERSION);
   
  if (strcmp(lat->input_filename, "stdin") != 0)
  {
    fprintf(inf_stream, " %s (%.0f;%.0f;%.0f)\n", lat->atoms[0].element, 
            lat->vec_h, lat->vec_k, lat->vec_l);
    fprintf(ctr_stream, " %s (%.0f;%.0f;%.0f)\n", lat->atoms[0].element, 
            lat->vec_h, lat->vec_k, lat->vec_l);

    switch(lat->latt_type)
    {
      case LAT_FCC: 
        sprintf(line_buffer, " lattice: FCC ");
        break;
      case LAT_HCP: 
        sprintf(line_buffer, " lattice: HCP ");
        break;
      case LAT_BCC:
        sprintf(line_buffer, " lattice: BCC ");
        break;
      case LAT_DIA: 
        sprintf(line_buffer, " lattice: DIA ");
        break;
      default:
        sprintf(line_buffer, " lattice: *** ");
    }
    
    fprintf(inf_stream, " %s a_latt = %7.3f, c_latt = %7.3f\n", 
            line_buffer, lat->a_latt, lat->c_latt);
    fprintf(ctr_stream, " %s a_latt = %7.3f, c_latt = %7.3f\n", 
            line_buffer, lat->a_latt, lat->c_latt);
  }
  else 
  {
    /*! TODO */
  }
 
/***********************************************************
    BEGIN CALCULATIONS
************************************************************/
  coord_t *a1 = coord_init();
  coord_t *a2 = coord_init();
  coord_t *a3 = coord_init();  
  coord_t *nor = coord_init();
  coord_t *bas = coord_init();
  
  basis_t *a = basis_init();
  
  lattice_setup(lat, a1, a2, a3, nor, bas, bas_name, &n_bas);
  
  basis_set_a1(a, a1);
  basis_set_a2(a, a2);
  basis_set_a3(a, a3);
  
/********************************************************
 Control output
********************************************************/
  fprintf(inf_stream, "\n Before rotation:\n" );
  basis_printf(inf_stream, a);
  fprintf(inf_stream, "normal: ");
  coord_printf(inf_stream, nor);
  fprintf(inf_stream, "|normal| = %7.3f \n", nor_len );
  fprintf(inf_stream, " Basis:\n" );
  
  fprintf(ctr_stream, "\n Before rotation:\n" );
  basis_printf(ctr_stream, a);
  fprintf(ctr_stream, "normal: ");
  coord_printf(ctr_stream, nor);
  fprintf(ctr_stream, "|normal| = %7.3f \n", nor_len );
  fprintf(ctr_stream, " Basis:\n" );

  for(i_bas = 0; i_bas < n_bas; i_bas ++)
  {
    fprintf(inf_stream, "\tbas[%d] = %s (%7.3f; %7.3f; %7.3f)\n", 
            i_bas, bas_name+(i_bas*NAMSZ), 
            bas[i_bas].x, bas[i_bas].y, bas[i_bas].z);
    fprintf(ctr_stream, "\tbas[%d] = %s (%7.3f; %7.3f; %7.3f)\n", 
            i_bas, bas_name+(i_bas*NAMSZ), 
            bas[i_bas].x, bas[i_bas].y, bas[i_bas].z);
  }
  
  
/********************************************************
 Calculate Rotation matrix
********************************************************/
 double **R = normal_get_rotation_matrix(nor);


  #if DEBUG == 1
  fprintf(stderr, "#rotation matrix: \n"
          "#  %9.4f %9.4f %9.4f\n"
          "#  %9.4f %9.4f %9.4f\n"
          "#  %9.4f %9.4f %9.4f\n", 
          R[0][0], R[0][1], R[0][2], 
          R[1][0], R[1][1], R[1][2], 
          R[2][0], R[2][1], R[2][2]);
  #endif

/********************************************************
 Rotate lattice vectors and basis vectors
********************************************************/

  basis_t *rot_a = basis_rotate_basis(a, R);
  coord_t *rot_nor = basis_rotate_normal(nor, R);

  coord_t *rot_bas = basis_rotate_vector_list(bas, n_bas, R);
  
  coord_t *b1 = coord_init();
  coord_t *b2 = coord_init();
  coord_t *b3 = coord_init();

  fprintf(ctr_stream, "\n First rotation:\n" );
  basis_printf(ctr_stream, rot_a);
  fprintf(ctr_stream, "normal: ");
  coord_printf(ctr_stream, rot_nor);
  fprintf(ctr_stream, "|normal| = %7.3f \n", coord_get_magnitude(rot_nor));
  
  basis_copy(a, rot_a);
  for (i_bas = 0; i_bas < n_bas; i_bas ++) 
  {
    coord_copy(&bas[i_bas], &rot_bas[i_bas]); 
  }

/********************************************************
 Find lattice vectors b1, b2, b3
********************************************************/

  b2_len = b1_len = lat->max_disp * lat->a_nn; /* = 10. * a_nn; */

  coord_set(b3, 0., 0., -(lat->max_disp_z) * lat->a_nn); /* -10. * a_nn; */
  b3_len = COORD_MAGNITUDE(b3);

  n_max = (int) lat->max_disp_z * 2; /* n_max = 20; */

  #if (DEBUG == 1)
  lattice_debug(lat);
  fprintf(stderr, "#n_max = %i\n", n_max);
  fprintf(stderr, "#max_disp=%7.4f\n#a_nn=%7.4f\n", lat->max_disp, lat->a_nn);
  fprintf(stderr, "n1 n2 n3 x y z within_limits?\n");
  #endif 
  
  coord_t *faux = coord_init();
  
  for(n1 = -n_max; n1 <= n_max; n1 ++)
  {
    for(n2 = -n_max; n2 <= n_max; n2 ++)
    {
      for(n3 = -n_max; n3 <= n_max; n3 ++)
      {
        faux->z = (n1 * a1->z) + (n2 * a2->z) + (n3 * a3->z);
        
        #if DEBUG == 1
        fprintf(stderr, "%3d %3d %3d ", n1, n2, n3);
        fprintf(stderr, "%9.4f %9.4f %9.4f ", faux->x, faux->y, faux->z); 
        #endif 
        if ( faux->z < -TOLERANCE ) /* neg z coordinate */
        {
          faux->x = (n1 * a1->x) + (n2 * a2->x) + (n3 * a3->x);
          faux->y = (n1 * a1->y) + (n2 * a2->y) + (n3 * a3->y);
            
          if ( ( COORD_MAGNITUDE(faux) < (b3_len + TOLERANCE) ) && 
               ( faux->z > (b3->z - TOLERANCE) ) )
          {

            COORD_COPY(b3, faux);
            b3_len = COORD_MAGNITUDE(b3);

            ind[2][0] = n1;
            ind[2][1] = n2;
            ind[2][2] = n3;

            fprintf(ctr_stream, " ver: (%d, %d, %d) (%7.3f; %7.3f; %7.3f)\n", 
                    n1, n2, n3, b3->x, b3->y, b3->z);
            }
        }
    
        if ( fabs(faux->z) < TOLERANCE )
        {
          faux->x = (n1 * a1->x) + (n2 * a2->x) + (n3 * a3->x);
          faux->y = (n1 * a1->y) + (n2 * a2->y) + (n3 * a3->y);
          faux_len = COORD_MAGNITUDE(faux);

          if (faux_len > TOLERANCE)
          {
            if (faux_len < b1_len)
            {   
              COORD_COPY(b1, faux);
              b1_len = faux_len;

              ind[0][0] = n1;
              ind[0][1] = n2;
              ind[0][2] = n3;
            }

            /* for b2 make sure the vector is not a multiple of b1 */
            else if ( (faux_len < b2_len) && ( fabs((faux->x * b1->y) - 
                                            (faux->y * b1->x)) > TOLERANCE ) )
            {
              COORD_COPY(b2, faux);
              b2_len = faux_len;

              ind[1][0] = n1;
              ind[1][1] = n2;
              ind[1][2] = n3;
            }
          }
        }  /* if faux_z = 0 */
          
          #if DEBUG == 1
          fprintf(stderr, "\n");
          #endif
      }
    }
  } /* for n1, n2, n3 */

  fprintf(ctr_stream, "\n interm vectors (1):\n");
  
  basis_t *b = basis_init_vectors(b1, b2, b3);
  
  basis_printf(ctr_stream, b);

/********************************************************
 rotate lattice vectors b1, b2, b3 such that b1 || x-axis
 make sure, b1 / b2 is a right-handed system 
 make sure, < b1,b2 is  greater than 90deg 
********************************************************/
  b = basis_rotate_parallel_to_x_axis_rhs(b, R);

  for(i_bas = 0; i_bas < n_bas; i_bas ++)
  {
    faux_x = bas[i_bas].x; 
    faux_y = bas[i_bas].y; 

    bas[i_bas].x = (R[0][0] * faux_x) + (R[0][1] * faux_y);
    bas[i_bas].y = (R[1][0] * faux_x) + (R[1][1] * faux_y);
  }

  fprintf(ctr_stream, "\n interm vectors (2):\n");
  basis_printf(ctr_stream, b);

/********************************************************
 Make sure, b1 / b2 is a right-handed system:
 b2->y must be positive, if not, replace b2 by -b2.
********************************************************/

  if( b2->y < 0. )
  {
    b2->x *= -1;
    b2->y *= -1;

    ind[1][0] *= -1;
    ind[1][1] *= -1;
    ind[1][2] *= -1;
  }

/********************************************************
 Make sure, < b1,b2 is  greater than 90deg
 cos must be <= 0, if not, replace b2 by b2-b1.
********************************************************/

  if( (b1->x*b2->x + b1->y*b2->y) > TOLERANCE )
  {
    b2->x -= b1->x;
    b2->y -= b1->y;

    ind[1][0] -= ind[0][0];
    ind[1][1] -= ind[0][1];
    ind[1][2] -= ind[0][2];
  }

/********************************************************
 Indices for fcc and bcc:
********************************************************/

  if (lat->latt_type == LAT_FCC)
  {
    miller.h = (ind[0][0] + ind[0][1]);
    miller.k = (ind[0][1] + ind[0][2]);
    miller.l = (ind[0][0] + ind[0][2]);
    
    ind[0][0] = miller.h; 
    ind[0][1] = miller.k; 
    ind[0][2] = miller.l;

    miller.h = (ind[1][0] + ind[1][1]);
    miller.k = (ind[1][1] + ind[1][2]);
    miller.l = (ind[1][0] + ind[1][2]);
    
    ind[1][0] = miller.h; 
    ind[1][1] = miller.k; 
    ind[1][2] = miller.l;

    miller.h = (ind[2][0] + ind[2][1]);
    miller.k = (ind[2][1] + ind[2][2]);
    miller.l = (ind[2][0] + ind[2][2]);
    
    ind[2][0] = miller.h; 
    ind[2][1] = miller.k; 
    ind[2][2] = miller.l;
  }

  if (lat->latt_type == LAT_BCC)
  {
    miller.h = (2*ind[0][0] + ind[0][2]);
    miller.k = (2*ind[0][1] + ind[0][2]);
    miller.l = (ind[0][2]);
    
    ind[0][0] = miller.h; 
    ind[0][1] = miller.k; 
    ind[0][2] = miller.l;

    miller.h = (2*ind[1][0] + ind[1][2]);
    miller.k = (2*ind[1][1] + ind[1][2]);
    miller.l = (ind[1][2]);
    
    ind[1][0] = miller.h; 
    ind[1][1] = miller.k; 
    ind[1][2] = miller.l;

    miller.h = (2*ind[2][0] + ind[2][2]);
    miller.k = (2*ind[2][1] + ind[2][2]);
    miller.l = (ind[2][2]);
    
    ind[2][0] = miller.h; 
    ind[2][1] = miller.k; 
    ind[2][2] = miller.l;
  }
  

  fprintf(inf_stream, "\n Final vectors:\n");
  fprintf(inf_stream, " b1 = (%7.3f; %7.3f; %7.3f): [%d; %d; %d]\n", 
                      b1->x, b1->y, b1->z, ind[0][0], ind[0][1], ind[0][2]);
  fprintf(inf_stream, " b2 = (%7.3f; %7.3f; %7.3f): [%d; %d; %d]\n", 
                      b2->x, b2->y, b2->z, ind[1][0], ind[1][1], ind[1][2]);
  fprintf(inf_stream, " b3 = (%7.3f; %7.3f; %7.3f): [%d; %d; %d]\n", 
                      b3->x, b3->y, b3->z, ind[2][0], ind[2][1], ind[2][2]);
  fprintf(inf_stream, " Basis:\n" );
  
  fprintf(ctr_stream, "\n Final vectors:\n");
  fprintf(ctr_stream, " b1 = (%7.3f; %7.3f; %7.3f): [%d; %d; %d]\n", 
                      b1->x, b1->y, b1->z, ind[0][0], ind[0][1], ind[0][2]);
  fprintf(ctr_stream, " b2 = (%7.3f; %7.3f; %7.3f): [%d; %d; %d]\n", 
                      b2->x, b2->y, b2->z, ind[1][0], ind[1][1], ind[1][2]);
  fprintf(ctr_stream, " b3 = (%7.3f; %7.3f; %7.3f): [%d; %d; %d]\n", 
                      b3->x, b3->y, b3->z, ind[2][0], ind[2][1], ind[2][2]);
  
  fprintf(ctr_stream, " Basis:\n" );
  for(i_bas = 0; i_bas < n_bas; i_bas ++)
  {
    fprintf(inf_stream, "\tbas[%d] = %s (%7.3f; %7.3f; %7.3f)\n", 
            i_bas, bas_name+(i_bas*NAMSZ), 
            bas[i_bas].x, bas[i_bas].y, bas[i_bas].z);
    fprintf(ctr_stream, "\tbas[%d] = %s (%7.3f; %7.3f; %7.3f)\n", 
            i_bas, bas_name+(i_bas*NAMSZ), 
            bas[i_bas].x, bas[i_bas].y, bas[i_bas].z);
  }

/* area of unit cell */
  faux->x = fabs(b1->x*b2->y - b2->x*b1->y);

/* volume of unit cell */
  faux->y = fabs(-faux->x * b3->z);
  fprintf(inf_stream, "\n area = %7.3f; volume = %7.3f\n", faux->x, faux->y);
  fprintf(ctr_stream, "\n area = %7.3f; volume = %7.3f\n", faux->x, faux->y);

/********************************************************
 Create list of atoms
 i_pass = 0: find number of atoms in list (n_atoms).
 i_pass = 1: write atoms to list.
********************************************************/

/* n_atom will be changed later */
  n_atoms = lat->n_atoms;
  
/* cycle through layers */
  if (lat->max_layers == 0) 
  {
    lat->max_layers = (int)( rint (-3. * lat->a_nn / (b3->z) ) );
  }

  faux->x = lat->image_len / b1->x;
  if( (lat->image_len / b2->y) > faux->x) 
  {
    faux->x = lat->image_len / b2->y;
  }
  n_max = (int)( rint( 4. * faux->x) );

  for(i_pass = 0; i_pass < 2; i_pass ++)
  {
    i_atom = 0;

    if(i_pass > 0) 
    {
      fprintf(ctr_stream, " n_atom = %d; n_layers = %d\n", 
              lat->n_atoms, lat->max_layers); 
    }


/* complete unit cells if n_unit_cell > 0 */

    n_max = lat->max_cells;

    if(lat->max_cells > 0)
    {
      for(n3 = -1; n3 < lat->max_layers; n3 ++)
      {
      /* n_max -> n_cells */
        for(n1 = -n_max; n1 < n_max; n1 ++)
        {
          for(n2 = -n_max; n2 < n_max; n2 ++)
          {
  
            faux->x = n1 * b1->x + n2 * b2->x + n3 * b3->x;
            faux->y = n1 * b1->y + n2 * b2->y + n3 * b3->y;
            faux->z = n1 * b1->z + n2 * b2->z + n3 * b3->z;
  
            if(    (faux->x >= -TOLERANCE ) && (faux->x < lat->image_len)
                && (faux->y >= -TOLERANCE ) && (faux->y < lat->image_len)
                && (faux->z <= TOLERANCE)  )
            {
              for(i_bas = 0; i_bas < n_bas; i_bas ++)
              {
  
                if(i_pass > 0)
                {
                  lat->atoms[i_atom].x = n1 * b1->x + 
                                         n2 * b2->x + n3 * b3->x + bas[i_bas].x;
                  lat->atoms[i_atom].y = n1 * b1->y + 
                                         n2 * b2->y + n3 * b3->y + bas[i_bas].y;
                  lat->atoms[i_atom].z = n1 * b1->z +
                                         n2 * b2->z + n3 * b3->z + bas[i_bas].z;
                  strncpy(lat->atoms[i_atom].element, 
                          bas_name+(i_bas+NAMSZ), NAMSZ);
                }
  
                i_atom ++;
  
              }  /* for i_bas */
            }  /* if */
          } 
        }/* for n1, n2 */
      } /* for n3 */
    } /* keep unit cell */

/* only atoms within limits if n_unit_cell <= 0 */
    else 
    {
      for(n3 = -1; n3 < lat->max_layers; n3 ++)
      {
        for(n1 = -n_max; n1 < n_max; n1 ++)
        {
          for(n2 = -n_max; n2 < n_max; n2 ++)
          {
            for(i_bas = 0; i_bas < n_bas; i_bas ++)
            {
              faux->x = n1 * b1->x + n2 * b2->x + n3 * b3->x + bas[i_bas].x;
              faux->y = n1 * b1->y + n2 * b2->y + n3 * b3->y + bas[i_bas].y;
              faux->z = n1 * b1->z + n2 * b2->z + n3 * b3->z + bas[i_bas].z;
  
              if (   (faux->x >= -TOLERANCE ) && (faux->x < lat->image_len)
                  && (faux->y >= -TOLERANCE ) && (faux->y < lat->image_len) 
                  && (faux->z <= TOLERANCE)  )
              {
                if(i_pass > 0) 
                {
                  lat->atoms[i_atom].x = faux->x;
                  lat->atoms[i_atom].y = faux->y;
                  lat->atoms[i_atom].z = faux->z;
                  strncpy(lat->atoms[i_atom].element, 
                          bas_name+(i_bas+NAMSZ), NAMSZ);
                }
  
                i_atom ++;
  
              }
  
            }  /* for i_bas */
          }
        }        /* for n1, n2 */
      } /* for n3 */
    } /* n_unit_cell <= 0 */

    n_atoms = i_atom;

  } /* i_pass */

/********************************************************
 Sort list of atoms
 Find shortest / longest inter-layer distance
********************************************************/

  for(i_atom = 0; i_atom < lat->n_atoms; i_atom ++)
  {
    for(j_atom = i_atom+1; j_atom < lat->n_atoms; j_atom ++)
    {
      if(lat->atoms[j_atom].z > lat->atoms[i_atom].z + TOLERANCE)
      {
        lattice_atom_index_swap(lat, i_atom, j_atom);
      }
    } /* for j_atoms */
  } /* for i_atoms */

  dist_min = lat->a_nn;
  dist_max = 0.;
  for(i_atom = 1; i_atom < lat->n_atoms; i_atom ++)
  {
    faux_z = lat->atoms[i_atom-1].z - lat->atoms[i_atom].z;
    if( (faux_z > TOLERANCE) && (faux_z < dist_min) ) dist_min = faux_z;
    if( faux_z > dist_max ) dist_max = faux_z;
  }

  fprintf(ctr_stream, " dist_min/dist_max = %.3f /%.3f \n", dist_min, dist_max);
  fprintf(inf_stream, " shortest/longest layer distance = %.3f /%.3f \n", 
          dist_min, dist_max);

  lattice_printf(stdout, lat);
  fprintf(inf_stream, "\n Atoms in list = %d; layers = %d\n", n_atoms, lat->max_layers);
  
  fclose(inf_stream);
  fclose(ctr_stream);
  fclose(out_stream);

  /* clean up */
  coord_free(a1);
  coord_free(a2);
  coord_free(a3);
  coord_free(nor);
  coord_free(bas);
  coord_free(rot_bas);
  coord_free(rot_nor);
  coord_free(faux);
  basis_free(a);
  basis_free(b);
  basis_free(rot_a);

  free(R[0]);
  free(R[1]);
  free(R[2]);
  free(R);
  R = NULL;
  
  return (0);
} /* end of main */

