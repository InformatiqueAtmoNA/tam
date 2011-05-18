#-------------------------------------------------
#
# Project created by QtCreator 2011-03-18T09:33:21
#
#-------------------------------------------------

QT       += sql xml

TARGET = TAM
TEMPLATE = app

CONFIG += static += console
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
    definitions_globales.cpp \
    tei_49ps.cpp \
    spanhandler.cpp \
    creationtest.cpp \
    bdhandler.cpp \
    dlg_molecule.cpp \
    dlg_protocole.cpp \
    dlg_marque.cpp \
    dlg_tx_transmission.cpp \
    dlg_modele.cpp \
    dlg_equipement.cpp \
    dlg_concentration.cpp \
    dlg_systeme_etalon.cpp \
    ct_phasewidget.cpp

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
    definitions_globales.h \
    tei_49ps.h \
    spanhandler.h \
    creationtest.h \
    bdhandler.h \
    dlg_molecule.h \
    dlg_protocole.h \
    dlg_marque.h \
    dlg_tx_transmission.h \
    dlg_modele.h \
    dlg_equipement.h \
    dlg_concentration.h \
    dlg_systeme_etalon.h \
    ct_phasewidget.h

FORMS    += mainwindow.ui \
    creationtest.ui \
    dlg_molecule.ui \
    dlg_protocole.ui \
    dlg_marque.ui \
    dlg_tx_transmission.ui \
    dlg_modele.ui \
    dlg_equipement.ui \
    dlg_concentration.ui \
    dlg_systeme_etalon.ui \
    ct_phasewidget.ui

INCLUDEPATH += ./qserialdevice \
               ./
LIBS += -L./ \
    -lqserialdevice

OTHER_FILES += \
    licence.txt \
    StructureTest.dtd \
    CreationBaseMySql.sql
