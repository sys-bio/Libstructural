#include(${CMAKE_CURRENT_SOURCE_DIR}/FindSWIG.cmake) # this is slightly modified FindSWIG.cmake file -  had to tune it to work with OSX

FIND_PACKAGE(SWIG REQUIRED)

INCLUDE(UseSWIG)

if(POLICY CMP0025)
cmake_policy(SET CMP0025 NEW)
 endif()

find_package(PythonInterp)
# specify that the same python library should be found
# as the one the selected interpreter uses
set (Python_ADDITIONAL_VERSIONS ${PYTHON_VERSION_STRING})
find_package(PythonLibs)

# FIND_PACKAGE(NumPy)
#
# MESSAGE ("THIS IS NUMPY_VERSION " ${NUMPY_VERSION})
#
# MESSAGE ("THIS IS NUMPY_INCLUDE_DIRS " ${NUMPY_INCLUDE_DIRS})
#
# MESSAGE ("THIS IS PYTHON_INCLUDE_PATH " ${PYTHON_INCLUDE_PATH})
#

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH}
${CMAKE_CURRENT_SOURCE_DIR}/../include
${CMAKE_CURRENT_SOURCE_DIR}/../LibStructural/include)

set(CMAKE_SWIG_FLAGS, -python)
set(CMAKE_SWIG_FLAGS "-c++" )
set(CMAKE_SWIG_FLAGS -DLIB_EXTERN)
set(CMAKE_SWIG_OUTDIR ${CMAKE_INSTALL_PREFIX}/wrapper/structural)
set(SWIG_OUTFILE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../SWIG)

set_source_files_properties(libStructural.i PROPERTIES CPLUSPLUS on)

swig_add_library(structural
LANGUAGE python
SOURCES libstructural.i
)

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

set_target_properties(_structural PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
swig_link_libraries(structural
LibStructural-static
${LIBXML2_LIBRARY}
${BZ2_LIBRARY}
${LIBZ_LIBRARY}
${LIBICONV_LIBRARY}
)

else ()
swig_link_libraries(structural
LibStructural-static
${LIBXML2_LIBRARY}
${BZ2_LIBRARY}
${LIBZ_LIBRARY}
${LIBICONV_LIBRARY}
${PYTHON_LIBRARY}
)
endif()

set(PYTHON_OUTPUT_DIR ${CMAKE_INSTALL_PREFIX}/wrapper/structural)

INSTALL(TARGETS _structural
DESTINATION ${PYTHON_OUTPUT_DIR}
)

INSTALL(FILES __init__.py
DESTINATION ${PYTHON_OUTPUT_DIR}
)

INSTALL(DIRECTORY test/
DESTINATION ${PYTHON_OUTPUT_DIR}/test
)
