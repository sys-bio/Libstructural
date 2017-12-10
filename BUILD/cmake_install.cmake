# Install script for directory: C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/install_x64")
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

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES
    "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Redist/MSVC/14.10.25008/x64/Microsoft.VC150.CRT/msvcp140.dll"
    "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Redist/MSVC/14.10.25008/x64/Microsoft.VC150.CRT/vcruntime140.dll"
    "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Redist/MSVC/14.10.25008/x64/Microsoft.VC150.CRT/concrt140.dll"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE DIRECTORY FILES "")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/blaswrap.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/clapack.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/complex.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/constraint.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/f2c.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/fluxbalance.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/libla.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/libutil.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/lp_Hash.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/lp_SOS.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/lp_lib.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/lp_matrix.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/lp_mipbb.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/lp_utils.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/lpresult.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/matrix.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/objective.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/sbmlmodel.h"
    "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/include/util.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/BUILD/LibLA/cmake_install.cmake")
  include("C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/BUILD/LibStructural/cmake_install.cmake")
  include("C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/BUILD/SWIG/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "C:/Users/hsauro/Documents/Visual Studio 2017/Projects/Libstructural/BUILD/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
