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

INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH}
${CMAKE_CURRENT_SOURCE_DIR}/../include
${CMAKE_CURRENT_SOURCE_DIR}/../LibStructural/include)

set(CMAKE_SWIG_FLAGS, -python)
set(CMAKE_SWIG_FLAGS "-c++" )
set(CMAKE_SWIG_FLAGS -module structural)
set(CMAKE_SWIG_FLAGS -DBEGIN_C_DECLS)
set(CMAKE_SWIG_FLAGS -DEND_C_DECLS)
set(CMAKE_SWIG_FLAGS -DLIB_EXTERN)
set(CMAKE_SWIG_FLAGS -cpperraswarn)


#C:\Users\yosef\Downloads\swigwin-3.0.12\swigwin-3.0.12\swig.exe -python -c++ -module structural -DBEGIN_C_DECLS -DEND_C_DECLS -DLIB_EXTERN -cpperraswarn -outdir "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/build_x64/SWIG" "-IC:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/dependencies/libsbml-5.15.0-vs2017-release-64/include" -IC:/ProgramData/Anaconda2/include "-IC:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/SWIG" "-IC:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/SWIG/include" "-IC:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/SWIG/../include" "-IC:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/SWIG/../LibStructural/include" -o "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/build_x64/SWIG/libstructuralPYTHON_wrap.cxx" "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/SWIG/libstructural.i"


set_source_files_properties(libStructural.i PROPERTIES CPLUSPLUS on)
set_source_files_properties(libStructural.i PROPERTIES SWIG_FLAGS "-threads")
set_source_files_properties(libStructural.i PROPERTIES SWIG_FLAGS "-c++")
set_source_files_properties(libStructural.i PROPERTIES SWIG_FLAGS "-module structural")
set_source_files_properties(libStructural.i PROPERTIES SWIG_FLAGS "-DBEGIN_C_DECLS")
set_source_files_properties(libStructural.i PROPERTIES SWIG_FLAGS "-DEND_C_DECLS")
set_source_files_properties(libStructural.i PROPERTIES SWIG_FLAGS "-DLIB_EXTERN")

swig_add_library(structural
LANGUAGE python
SOURCES libstructural.i
)

swig_link_libraries(structural
LibStructural-static
${EXTRA_LIBS}
${PYTHON_LIBRARIES}
)


INSTALL(TARGETS _structural
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
  )
