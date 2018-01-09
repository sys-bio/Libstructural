#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "sbml" for configuration "Debug"
set_property(TARGET sbml APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(sbml PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_DEBUG "/usr/lib/libbz2.dylib;/usr/lib/libz.dylib;/usr/lib/libxml2.dylib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/libsbml.5.15.0.dylib"
  IMPORTED_SONAME_DEBUG "libsbml.5.dylib"
  )

list(APPEND _IMPORT_CHECK_TARGETS sbml )
list(APPEND _IMPORT_CHECK_FILES_FOR_sbml "${_IMPORT_PREFIX}/lib/libsbml.5.15.0.dylib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
