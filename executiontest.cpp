/*////////////////////////////////////////////////////////////
// \file executiontest.cpp
// \brief Classe implémentant l'exécution des tests métrologiques
// \author FOUQUART Christophe
// \version 1.0
// \date 20/06/2011
//
// TAM - Tests Automatiques Métrologiques
// Copyright (C) 2011-2012 TAM Team
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
////////////////////////////////////////////////////////////*/

#include "executiontest.h"

ExecutionTest::ExecutionTest(const QPointer<et_ParamsTest> paramsTest,const QPointer<BdHandler> bdHandler,const short idListeTestEnCours)
{
    m_idListeTestEnCours = idListeTestEnCours;
    m_paramsTest = QWeakPointer<et_ParamsTest>(paramsTest.data());
    m_nbPhasesRestantes = m_paramsTest.data()->m_test->getNbPhases();
    m_nbCyclesPhasesRestants = m_paramsTest.data()->m_test->getNbCyclesDePhases();
    m_moyennerMesures = false;
    m_noPhaseSuivante = 1;
    m_cyclePhaseEnCours = 0;

    if(bdHandler.isNull()) {
        QString driver = getParam("BD_Driver").toString();
        QString host = getParam("Host").toString();
        QString userName = getParam("UserName").toString();
        QString password = getParam("Password").toString();
        QString dbName = getParam("DB_Name").toString();

        m_bdHandler = new BdHandler(driver,host,userName,password,dbName);
    }
    else
        m_bdHandler = bdHandler;

    connect(m_bdHandler,SIGNAL(afficherTrace(QString)),this,SLOT(messageBdHandler(QString)));

    m_remplirFichierCSV = false;
    if(!paramsTest->m_fichierCSV.isNull() && paramsTest->m_fichierCSV->isOpen())
        m_remplirFichierCSV = true;

    this->getInfosEquipements();

    m_machine = new QStateMachine();

    m_timerDemmarageMachine = new QTimer();
    m_timerDemmarageMachine->setInterval(1000);
    m_timerDemmarageMachine->setSingleShot(true);

    m_timerTempsMaxPhase = new QTimer();
    m_timerTempsAcquisition = new QTimer();
    m_timerTempsStabilisation = new QTimer();
    m_timerTempsMoyennageMesures = new QTimer();
    m_timerTempsAttenteEntreMesures = new QTimer();

    m_timerTempsMaxPhase->setSingleShot(true);
    m_timerTempsAcquisition->setSingleShot(true);
    m_timerTempsStabilisation->setSingleShot(true);
    m_timerTempsMoyennageMesures->setSingleShot(true);
    m_timerTempsAttenteEntreMesures->setSingleShot(true);

    m_flagPhaseInitialisee = false;

    this->constructionMachineEtat();
}

ExecutionTest::~ExecutionTest()
{
    m_machine->deleteLater();

    QMapIterator<ushort,QPointer<Protocole> > it_anaDesignProto(m_analyseursProtocole);
    while (it_anaDesignProto.hasNext()) {
        it_anaDesignProto.next();
        if(!it_anaDesignProto.value().isNull()) {
            it_anaDesignProto.value()->deleteLater();
        }
    }
    if(!m_protocoleCalibrateur.isNull()) {
        m_protocoleCalibrateur->deleteLater();
    }

    QCoreApplication::processEvents();

    delete m_infosCalibrateur;

//    QMapIterator<ushort,QSqlRecord*> it_anaInfos(m_analyseursInfos);
//    while (it_anaInfos.hasNext()) {
//        it_anaInfos.next();
//        QSqlRecord* record = it_anaInfos.value();
//        if(record != NULL) {
//            delete record;
//        }
//    }
    m_analyseursInfos.clear();

    if (m_remplirFichierCSV)
        m_paramsTest.data()->m_fichierCSV->close();

    delete m_paramsTest.data()->m_fichierCSV;
    delete m_timerDemmarageMachine;
    delete m_timerTempsMaxPhase;
    delete m_timerTempsAcquisition;
    delete m_timerTempsStabilisation;
    delete m_timerTempsMoyennageMesures;
    delete m_timerTempsAttenteEntreMesures;
}

void ExecutionTest::getInfosEquipements()
{
    QMapIterator<ushort,QString> iterator(m_paramsTest.data()->m_listeInterfaceAnalyseurs);
    while (iterator.hasNext()) {
        iterator.next();
        ushort idAnalyseur = iterator.key();

        QSqlRecord* equipementRecord= m_bdHandler->getEquipementRow(idAnalyseur);
        m_analyseursInfos.insert(idAnalyseur,equipementRecord);

        ushort offset = equipementRecord->value(EQUIPEMENT_OFFSET).toUInt();
        m_analyseursoffset.insert(idAnalyseur,offset);

        ushort idTxTransmission = equipementRecord->value(EQUIPEMENT_ID_TX_TRANSMISSION).toUInt();
        m_analyseursTxTransmission.insert(idAnalyseur,idTxTransmission);

        DesignationProtocole designationProtocoleAnalyseur = m_bdHandler->getDesignationProtocole(idAnalyseur);
        QString adresse = equipementRecord->value(EQUIPEMENT_ADRESSE).toString();

        QPointer<ThreadComHandler> threadCommunication = new ThreadComHandler();
        threadCommunication->configureRS232(m_paramsTest.data()->m_listeInterfaceAnalyseurs.value(idAnalyseur));

        QPointer<Protocole> protocoleAnalyseur = Protocole::getProtocoleObject(designationProtocoleAnalyseur,adresse);
        protocoleAnalyseur->setThreadComHandler(threadCommunication);
        protocoleAnalyseur->setTimeOut(500);

        protocoleAnalyseur->init();

        m_analyseursProtocole.insert(idAnalyseur,protocoleAnalyseur);
        delete equipementRecord;

        QVector<QWeakPointer<MesureIndividuelle> > tabMesuresParCycle;
        m_tabMesuresParCycle.insert(idAnalyseur,tabMesuresParCycle);

        QVector<QWeakPointer<MesureIndividuelle> > tableauMoyennesParPhase;
        m_tabMoyennesMesuresParPhase.insert(idAnalyseur,tableauMoyennesParPhase);
    }

    uint idSystemeEtalon = m_paramsTest.data()->m_test->getIdSystemeEtalon();
    ushort idCalibrateur = m_bdHandler->getIdCalibrateur(idSystemeEtalon);
    m_infosCalibrateur = m_bdHandler->getEquipementRow(idCalibrateur);
    m_txTransmissionCalibrateur = m_infosCalibrateur->value(EQUIPEMENT_ID_TX_TRANSMISSION).toUInt();

    DesignationProtocole designationProtocoleCalibrateur = m_bdHandler->getDesignationProtocole(idCalibrateur);

    QString adresse = m_infosCalibrateur->value(EQUIPEMENT_ADRESSE).toString();
    m_protocoleCalibrateur = Protocole::getProtocoleObject(designationProtocoleCalibrateur,adresse);

    QPointer<ThreadComHandler> threadCommunication = new ThreadComHandler();
    threadCommunication->configureRS232(m_paramsTest.data()->m_interfaceCalibrateur);

    m_protocoleCalibrateur->setThreadComHandler(threadCommunication);
    m_protocoleCalibrateur->setTimeOut(500);

    m_protocoleCalibrateur->init();
}

void ExecutionTest::constructionMachineEtat()
{

//////////////////////////////////////////////////
/*////////////////// Test //////////////////////*/
//////////////////////////////////////////////////
    QState* st_etatInitial = new QState(m_machine);
    QState* st_testPeripherique = new QState(m_machine);
    QState* st_testMetro = new QState(QState::ParallelStates,m_machine);
    QFinalState*  st_etatFinalTest = new QFinalState(m_machine);

    m_machine->setInitialState(st_etatInitial);

    st_etatInitial->addTransition(m_timerDemmarageMachine,SIGNAL(timeout()),st_testPeripherique);
    st_testPeripherique->addTransition(this,SIGNAL(peripheriquesOk()),st_testMetro);
    st_testMetro->addTransition(this,SIGNAL(testMetroTermine()),st_etatFinalTest);

    connect(st_testPeripherique,SIGNAL(entered()),this,SLOT(testerPeripheriques()));
    connect(m_machine,SIGNAL(finished()),this,SLOT(testTermine()));
    connect(this,SIGNAL(erreurPeripherique()),this,SLOT(erreurPeripheriqueRecue()));

////////////////////////////////////////////////////////////////
/*////////////////// Déroulement du test//////////////////////*/
////////////////////////////////////////////////////////////////
    QState* st_deroulementTest = new QState(st_testMetro);
    QState* st_debutDeroulementTest = new QState(st_deroulementTest);
    QState* st_testCyclesPhasesRestants = new QState(st_deroulementTest);
    QState* st_testPhasesDebutTest = new QState(st_deroulementTest);
    QState* st_phaseTerminee = new QState(st_deroulementTest);
    QState* st_phaseTest = new QState(QState::ParallelStates,st_deroulementTest);
    QFinalState* st_finDeroulementTest = new QFinalState(st_deroulementTest);
    st_deroulementTest->setInitialState(st_debutDeroulementTest);

    connect(st_finDeroulementTest,SIGNAL(entered()),this,SLOT(standByCalibrateur()));

    connect(st_testPhasesDebutTest,SIGNAL(entered()),this,SLOT(testerPhasesRestantes()));

    st_phaseTest->addTransition(st_phaseTest,SIGNAL(finished()),st_phaseTerminee);

    // Test du nombre de cycles de phases
    et_TransitionCyclesPhasesRestants* tr_testCyclesPhasesRestantsDebutTest = new et_TransitionCyclesPhasesRestants();
    tr_testCyclesPhasesRestantsDebutTest->setTargetState(st_testPhasesDebutTest);
    st_debutDeroulementTest->addTransition(tr_testCyclesPhasesRestantsDebutTest);

    et_TransitionCyclesPhasesRestants* tr_testCyclesPhasesRestants = new et_TransitionCyclesPhasesRestants();
    tr_testCyclesPhasesRestants->setTargetState(st_testPhasesDebutTest);
    st_testCyclesPhasesRestants->addTransition(tr_testCyclesPhasesRestants);

    et_TransitionCyclesPhasesTermines* tr_testCyclesPhasesTerminesDebutTest = new et_TransitionCyclesPhasesTermines();
    tr_testCyclesPhasesTerminesDebutTest->setTargetState(st_finDeroulementTest);
    st_debutDeroulementTest->addTransition(tr_testCyclesPhasesTerminesDebutTest);

    et_TransitionCyclesPhasesTermines* tr_testCyclesPhasesTermines = new et_TransitionCyclesPhasesTermines();
    tr_testCyclesPhasesTermines->setTargetState(st_finDeroulementTest);
    st_testCyclesPhasesRestants->addTransition(tr_testCyclesPhasesTermines);

    // Test du nombre de phases
    et_TransitionPhasesRestantes* tr_testPhasesRestantesDebutTest = new et_TransitionPhasesRestantes();
    tr_testPhasesRestantesDebutTest->setTargetState(st_phaseTest);
    st_testPhasesDebutTest->addTransition(tr_testPhasesRestantesDebutTest);

    et_TransitionPhasesRestantes* tr_testPhasesRestantes = new et_TransitionPhasesRestantes();
    tr_testPhasesRestantes->setTargetState(st_phaseTest);
    st_phaseTerminee->addTransition(tr_testPhasesRestantes);

    et_TransitionFinCycle* tr_finCyclePhase = new et_TransitionFinCycle();
    tr_finCyclePhase->setTargetState(st_testCyclesPhasesRestants);
    st_phaseTerminee->addTransition(tr_finCyclePhase);

    connect(st_phaseTerminee,SIGNAL(entered()),this,SLOT(decrementerNbPhasesRestantes()));
    connect(st_phaseTerminee,SIGNAL(entered()),this,SLOT(IncrementerNoPhasesSuivant()));
    connect(st_testCyclesPhasesRestants,SIGNAL(entered()),this,SLOT(decrementerNbCyclesPhasesRestants()));
    connect(st_debutDeroulementTest,SIGNAL(entered()),this,SLOT(testerCyclesPhases()));

///////////////////////////////////////////////////////////
/*////////////////// Phase de test //////////////////////*/
///////////////////////////////////////////////////////////
    QState* st_tempsMaxPhase = new QState(st_phaseTest);
    QState* st_acquisisition = new QState(st_phaseTest);
    QState* st_deroulementPhase = new QState(st_phaseTest);

////////////////////////////////////////////////////////////////////
/*////////////////// Temps maximum de phase //////////////////////*/
////////////////////////////////////////////////////////////////////
    QState* st_testTempsMaxPhase = new QState(st_tempsMaxPhase);
    QState* st_attenteTempsMaxPhase = new QState(st_tempsMaxPhase);
    QFinalState* st_finTempsMaxPhase = new QFinalState(st_tempsMaxPhase);

    st_tempsMaxPhase->setInitialState(st_testTempsMaxPhase);
    connect(st_testTempsMaxPhase,SIGNAL(entered()),this,SLOT(testerTempsMaxPhase()));
    connect(st_attenteTempsMaxPhase,SIGNAL(entered()),this,SLOT(lancerTimerTempsMaxPhase()));

    et_TransitionTempsMaxPhase* tr_tempsMaxPhaseDefini = new et_TransitionTempsMaxPhase(m_paramsTest.data()->m_test->getListeTempsMaxPhase());
    tr_tempsMaxPhaseDefini->setTargetState(st_attenteTempsMaxPhase);
    st_testTempsMaxPhase->addTransition(tr_tempsMaxPhaseDefini);

    et_TransitionTempsMaxPhaseNonDefini* tr_tempsMaxPhaseNonDefini = new et_TransitionTempsMaxPhaseNonDefini(m_paramsTest.data()->m_test->getListeTempsMaxPhase());
    tr_tempsMaxPhaseNonDefini->setTargetState(st_finTempsMaxPhase);
    st_testTempsMaxPhase->addTransition(tr_tempsMaxPhaseNonDefini);

    st_attenteTempsMaxPhase->addTransition(m_timerTempsMaxPhase,SIGNAL(timeout()),st_finTempsMaxPhase);
    st_attenteTempsMaxPhase->addTransition(st_deroulementPhase,SIGNAL(finished()),st_finTempsMaxPhase);
    connect(m_timerTempsMaxPhase,SIGNAL(timeout()),this,SIGNAL(tempsMaxPhaseAtteint()));

    connect(st_finTempsMaxPhase,SIGNAL(entered()),this,SLOT(etatFinalTempsMaxPhase()));

////////////////////////////////////////////////////////////////////
/*////////////////// Acquisition des mesures //////////////////////*/
////////////////////////////////////////////////////////////////////
    QState* st_lancementTempsAcquisition = new QState(st_acquisisition);
    QState* st_enregistrementMesures = new QState(st_acquisisition);
    QFinalState* st_finAcquisition = new QFinalState(st_acquisisition);

    st_acquisisition->setInitialState(st_lancementTempsAcquisition);

    st_lancementTempsAcquisition->addTransition(m_timerTempsAcquisition,SIGNAL(timeout()),st_enregistrementMesures);
    st_lancementTempsAcquisition->addTransition(this,SIGNAL(testMetroTermine()),st_finAcquisition);
    st_lancementTempsAcquisition->addTransition(this,SIGNAL(phaseTerminee()),st_finAcquisition);
    st_lancementTempsAcquisition->addTransition(m_timerTempsMaxPhase,SIGNAL(timeout()),st_finAcquisition);

    st_enregistrementMesures->addTransition(this,SIGNAL(mesuresEnregistrees()),st_lancementTempsAcquisition);
    st_enregistrementMesures->addTransition(m_timerTempsMaxPhase,SIGNAL(timeout()),st_finAcquisition);

    connect(st_lancementTempsAcquisition,SIGNAL(entered()),this,SLOT(lancerTimerTempsAcquisition()));
    connect(st_enregistrementMesures,SIGNAL(entered()),this,SLOT(enregistrerMesures()));

/////////////////////////////////////////////////////////////////////
/*////////////////// Déroulement de la phase //////////////////////*/
/////////////////////////////////////////////////////////////////////
    QState* st_initialisationPhase = new QState(st_deroulementPhase);
    QState* st_envoiConcentration = new QState(st_deroulementPhase);
    QState* st_attenteTempsStabilisation = new QState(st_deroulementPhase);
    QState* st_testNbCyclesMesure = new QState(st_deroulementPhase);
    QState* st_attenteTempsMoyennageMesure = new QState(st_deroulementPhase);
    QState* st_enregistrementMoyenneMesures = new QState(st_deroulementPhase);
    QState* st_attenteTempsAttenteEntreMesures = new QState(st_deroulementPhase);
    QState* st_testCritereArretDefini = new QState(st_deroulementPhase);
    QState* st_verifCritereArret = new QState(st_deroulementPhase);
    QFinalState* st_finDeroulementPhase = new QFinalState(st_deroulementPhase);

    st_deroulementPhase->setInitialState(st_initialisationPhase);

    st_initialisationPhase->addTransition(this,SIGNAL(initialisationPhaseTerminee()),st_envoiConcentration);
    connect(st_initialisationPhase,SIGNAL(entered()),this,SLOT(initialiserPhase()));

    /*////////////////// Envoi de concentration(s) et attente temps stabilisation //////////////////////*/
    connect(st_envoiConcentration,SIGNAL(entered()),this,SLOT(envoiConcentration()));
    st_envoiConcentration->addTransition(this,SIGNAL(concentrationEnvoyee()),st_attenteTempsStabilisation);
    st_attenteTempsStabilisation->addTransition(m_timerTempsStabilisation,SIGNAL(timeout()),st_testNbCyclesMesure);
    st_attenteTempsStabilisation->addTransition(m_timerTempsMaxPhase,SIGNAL(timeout()),st_finDeroulementPhase);
    connect(this->m_timerTempsStabilisation,SIGNAL(timeout()),this,SLOT(tempsStabilisationFini()));

    connect(st_testNbCyclesMesure,SIGNAL(entered()),this,SLOT(testerCyclesMesures()));
    st_testNbCyclesMesure->addTransition(m_timerTempsMaxPhase,SIGNAL(timeout()),st_finDeroulementPhase);
    connect(st_attenteTempsStabilisation,SIGNAL(entered()),this,SLOT(lancerTimerTempsStabilisation()));

    /*////////////////// Test du nombre de cycles de mesure restants //////////////////////*/
    et_NbCyclesMesureRestants* tr_NbCyclesMesureRestants;
    tr_NbCyclesMesureRestants = new et_NbCyclesMesureRestants(st_testNbCyclesMesure);
    tr_NbCyclesMesureRestants->setTargetState(st_attenteTempsMoyennageMesure);
    st_testNbCyclesMesure->addTransition(tr_NbCyclesMesureRestants);

    et_CyclesMesureFinis* tr_CyclesMesureFinis;
    tr_CyclesMesureFinis = new et_CyclesMesureFinis(st_testNbCyclesMesure);
    tr_CyclesMesureFinis->setTargetState(st_finDeroulementPhase);
    st_testNbCyclesMesure->addTransition(tr_CyclesMesureFinis);

    /*////////////////// Cycle de mesure //////////////////////*/
    connect(st_attenteTempsMoyennageMesure,SIGNAL(entered()),this,SLOT(lancerTimerTempsMoyennageMesures()));
    st_attenteTempsMoyennageMesure->addTransition(m_timerTempsMaxPhase,SIGNAL(timeout()),st_finDeroulementPhase);
    st_attenteTempsMoyennageMesure->addTransition(m_timerTempsMoyennageMesures,SIGNAL(timeout()),st_enregistrementMoyenneMesures);
    connect(st_enregistrementMoyenneMesures,SIGNAL(entered()),this,SLOT(enregistrerMoyenneMesures()));

    st_enregistrementMoyenneMesures->addTransition(this,SIGNAL(moyenneMesuresEnregistree()),st_attenteTempsAttenteEntreMesures);
    connect(st_attenteTempsAttenteEntreMesures,SIGNAL(entered()),this,SLOT(lancerTimerTempsAttenteEntreMesures()));
    st_attenteTempsAttenteEntreMesures->addTransition(m_timerTempsMaxPhase,SIGNAL(timeout()),st_finDeroulementPhase);
    st_attenteTempsAttenteEntreMesures->addTransition(m_timerTempsAttenteEntreMesures,SIGNAL(timeout()),st_testCritereArretDefini);

    /*////////////////// Critère d'arrêt //////////////////////*/
    connect(st_testCritereArretDefini,SIGNAL(entered()),this,SLOT(testerCritereArret()));

    et_TransitionCritereArretDefini* tr_CritereArretDefini;
    tr_CritereArretDefini = new et_TransitionCritereArretDefini(m_paramsTest.data()->m_test->getListeCritereArretDefinis());
    tr_CritereArretDefini->setTargetState(st_verifCritereArret);
    st_testCritereArretDefini->addTransition(tr_CritereArretDefini);

    et_TransitionCritereArretNonDefini* tr_CritereArretNonDefini;
    tr_CritereArretNonDefini = new et_TransitionCritereArretNonDefini(m_paramsTest.data()->m_test->getListeCritereArretDefinis());
    tr_CritereArretNonDefini->setTargetState(st_testNbCyclesMesure);
    st_testCritereArretDefini->addTransition(tr_CritereArretNonDefini);

    st_verifCritereArret->addTransition(this,SIGNAL(critereArretAtteint()),st_finDeroulementPhase);
    st_verifCritereArret->addTransition(this,SIGNAL(critereArretNonAtteint()),st_attenteTempsMoyennageMesure);

    connect(st_verifCritereArret,SIGNAL(entered()),this,SLOT(verifierCritereArret()));
    connect(st_finDeroulementPhase,SIGNAL(entered()),this,SLOT(finPhase()));

    m_machine->start();
}

void ExecutionTest::testerPeripheriques()
{
    emit(traceTest("test des périphériques",0));
    QMapIterator<ushort,QPointer<Protocole> > it_anaDesignProto(m_analyseursProtocole);
    while (it_anaDesignProto.hasNext()) {
        it_anaDesignProto.next();
        QPointer<Protocole> analyseur = it_anaDesignProto.value();
        QString trace = "code alarme analyseur id = ";
        trace.append(QString::number(it_anaDesignProto.key()));
        trace.append(" -> ");
        trace.append(QString::number(analyseur->demandeAlarme()));

        emit(traceTest(trace,1));
    }
    emit(traceTest("code alarme calibrateur "+m_protocoleCalibrateur->demandeAlarme(),1));

    emit(this->peripheriquesOk());
}

void ExecutionTest::run()
{
    emit(traceTest("------------------------------------------------------",0));
    emit(traceTest("           Début du test "+ QString::number(m_paramsTest.data()->m_test->getIdTest()),0));
    emit(traceTest("------------------------------------------------------\n",0));

    m_timerDemmarageMachine->start();
}

void ExecutionTest::decrementerNbCyclesPhasesRestants()
{
    m_nbCyclesPhasesRestants -= 1;
    m_nbPhasesRestantes = m_paramsTest.data()->m_test->getNbPhases();
    m_noPhaseSuivante=1;
    testerCyclesPhases();
}

void ExecutionTest::lancerTimerTempsMaxPhase()
{
    int msToTempsMaxPhase = QTime(0,0,0,0).msecsTo(m_paramsTest.data()->m_test->getPhase(m_noPhaseSuivante).getTpsMaxPhase());

    m_timerTempsMaxPhase->start(msToTempsMaxPhase);
}

void ExecutionTest::lancerTimerTempsAcquisition()
{
    ushort tempsAcquisition = m_paramsTest.data()->m_test->getTempsAcquisition()*1000;
    emit(traceTest("Timer temps acquisition lancé : "+QString::number(tempsAcquisition),2));
    m_timerTempsAcquisition->start(tempsAcquisition);

    if(m_flagPhaseInitialisee) {
        QMapIterator<ushort,QPointer<Protocole> > it_anaDesignProto(m_analyseursProtocole);
        while (it_anaDesignProto.hasNext()) {
            it_anaDesignProto.next();
            QPointer<Protocole> analyseur = it_anaDesignProto.value();

            QWeakPointer< MesureIndividuelle > mesures(analyseur->demandeMesure());
            m_tabMesuresIndividuelles.insert(it_anaDesignProto.key(),mesures.data());
        }
    }
    emit(this->mesuresEffectuees());
}

void ExecutionTest::lancerTimerTempsStabilisation()
{
    m_etatAutomate = "S";

    if(m_remplirFichierCSV){
        m_paramsTest.data()->m_fichierCSV->write("");
        m_paramsTest.data()->m_fichierCSV->flush();
    }

    QTime temps_stabilisation = m_paramsTest.data()->m_test->getPhase(m_noPhaseSuivante).getTpsStabilisation();
    int msToTempsStabilisation = QTime(0,0,0,0).msecsTo(temps_stabilisation);

    emit(traceTest("Lancement temps de stabilisation : "+temps_stabilisation.toString("HH:mm:ss"),2));
    m_NbCyclesMesuresRestants = m_paramsTest.data()->m_test->getPhase(m_noPhaseSuivante).getNbCyclesMesures();

    m_timerTempsStabilisation->start(msToTempsStabilisation);
}

void ExecutionTest::lancerTimerTempsMoyennageMesures()
{
    m_etatAutomate = "M"+QString::number(m_cycleMesureEnCours);

    QTime tempsMoyennageMesure = m_paramsTest.data()->m_test->getPhase(m_noPhaseSuivante).getTpsMoyennageMesure();
    int msToTempsMoyennageMesures = QTime(0,0,0,0).msecsTo(tempsMoyennageMesure);
    emit(traceTest("Lancement temps de moyennage des mesures : "+tempsMoyennageMesure.toString("HH:mm:ss"),2));

    m_moyennerMesures = true;
    m_timerTempsMoyennageMesures->start(msToTempsMoyennageMesures);
}

void ExecutionTest::lancerTimerTempsAttenteEntreMesures()
{
    m_etatAutomate = "A";

    QTime tempsAttenteEntreMesures = m_paramsTest.data()->m_test->getPhase(m_noPhaseSuivante).getTpsAttenteEntreMesure();

    emit(traceTest("Lancement temps d'attente entre mesures : "+tempsAttenteEntreMesures.toString("HH:mm:ss"),2));

    int msToTempsAttenteEntreMesures = QTime(0,0,0,0).msecsTo(tempsAttenteEntreMesures);

    m_timerTempsAttenteEntreMesures->start(msToTempsAttenteEntreMesures);
}

void ExecutionTest::enregistrerMesures()
{
    if(!m_flagPhaseInitialisee || m_tabMesuresIndividuelles.isEmpty()) {
        emit(this->mesuresEnregistrees());
        return;
    }

    QDateTime date = QDateTime::currentDateTime();

    QString strMesure = "\n" + date.toString("yyyy-MM-dd HH:mm:ss") + ";";

    strMesure.append(QString::number(m_cyclePhaseEnCours)+";"+QString::number(m_noPhaseSuivante)+";"+m_etatAutomate+";");

    QMapIterator<ushort,QWeakPointer<MesureIndividuelle> > it_tableauMesures(m_tabMesuresIndividuelles);

    while (it_tableauMesures.hasNext()) {
        it_tableauMesures.next();

        QWeakPointer<MesureIndividuelle> mesures = it_tableauMesures.value();
        if(mesures.data()->isEmpty())
            strMesure.append(";NULL;NULL;NULL");
        else {
            for(ushort i=0;i< mesures.data()->count();i++) {
                emit(traceTest("Mesure polluant "+QString::number(i)+" = "+QString::number(mesures.data()->at(i)),1));
                strMesure.append(QString::number(mesures.data()->at(i)-m_analyseursoffset.value(it_tableauMesures.key())));
                if(i+1 < mesures.data()->count())
                     strMesure.append(";");
            }
        }
        strMesure.append(";");

        if(m_remplirFichierCSV){
            m_paramsTest.data()->m_fichierCSV->write(strMesure.toAscii());
            m_paramsTest.data()->m_fichierCSV->flush();
        }

        strMesure.clear();

        if(m_moyennerMesures)
            m_tabMesuresParCycle[it_tableauMesures.key()].append(mesures);
    }

    emit(this->mesuresEnregistrees());
}

void ExecutionTest::initialiserPhase()
{
    Phase phase = m_paramsTest.data()->m_test->getPhase(m_noPhaseSuivante);
    Commandes cmdDebutPhase = phase.getCmdDebutPhase();
    ushort idMolecule = phase.getIdMolecule();
    QSqlRecord* moleculeRow = m_bdHandler->getMoleculeRow(idMolecule);
    m_cycleMesureEnCours = 0;

    QMapIterator<ushort,QPointer<Protocole> > it_anaDesignProto(m_analyseursProtocole);
    while (it_anaDesignProto.hasNext()) {
        it_anaDesignProto.next();
        QPointer<Protocole> analyseur = it_anaDesignProto.value();

        if(cmdDebutPhase != NO_CMD) {
            if(cmdDebutPhase == MODE_ETALON)
                analyseur->passageEtalon();
            if(cmdDebutPhase == MODE_MESURE)
                analyseur->passageMesure();
        }
        analyseur->setTypePolluant(stringToTypePolluant(moleculeRow->value(MOLECULE_FORMULE).toString()));
    }

    m_protocoleCalibrateur->setTypePolluant(stringToTypePolluant(moleculeRow->value(MOLECULE_FORMULE).toString()));

    delete moleculeRow;

    m_flagPhaseInitialisee = true;

    emit(traceTest("Début du la phase n° "+QString::number(m_noPhaseSuivante),0));

    emit(this->initialisationPhaseTerminee());
}

void ExecutionTest::envoiConcentration()
{
    Phase phase = m_paramsTest.data()->m_test->getPhase(m_noPhaseSuivante);
    QSqlRecord* record = m_bdHandler->getConcentrationRow(phase.getIdConcentration());

    ushort pointConcentration = record->value(CONCENTRATION_POINT).toUInt();
    ushort concReelle = record->value(CONCENTRATION_REELLE).toUInt();
    ushort concOzone = record->value(CONCENTRATION_OZONE).toUInt();
    ushort idMolecule = record->value(CONCENTRATION_ID_MOLECULE).toUInt();

    delete record;

    record = m_bdHandler->getMoleculeRow(idMolecule);

    QString formule = record->value(MOLECULE_FORMULE).toString();

    if(concReelle==0 && concOzone==0) {
        m_protocoleCalibrateur->commandeSpanZero(m_paramsTest.data()->m_canalCalibrateur);
    }
    else {
        SpanHandler* spanHandler;
        if(concOzone == 0) {
            spanHandler = m_protocoleCalibrateur->getSpanHandler(SPAN);
            spanHandler->setSpanArguments(m_paramsTest.data()->m_canalCalibrateur,pointConcentration,0);
            m_protocoleCalibrateur->commandeSpan(*spanHandler);
        }
        else {
            if(concReelle == 0 || formule.contains("O3")) {
                spanHandler = m_protocoleCalibrateur->getSpanHandler(SPAN_O3);
                spanHandler->setSpanArguments(m_paramsTest.data()->m_canalCalibrateur,0,concOzone);
                m_protocoleCalibrateur->commandeSpanO3(*spanHandler);
            }
            else {
                spanHandler = m_protocoleCalibrateur->getSpanHandler(SPAN_TPG);
                spanHandler->setSpanArguments(m_paramsTest.data()->m_canalCalibrateur,pointConcentration,concOzone);
                m_protocoleCalibrateur->commandeSpanTpg(*spanHandler);
            }
        }
    }
    emit(this->concentrationEnvoyee());
}

void ExecutionTest::enregistrerMoyenneMesures()
{
    m_moyennerMesures = false;

    QMapIterator<ushort,QVector<QWeakPointer<MesureIndividuelle> > > it_tabMesuresParCycle(m_tabMesuresParCycle);
    while(it_tabMesuresParCycle.hasNext()) {
        it_tabMesuresParCycle.next();

        QVector<QWeakPointer<MesureIndividuelle> > tableaumesureParCycle = it_tabMesuresParCycle.value();

        QWeakPointer<MesureIndividuelle> moyenneMesure= new MesureIndividuelle();
        for(ushort j=0;j<3;j++) {
            ushort nbMesuresMoyennees=0;
            float sommeMesuresIndividuelles=0;
            for(ushort i=0;i<tableaumesureParCycle.count();i++) {
                QWeakPointer<MesureIndividuelle> mesure = tableaumesureParCycle.at(i);
                if(j<mesure.data()->count()) {
                    float mesureIndividuelle = mesure.data()->at(j)-m_analyseursoffset.value(it_tabMesuresParCycle.key());
                    sommeMesuresIndividuelles += mesureIndividuelle;
                    nbMesuresMoyennees++;
                }
            }
            //if(!QString::number(sommeMesuresIndividuelles/nbMesuresMoyennees).contains("inf")) {
            if(!nbMesuresMoyennees==0) {
                emit(traceTest("Moyenne polluant "+QString::number(j)+" = "+QString::number(sommeMesuresIndividuelles/nbMesuresMoyennees),0));
                moyenneMesure.data()->append(sommeMesuresIndividuelles/nbMesuresMoyennees);
            }
        }

        m_tabMoyennesMesuresParPhase[it_tabMesuresParCycle.key()].append(moyenneMesure);

        MesureInfo mesureInfos;
        mesureInfos.idTest = m_paramsTest.data()->m_id_TestMetro;
        mesureInfos.idEquipement = it_tabMesuresParCycle.key();
        mesureInfos.noCyclePhase = m_cyclePhaseEnCours;
        mesureInfos.noPhase = m_noPhaseSuivante;
        mesureInfos.noCycleMesure = m_cycleMesureEnCours;
        mesureInfos.mesure = moyenneMesure;

        if (!mesureInfos.mesure.data()->isEmpty()){
                m_bdHandler->insertIntoMesure(mesureInfos);
            }

        m_tabMesuresParCycle.clear();
    }

    emit(this->moyenneMesuresEnregistree());
}

void ExecutionTest::testTermine()
{
    emit(traceTest("test terminé",0));

    m_bdHandler->miseAjourDateHeureFinTest(m_paramsTest.data()->m_id_TestMetro);

    m_timerTempsAttenteFinAcquisition = new QTimer;

    QMapIterator<ushort,QPointer<Protocole> > it_anaDesignProto(m_analyseursProtocole);
    while (it_anaDesignProto.hasNext()) {
        it_anaDesignProto.next();
        if(!it_anaDesignProto.value().isNull()) {
            it_anaDesignProto.value()->passageMesure();
            it_anaDesignProto.value()->quitter();
        }
    }
    if(!m_protocoleCalibrateur.isNull()) {
        m_protocoleCalibrateur->quitter();
    }

    m_timerTempsAttenteFinAcquisition->setInterval(1000);
    m_timerTempsAttenteFinAcquisition->setSingleShot(true);

    connect(m_timerTempsAttenteFinAcquisition,SIGNAL(timeout()),this,SLOT(attenteFinAcquisition()));

    m_timerTempsAttenteFinAcquisition->start();

}

void ExecutionTest::attenteFinAcquisition()
{
    if(m_idListeTestEnCours>-1)
        emit(killMeAndMyThread(m_idListeTestEnCours));
    else
        emit(exitTest());
}

void ExecutionTest::tempsStabilisationFini()
{
    emit(traceTest("Temps de stabilisation fini",0));
}

void ExecutionTest::verifierCritereArret()
{
    emit(traceTest("Vérification du critère d'arrêt",0));

    QMapIterator<ushort,QVector<QWeakPointer<MesureIndividuelle> > > it_tabMoyennesMesuresParPhase(m_tabMoyennesMesuresParPhase);
    while(it_tabMoyennesMesuresParPhase.hasNext()) {
        it_tabMoyennesMesuresParPhase.next();

        QVector<QWeakPointer<MesureIndividuelle> > tableauMoyennesMesuresParPhase = it_tabMoyennesMesuresParPhase.value();
        ushort nbCyclesMesuresCritereArret = m_paramsTest.data()->m_test->getPhase(m_noPhaseSuivante).getCritereArret_NbCyclesMesures();
        ushort nbMoyenneEnregistrees = tableauMoyennesMesuresParPhase.count();
        ushort uniteCritereArret = m_paramsTest.data()->m_test->getPhase(m_noPhaseSuivante).getCritereArret_Unite();
        if(nbMoyenneEnregistrees< nbCyclesMesuresCritereArret) {
            emit(traceTest("Critère d'arrêt non atteint",0));
            emit(this->critereArretNonAtteint());
            return;
        }
        float min = FLT_MAX;
        float max = FLT_MIN;

        for(ushort i=(nbMoyenneEnregistrees-nbCyclesMesuresCritereArret);i<tableauMoyennesMesuresParPhase.count()-1;i++) {
            if(tableauMoyennesMesuresParPhase.at(i).data()->at(0)<min)
                min = tableauMoyennesMesuresParPhase.at(i).data()->at(0);
            if(tableauMoyennesMesuresParPhase.at(i).data()->at(0)>max)
                max = tableauMoyennesMesuresParPhase.at(i).data()->at(0);
        }

        float variation = (fabs(max -min));
        if(uniteCritereArret==POURCENTAGE) variation = (variation*100) / max;
        float critereStabilisation = m_paramsTest.data()->m_test->getPhase(m_noPhaseSuivante).getCritereArret_PourcentageStabilisation();
        emit(traceTest("Critère de stabilisation = "+QString::number(critereStabilisation),0));
        emit(traceTest("Variation = "+QString::number(variation),0));
        if(variation > critereStabilisation)
        {
            emit(traceTest("Critère d'arrêt non atteint",0));
            emit(this->critereArretNonAtteint());
            return;
        }
    }
    emit(traceTest("Critère d'arrêt atteint",0));
    emit(this->critereArretAtteint());
}

void ExecutionTest::testerCyclesPhases()
{
    m_cyclePhaseEnCours++;
    emit(traceTest("Nombres de cycles de phases restants = "+QString::number(m_nbCyclesPhasesRestants),1));
    m_machine->postEvent(new ev_nbCyclesPhasesRestants(m_nbCyclesPhasesRestants));
}

void ExecutionTest::testerPhasesRestantes()
{
    QMapIterator<ushort,QVector<QWeakPointer<MesureIndividuelle> > > it_tabMoyennesMesuresParPhase(m_tabMoyennesMesuresParPhase);
    while(it_tabMoyennesMesuresParPhase.hasNext()) {
        it_tabMoyennesMesuresParPhase.next();
        QVector<QWeakPointer<MesureIndividuelle> > vecteurVide;
        m_tabMoyennesMesuresParPhase[it_tabMoyennesMesuresParPhase.key()] = vecteurVide;
    }
    m_flagPhaseInitialisee = false;
    emit(traceTest("Nombres de phases restantes = "+QString::number(m_nbPhasesRestantes),1));
    m_machine->postEvent(new ev_PhasesRestantes(m_nbPhasesRestantes));
}

void ExecutionTest::testerCyclesMesures()
{
    m_cycleMesureEnCours++;
    QMapIterator<ushort, QVector<QWeakPointer<MesureIndividuelle> > > it_tabMesuresParCycle(m_tabMesuresParCycle);
    while(it_tabMesuresParCycle.hasNext()) {
        it_tabMesuresParCycle.next();

        QVector<QWeakPointer<MesureIndividuelle> > tabMesuresParCycle = it_tabMesuresParCycle.value();
        for(int i=0;i<tabMesuresParCycle.count();i++) {
            QWeakPointer<MesureIndividuelle> mesureIndividuelle = tabMesuresParCycle.at(i);
            delete mesureIndividuelle.data();
            mesureIndividuelle.clear();
        }
        QVector<QWeakPointer<MesureIndividuelle> > vecteurVide;
        m_tabMesuresParCycle[it_tabMesuresParCycle.key()] = vecteurVide;
    }
    emit(traceTest("Nombres de cycles de mesures restants = "+QString::number(m_NbCyclesMesuresRestants),1));
    m_machine->postEvent(new ev_NbCyclesMesure(m_NbCyclesMesuresRestants));
}

void ExecutionTest::messageBdHandler(const QString trace)
{
    emit(traceTest(trace,2));
}
