/*////////////////////////////////////////////////////////////
// \file executiontest.h
// \brief Classe impl�mentant l'ex�cution des tests m�trologiques
// \author FOUQUART Christophe
// \version 1.0
// \date 20/06/2011
//
// TAM - Tests Automatiques M�trologiques
// Copyright (C) 2011 FOUQUART Christophe
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

#ifndef EXECUTIONTEST_H
#define EXECUTIONTEST_H
#include "definitions_globales.h"
#include "test.h"
#include "et_TransitionPhasesRestantes.h"
#include "et_TransitionTempsMaxPhase.h"
#include "et_TransitionTempsMaxTest.h"
#include "et_TransitionCritereArret.h"
#include "et_TransitionNbCyclesMesure.h"
#include "et_TransitionCyclesPhases.h"
#include "bdhandler.h"
#include "protocole.h"
#include "et_paramsTest.h"
#include "generateurrapporttest.h"

class QThreadEx : public QThread
{
protected:
    inline void run() {
        exec();
        qDebug()<<"fin du thread de test";
    }
};

/*////////////////////////////////////////////////////////////////////////////////////
// \class ExecutionTest
// \brief Classe impl�mentant l'ex�cution des tests m�trologiques
//
// Classe impl�mentant l'ex�cution des tests m�trologiques
////////////////////////////////////////////////////////////////////////////////////*/
//class ExecutionTest : public QThread
class ExecutionTest : public QThread
{
    Q_OBJECT
private :
    short m_idListeTestEnCours;
    ushort m_id_TestMetro;
    QPointer<QStateMachine> m_machine; // Pointeur vers instance de QStateMachine. Sert � ex�ctuer le test
    QWeakPointer<et_ParamsTest> m_paramsTest;
    QPointer<BdHandler> m_bdHandler; // Pointeur vers une instance de la classe BdHandler
    QMap<ushort,QSqlRecord*> m_analyseursInfos; // Liste des analyseurs et de leur enregistrement dans la base de donn�es
    QMap<ushort,int> m_analyseursTxTransmission; // // Liste des analyseurs et de leur taux de transmission de donn�es
    QMap<ushort, QPointer<Protocole> > m_analyseursProtocole; // // Liste des analyseurs et des pointeurs vers leur instance de l'objet Protocole associ�
    QPointer<QFile> m_fichierMesures;

    QSqlRecord* m_infosCalibrateur; // Enregistrement dans la base de donn�es du calibrateur utilis� pout le test
    int m_txTransmissionCalibrateur; // // Taux de transmission de donn�es du calibrateur utilis� pout le test
    QPointer<Protocole> m_protocoleCalibrateur; // Pointeur vers une instance d'un objet Protocole

    ushort m_nbPhasesRestantes; // Nombre de phases restantes � ex�cuter
    ushort m_noPhaseSuivante; // Num�ro de la phase � ex�cuter
    ushort m_nbCyclesPhasesRestants; // Nombre de cycles de phases restants � ex�cuter
    ushort m_NbCyclesMesuresRestants; // Nombre de cycles de mesures restants � effectuer lors d'une phase
    ushort m_cycleMesureEnCours; // Cycle de mesures en cours
    ushort m_cyclePhaseEnCours; // Cycle de phase en cours

    QMap<ushort,QWeakPointer<MesureIndividuelle> > m_tabMesuresIndividuelles;
    QMap<ushort,QVector<QWeakPointer<MesureIndividuelle> > > m_tabMesuresParCycle;
    QMap<ushort,QVector<QWeakPointer<MesureIndividuelle> > > m_tabMoyennesMesuresParPhase;
    QSharedPointer<QVector<MesureInfo> > m_tableauCompletMesures;

    QPointer<QTimer> m_timerDemmarageMachine;
    QPointer<QTimer> m_timerTempsMaxPhase;
    QPointer<QTimer> m_timerTempsAcquisition;
    QPointer<QTimer> m_timerTempsStabilisation;
    QPointer<QTimer> m_timerTempsMoyennageMesures;
    QPointer<QTimer> m_timerTempsAttenteEntreMesures;

    QString m_etatAutomate;

    bool m_moyennerMesures;
    bool m_flagPhaseInitialisee;

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void getInfosEquipements()
    // \brief R�cup�re les infos sur les �quipements dans la base de donn�es
    ///////////////////////////////////////////////////////////////////////////*/
    void getInfosEquipements();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void constructionMachineEtat()
    // \brief Initialise la machine � �tat servant � l'ex�cution des tests
    ///////////////////////////////////////////////////////////////////////////*/
    void constructionMachineEtat();

public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn ExecutionTest(QPointer<Test> test)
    // \brief Constructeur
    //
    // \param test Pointeur vers une instance de la classe Test
    // \param idAnalyseurs Liste des identifiants des analyseurs test�s et de leur interface
    ///////////////////////////////////////////////////////////////////////////*/
    ExecutionTest(const QPointer<et_ParamsTest> paramsTest,const QPointer<BdHandler> bdHandler=0,const short idListeTestEnCours=-1);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ~ExecutionTest()
    // \brief Destructeur
    ///////////////////////////////////////////////////////////////////////////*/
    ~ExecutionTest();

private Q_SLOTS:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn void testerPeripheriques()
    // \brief Teste les diff�rents p�riph�riques
    //
    // La communication et les alarmes des p�riph�riques sont test�s. En cas d'erreur,
    // les variables m_erreurPeripherique et m_messageErreur sont initialis�es.
    ///////////////////////////////////////////////////////////////////////////*/
    void testerPeripheriques();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void decrementerNbCyclesPhasesRestants()
    // \brief D�cr�mente le compteur du nombre de cycles de phases restants � ex�cuter
    ///////////////////////////////////////////////////////////////////////////*/
    void decrementerNbCyclesPhasesRestants();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void decrementerNbPhasesRestantes()
    // \brief D�cr�mente le compteur du nombre de phase restant � ex�cuter
    ///////////////////////////////////////////////////////////////////////////*/
    inline void decrementerNbPhasesRestantes() {m_nbPhasesRestantes -= 1;testerPhasesRestantes();}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void decrementerNbCyclesMesureRestants()
    // \brief D�cr�mente le compteur du nombre de cycles de mesure restant � ex�cuter
    ///////////////////////////////////////////////////////////////////////////*/
    inline void decrementerNbCyclesMesureRestants() {m_NbCyclesMesuresRestants -= 1;testerCyclesMesures();}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void IncrementerNoPhasesSuivant()
    // \brief Incr�mente le num�ro de phase � ex�cuter
    //
    // Incr�mente le num�ro de phase � ex�cuter
    ///////////////////////////////////////////////////////////////////////////*/
    inline void IncrementerNoPhasesSuivant() {m_noPhaseSuivante += 1;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void initTransitionTempsMaxPhase()
    // \brief Initialise la transition testant le temps maximum de phase
    ///////////////////////////////////////////////////////////////////////////*/
    inline void recupererTempsMaxPhase() {m_machine.data()->postEvent(new ev_TempsMaxPhase(m_noPhaseSuivante));}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void testerTempsMaxTest()
    // \brief Teste si un temps maximum de phase est d�fini
    ///////////////////////////////////////////////////////////////////////////*/
    inline void testerTempsMaxPhase() {m_machine.data()->postEvent(new ev_TempsMaxPhase(m_noPhaseSuivante));}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void lancerTimerTempsMaxPhase()
    // \brief Lance le timer de temps maximum de phase
    ///////////////////////////////////////////////////////////////////////////*/
    void lancerTimerTempsMaxPhase();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void lancerTimerTempsAcquisition()
    // \brief Lance le timer de temps d'acquisition
    ///////////////////////////////////////////////////////////////////////////*/
    void lancerTimerTempsAcquisition();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void lancerTimerTempsStabilisation()
    // \brief Lance le timer de temps d'acquisition
    ///////////////////////////////////////////////////////////////////////////*/
    void lancerTimerTempsStabilisation();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void lancerTimerTempsMoyennageMesures()
    // \brief Lance le timer de temps de moyennage des mesures
    ///////////////////////////////////////////////////////////////////////////*/
    void lancerTimerTempsMoyennageMesures();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void lancerTimerTempsAttenteEntreMesures()
    // \brief Lance le timer de temps d'attente entre les mesures
    ///////////////////////////////////////////////////////////////////////////*/
    void lancerTimerTempsAttenteEntreMesures();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void enregistrerMesures()
    // \brief Enregistrer les mesures des analyseurs
    ///////////////////////////////////////////////////////////////////////////*/
    void enregistrerMesures();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void initialiserPhase()
    // \brief Initialise les param�tres en d�but de phase
    ///////////////////////////////////////////////////////////////////////////*/
    void initialiserPhase();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void envoiConcentration()
    // \brief Envoi les concentrations d�finies au syst�me �talon
    ///////////////////////////////////////////////////////////////////////////*/
    void envoiConcentration();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void enregistrerMoyenneMesures()
    // \brief Enregistrer la moyenne des mesures des analyseurs
    ///////////////////////////////////////////////////////////////////////////*/
    void enregistrerMoyenneMesures();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void verifierCritereArret()
    // \brief V�rifier qu'un crit�re d'arr�t a �t� d�fini pour la phase en cours
    ///////////////////////////////////////////////////////////////////////////*/
    inline void testerCritereArret() {qDebug()<<"Test du crit�re d'arr�t";m_NbCyclesMesuresRestants--;m_machine.data()->postEvent(new ev_TestCritereArret(this->m_noPhaseSuivante));}

    void testTermine();

    inline void erreurPeripheriqueRecue() {qDebug()<<"Erreur de p�riph�rique";}

    void testerPhasesRestantes();

    void testerCyclesPhases();

    void testerCyclesMesures();

    void tempsStabilisationFini();

    void verifierCritereArret();

    void etatFinalTempsMaxPhase() {qDebug()<<"Temps max phase : etat final atteint";}

    void afficherTrame(const QString trame) {emit(traceTest(trame,2));}

    void messageBdHandler(const QString trace);

    void finPhase() {qDebug("Phase termin�e");emit(this->phaseTerminee());}

public Q_SLOTS:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn void run()
    // \brief Lance l'ex�cution du test
    ///////////////////////////////////////////////////////////////////////////*/
    void run();

    void standByCalibrateur(const bool emitSignalFinTest = true) {m_protocoleCalibrateur.data()->standBy();if(emitSignalFinTest) emit(testMetroTermine());}

Q_SIGNALS:

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void erreurPeripherique()
    // \brief Signal envoy� si erreur produite lors de testerPeripheriques()
    ///////////////////////////////////////////////////////////////////////////*/
    void erreurPeripherique();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void peripheriquesOk()
    // \brief Signal envoy� si aucune erreur lors de testerPeripheriques()
    ///////////////////////////////////////////////////////////////////////////*/
    void peripheriquesOk();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void testMetroTermine()
    // \brief Signal envoy� lors de la fin du test m�trologique
    ///////////////////////////////////////////////////////////////////////////*/
    void testMetroTermine();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void testMetroTermine()
    // \brief Signal envoy� lors de la fin d'une phase du test m�trologique
    ///////////////////////////////////////////////////////////////////////////*/
    void phaseTerminee();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void tempsMaxPhaseAtteint()
    // \brief Signal envoy� si le temps maximum imparti pour une phase du test est d�pass�
    ///////////////////////////////////////////////////////////////////////////*/
    void tempsMaxPhaseAtteint();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void mesuresEffectuees()
    // \brief Signal envoy� � la fin des mesures
    ///////////////////////////////////////////////////////////////////////////*/
    void mesuresEffectuees();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void mesuresEnregistrees()
    // \brief Signal envoy� � la fin de l'enregistrement des mesures
    ///////////////////////////////////////////////////////////////////////////*/
    void mesuresEnregistrees();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void initialisationPhaseTerminee()
    // \brief Signal envoy� lorsque l'initialisation de la phase est termin�e
    ///////////////////////////////////////////////////////////////////////////*/
    void initialisationPhaseTerminee();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void concentrationEnvoyee()
    // \brief Signal envoy� lorsque les concentrations d�finies ont �t� envoy�es
    ///////////////////////////////////////////////////////////////////////////*/
    void concentrationEnvoyee();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void moyenneMesuresEnregistree()
    // \brief Signal envoy� lorsque la moyenne des mesures a �t� enregistr�e
    ///////////////////////////////////////////////////////////////////////////*/
    void moyenneMesuresEnregistree();

    void critereArretAtteint();

    void critereArretNonAtteint();

    void exitTest();

    void traceTest(const QString trace,const ushort niveau);

    void killMeAndMyThread(const short id);
};

#endif // EXECUTIONTEST_H
