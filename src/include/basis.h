/************************************************************************
 * basis.h
 *
 *  Copyright 2014 Liam Deacon <liam.deacon@diamond.ac.uk>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *  Some rights reserved. See COPYING, AUTHORS.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 *
 * Description: Header provides basis vectors data type and associated 
 *              functions.
 * Change Log:
 *   2014/07/30 - Creation
 ************************************************************************/
 
#ifndef BASIS_H
#define BASIS_H

#include "coord.h"
#include "miller_index.h"

#define BASIS_VECTOR_PTR(basis, i)  (basis->a+(i*sizeof(coord_t)))

#define CALC_ROTATION_MATRIX(normal, R)                                         \
{                                                                               \
  if (fabs(sqrt(normal->x*normal->x + normal->y*normal->y)) > TOLERANCE)        \
  {                                                                             \
    R[0][0] = (normal->z / coord_get_magnitude(normal)) *                       \
              (normal->x / sqrt(normal->x*normal->x + normal->y*normal->y));    \
    R[0][1] = (normal->z / coord_get_magnitude(normal)) *                       \
              (normal->y / sqrt(normal->x*normal->x + normal->y*normal->y));    \
    R[0][2] = -sqrt ( 1 - (normal->z /                                          \
                coord_get_magnitude(normal) * normal->z /                       \
                coord_get_magnitude(normal)));                                  \
                                                                                \
    R[1][0] = -sqrt ( 1 - (normal->z /                                          \
                        coord_get_magnitude(normal) * normal->z /               \
                        coord_get_magnitude(normal)));                          \
    R[1][1] =  normal->x / sqrt(normal->x*normal->x + normal->y*normal->y);     \
    R[1][2] =  0.0;                                                             \
                                                                                \
    R[2][0] = sqrt ( 1 - (normal->z /                                           \
                coord_get_magnitude(normal) * normal->z /                       \
                coord_get_magnitude(normal))) *                                 \
                (normal->x / sqrt(normal->x*normal->x + normal->y*normal->y));  \
    R[2][1] = sqrt ( 1 - (normal->z /                                           \
                coord_get_magnitude(normal) * normal->z /                       \
                coord_get_magnitude(normal))) *                                 \
                (normal->y / sqrt(normal->x*normal->x + normal->y*normal->y));  \
    R[2][2] = normal->z / coord_get_magnitude(normal);                          \
  }                                                                             \
  else                                                                          \
  {                                                                             \
    R[0][0] = normal->z / coord_get_magnitude(normal);                          \
    R[0][1] = 0.;                                                               \
    R[0][2] = -sqrt ( 1 - (normal->z /                                          \
                coord_get_magnitude(normal) * normal->z /                       \
                coord_get_magnitude(normal)));                                  \
                                                                                \
    R[1][0] =  0.;                                                              \
    R[1][1] =  1.;                                                              \
    R[1][2] =  0.;                                                              \
                                                                                \
    R[2][0] = sqrt ( 1 - (normal->z / coord_get_magnitude(normal) * normal->z / \
                    coord_get_magnitude(normal)));                              \
    R[2][1] = 0.;                                                               \
    R[2][2] = normal->z / coord_get_magnitude(normal);                          \
  }                                                                             \
}

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

/*! \struct basis_t
 *  \brief structure for the three basis vectors a1, a2, & a3. */
typedef struct {
  coord_t a[3];
} basis_t; 

basis_t *basis_matrix_rotate(basis_t *a, double **R);
basis_t *basis_ptr_rotate(basis_t *a, double *R[]);
basis_t *basis_angle_rotate(const basis_t *a, double alpha, double beta, double gamma);

basis_t *basis_rotate_basis(const basis_t *basis, double **R);
coord_t *basis_rotate_normal(const coord_t *nor, double **R);
coord_t *basis_rotate_vector_list(const coord_t *list, size_t n, double **R);
basis_t *basis_rotate_parallel_to_x_axis_rhs(const basis_t *basis, double **R);
basis_t *basis_init();
void basis_free(basis_t *basis);
void basis_copy(basis_t *dst, const basis_t *src);
basis_t *basis_init_vectors(const coord_t *a1, const coord_t *a2, const coord_t *a3);

coord_t *basis_get_a1(const basis_t *basis);
coord_t *basis_get_a2(const basis_t *basis);
coord_t *basis_get_a3(const basis_t *basis);
void basis_get_vectors(const basis_t *basis, coord_t *a1, coord_t *a2, coord_t *a3);
coord_t *basis_get_normal(const basis_t *basis, const miller_hkl_t *hkl);
size_t basis_get_allocated_size(const basis_t *basis);

void basis_set_a1(basis_t *basis, const coord_t *a1);
void basis_set_a2(basis_t *basis, const coord_t *a2);
void basis_set_a3(basis_t *basis, const coord_t *a3);
void basis_set_vectors(basis_t *basis, const coord_t *a1, const coord_t *a2, const coord_t *a3);

void basis_printf(FILE *f, const basis_t *basis);
double **normal_get_rotation_matrix(const coord_t *normal);

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
} /* extern "C" */

namespace cleed {

class Basis {
  public:
    Basis();
    Basis(basis_t *a);
    ~Basis();
    
    /* setters */
    void setA1(const Coordinate &a1);
    void setA1(double a1_x, double a1_y, double a1_z);
    void setA2(const Coordinate &a3);
    void setA2(double a2_x, double a2_y, double a2_z);
    void setA3(const Coordinate &a3);
    void setA3(double a3_x, double a3_y, double a3_z);
    void setBasis(const Coordinate &a1, const Coordinate &a2, const Coordinate &a3);
    void setBasis(const Basis &basis);
    void setBasis(const Basis *basis);
    
    /* getters */
    const &Coordinate getA1();
    const &Coordinate getA2();
    const &Coordinate getA3();
    
    /* other methods */
    double **calculateRotationMatrix();
    void rotate(double **R);
    void rotate(double alpha, double beta, double gamma);
    
  protected:
    basis_t *basis;
    
    friend class Coordinate;
};


} /* namespace cleed */ 

#endif /* ifdef __cplusplus */

#endif /* BASIS_H */
