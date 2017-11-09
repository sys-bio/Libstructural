"@echo off" 

cd LibLA
qmake -t vclib SBW_DIR=..\..\core LIBSBML_DIR=..\..\libsbml -o libla-vs2008.vcproj
cd ..

cd LibLA-static
qmake -t vclib SBW_DIR=..\..\core LIBSBML_DIR=..\..\libsbml -o libla-vs2008-static.vcproj
cd ..

cd LibStructural
qmake -t vclib SBW_DIR=..\..\core LIBSBML_DIR=..\..\libsbml -o libstructural-vs2008.vcproj
cd .. 

cd LibStructural-static
qmake -t vclib SBW_DIR=..\..\core LIBSBML_DIR=..\..\libsbml -o libstructural-vs2008-static.vcproj
cd ..

cd SBW_Structural
qmake -t vcapp SBW_DIR=..\..\core LIBSBML_DIR=..\..\libsbml -o SBW_Structural-vs2008.vcproj
cd ..

cd SBW_CLAPACK
qmake -t vcapp SBW_DIR=..\..\core LIBSBML_DIR=..\..\libsbml -o SBW_CLAPACK-vs2008.vcproj
cd ..


