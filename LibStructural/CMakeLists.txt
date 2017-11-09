####################################################################
# CMake Build Script for libStructural
#
# Author: Frank T. Bergmann
# 


####################################################################
#
# Find all sources 
#

file(GLOB LA_SOURCES 
	${CMAKE_CURRENT_SOURCE_DIR}/../LibLA/*.cpp 	
	)

file(GLOB LS_SOURCES *.cpp )

	
include_directories(${CMAKE_CURRENT_SOURCE_DIR})	
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

####################################################################
#
# Build library
#
add_definitions(-DLPSOLVE_STATIC)
add_library (LibStructural SHARED ${LA_SOURCES} ${LS_SOURCES}  )

if (MSVC)
find_library(LPSOLVE_STATIC_LIBRARY 
	NAMES liblpsolve55
	PATHS ${DEPENDENCY_DIR}/lib 
	       ${CMAKE_SOURCE_DIR}/dependencies/lib
    )
	
target_link_libraries(LibStructural ${LIBSBML_LIBRARY} ${EXTRA_LIBS} ${LA_LIBS} ${CLAPACK_LIBS}  
    ${LPSOLVE_STATIC_LIBRARY} )

else()
target_link_libraries(LibStructural ${LIBSBML_LIBRARY} LibLA-static ${EXTRA_LIBS}  ${CLAPACK_LIBS} lpsolve-static )
endif()

add_library (LibStructural-static STATIC ${LS_SOURCES} )
if (WIN32 AND NOT CYGWIN)

	# don't decorate static library 
	SET_TARGET_PROPERTIES(LibStructural-static PROPERTIES COMPILE_DEFINITIONS "STATIC_LIB=1")
	
endif(WIN32 AND NOT CYGWIN)

target_link_libraries(LibStructural-static ${LA_LIBS} ${LIBSBML_LIBRARY} LibLA-static ${EXTRA_LIBS} lpsolve-static)

INSTALL(TARGETS LibStructural LibStructural-static
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
  )

