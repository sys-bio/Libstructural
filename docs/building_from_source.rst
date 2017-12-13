Building From Source
=====================

The LibStructural API library can be built on your machine from the source code found on `github <https://github.com/yosefmaru/Libstructural>`_. To build libStructural, you will need:

* Source code and dependencies from github (you will need `git <https://git-scm.com/>`_ to clone/download the repo)
* libSBML dependency `library <https://sourceforge.net/projects/sbml/files/libsbml/win-dependencies/>`_. Make sure to get the `vs15_release_x64 version <https://sourceforge.net/projects/sbml/files/libsbml/win-dependencies/libSBML_dependencies_vs15_release_x64.zip/download>`_.
* `libSBML version 5.15.0 <https://sourceforge.net/projects/sbml/files/libsbml/5.15.0/stable/>`_ source file for generating Mac OS compatible libraries. Get the one named "libSBML-5.15.0-core-src.zip".
* `CMake version 3.9.3 <https://cmake.org/files/v3.9/>`_ for generating solution files.
* Visual Studio 2017 for Windows or Xcode for MacOS
* Python 2.6 or 2.7 for generating python wrappers (optional)
* `SWIG <http://www.swig.org/>`_ for generating python wrappers in Windows (optional).

-------
Windows
-------

1. Clone the source code from the `github repo <https://github.com/yosefmaru/Libstructural>`_ by running the following command from the command line or git software: ``git clone https://github.com/yosefmaru/Libstructural``

2. Download the libSBML dependency `library <https://sourceforge.net/projects/sbml/files/libsbml/win-dependencies/>`_. Extract the folder in to LibStructural/dependencies folder and rename the folder to "libsbml-vs2017-release-64". This step is very important since the CMakeLists file that is used to generate the solution object looks for libSBML libraries inside a folder named "libsbml-vs2017-release-64"

3. Open CMake-gui. Copy or Browse to the directory where LibStructural is located at for the "Where is the source code" inquiry. For the "Where to build the binaries" inquiry, use the same directory, and add /BUILD_x64.

4. On the CMake-gui click on Configure. A prompt will ask you to create a "BUILD_x64" folder inside LibStructural. Click on yes. Next, it will ask you to specify a generator for the project and a what compiler to use as shown below. For the generator, select "Visual Studio 15 2017 Win64". Then, mark the "Use default native compilers" option and click on Finish.

.. figure:: cmake_2.JPG
    :align: center
    :figclass: align-center

5. If you wish to generate a python wrapper, you will have to checkmark "ENABLE_PYTHON_WRAPPER" option. Click on Configure again and an error will appear in red on the top, asking for the location of a SWIG executable file. Click on the "SWIG_EXECUTABLE-NOTFOUND" and copy the swig.exe directory. The swig.exe is found in the `SWIG <http://www.swig.org/>`_ you downloaded. Click on Generate, and a LibStructural.sln file will be created in the BUILD_x64 folder. NOTE: If you don't want to generate a python wrapper, simply click configure and then generate after step 4.

.. figure:: cmake_SWIG_exe.JPG
    :align: center
    :figclass: align-center

6. To build the libraries, open the solution file located in BUILD_x64 folder using Visual Studio 2017. Right click on the INSTALL project and click on Build. The built library will be stored in BUILD_x64/LibStructural/Release directory. Make sure your build target is set to "Release" instead of "Debug" with "x64" configuration. If you chose to generate a python wrapper, it will be located inside INSTALL_x64/wrapper/structural.

-----
Mac
-----
For Mac, you will have to separately compile CLAPACK and libSBML dependencies. You need to generate MacOS compatible dependency libraries prior to building LibStructural. The CLAPACK package is included in the LibStructural repo that is cloned/downloaded from github. It is loacated inside the directory "dependencies/third-party". However, the libSBML is the one you should `download <https://sourceforge.net/projects/sbml/files/libsbml/5.15.0/stable/>`_ from sourceforge.

Building CLAPACK
~~~~~~~~~~~~~~~~
1. Open CMAKE-gui. Copy or Browse to the directory where CLAPCK's CMakeLists.txt file is located at for the "Where is the source code" inquiry (i.e. dependencies/third-party/clapack/3.2.1/). For the "Where to build the binaries" inquiry, use the same directory, and add /BUILD_x64.

.. figure:: cmake_xcode_1.png
    :align: center
    :figclass: align-center

2. Click on Configure. CMake will ask for what generator and compiler to use. Find and select Xcode from the dropdown menu. Select "Specify native compilers" and click on continue. On the next window, under C++ box, click on the three dots and locate you Xcode application. It should be inside the Applications folder. Click on Done and then Generate.

.. figure:: cmake_xcode_2.png
    :align: center
    :figclass: align-center

.. figure:: cmake_xcode_compilers.png
    :align: center
    :figclass: align-center

3. Your Xcode project file will be generated inside BUILD_x64, or you can simply click on Open Project on CMake to automatically launch Xcode.

4. Once Xcode finishes loading/indexing the project, go to Product -> Scheme -> Edit Scheme (keyboard shortcut: Command + Shift + <). You will need to set the Build Configuration to "Release". Click on Product -> Build or the Play button.

.. figure:: xcode_scheme.png
    :align: center
    :figclass: align-center

5. This will generate the libraries inside BUILD_x64. There are three libraries you will need to locate. The first one is libblas.a which will be found in the directory "BUILD_x64/BLAS/SRC/Release". The second one is libf2c.a which will be found in the directory "BUILD_x64/F2CLIBS/libf2c/Release". The third one is liblapack.a inside "BUILD_x64/SRC/Release. Move these three libraries to "Libstructural/dependencies/third-party/clapack/3.2.1/LIB".

Building libSBML
~~~~~~~~~~~~~~~~
You use the same steps as shown above with minor changes. Your source code is `libSBML package <libSBML-5.15.0-core-src.zip>`_ you downloaded from sourceforge. After you extract it, locate the folder from CMake-gui i.e. Downloads/libsbml-5.15.0. Click on Browse Build and use the same directory, but add BUILD_x64. Compile the generated project file and locate the libsbml-static.a library. It will be located inside "BUILD_x64/src/Release". Move the library into "Libstructural/dependencies/libsbml-vs2017-release-64/lib".

Building LibStructural
~~~~~~~~~~~~~~~~~~~~~~
1. Open CMake-gui. For the "Where is the source code" inquiry, click on Browse Source to get the directory where LibStructural is located at. For the "Where to build the binaries" inquiry, use the same directory but add /BUILD_x64.

2. Follow step two from the "Building CLAPACK" section.

3. If you chose build a python wrapper, checkmark the "ENABLE_PYTHON_WRAPPER" value and configure again. If no errors are raised click configure. Note: You don't need to download SWIG as it is included in with your Mac OS.

4. Follow step three from the "Building CLAPACK" section.

5. Once Xcode finishes loading/indexing the project, go to Product -> Scheme -> Edit Scheme (keyboard shortcut: Command + Shift + <). You will need to set the Build Configuration to "Release". If you chose to generate python wrapper from step three, go to Product -> Scheme and select "install". Otherwise, click on Product -> Build or the Play button (keyboard shortcut: Command + B).

6. The built library will be stored in BUILD_x64/LibStructural/Release directory. If you chose to generate a python wrapper, it will be located inside INSTALL_x64/wrapper/structural.
