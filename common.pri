

# In the case the BUILD_OS is not specified we make a guess.
isEmpty(BUILD_OS) {
  win32 {
    BUILD_OS = WIN32
  } else {
    BUILD_OS = $$system(uname)
  }
}

DEFINES += $$BUILD_OS

mac:DEFINES += LINUX DARWIN
unix:DEFINES += LINUX
win32:DEFINES += WIN32 LIB_EXPORTS

mac:CONFIG -= app_bundle
mac:CONFIG += x86 ppc

DESTDIR      = ../bin/

!isEmpty(SBW_DIR) { 
    
}

isEmpty(LIBSBML_DIR) {
win32:LIBSBML_DIR=..\libSBML-3.3.2
unix:LIBSBML_DIR=/usr/local/
mac:LIBSBML_DIR=/usr/local/
}


LIBS += -L/usr/local/lib
LIBS += -Ldependencies/lib
LIBS += -L../dependencies/lib



win32:LIBS += -lblas -lclapack -llibF77 -llibI77
win32:LIBS += -LLibLA\lib

message("Configuring for $${BUILD_OS} with QTDIR=$$QTDIR and LIBSBML_DIR=$$LIBSBML_DIR")

TARGETDEPS += Makefile

# Common configuration settings
CONFIG += exceptions
CONFIG += rtti
CONFIG += thread


# add CLAPACK
mac:LIBS += -framework vecLib
unix:LIBS += -llapack

mac:LIBS += -lxml2 -lbz2 -lz
unix:LIBS += -lxml2 -lbz2 -lz

# add libsbml
mac:LIBS += -lsbml
unix:LIBS += -lsbml
win32:LIBS += -llibsbml


INCLUDEPATH += /usr/local/include
INCLUDEPATH += dependencies/include
INCLUDEPATH += include
INCLUDEPATH += ../include
INCLUDEPATH += ../dependencies/include


INCLUDEPATH += LibStructural/include LibLA/include
INCLUDEPATH += ../LibStructural/include ../LibLA/include

win32:INCLUDEPATH += "$${LIBSBML_DIR}\include" "$${LIBSBML_DIR}\include\sbml"
win32:QMAKE_LFLAGS += /LIBPATH:"$${LIBSBML_DIR}\lib" 
win32:QMAKE_LFLAGS += /LIBPATH:"$${LIBSBML_DIR}\bin" 
win32:QMAKE_LFLAGS += /LIBPATH:"$${LIBSBML_DIR}\win\bin" 
win32:QMAKE_LFLAGS += /LIBPATH:"$${LIBSBML_DIR}\win32\bin" 


QMAKE_CFLAGS   += $$(CFLAGS)
QMAKE_CXXFLAGS += $$(CXXFLAGS)
QMAKE_LFLAGS  += $$(LDFLAGS)
