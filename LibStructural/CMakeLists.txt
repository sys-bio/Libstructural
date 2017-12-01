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
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)

####################################################################
#
# Build library
#

add_library (LibStructural SHARED ${LA_SOURCES} ${LS_SOURCES}  )
target_link_libraries(LibStructural ${LIBSBML_LIBRARY} ${EXTRA_LIBS} ${LA_LIBS} ${CLAPACK_LIBS} Ws2_32.lib )

add_library (LibStructural-static STATIC ${LA_SOURCES} ${LS_SOURCES} )
if (WIN32 AND NOT CYGWIN)

	# don't decorate static library
	SET_TARGET_PROPERTIES(LibStructural-static PROPERTIES COMPILE_DEFINITIONS "STATIC_LIB=1")

endif(WIN32 AND NOT CYGWIN)

target_link_libraries(LibStructural-static ${LA_LIBS} ${LIBSBML_LIBRARY} LibLA-static ${EXTRA_LIBS} ${CLAPACK_LIBS} Ws2_32.lib)

INSTALL(TARGETS LibStructural LibStructural-static
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
  )
