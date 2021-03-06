####################################################################
# CMake Build Script for the libstructural project
#
# Author: Frank T. Bergmann
#

cmake_minimum_required(VERSION 2.8)
project(LibStructural)


SET(32-BIT-BUILD FALSE CACHE BOOL "Set 32/64 bit target, set unchecked for a 64-bit build")

if (${32-BIT-BUILD})
    message ("Building 32-bit Release")
    SET(CBINARYTYPE "32")
else()
    message ("Building 64-bit Release")
    SET(CBINARYTYPE "64")
endif()


if (CBINARYTYPE STREQUAL "32")
   set (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/INSTALL_x32" CACHE PATH "default install path" FORCE)
else()
   set (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/INSTALL_x64" CACHE PATH "default install path" FORCE)
endif()



include (CMakeTestCCompiler)
include (CheckCSourceCompiles)
include (CheckCXXSourceCompiles)
include (CheckStructHasMember)
include (CheckLibraryExists)
include (CheckFunctionExists)
include (CheckCCompilerFlag)
include (CheckCSourceRuns)
include (CheckSymbolExists)
include (CheckTypeSize)

####################################################################

# Set up version information, it might be better to just write it here
# rather than parsing VERSION.txt

SET(STRUCTURAL_VERSION_MAJOR 1)
SET(STRUCTURAL_VERSION_MINOR 1)
SET(STRUCTURAL_VERSION_PATCH 0)
SET(STRUCTURAL_VERSION_RELEASE "-a1")

SET(STRUCTURAL_VERSION_NUMERIC 50000)
SET(PACKAGE_VERSION "${STRUCTURAL_VERSION_MAJOR}.${STRUCTURAL_VERSION_MINOR}.${STRUCTURAL_VERSION_PATCH}${STRUCTURAL_VERSION_RELEASE}")



####################################################################
#
# the next lines configure the parameters for packaging the binaries
# they can be invoked with: make package / nmake package or by using
# cpack -G zip|deb|rpm|dmg|nsis
#

INCLUDE(InstallRequiredSystemLibraries)

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The LibStructural Library")
SET(CPACK_PACKAGE_NAME "LibStruct")
SET(CPACK_PACKAGE_VENDOR "The SBW Team")
SET(CPACK_PACKAGE_CONTACT "SBW Team <hsauro@uw.edu>")
SET(CPACK_PACKAGE_VERSION_MAJOR "${STRUCTURAL_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${STRUCTURAL_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${STRUCTURAL_VERSION_PATCH}")
INCLUDE(CPack)

# Configuration options
set (ENABLE_PYTHON_WRAPPER FALSE CACHE BOOL "Enable Python bindings")

# set build type default
set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS are used), Debug, Release, RelWithDebInfo, MinSizeRel" FORCE )


###############################################################################
#
# list of additional files to link against.
#


if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

include_directories(${LIBXML2_INCLUDE})

find_library(BZ2_LIBRARY
  NAMES libbz2.a libbz2.so libbz2.dylib
  PATHS ${DEPENDENCY_DIR}/lib
    	${SBML_DIR}/lib
    	/usr/lib /usr/local/lib
    	${CMAKE_SOURCE_DIR}
      ${CMAKE_SOURCE_DIR}/dependencies/libsbml-vs2017-release-${CBINARYTYPE}/lib
    )


include_directories(${BZ2_INCLUDE})

find_library(LIBZ_LIBRARY
  NAMES libz.a libz.so libz.dylib
  PATHS ${DEPENDENCY_DIR}/lib
      ${SBML_DIR}/lib
      /usr/lib /usr/local/lib
      ${CMAKE_SOURCE_DIR}
    )
include_directories(${LIBZ_INCLUDE})


find_library(LIBICONV_LIBRARY
  NAMES libiconv.a libiconv.so libiconv.dylib
  PATHS ${DEPENDENCY_DIR}/lib
      ${SBML_DIR}/lib
      /usr/lib /usr/local/lib
      ${CMAKE_SOURCE_DIR}
      ${CMAKE_SOURCE_DIR}/dependencies/libsbml-vs2017-release-${CBINARYTYPE}/lib
    )



include_directories(${LIBICONV_INCLUDE})

  find_library(ZDLL_LIBRARY
    NAMES zdll.a zdll.so zdll.dylib
    PATHS ${DEPENDENCY_DIR}/lib
        ${SBML_DIR}/lib
        /usr/lib /usr/local/lib
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/dependencies/libsbml-vs2017-release-${CBINARYTYPE}/lib
      )

  include_directories(${ZDLL_INCLUDE})

else()

SET(EXTRA_LIBS_PATH ${CMAKE_SOURCE_DIR}/dependencies/libsbml-vs2017-release-${CBINARYTYPE}/lib)
set(EXTRA_LIBS
${EXTRA_LIBS_PATH}/libbz2.lib;
${EXTRA_LIBS_PATH}/libxml2.lib;
${EXTRA_LIBS_PATH}/zdll.lib;
${EXTRA_LIBS_PATH}/libiconv.lib
CACHE STRING "List of additional libraries to link against." FORCE)

endif()


####################################################################
#
# Locate Libxml2 if needed
#

# This will force CMAKE to reload library paths after selecting 32-BIT-BUILD option
unset(LIBSBML_LIBRARY CACHE)
unset(LIBSBML_INCLUDE CACHE)
unset(CLAPACK_LIBRARY CACHE)
unset(CLAPACK_BLAS_LIBRARY CACHE)
unset(CLAPACK_F2C_LIBRARY CACHE)
unset(LIBXML2_LIBRARY CACHE)



find_library(LIBSBML_LIBRARY
	NAMES libsbml-static.a libsbml-static.lib libsbml.lib sbml
	PATHS #${DEPENDENCY_DIR}/lib
		  # ${SBML_DIR}/lib
		  /usr/lib /usr/local/lib
		  # ${CMAKE_SOURCE_DIR}
		  ${CMAKE_SOURCE_DIR}/dependencies/libsbml-vs2017-release-${CBINARYTYPE}/lib
    )

find_path(LIBSBML_INCLUDE
	NAMES sbml/SBase.h
	PATHS ${DEPENDENCY_DIR}/include
		  ${SBML_DIR}/include
		  /usr/include /usr/local/include
		  ${CMAKE_SOURCE_DIR}/dependencies/libsbml-vs2017-release-${CBINARYTYPE}/include
		  )

set(STRUCTURAL_LIBS ${STRUCTURAL_LIBS} ${LIBSBML_LIBRARY})
include_directories(${LIBSBML_INCLUDE})

find_library(CLAPACK_LIBRARY
NAMES liblapack.a clapack.lib  clapack lapack
PATHS /usr/lib /usr/local/lib
	  ${CMAKE_SOURCE_DIR}
	  ${CMAKE_SOURCE_DIR}/dependencies/third-party/clapack-${CBINARYTYPE}/3.2.1/LIB
	  ${CMAKE_SOURCE_DIR}/lib
  )

set(CLAPACK_LIBS ${CLAPACK_LIBRARY})

find_library(CLAPACK_BLAS_LIBRARY
	NAMES libblas.a blas.lib blas
	PATHS /usr/lib /usr/local/lib
		${CMAKE_SOURCE_DIR}
		${CMAKE_SOURCE_DIR}/dependencies/third-party/clapack-${CBINARYTYPE}/3.2.1/LIB
		${CMAKE_SOURCE_DIR}/lib
  )

set(CLAPACK_LIBS ${CLAPACK_LIBS} ${CLAPACK_BLAS_LIBRARY})

find_library(CLAPACK_F2C_LIBRARY
	NAMES libf2c.a libf2c.lib f2c
	PATHS /usr/lib /usr/local/lib
		${CMAKE_SOURCE_DIR}
		${CMAKE_SOURCE_DIR}/dependencies/third-party/clapack-${CBINARYTYPE}/3.2.1/LIB
		${CMAKE_SOURCE_DIR}/lib
  )

set(CLAPACK_LIBS ${CLAPACK_LIBS} ${CLAPACK_F2C_LIBRARY})

find_library(LIBXML2_LIBRARY
	NAMES libxml2.a libxml2.so libxml2.dylib libxml2.lib
	PATHS ${DEPENDENCY_DIR}/lib
		  ${SBML_DIR}/lib
		  /usr/lib /usr/local/lib
		  ${CMAKE_SOURCE_DIR}
      ${CMAKE_SOURCE_DIR}/dependencies/libsbml-vs2017-release-${CBINARYTYPE}/lib
    )

####################################################################
#
# Need some variables set up, such as the name for the libSBML
# library and the Path and file separator characters
#

if(UNIX)
	set(PATH_SEP "/")
	set(FILE_SEP ":")
else()
	set(PATH_SEP "\\")
	set(FILE_SEP ";")
endif()


####################################################################
#
# Set up remaining variables, add option for universal binaries
#
set(BUILD_DEFINITIONS)
set(USING_INTEL)
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    if(APPLE)
		add_definitions(-DMACOSX -DDARWIN -DLINUX)
		set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -DMACOSX")
       # On OSX it is common to build universal binaries to support multiple
        # processor architectures. The default behavior is not to build
        # multiple architectures, as most users might not need that.
        option(ENABLE_UNIVERSAL "Create universal binaries on Mac OS X." OFF)

        set(CMAKE_OSX_ARCHITECTURES "${CMAKE_OSX_ARCHITECTURES}" CACHE STRING
           "A semicolon-separated list of build architectures to be used.")
        if(ENABLE_UNIVERSAL)
            # if universal binaries are requested and none defined so far
            # overwrite them with all three common architectures. If the user
            # specified their own list of architectures do not touch!
            if(CMAKE_OSX_ARCHITECTURES STREQUAL "")
                STRING(REGEX REPLACE "^.*MacOSX([0-9]*\\.[0-9]*)\\.sdk$" "\\1"
                       OSX_SDK_VERSION "${CMAKE_OSX_SYSROOT}")
                if(OSX_SDK_VERSION  VERSION_EQUAL "10.7" )
                   # OSX Lion no longer supports ppc architecture
                   set(CMAKE_OSX_ARCHITECTURES "i386;x86_64" CACHE STRING
                      "A semicolon-separated list of build architectures to be used." FORCE)
                else()
                   set(CMAKE_OSX_ARCHITECTURES "i386;ppc;x86_64" CACHE STRING
                       "A semicolon-separated list of build architectures to be used." FORCE)
                endif()
            endif()
        endif(ENABLE_UNIVERSAL)
	else(APPLE)
		add_definitions(-DLINUX)
		set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -DLINUX")
	endif(APPLE)
	add_definitions( -DPACKAGE_VERSION=\"${PACKAGE_VERSION}\"  -DPACKAGE_NAME=\"${PROJECT_NAME}\")
	set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -DPACKAGE_VERSION=\"${PACKAGE_VERSION}\"  -DPACKAGE_NAME=\"${PROJECT_NAME}\"")

else(UNIX)

    option(LIBSBML_STATIC_LIB "Check this if linking against static libSBML." ON)
	if (WIN32 AND CMAKE_C_COMPILER AND ${CMAKE_C_COMPILER} MATCHES ".*icl.*$")
      message(STATUS "Detected Intel Compiler")
      set(USING_INTEL TRUE)
    endif ()

	set(STRUCTURAL_LIBS ${STRUCTURAL_LIBS} )
 	add_definitions(-DWIN32 -DLIB_EXPORTS -DLIBLAX_EXPORTS)
 	set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -DWIN32 -DLIB_EXPORTS -DLIBLAX_EXPORTS")
	if (LIBSBML_STATIC_LIB)
 	  add_definitions(-DLIBSBML_STATIC -DLIBLAX_STATIC)
 	  set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -DLIBSBML_STATIC -DLIBLAX_STATIC")
	else(LIBSBML_STATIC_LIB)
 	  add_definitions(-DLIBLAX_EXPORTS -DLIBSBML_STATIC)
 	  set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -DLIBLAX_EXPORTS -DLIBSBML_STATIC")
	endif(LIBSBML_STATIC_LIB)
	if(MSVC OR USING_INTEL)
		add_definitions(-D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_DEPRECATE)
		set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -D_CRT_SECURE_NO_WARNINGS")
		option(WITH_STATIC_RUNTIME "Compile using the static MSVC Runtime" OFF)
		if (WITH_STATIC_RUNTIME)
			foreach(flag_var
				CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
				CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)

				#if(${flag_var} MATCHES "/MD")
				#	string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
      #endif(${flag_var} MATCHES "/MD")
			endforeach(flag_var)
		add_definitions( -D_MT)
		endif(WITH_STATIC_RUNTIME)

		file(GLOB WIN32_BINARIES ${CMAKE_SOURCE_DIR}/dependencies/bin/*.dll)
		INSTALL(FILES ${WIN32_BINARIES} DESTINATION bin)

	elseif(CYGWIN)
		add_definitions(-DCYGWIN)
		set(BUILD_DEFINITIONS "${BUILD_DEFINITIONS} -DCYGWIN")
	elseif(MINGW)
		if(WITH_LIBXML)
			# this is necessary to build with libxml2 on mingw
			add_definitions(-DLIBXML_STATIC)
		endif(WITH_LIBXML)
	endif(MSVC OR USING_INTEL)

endif(UNIX)


####################################################################
#
# Build the actual libraries
#


add_subdirectory(LibLA)
add_subdirectory(LibStructural)

if(ENABLE_PYTHON_WRAPPER)
	add_subdirectory(SWIG)
endif()

SET(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD ALL)

file(GLOB HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h )
install(FILES ${HEADER_FILES} DESTINATION include)
