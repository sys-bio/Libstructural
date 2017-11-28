####################################################################
#
# CMake Build Script for LibLA
#

####################################################################
#
# Find all sources
#

file(GLOB LA_SOURCES
	*.cpp
	)


include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../LibStructural/include)
####################################################################
#
# Build library
#

add_library (LibLA SHARED ${LA_SOURCES} )
target_link_libraries(LibLA ${LA_LIBS} ${CLAPACK_LIBS} Ws2_32.lib)

add_library (LibLA-static STATIC ${LA_SOURCES} )
if (WIN32 AND NOT CYGWIN)

	# don't decorate static library
	SET_TARGET_PROPERTIES(LibLA-static PROPERTIES COMPILE_DEFINITIONS "STATIC_LIB=1")

endif(WIN32 AND NOT CYGWIN)
target_link_libraries(LibLA-static ${LA_LIBS} ${CLAPACK_LIBS} Ws2_32.lib)

INSTALL(TARGETS LibLA LibLA-static
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
  )
