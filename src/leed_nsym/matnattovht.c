/*********************************************************************
 *                        MATNATTOVHT.C
 *
 *  Copyright 1992-2014 Georg Held <g.held@reading.ac.uk>
 *
 *  Licensed under GNU General Public License 3.0 or later.
 *  Some rights reserved. See COPYING, AUTHORS.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 *
 * Changes:
 *   GH/02.02.95 - Creation
 *********************************************************************/

/*! \file
 *
 * Implements matline() and matnattovht() functions to print a matrix in the
 * same order as the van Hove-Tong LEED package.
 */

#include <math.h>
#include <malloc.h>
#include <stdio.h>

#include "leed.h"

/*!
 * Prints a matrix \p Mbg in the same order as van Hove-Tong package.
 *
 * \param[in] Mbg Pointer to input matrix to display.
 * \param l_max Maximum linear angular momentum quantum number.
 * \param n_atoms Number of atoms.
 */
void matnattovht(const mat Mbg, size_t l_max, size_t n_atoms)
{
  size_t l_max_2, i1, l1, n1, off;
  size_t i_atoms;
  int m1;

  l_max_2 = (l_max + 1)*(l_max + 1);

  for (i_atoms = 0, off = 1, n1 = 0; i_atoms < n_atoms;
       i_atoms++, off += l_max_2)
  {
    for(l1 = 0; l1 <= l_max; l1 += 2)
    {
      for(m1 = -(int)l1; m1 <= (int)l1; m1 += 2)
      {
        if (m1 < 0) i1 = off + l1 * (l1+1) - (size_t)abs(m1);
        else        i1 = off + l1 * (l1+1) + (size_t)m1;
        n1++;

        fprintf(STDOUT, "%3d:    ", n1);
        matline(Mbg, i1, l_max, l_max_2, n_atoms);
      } /* m1 */
    } /* l1 */

    for(l1 = 1; l1 <= l_max; l1 += 2)
    {
      for(m1 = -(int)l1; m1 <= (int)l1; m1 += 2)
      {
        if (m1 < 0) i1 = off + l1 * (l1+1) - (size_t)abs(m1);
        else        i1 = off + l1 * (l1+1) + (size_t)m1;
        n1++;

        fprintf(STDOUT, "%3d:    ", n1);
        matline(Mbg, i1, l_max, l_max_2, n_atoms);
      } /* m1 */
    } /* l1 */

    for(l1 = 1; l1 <= l_max; l1 += 2)
    {
      for(m1 = -(int)l1+1; m1 <= (int)l1; m1 += 2)
      {
        if (m1 < 0) i1 = off + l1 * (l1+1) - (size_t)abs(m1);
        else        i1 = off + l1 * (l1+1) + (size_t)m1;
        n1++;

        fprintf(STDOUT, "%3d:    ", n1);
        matline(Mbg, i1, l_max, l_max_2, n_atoms);
      } /* m1 */
    } /* l1 */

    for(l1 = 2; l1 <= l_max; l1 += 2)
    {
      for(m1 = -(int)l1+1; m1 <= (int)l1; m1 += 2)
      {
        if (m1 < 0) i1 = off + l1 * (l1+1) - (size_t)abs(m1);
        else        i1 = off + l1 * (l1+1) + (size_t)m1;
        n1++;

        fprintf(STDOUT, "%3d:    ", n1);
        matline(Mbg, i1, l_max, l_max_2, n_atoms);
      } /* m1 */
    } /* l1 */
  } /* i_atoms */
  fprintf(STDOUT, "\n");

  return;
} /* end of function matnattovht */

/*!
 * Prints a line from the native \p Mbg matrix in the format expected by
 * the van Hove-Tong LEED package.
 *
 * \param[in] Mbg Pointer to input matrix.
 * \param i1
 * \param l_max Maximum angular momentum quantum number.
 * \param l_max_2 Maximum angular momentum quantum number.
 * \param n_atoms Number of atoms.
 */
void matline(const mat Mbg, size_t i1, size_t l_max, size_t l_max_2, size_t n_atoms)
{
  size_t i2, n2, l2, off2, i_atoms2;
  int m2;

  for (i_atoms2 = 0, n2 = 0, off2 = 1; i_atoms2 < n_atoms;
       i_atoms2 ++, off2 += l_max_2)
  {
    for(l2 = 0; l2 <= l_max; l2 += 2)
    {
      for(m2 = (int)l2; m2 <= (int)l2; m2 += 2)
      {
        if (m2 < 0) i2 = off2 + l2 * (l2+1) - (size_t)abs(m2);
        else        i2 = off2 + l2 * (l2+1) + (size_t)m2;
        n2 ++;

        fprintf(STDOUT, "(%6.3f,%6.3f)[%4.2f]",
                *rmatel(i1, i2, Mbg),
                *imatel(i1, i2, Mbg),
                cri_abs(*rmatel(i1, i2, Mbg), *imatel(i1, i2, Mbg)) );
        if(n2 % 6 == 0) fprintf(STDOUT, "\n  (%3d) ", n2+1);
      } /* m2 */
    } /* l2 */

    for(l2 = 1; l2 <= l_max; l2 += 2)
    {
      for(m2 = -(int)l2; m2 <= (int)l2; m2 += 2)
      {
        if (m2 < 0) i2 = off2 + l2 * (l2+1) - (size_t)abs(m2);
        else        i2 = off2 + l2 * (l2+1) + (size_t)m2;
        n2 ++;

        fprintf(STDOUT, "(%6.3f,%6.3f)[%4.2f]",
                *rmatel(i1, i2, Mbg),
                *imatel(i1, i2, Mbg),
                cri_abs(*rmatel(i1, i2, Mbg), *imatel(i1, i2, Mbg)) );
        if(n2 % 6 == 0) fprintf(STDOUT, "\n  (%3d) ", n2+1);
      } /* m2 */
    } /* l2 */

    for(l2 = 1; l2 <= l_max; l2 += 2)
    {
      for(m2 = -(int)l2+1; m2 <= (int)l2; m2 += 2)
      {
        if (m2 < 0) i2 = off2 + l2 * (l2+1) - (size_t)abs(m2);
        else        i2 = off2 + l2 * (l2+1) + (size_t)m2;
        n2 ++;

        fprintf(STDOUT, "(%6.3f,%6.3f)[%4.2f]",
                *rmatel(i1, i2, Mbg),
                *imatel(i1, i2, Mbg),
                cri_abs(*rmatel(i1, i2, Mbg), *imatel(i1, i2, Mbg)) );
        if(n2 % 6 == 0) fprintf(STDOUT, "\n  (%3d) ", n2+1);
      } /* m2 */
    } /* l2 */

    for(l2 = 2; l2 <= l_max; l2 += 2)
    {
      for(m2 = -(int)l2+1; m2 <= (int)l2; m2 += 2)
      {
        if (m2 < 0) i2 = off2 + l2 * (l2+1) - (size_t)abs(m2);
        else        i2 = off2 + l2 * (l2+1) + (size_t)m2;
        n2 ++;

        fprintf(STDOUT, "(%6.3f,%6.3f)[%4.2f]",
                *rmatel(i1, i2, Mbg), *imatel(i1, i2, Mbg),
                cri_abs(*rmatel(i1, i2, Mbg), *imatel(i1, i2, Mbg)) );
        if(n2 % 6 == 0) fprintf(STDOUT, "\n  (%3d) ", n2+1);
      } /* m2 */
    } /* l2 */
  } /* i_atoms2 */
  fprintf(STDOUT, "\n");

  return;
} /* end of function matline */
