# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.arithchk.Debug:
/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/F2CLIBS/libf2c/Debug/arithchk:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/F2CLIBS/libf2c/Debug/arithchk


PostBuild.f2c.Debug:
/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/F2CLIBS/libf2c/Debug/libf2c.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/F2CLIBS/libf2c/Debug/libf2c.a


PostBuild.blas.Debug:
/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/BLAS/SRC/Debug/libblas.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/BLAS/SRC/Debug/libblas.a


PostBuild.lapack.Debug:
/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/SRC/Debug/liblapack.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/SRC/Debug/liblapack.a


PostBuild.arithchk.Release:
/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/F2CLIBS/libf2c/Release/arithchk:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/F2CLIBS/libf2c/Release/arithchk


PostBuild.f2c.Release:
/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/F2CLIBS/libf2c/Release/libf2c.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/F2CLIBS/libf2c/Release/libf2c.a


PostBuild.blas.Release:
/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/BLAS/SRC/Release/libblas.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/BLAS/SRC/Release/libblas.a


PostBuild.lapack.Release:
/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/SRC/Release/liblapack.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/SRC/Release/liblapack.a


PostBuild.arithchk.MinSizeRel:
/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/F2CLIBS/libf2c/MinSizeRel/arithchk:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/F2CLIBS/libf2c/MinSizeRel/arithchk


PostBuild.f2c.MinSizeRel:
/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/F2CLIBS/libf2c/MinSizeRel/libf2c.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/F2CLIBS/libf2c/MinSizeRel/libf2c.a


PostBuild.blas.MinSizeRel:
/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/BLAS/SRC/MinSizeRel/libblas.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/BLAS/SRC/MinSizeRel/libblas.a


PostBuild.lapack.MinSizeRel:
/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/SRC/MinSizeRel/liblapack.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/SRC/MinSizeRel/liblapack.a


PostBuild.arithchk.RelWithDebInfo:
/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/F2CLIBS/libf2c/RelWithDebInfo/arithchk:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/F2CLIBS/libf2c/RelWithDebInfo/arithchk


PostBuild.f2c.RelWithDebInfo:
/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/F2CLIBS/libf2c/RelWithDebInfo/libf2c.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/F2CLIBS/libf2c/RelWithDebInfo/libf2c.a


PostBuild.blas.RelWithDebInfo:
/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/BLAS/SRC/RelWithDebInfo/libblas.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/BLAS/SRC/RelWithDebInfo/libblas.a


PostBuild.lapack.RelWithDebInfo:
/Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/SRC/RelWithDebInfo/liblapack.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/install_x64/SRC/RelWithDebInfo/liblapack.a




# For each target create a dummy ruleso the target does not have to exist
