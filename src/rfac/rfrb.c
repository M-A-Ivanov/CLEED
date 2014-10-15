/*********************************************************************
 *                           RFRB.C
 *
 *  Copyright 1992-2014 Georg Held <g.held@reading.ac.uk>
 *
 *  Licensed under GNU General Public License 3.0 or later.
 *  Some rights reserved. See COPYING, AUTHORS.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 *
 * Changes:
 *   GH/1993.02.19 - Creation
 *   GH/1994.02.08  Change Normalistion (Cuncorr = 0.67)
 *   GH/1995.09.07 -  Adaption for CRFAC
 ********************************************************************/

/*! \file rfrb.c
 *
 * Implemenation file for Rb1 factor calculation.
 */

#include <stdio.h>
#include <math.h>           /* needed for sqrt */
#include "rfac.h"           /* specific definitions etc. */

#define CNORM 0.666667     /* 0.67 normalisation to uncorrelated curves */

/*!
 * \fn rf_rb
 *
 * \brief Calculate Rb1-factor.
 *
 * Calculate Rb1-factor:
 *
 * \f$ R_{b1} = (1 - C) / (1 - C_NORM) \f$
 *
 * \f$ C =  S(I_theory*I_exp) / \sqrt{ (S * I_theory^2) * (S * I_exp^2) } \f$
 * \f$ C_NORM = 2/3 \f$ (cf. G. Held, Dissertation)
 *
 * \param[in] *eng pointer to list of energies.
 * \param[in] *e_int pointer to list of experimental intensities.
 * \param[in] *t_int lists pointer to list of theoretical intensities.
 *
 * \note \p eng must be terminated by \ref F_END_OF_LIST.
 *
 * \return R2-factor if successful.
 * \retval \ref RFAC_FAILURE, if failed.
 *
 */
real rf_rb(const real *eng, const real *e_int, const real *t_int)
{
  size_t i_eng;
  real exp_sq_sum, the_sq_sum;
  real rf_sum;

  /* Calculate integrals and R-factor */
  exp_sq_sum = 0.;
  the_sq_sum = 0.;
  rf_sum = 0.;

  for(i_eng = 0; ! IS_EQUAL_REAL(eng[i_eng], F_END_OF_LIST); i_eng ++)
  {
    rf_sum += e_int[i_eng] * t_int[i_eng];

    exp_sq_sum += SQUARE(e_int[i_eng]);
    the_sq_sum += SQUARE(t_int[i_eng]);
  }

  rf_sum /= R_sqrt(the_sq_sum * exp_sq_sum);

  #ifdef CONTROL
  fprintf(STDCTR, "(cr_rb): rf_sum: %f,", rf_sum);
  #endif

  rf_sum = (real)( (1. - rf_sum) / (1. - CNORM) );

  #ifdef CONTROL
  fprintf(STDCTR," rfac: %f \n", rf_sum);
  #endif

  return (rf_sum);
}
