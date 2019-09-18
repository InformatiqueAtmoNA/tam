# -------------------------------------------------
# Project created by QtCreator 2011-03-18T09:33:21
# -------------------------------------------------
QT += sql \
    xml
TARGET = TAM
TEMPLATE = app
CONFIG += static \
    += \
    console
SOURCES += main.cpp \
    mainwindow.cpp \
    threadcomhandler.cpp \
    tei_146i.cpp \
    tei_146c.cpp \
    tei.cpp \
    protocole.cpp \
    mode4_sx_6000_17.cpp \
    mode4.cpp \
    communicationserie.cpp \
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
    homewidget.cpp \
    tei_146c_v2.cpp \
    executiontest.cpp \
    ct_phasewidget.cpp \
    et_interfaceexecutiontest.cpp \
    et_infostestencours.cpp \
    dlg_testxml.cpp \
    et_listeattentetests.cpp \
    dlg_lieu.cpp \
    dlg_operateur.cpp \
    dlg_port_serie.cpp \
    et_generateurrapporttest.cpp \
    et_resultatpolluant.cpp \
    parametreshandler.cpp \
    mode4_sx_6000_1808.cpp
HEADERS += mainwindow.h \
    threadcomhandler.h \
    tei_146i.h \
    tei_146c.h \
    tei.h \
    protocole.h \
    mode4_sx_6000_17.h \
    mode4StructConfig.h \
    mode4.h \
    communicationserie.h \
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
    homewidget.h \
    tei_146c_v2.h \
    tei_49c.h \
    executiontest.h \
    et_TransitionTempsMaxTest.h \
    et_TransitionTempsMaxPhase.h \
    et_TransitionPhasesRestantes.h \
    et_TransitionNbCyclesMesure.h \
    et_TransitionCritereArret.h \
    et_TransitionCmdDebutPhase.h \
    et_TransitionCyclesPhases.h \
    ct_phasewidget.h \
    et_interfaceexecutiontest.h \
    et_paramsTest.h \
    et_infostestencours.h \
    dlg_testxml.h \
    et_listeattentetests.h \
    MesureIndividuelle.h \
    dlg_lieu.h \
    dlg_operateur.h \
    dlg_port_serie.h \
    et_generateurrapporttest.h \
    et_resultatpolluant.h \
    parametreshandler.h \
    mode4_sx_6000_1808.h
FORMS += mainwindow.ui \
    creationtest.ui \
    dlg_molecule.ui \
    dlg_protocole.ui \
    dlg_marque.ui \
    dlg_tx_transmission.ui \
    dlg_modele.ui \
    dlg_equipement.ui \
    dlg_concentration.ui \
    dlg_systeme_etalon.ui \
    homewidget.ui \
    ct_phasewidget.ui \
    et_interfaceexecutiontest.ui \
    et_infostestencours.ui \
    dlg_testxml.ui \
    et_listeattentetests.ui \
    dlg_lieu.ui \
    dlg_operateur.ui \
    dlg_port_serie.ui \
    et_generateurrapporttest.ui \
    et_resultatpolluant.ui \
    parametreshandler.ui
INCLUDEPATH += ./qserialdevice \
    ./
LIBS += -L./ \
    -lqserialdevice
OTHER_FILES += licence.txt \
    StructureTest.dtd \
    CreationBaseMySql.sql





win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../qserialdevice/src/build-qserialdevice-Desktop-Release/release/release/ -lqserialdevice
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../qserialdevice/src/build-qserialdevice-Desktop-Release/release/debug/ -lqserialdevice
else:unix: LIBS += -L$$PWD/../qserialdevice/src/build-qserialdevice-Desktop-Release/release/ -lqserialdevice

INCLUDEPATH += $$PWD/../qserialdevice/src/build-qserialdevice-Desktop-Release/release
DEPENDPATH += $$PWD/../qserialdevice/src/build-qserialdevice-Desktop-Release/release

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../qserialdevice/src/build-qserialdevice-Desktop-Release/release/release/libqserialdevice.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../qserialdevice/src/build-qserialdevice-Desktop-Release/release/debug/libqserialdevice.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../qserialdevice/src/build-qserialdevice-Desktop-Release/release/release/qserialdevice.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../qserialdevice/src/build-qserialdevice-Desktop-Release/release/debug/qserialdevice.lib
else:unix: PRE_TARGETDEPS += $$PWD/../qserialdevice/src/build-qserialdevice-Desktop-Release/release/libqserialdevice.a
