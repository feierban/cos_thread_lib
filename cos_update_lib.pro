#-------------------------------------------------
#
# Project created by QtCreator 2018-06-19T14:35:46
#
#-------------------------------------------------

QT       -= gui

TARGET = cos_update_lib
TEMPLATE = lib

DEFINES += COS_UPDATE_LIB_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#cos base path from tencent
INCLUDEPATH +=  /home/feijf/work/git_dir/cos-cpp-sdk-v5/include


SOURCES +=  cos_update_lib.cpp \
            cos_operat.cpp \

HEADERS += cos_update_lib.h\
        cos_update_lib_global.h \


unix {
    target.path = /usr/lib
    INSTALLS += target
}


