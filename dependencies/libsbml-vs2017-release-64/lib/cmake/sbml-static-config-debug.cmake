#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "sbml-static" for configuration "Debug"
set_property(TARGET sbml-static APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(sbml-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C;CXX"
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "/usr/lib/libbz2.dylib;/usr/lib/libz.dylib;/usr/lib/libxml2.dylib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libsbml-static.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS sbml-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_sbml-static "${_IMPORT_PREFIX}/lib/libsbml-static.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
