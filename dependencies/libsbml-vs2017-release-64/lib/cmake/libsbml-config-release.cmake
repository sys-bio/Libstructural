#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libsbml" for configuration "Release"
set_property(TARGET libsbml APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libsbml PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/libsbml.lib"
  IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "C:/Users/yosef/Documents/Develop/Source/libsbml-deps-vs14-win64-static/libSBML_dependencies_vs14_release_x64_static/libSBML-Dependencies-1.0.0-b1-win64/lib/libbz2.lib;C:/Users/yosef/Documents/Develop/Source/libsbml-deps-vs14-win64-static/libSBML_dependencies_vs14_release_x64_static/libSBML-Dependencies-1.0.0-b1-win64/lib/zdll.lib;C:/Users/yosef/Documents/Develop/Source/libsbml-deps-vs14-win64-static/libSBML_dependencies_vs14_release_x64_static/libSBML-Dependencies-1.0.0-b1-win64/lib/libxml2.lib;WS2_32.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/bin/libsbml.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS libsbml )
list(APPEND _IMPORT_CHECK_FILES_FOR_libsbml "${_IMPORT_PREFIX}/lib/libsbml.lib" "${_IMPORT_PREFIX}/bin/libsbml.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
