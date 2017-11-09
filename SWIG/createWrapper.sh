test -d java && rm -rf java 
test -d java || mkdir java

test -d python && rm -rf python 
test -d python || mkdir python

test -d csharp && rm -rf csharp 
test -d csharp || mkdir csharp

test -d ruby && rm -rf ruby 
test -d ruby || mkdir ruby

test -d r && rm -rf r 
test -d r || mkdir r

/usr/local/bin/swig -java -c++ -module structural -DBEGIN_C_DECLS -DEND_C_DECLS -DLIB_EXTERN -I../LibStructural/include -I../LibLA/include -I/usr/local/share/swig/1.3.33/java -I/usr/local/share/swig/1.3.33 -cpperraswarn -outdir java -o libstructural_wrap_j.cpp java.i
/usr/local/bin/swig -csharp -namespace structuralCS -c++  -module structural -DBEGIN_C_DECLS -DEND_C_DECLS -DLIB_EXTERN -I../LibStructural/include -I../LibLA/include -I/usr/local/share/swig/1.3.33/csharp -I/usr/local/share/swig/1.3.33 -cpperraswarn -outdir csharp -o libstructural_wrap_cs.cpp csharp.i
/usr/local/bin/swig -python -c++ -module structural -DBEGIN_C_DECLS -DEND_C_DECLS -DLIB_EXTERN -I../LibStructural/include -I../LibLA/include -I/usr/local/share/swig/1.3.33/python -I/usr/local/share/swig/1.3.33 -cpperraswarn -outdir python -o libstructural_wrap_python.cpp libstructural.i
/usr/local/bin/swig -ruby -c++ -module structural -DBEGIN_C_DECLS -DEND_C_DECLS -DLIB_EXTERN -I../LibStructural/include -I../LibLA/include -I/usr/local/share/swig/1.3.33/ruby -I/usr/local/share/swig/1.3.33 -cpperraswarn -outdir ruby -o libstructural_wrap_ruby.cpp libstructural.i
/usr/local/bin/swig -r -c++ -module structural -DBEGIN_C_DECLS -DEND_C_DECLS -DLIB_EXTERN -I../LibStructural/include -I../LibLA/include -I/usr/local/share/swig/1.3.33/r -I/usr/local/share/swig/1.3.33 -cpperraswarn -outdir r -o libstructural_wrap_rlang.cpp libstructural.i


scons PYTHON=/System/Library/Frameworks/Python.framework/Versions/Current/  RUBY=/sw/lib/ruby/1.8/i686-darwin/ R_DIR=/Library/Frameworks/R.framework/Versions/Current/Headers

cd csharp
rm StringDouble.cs
rm StringDoubleVector.cs
gmcs *.cs -target:library -out:../bin/libstructuralcs.dll
