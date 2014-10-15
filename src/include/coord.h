/************************************************************************
 *                        COORD.H
 *
 *  Copyright 2014 Liam Deacon <liam.deacon@diamond.ac.uk>
 *
 *  Licensed under GNU General Public License 3.0 or later. 
 *  Some rights reserved. See COPYING, AUTHORS.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 *
 * Description: Header provides an xyz coordinate data type and associated
 *              functions.
 *
 * Change Log:
 *   LD/2014.07.30 - Creation
 ************************************************************************/
 
#ifndef COORD_H
#define COORD_H

#define COORD_COPY(a, b) {a->x = b->x; a->y = b->y; a->z = b->z;}
#define COORD_COPY_XYZ(a, x0, y0, z0) {a->x = x0; a->y = y0; a->z = z0;}
#define COORD_MAGNITUDE(a) (sqrt(a->x*a->x + a->y*a->y + a->z*a->z))

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

/*! \typedef \struct coord
 *  \brief structure for x, y, z coordinate.
 */
typedef struct {
  double x;       /*!< x-axis position */
  double y;       /*!< y-axis position */
  double z;       /*!< z-axis position */
} coord;

coord *coord_init();
void coord_free(coord *pos);
double coord_get_x(const coord *pos);
double coord_get_y(const coord *pos);
double coord_get_z(const coord *pos);
double coord_get_magnitude(const coord *pos);
size_t coord_get_allocated_size(const coord *pos);
void coord_set_x(coord *pos, double x);
void coord_set_y(coord *pos, double y);
void coord_set_z(coord *pos, double z);
void coord_set(coord *pos, double x, double y, double z);
void coord_copy(coord *dst, const coord *src);
void coord_printf(FILE *f, const coord *pos);

#ifdef __cplusplus
}

namespace cleed {

class Coordinate {

  public:
    explicit Coordinate() : pos(::coord_init()) {}
    Coordinate(double x, double y, double z);
    Coordinate(const Coordinate &position);
    Coordinate(const coord *position);
    ~Coordinate();
    
    /* setters */
    void setX(double x);
    void setY(double y);
    void setZ(double z);
    void setCoordinate(double x, double y, double z);
    void setCoordinate(const Coordinate &position);
    void setCoordinate(const coord *position);
    
    /* getters */
    double getMagnitude();
    double getX();
    double getY();
    double getZ();
    const coord *get_coord();
    
    /* other methods */
    void print(FILE *f = stdout);
    
  protected:
    coord *pos;
};

} /* namespace cleed */
#endif

#endif /* COORD_H */
