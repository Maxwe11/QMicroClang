#-------------------------------------------------
#
# Project created by QtCreator 2011-11-07T17:41:51
#
#-------------------------------------------------

QT       += core gui

TARGET = QMicroClang
TEMPLATE = app


SOURCES += main.cpp\
        qmicroclangide.cpp \
        codeeditor.cpp \
        qmicroclangtables.cpp \
    scanner.cpp \
    polish_elemnts.cpp \
    parser.cpp \
    compiler.cpp \
    codegenerator.cpp

HEADERS  += qmicroclangide.h \
            codeeditor.h \
            qmicroclangtables.h \
    utility.h \
    scanner.h \
    polish_elemnts.h \
    parser.h \
    compiler.h \
    codegenerator.h

FORMS    += qmicroclangide.ui \
            qmicroclangtables.ui







