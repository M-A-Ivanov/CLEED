#############################################################################
# Makefile for building: cleed_nsym                                         #
# Author: Liam Deacon (Email: liam.deacon@diamond.ac.uk)                    #
# Project: CLEED                                                            #
# Date: 2014-04-01                                                          #
# Notes:                                                                    #
#	* Assumes modern GNU gcc toolchain ( > 4.4.0 )                          #
#	* Windows users: install MinGW and add bin directory to PATH            #
#   * Multicore code uses openmp by default - remove '-D_USE_OPENMP' if     #
#     not desired.                                                          #
#   * Extra optimisations included in $(OPTS) . Linux users may wish to     #
#     add '-flto' for further HPC link time optimisations. These            #
#     optimisations are highly tuned to the platform they are compiled on   #
#     and both reduce portability and are potentially unsafe; as such it    #
#     may be better to use 'OPTS = -O2 -mtune=native' if there are concerns #
#   * For GPU acceleration uncomment the line with 'OPENCL = 1' and ensure  #
#     the OPENCL_LIB and OPENCL_INCLUDE environment variables point to the  #
#     correct locations for your platform.                                  #
#############################################################################

pkgname        = CLEED
TARGET         = cleed_nsym
PREFIX         = cleed
	  
#============================================================================
#  GH/21.06.2006
#  makefile for the non-symmetry LEED program
#============================================================================
.SUFFIXES: .o .c .h
#=============================================================================
MYPRG = ..
#INCLUDEDIR = $(MYPRG)$(SEPARATOR)INCLUDE
INCLUDEDIR = $(MYPRG)/INCLUDE
LIBDIR = .

#==flags=====================================================================
CCOMP = cc
OPT  = -O2 -mtune=native -g -std=c99
DEFINES = -DCONTROL -DCONTROL_X
WARNINGS  = -Wall -Wextra
CFLAGSSUB = -c $(WARNINGS) $(DEFINES) -I$(INCLUDEDIR) -L$(LIBDIR) 
FFLAGSSUB = -c $(WARNINGS)
CFLAGS = $(WARNINGS) $(DEFINES) $(OPT) -I$(INCLUDEDIR) -L$(LIBDIR)
LDFLAGS = -lm 
#============================================================================
# Disable extreme optimisations if experiencing stability problems
# (uncomment to enable)
#OPT = -O3 -march=native -std=c99 -mfpmath=sse -ffast-math -funroll-loops
#============================================================================
# OpenMP (uncomment to enable multicore support)
DEFINES += -D_USE_OPENMP 
OPT += -fopenmp 
#============================================================================
# OpenCL support
# Uncomment the following lines to use OpenCL
#DEFINES += -D_USE_OPENCL
#LDFLAGS += -lOpenCL
CL_LIB = $(shell $(ECHO) "$(OPENCL_LIB)") 
CL_INCLUDE = $(shell $(ECHO) "$(OPENCL_INCLUDE)")
CFLAGSSUB += -I$(CL_INCLUDE) -L$(CL_LIB)
CFLAGS += -I$(CL_INCLUDE) -L$(CL_LIB)
#============================================================================
#  header files
#
FILES.h = $(INCLUDEDIR)/stddef.h \
          $(INCLUDEDIR)/leed.h
#============================================================================
#  subroutines  for matrix operations
#
# complex library 
CPLOBJ =  cribasfun.o

# basic qm functions
QMOBJ  =  qmbessm.o \
          qmcgc.o   \
          qmhank.o  \
          qmylm.o

# matrix library
MATOBJ =  matabs.o      \
          matalloc.o    \
          matarralloc.o \
          matarrfree.o  \
          matcgau.o     \
          matcheck.o    \
          matclu.o      \
          matcol.o      \
          matconj.o     \
          matcop.o      \
          matdiff.o     \
          matext.o      \
          matfree.o     \
          matins.o      \
          matinv.o      \
          matmul.o      \
          matnattovht.o \
          matread.o     \
          matrlu.o      \
          matrow.o      \
          matrm.o       \
          matscal.o     \
          matshow.o     \
          matshowabs.o  \
          matshowpar.o  \
          matsqmod.o    \
          mattrace.o    \
          mattrans.o    \
          matwrite.o

# input for LEED programs
INPOBJ =  linpbullayer.o \
          linpdebtemp.o  \
          linpovllayer.o \
          linpphasend.o  \
          linprdbulnd.o  \
          linprdpar.o    \
          linprdovlnd.o  \
          linpshowbop.o  \
          linpmatlm.o 

INPOBJSYM = linpbullaysym.o \
            linpovlaysym.o  \
            linprdbulsym.o  \
            linprdovlsym.o  \
          # linpphase.o     \
            lwritepar.o     \
            lreadpar.o

# output for LEED programs
OUTOBJ =  loutbmlist.o \
          louthead.o   \
          loutint.o 
          
OUTOBJSYM = louthead2.o  \
            loutintsym.o
# beams:
BEAMOBJ = lbmgen.o    \
          lbmselect.o \
          lbmset.o

BEAMOBJSYM = lbmgensym.o \
             lbmrotmat.o 
             
# parameter control:
PCOBJ =   lpcmktlnd.o \
          lpctemtl.o \
          lpcupdate.o

PCOBJSYM = lsymcheck.o
         # lpcmktl.o

# layer doubling:
LDOBJ  = lld2n.o      \
         lld2lay.o    \
         lld2layrpm.o \
         lldpotstep.o \
         lldpotstep0.o

# multiple scattering:
MSOBJ  = lmsbravlnd.o  \
         lmscomplnd.o  \
         lmslsumii.o   \
         lmslsumij.o   \
         lmspartinv.o  \
         lmstmatii.o   \
         lmstmatndii.o \
         lmstmatij.o   \
         lmsymat.o     \
         lmsymatset.o  \
         lmsymmat.o    \
         lmsypy.o

MSOBJSYM  = lmscomplsym.o \
            lmscompksum.o \
            lmsbravlsym.o \
          # lmsbravl.o    \
          # lmscompl.o    \
            lmsltok.o

PRGOBJ = prgcheck.o \
         lhelp.o
         
PRGOBJSYM = lhelpsym.o

TMAOBJ = lpccumtl.o \
         lpcmkms.o

LEEDOBJ = $(CPLOBJ)  \
          $(QMOBJ)   \
          $(MATOBJ)  \
          $(INPOBJ)  \
          $(OUTOBJ)  \
          $(BEAMOBJ) \
          $(PCOBJ)   \
          $(LDOBJ)   \
          $(MSOBJ)   \
          $(PRGOBJ)

TMATOBJ = $(CPLOBJ)  \
          $(QMOBJ)   \
          $(MATOBJ)  \
          $(INPOBJ)  \
          $(PCOBJ)   \
          $(PRGOBJ)  \
          $(OUTOBJ)  \
          $(BEAMOBJ) \
          $(LDOBJ)   \
          $(MSOBJ)   \
          $(TMAOBJ)

OBJ = $(TMATOBJ)

SYMOBJ = $(MSOBJSYM)  \
         $(OUTOBJSYM) \
         $(INPOBJSYM) \
         $(PRGOBJSYM) \
         $(BEAMOBJSYM) 

LIBOBJ = $(OBJ) 
LIB = libLEED

#customise the following to best suite your system

ifndef CONFIGURED
ifdef ComSpec
#setup Windows based commands
AR            ?= ar
CCC           ?= g++
CFLAGS        ?= -c -Wall -O2 -mtune=native
CHK_DIR_EXISTS?= if not exist
CLASSPATH     ?=
COPY           = cmd.exe /C copy /y
COPY_FILE      = $(COPY)
COPY_DIR       = xcopy -s -q -y -i
DEL_FILE       = cmd.exe /C del /Q
FC            ?= gfortran
JCC           ?= javac
JFLAGS        ?=
LDFLAGS       += -lws2_32 -lpsapi
LS             = cmd /C dir /B
MAKEFILE      ?= Makefile
MKDIR          = cmd.exe /C mkdir
MOVE           = cmd.exe /C move /Y
OBJ           += getrusage_win32.o
SYMLINK       ?= $(COPY)
RM             = $(DEL_FILE)
RMDIR          = cmd.exe /C rmdir /S /Q
EXE           ?= .exe
ECHO           = cmd.exe /C echo off >NUL 2>NUL & echo 

bindir        ?= $(prefix)$(SEPARATOR)bin
libexecdir    ?= $(prefix)$(SEPARATOR)lib
libdir        ?= $(libexecdir)
localstatedir ?= $(prefix)$(SEPARATOR)var
localedir     ?= $(datarootdir)$(SEPARATOR)locale
datarootdir   ?= $(prefix)$(SEPARATOR)share
datadir       ?= $(datarootdir)
docdir        ?= $(datarootdir)$(SEPARATOR)doc$(SEPARATOR)$(pkgname)
includedir    ?= $(prefix)$(SEPARATOR)include
infodir       ?= $(datarootdir)$(SEPARATOR)info
mandir        ?= $(datarootdir)$(SEPARATOR)man
oldincludedir ?=
prefix        ?= \"$(ProgramFiles)\"
sbindir       ?=
sharedstatedir?= $(prefix)$(SEPARATOR)com
srcdir        ?= src
sysconfdir    ?= $(prefix)$(SEPARATOR)etc

#custom paths & files
BIN_DIR       ?= bin$(SEPARATOR)
EXAMPLES      ?= examples$(SEPARATOR)
INSTALL_PATH  ?= $(prefix)$(SEPARATOR)$(pkgname)$(SEPARATOR)
INSTALL_NAME  ?=
LINK_PATH     ?=
LIB_ARGS      ?= -Wl,--subsystem,windows
LIB_EXT       ?= dll
LIB_DIR       ?= lib\\
SRC           ?= src\\
SEPARATOR     ?= \\

else
#check system using uname
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
#setup Linux based commands
AR            ?= ar
CC            ?= gcc
CXX           ?= g++
CFLAGS        ?= -c -Wall -O2 -mtune=native
CHK_DIR_EXISTS?= if not exist
CLASSPATH     ?=
COPY           = cp --force
COPY_FILE      = $(COPY)
COPY_DIR       = cp -r
DEL_FILE       = rm
EXE           ?=
FC            ?= gfortran
JCC           ?= javac
JFLAGS        ?=
LDFLAGS       ?=
LS             = ls -1
MAKEFILE      ?= Makefile
MKDIR          = mkdir -p
MOVE           = mv
SYMLINK        = ln -s
RM             = rm -f
RMDIR          = rm -rf
ECHO           = echo

#custom paths & files

bindir        ?= $(prefix)/bin
libexecdir    ?= $(prefix)/libexec
libdir        ?= $(libexecdir)
localstatedir ?= $(prefix)/var
localedir     ?= $(datarootdir)/locale
datarootdir   ?= $(prefix)/share
datadir       ?= $(datarootdir)
docdir        ?= $(datarootdir)/doc/$(pkgname)
includedir    ?= $(prefix)/include
infodir       ?= $(datarootdir)/info
mandir        ?= $(datarootdir)/man
oldincludedir ?=
prefix        ?= /usr/local/
sbindir       ?=
sharedstatedir?= $(prefix)/com
srcdir        ?= src/
sysconfdir    ?= $(prefix)/etc

BIN_DIR       ?= bin/
EXAMPLES      ?= examples/
INSTALL_PATH  ?= $(prefix)
INSTALL_NAME  ?= $(pkgname)/
LINK_PATH     ?= /usr/local/bin/
LIB_ARGS      ?=
LIB_EXT       ?= so
LIB_DIR       ?= lib/
SRC           ?= src/
SEPARATOR     ?= /

endif
endif
endif

#maintain compatibility with Georg's original makefile syntax
ifdef CC
CCOMP = $(CC)
else
CCOMP = cc
endif

#setup phony targets
.PHONY = all clean install lib dll uninstall
.DEFAULT = $(TARGET)

all: lib $(TARGET)
	@echo
	@echo "---------------------------------------"
	@echo "Completed compiling $(subst test,$(PREFIX),$(TARGET))$(EXE)"
	@echo
	
$(TARGET): $(OBJ)
	$(CCOMP) -o $(TARGET)$(EXE) $(CFLAGS) $(TARGET).c $(OBJ) $(LDFLAGS)
	@-$(MKDIR) ..$(SEPARATOR)$(BIN_DIR)
	-$(MOVE) $(TARGET)$(EXE) ..$(SEPARATOR)$(BIN_DIR)$(TARGET)$(EXE)

%.o: $(TARGET).h
	$(CCOMP) $(CFLAGSSUB) $(subst .o,.c,$@) -o $@

install:
	@echo
	@echo "---------------------------------------"
	@echo "Installing files to $(INSTALL_PATH)$(INSTALL_NAME)..."
	@echo
	@-$(MKDIR) "$(INSTALL_PATH)$(INSTALL_NAME)$(BIN_DIR)"
    @-$(MKDIR) "$(INSTALL_PATH)$(LIB_DIR)"
	@echo
    # add executable
	@$(COPY) $(BIN_DIR)*$(EXE) "$(INSTALL_PATH)$(INSTALL_NAME)$(BIN_DIR)"
    # add library files
	ifeq ($(LIB_EXT), dll)
	@$(COPY) $(BIN_DIR)$(LIB).$(LIB_EXT) "$(INSTALL_PATH)$(BIN_DIR)"
	else
	@$(COPY) $(BIN_DIR)$(LIB).$(LIB_EXT) "$(INSTALL_PATH)$(LIB_DIR)"
	endif
	@echo
	@echo "---------------------------------------"
	@echo
	
uninstall:
	@echo
	@echo "---------------------------------------"
	@echo "Removing installed files from $(INSTALL_PATH)$(INSTALL_NAME)..."
	@echo
	@$(DEL_FILE) "$(INSTALL_PATH)$(INSTALL_NAME)$(BIN_DIR)$(TARGET)$(EXE)"
	ifeq ($(LIB_EXT), dll)
	@-$(DEL_FILE) "$(INSTALL_PATH)$(BIN_DIR)$(LIB).$(LIB_EXT)"
	else
	@-$(DEL_FILE) "$(INSTALL_PATH)$(LIB_DIR)$(LIB).$(LIB_EXT)"
	endif
	@-$(DEL_FILE) "$(INSTALL_PATH)$(LIB_DIR)$(LIB).a"
	@echo
	@echo "---------------------------------------"
	@echo

clean: 
	@echo
	@echo "---------------------------------------"
	@echo "Cleaning up files..."
	@echo
	$(DEL_FILE) *.o $(LIB).a $(LIB).so $(LIB).dll
	@echo 
	@echo "Cleaned"
	@echo "---------------------------------------"
	@echo 

mostlyclean: 
	@echo
	@echo "---------------------------------------"
	@echo "Cleaning up object files..."
	@echo
	$(DEL_FILE) *.o
	@echo 
	@echo "Cleaned"
	@echo "---------------------------------------"
	@echo 

lib: shared_lib static_lib 
    
static_lib: $(OBJ)
	@echo
	@echo "---------------------------------------"
	@echo "Creating static library: $(LIB).a ..."
	@echo
	$(AR) -ruc $(LIB).a $(OBJ)
	@-$(MKDIR) ..$(SEPARATOR)$(LIB_DIR)
	@$(COPY) $(LIB).a ..$(SEPARATOR)$(LIB_DIR)$(SEPARATOR)
	@echo
	@echo "---------------------------------------"
	@echo

shared_lib: $(OBJ)
	@echo
	@echo "---------------------------------------"
	@echo "Creating shared library: $(LIB).$(LIB_EXT) ..."
	@echo
	$(CCOMP) -o $(LIB).$(LIB_EXT) $(OBJ) -shared $(LIB_ARGS) $(LDFLAGS)
	@-$(MKDIR) ..$(SEPARATOR)$(LIB_DIR)
	@$(COPY) $(LIB).$(LIB_EXT) ..$(SEPARATOR)$(LIB_DIR)$(SEPARATOR)
	@echo
	@echo "---------------------------------------"
	@echo
    
dll: $(OBJ) shared_lib
