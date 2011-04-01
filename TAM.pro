#-------------------------------------------------
#
# Project created by QtCreator 2011-03-18T09:33:21
#
#-------------------------------------------------

QT       += sql xml

TARGET = TAM
TEMPLATE = app

CONFIG += static
SOURCES += main.cpp\
        mainwindow.cpp \
    threadcomhandler.cpp \
    tei_146i.cpp \
    tei_146c.cpp \
    tei.cpp \
    protocole.cpp \
    mode4_sx_6000_17.cpp \
    mode4.cpp \
    communicationserie.cpp \
    commandesliste.cpp \
    api.cpp \
    test.cpp \
    phase.cpp \
    equipement.cpp \
    definitions_globales.cpp \
    tei_49ps.cpp \
    spanhandler.cpp

HEADERS  += mainwindow.h \
    threadcomhandler.h \
    tei_146i.h \
    tei_146c.h \
    tei.h \
    protocole.h \
    mode4_sx_6000_17.h \
    mode4StructConfig.h \
    mode4.h \
    communicationserie.h \
    commandesliste.h \
    api.h \
    test.h \
    phase.h \
    equipement.h \
    definitions_globales.h \
    tei_49ps.h \
    spanhandler.h

FORMS    += mainwindow.ui

INCLUDEPATH += ./qserialdevice \
               ./
LIBS += -L./ \
    -lqserialdevice

OTHER_FILES += \
    licence.txt
