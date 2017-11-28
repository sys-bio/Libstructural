# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.LibLA.Debug:
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/Debug/libLibLA.dylib:\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/lapack.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/blas.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/libf2c.lib
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/Debug/libLibLA.dylib


PostBuild.LibLA-static.Debug:
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/Debug/libLibLA-static.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/Debug/libLibLA-static.a


PostBuild.LibStructural.Debug:
PostBuild.LibLA-static.Debug: /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/Debug/libLibStructural.dylib
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/Debug/libLibStructural.dylib:\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/libsbml-5.15.0-vs2017-release-64/lib/libsbml.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/Debug/libLibLA-static.a\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/lapack.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/blas.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/libf2c.lib
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/Debug/libLibStructural.dylib


PostBuild.LibStructural-static.Debug:
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/Debug/libLibStructural-static.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/Debug/libLibStructural-static.a


PostBuild.LibLA.Release:
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/Release/libLibLA.dylib:\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/lapack.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/blas.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/libf2c.lib
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/Release/libLibLA.dylib


PostBuild.LibLA-static.Release:
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/Release/libLibLA-static.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/Release/libLibLA-static.a


PostBuild.LibStructural.Release:
PostBuild.LibLA-static.Release: /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/Release/libLibStructural.dylib
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/Release/libLibStructural.dylib:\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/libsbml-5.15.0-vs2017-release-64/lib/libsbml.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/Release/libLibLA-static.a\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/lapack.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/blas.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/libf2c.lib
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/Release/libLibStructural.dylib


PostBuild.LibStructural-static.Release:
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/Release/libLibStructural-static.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/Release/libLibStructural-static.a


PostBuild.LibLA.MinSizeRel:
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/MinSizeRel/libLibLA.dylib:\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/lapack.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/blas.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/libf2c.lib
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/MinSizeRel/libLibLA.dylib


PostBuild.LibLA-static.MinSizeRel:
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/MinSizeRel/libLibLA-static.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/MinSizeRel/libLibLA-static.a


PostBuild.LibStructural.MinSizeRel:
PostBuild.LibLA-static.MinSizeRel: /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/MinSizeRel/libLibStructural.dylib
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/MinSizeRel/libLibStructural.dylib:\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/libsbml-5.15.0-vs2017-release-64/lib/libsbml.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/MinSizeRel/libLibLA-static.a\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/lapack.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/blas.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/libf2c.lib
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/MinSizeRel/libLibStructural.dylib


PostBuild.LibStructural-static.MinSizeRel:
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/MinSizeRel/libLibStructural-static.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/MinSizeRel/libLibStructural-static.a


PostBuild.LibLA.RelWithDebInfo:
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/RelWithDebInfo/libLibLA.dylib:\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/lapack.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/blas.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/libf2c.lib
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/RelWithDebInfo/libLibLA.dylib


PostBuild.LibLA-static.RelWithDebInfo:
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/RelWithDebInfo/libLibLA-static.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/RelWithDebInfo/libLibLA-static.a


PostBuild.LibStructural.RelWithDebInfo:
PostBuild.LibLA-static.RelWithDebInfo: /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/RelWithDebInfo/libLibStructural.dylib
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/RelWithDebInfo/libLibStructural.dylib:\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/libsbml-5.15.0-vs2017-release-64/lib/libsbml.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/RelWithDebInfo/libLibLA-static.a\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/lapack.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/blas.lib\
	/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/libf2c.lib
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/RelWithDebInfo/libLibStructural.dylib


PostBuild.LibStructural-static.RelWithDebInfo:
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/RelWithDebInfo/libLibStructural-static.a:
	/bin/rm -f /Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibStructural/RelWithDebInfo/libLibStructural-static.a




# For each target create a dummy ruleso the target does not have to exist
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/Debug/libLibLA-static.a:
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/MinSizeRel/libLibLA-static.a:
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/RelWithDebInfo/libLibLA-static.a:
/Users/yosefbedaso/Documents/GitHub/Libstructural/build_x64/LibLA/Release/libLibLA-static.a:
/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/libsbml-5.15.0-vs2017-release-64/lib/libsbml.lib:
/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/blas.lib:
/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/lapack.lib:
/Users/yosefbedaso/Documents/GitHub/Libstructural/dependencies/third-party/clapack/3.2.1/LIB/libf2c.lib:
