# Scons Build script for the SBW conservation and clapack 
# libraries and modules
# 
# Author: Frank Bergmann (fbergman@u.washington.edu)
#

import glob
import sys


#SBW_DIR                        = '/home/fbergmann/dev/buildall/build/cvs-dl/core/include'
#LIBSBML_DIR                    = '/home/fbergmann/dev/buildall/build/cvs-dl/libsbml/'

#SBW_DIR                        = '/Users/fbergmann/Development/buildall/build/cvs-dl/core/include'
#SBW_DIR                         = '../buildall/build/cvs-dl/core/include'
#LIBSBML_DIR                     = '../buildall/build/cvs-dl/libsbml/'

SBW_DIR                        = '/Users/fbergmann/Development/core/include'
LIBSBML_DIR                    = '/Users/fbergmann/Development/libsbml/'
LPSOLVE							= '/Users/fbergman/Development/buildall/build/cvs-dl/lib/liblpsolve55.a'
#SBW_DIR                         = '../core/include'
#LIBSBML_DIR                     = '../libsbml/'

if ARGUMENTS.has_key('SBW'):
	SBW_DIR = ARGUMENTS.get('SBW')
	print "Using SBW directory: " + SBW_DIR 

if ARGUMENTS.has_key('LIBSBML'):
	LIBSBML_DIR = ARGUMENTS.get('LIBSBML')
	print "Using libsbml directory: " + LIBSBML_DIR

if ARGUMENTS.has_key('LPSOLVE'):
	LPSOLVE = ARGUMENTS.get('LPSOLVE')
	print "Using lpsolve: " + LPSOLVE


FRAMEWORKS = []
buildArchs = ''
compileFlags = ''
bzipLib = ''
# set up platform specific defines
if sys.platform in ['win32'] :
        sbw_target = ' -DWIN32'
elif sys.platform in ['darwin']:
		sbw_target = ' -DDARWIN -DLINUX '
		FRAMEWORKS =  [ 'vecLib' ]
		if ARGUMENTS.get('UNIVERSAL') == 'no':
			# make sure we compile little or big endiannes correctly
			if sys.byteorder in ['little']:
				sbw_target  = sbw_target + " -D__LITTLE_ENDIAN__"
			else:
				sbw_target  = sbw_target + " -D__BIG_ENDIAN__"
		else:
			buildArchs = '  -arch i386 -arch ppc '
			
		
else:
		sbw_target = ' -DLINUX'

if ARGUMENTS.get("BZIP") == 'no':
	print "BZ2: not linking against bzip"

else:
	print "BZ2: linking against bzip"
	bzipLib = 'bz2'


if ARGUMENTS.get('DEBUG') == 'yes':
        print 'BUILD: building LibStructural DEBUG'
        compileFlags    = compileFlags + ' -g -Wall -D_GNU_SOURCE ' + sbw_target + ' -D_DEBUG'
else:
        print 'BUILD: building LibStructural RELEASE'
        compileFlags    = compileFlags+ '  ' + sbw_target


externalLibs = []
if ARGUMENTS.has_key('EXTERNAL_LIBS'):
   #When a single item list is provided, Split does not return a list
   tmp = Split(ARGUMENTS['EXTERNAL_LIBS'].replace(',',' '))
   if(type(tmp)==type([])):
	externalLibs=tmp
   else:
	externalLibs=[tmp]

externalPaths = []
if ARGUMENTS.has_key('EXTERNAL_LIB_PATHS'):
   tmp = Split(ARGUMENTS['EXTERNAL_LIB_PATHS'].replace(',',' '))   
   if(type(tmp)==type([])):
	externalPaths = tmp
   else:
	externalPaths = [tmp]

externalIncPaths = []
if ARGUMENTS.has_key('EXTERNAL_INC_PATHS'):
   tmp = Split(ARGUMENTS['EXTERNAL_INC_PATHS'].replace(',',' '))
   if(type(tmp)==type([])):
	externalIncPaths=tmp
   else:
	externalIncPaths=[tmp]


srcLibLA              = Split ( glob.glob( './LibLA/*.cpp'          ) )
srcLibStructural      = Split ( glob.glob( './LibStructural/*.cpp'  ) )
srcSbwCLAPACK         = Split ( glob.glob( './SBW_CLAPACK/*.cpp'    ) ) 
srcSbwStructural      = Split ( glob.glob( './SBW_Structural/*.cpp' ) ) + [ './SBW_CLAPACK/sbwutil.cpp']

incLibLA              = externalIncPaths  + [ './include' ]
incLibStructural      = incLibLA + [ './include', LIBSBML_DIR + 'include' ]
incSbwCLAPACK         = incLibLA + [ './SBW_CLAPACK/', SBW_DIR, SBW_DIR + '/include' ]
incSbwStructural      = incLibStructural + [ './SBW_Structural/', SBW_DIR, SBW_DIR + '/include' ]

libsLibLA             = externalLibs + [  ]
libsLibStructural     = externalLibs + [ 'sbml-static', 'LibLA-static', 'xml2', 'z', bzipLib, 'sbml-static', 'lpsolve55' ]
libsSbwCLAPACK        = externalLibs + [ 'SBW-static', 'pthread', 'LibLA-static' ]
libsSbwStructural     = externalLibs + [ 'LibLA-static', 'sbml-static', 'SBW-static', 'pthread', 'LibLA-static', 'LibStructural-static', 'xml2', 'z', bzipLib ,'LibLA-static', 'sbml-static' ]


# platform specific alterations for the above
if sys.platform in ['win32'] :
		# none
		print 'PLATFORM: Windows'
elif sys.platform in ['darwin']:
		# none
		print 'PLATFORM: OS X'
else:
		print 'PLATFORM: *nix'
		lapackLibs  = ['lapack']
		libsLibStructural = libsLibStructural + lapackLibs
		libsSbwCLAPACK = libsSbwCLAPACK + lapackLibs
		libsSbwStructural = libsSbwStructural + lapackLibs



libPath                 = externalPaths + [ SBW_DIR + '/../lib', './lib', LIBSBML_DIR + '/src/']

if ARGUMENTS.has_key('LIBSBML_LIBPATH'):
	libPath = libPath + [ ARGUMENTS.get('LIBSBML_LIBPATH' ) ]


env                     = Environment(
                           CC         = 'g++',
			   CFLAGS     = compileFlags + buildArchs,
			   CXXFLAGS   = compileFlags + buildArchs,
			   LINKFLAGS  = buildArchs,
                           LIBPATH    = libPath,
                           FRAMEWORKS = FRAMEWORKS                      
                        )

env.SharedLibrary (
                           target     = './lib/LibLA',
                           CPPPATH    = incLibLA,
                           source     = srcLibLA,
                           LIBS       = libsLibLA
                )

env.SharedLibrary (
                target  = './lib/LibStructural',
                CPPPATH = incLibStructural,
                source  = srcLibStructural,
                LIBS    = libsLibStructural
                )

env.Library (
                target  = './lib/LibLA-static',
                CPPPATH = incLibLA,
                source  = srcLibLA,
                LIBS    = libsLibLA
                )

env.Library (
                target  = './lib/LibStructural-static',
                CPPPATH = incLibStructural,
                source  = srcLibStructural,
                LIBS    = libsLibStructural
                )

env.Program (
                target  = './bin/SBW_CLAPACK',
                source  = srcSbwCLAPACK,
                CPPPATH = incSbwCLAPACK,
                LIBS    = libsSbwCLAPACK
                )

env.Program (
                target  = './bin/SBW_STRUCTURAL',
                source  = srcSbwStructural,
                CPPPATH = incSbwStructural,
                LIBS    = libsSbwStructural
                )

if ARGUMENTS.get('EXAMPLE') == 'yes':
	env.Program(
		CC = 'gcc', CXX='gcc',
		target = './bin/printmatrices',
		source = './examples/c/loadstoichiometry.c', 
		CPPPATH = ['./LibLA/include', './LibStructural/include'],
		LIBS = ['LibStructural'], 
		LIBPATH = './lib' 
		
	)

