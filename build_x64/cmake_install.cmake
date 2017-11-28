# Install script for directory: /Users/yosefbedaso/Documents/GitHub/Libstructural

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/blaswrap.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/clapack.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/complex.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/constraint.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/f2c.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/fluxbalance.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/libla.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/libutil.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/lp_Hash.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/lp_SOS.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/lp_lib.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/lp_matrix.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/lp_mipbb.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/lp_types.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/lp_utils.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/lpresult.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/matrix.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/objective.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/sbmlmodel.h"
    "/Users/yosefbedaso/Documents/GitHub/Libstructural/include/util.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/cmake_install.cmake")
  include("/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
