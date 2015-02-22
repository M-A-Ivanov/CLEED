/*********************************************************************
 *                           PATT_VER.H
 *
 *  Copyright 2013-2014 Liam Deacon <liam.deacon@diamond.ac.uk>
 *
 *  Licensed under GNU General Public License 3.0 or later.
 *  Some rights reserved. See COPYING, AUTHORS.
 *
 * @license GPL-3.0+ <http://spdx.org/licenses/GPL-3.0+>
 *********************************************************************/

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

#ifndef PATT_VER_H
#define PATT_VER_H

#include "patt_def.h"

/* additional program information */
static const char PATT[] = "patt";
static const char PATT_COPYRIGHT[] = "Copyright (C) 1994-2014 Georg Held\n" \
"Copyright (C) 2013-2014 Liam Deacon";
static const char PATT_LICENSE[] = "GNU General Public License v3";
static const char PATT_VERSION[] = "2014.07.04 ";
static const char PATT_AUTHOR[] = "Georg Held (g.held@reading.ac.uk)";
static const char PATT_MAINTAINER[] = "Liam Deacon <liam.deacon@diamond.ac.uk>"; 
static const char PATT_SHORTDESC[] = "Generate LEED patterns";

/* platform information */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)
static const char PATT_PLAT[] = "Win32";
#elif defined(__CYGWIN__) || defined(__CYGWIN32__)
static const char PATT_PLAT[] = "Win32_Cygwin";
#elif defined(__ANDROID__)
static const char PATT_PLAT = "Android";
#elif defined(__linux__) || defined(__linux)
static const char PATT_PLAT[] = "Linux";
#else
static const char PATT_PLAT[] = "Unix";
#endif

#endif /* PATT_VER_H */

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif
