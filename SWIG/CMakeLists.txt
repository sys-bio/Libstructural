#include(${CMAKE_CURRENT_SOURCE_DIR}/FindSWIG.cmake) # this is slightly modified FindSWIG.cmake file -  had to tune it to work with OSX

FIND_PACKAGE(SWIG REQUIRED)

INCLUDE(UseSWIG)

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

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

# INCLUDE_DIRECTORIES(${NUMPY_INCLUDE_DIRS})
# INCLUDE_DIRECTORIES(${RR_SOURCE_PATH}/third_party/rr-libstruct)


swig_add_library(_libstructural
LANGUAGE python
SOURCES libstructural.i)

swig_link_libraries(_libstructural
${EXTRA_LIBS}
LibStructural
)

#
# SET(PYINTERFACE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/pyinterface)
# SET(CMAKE_SWIG_FLAGS "")

INSTALL(TARGETS _libstructural
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
  )


# install(FILES INSTALL.txt setup.py MANIFEST.in
#         DESTINATION .
#        )

# if(WITH_CONDA_BUILDER)
#   configure_file(conda/meta.yaml.in conda/meta.yaml @ONLY)
#   install(FILES ${CMAKE_CURRENT_BINARY_DIR}/conda/meta.yaml conda/build.sh conda/bld.bat DESTINATION .)
# endif()
