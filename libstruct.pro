TEMPLATE = subdirs

include(common.pri)

SUBDIRS =  LibLA
SUBDIRS += LibLA-static
SUBDIRS += LibStructural
SUBDIRS += LibStructural-static

!isEmpty(SBW_DIR) { 

SUBDIRS += SBW_CLAPACK
SUBDIRS += SBW_Structural

}

SUBDIRS += TestLibStruct
