#-------------------------------------------------
#
# Project created by QtCreator 2015-04-08T15:22:10
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ParserZP
TEMPLATE = app


SOURCES += main.cpp\
        mainform.cpp \
    parser.cpp \
    bankform.cpp \
    personform.cpp \
    selectimportform.cpp \
    aboutform.cpp \
    insertactiveform.cpp \
    minimalzpform.cpp \
    paylistbelarusbank.cpp

HEADERS  += mainform.h \
    parser.h \
    bankform.h \
    personform.h \
    selectimportform.h \
    aboutform.h \
    insertactiveform.h \
    minimalzpform.h \
    paylistbelarusbank.h

FORMS    += mainform.ui \
    bankform.ui \
    personform.ui \
    selectimportform.ui \
    aboutform.ui \
    insertactiveform.ui \
    insertactiveform.ui \
    minimalzpform.ui \
    paylistbelarusbank.ui

RESOURCES += \
    resources.qrc

OTHER_FILES += \
    version.txt
