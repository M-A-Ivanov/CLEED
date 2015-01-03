/*********************************************************************
LD/24.04.14

 Include file for

  - rfac version information

Changes:

*********************************************************************/

#ifndef RFAC_VER_H
#define RFAC_VER_H

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
extern "C" {
#endif

static const char *RFAC = "rfac";
static const char *RFAC_COPYRIGHT = "Copyright (c) 1994-2014 Georg Held"
"Copyright (C) 2014 Liam Deacon";
static const char *RFAC_LICENSE = "@LICENSE@";
static const char *RFAC_VERSION = "@RFAC_MAJOR@.@RFAC_MINOR@.@RFAC_PATCH@ @SOURCE_BRANCH@";
static const char *RFAC_AUTHOR = "Georg Held (g.held@reading.ac.uk)";
static const char *RFAC_MAINTAINER = "@RFAC_MAINTAINER@";
static const char *RFAC_SHORTDESC = "R-Factor program";

/* platform information */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) || defined(__WIN32__)
static const char *RFAC_PLAT = "Win32";
#elif defined(__CYGWIN__) || defined(__CYGWIN32__)
static const char *RFAC_PLAT = "Win32_Cygwin";
#elif defined(__ANDROID__)
static const char *RFAC_PLAT = "Android";
#elif defined(__linux__) || defined(__linux)
static const char *RFAC_PLAT = "Linux";
#else
static const char *RFAC_PLAT = "Unix";
#endif

#ifdef __cplusplus /* If this is a C++ compiler, use C linkage */
}
#endif

#endif /* RFAC_VER_H */
