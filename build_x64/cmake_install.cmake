# Install script for directory: C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/install_x64")
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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES
    "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Redist/MSVC/14.11.25325/x64/Microsoft.VC141.CRT/msvcp140.dll"
    "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Redist/MSVC/14.11.25325/x64/Microsoft.VC141.CRT/vcruntime140.dll"
    "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Redist/MSVC/14.11.25325/x64/Microsoft.VC141.CRT/concrt140.dll"
    )
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/blaswrap.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/clapack.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/complex.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/constraint.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/f2c.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/fluxbalance.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/libla.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/libutil.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/lp_Hash.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/lp_SOS.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/lp_lib.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/lp_matrix.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/lp_mipbb.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/lp_utils.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/lpresult.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/matrix - Copy.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/matrix.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/objective.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/sbmlmodel.h"
    "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/include/util.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/build_x64/LibLA/cmake_install.cmake")
  include("C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/build_x64/LibStructural/cmake_install.cmake")
  include("C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/build_x64/SWIG/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/yosef/Documents/Visual Studio 2017/Projects/libstruct/build_x64/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
