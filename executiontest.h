/*////////////////////////////////////////////////////////////
// \file executiontest.h
// \brief Classe implémentant l'exécution des tests métrologiques
// \author FOUQUART Christophe
// \version 1.0
// \date 20/06/2011
//
// TAM - Tests Automatiques Métrologiques
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
// \brief Classe implémentant l'exécution des tests métrologiques
//
// Classe implémentant l'exécution des tests métrologiques
////////////////////////////////////////////////////////////////////////////////////*/
//class ExecutionTest : public QThread
class ExecutionTest : public QThread
{
    Q_OBJECT
private :
    short m_idListeTestEnCours;
    ushort m_id_TestMetro;
    QPointer<QStateMachine> m_machine; // Pointeur vers instance de QStateMachine. Sert à exéctuer le test
    QWeakPointer<et_ParamsTest> m_paramsTest;
    QPointer<BdHandler> m_bdHandler; // Pointeur vers une instance de la classe BdHandler
    QMap<ushort,QSqlRecord*> m_analyseursInfos; // Liste des analyseurs et de leur enregistrement dans la base de données
    QMap<ushort,int> m_analyseursTxTransmission; // // Liste des analyseurs et de leur taux de transmission de données
    QMap<ushort, QPointer<Protocole> > m_analyseursProtocole; // // Liste des analyseurs et des pointeurs vers leur instance de l'objet Protocole associé
    QPointer<QFile> m_fichierMesures;

    QSqlRecord* m_infosCalibrateur; // Enregistrement dans la base de données du calibrateur utilisé pout le test
    int m_txTransmissionCalibrateur; // // Taux de transmission de données du calibrateur utilisé pout le test
    QPointer<Protocole> m_protocoleCalibrateur; // Pointeur vers une instance d'un objet Protocole

    ushort m_nbPhasesRestantes; // Nombre de phases restantes à exécuter
    ushort m_noPhaseSuivante; // Numéro de la phase à exécuter
    ushort m_nbCyclesPhasesRestants; // Nombre de cycles de phases restants à exécuter
    ushort m_NbCyclesMesuresRestants; // Nombre de cycles de mesures restants à effectuer lors d'une phase
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
    // \brief Récupère les infos sur les équipements dans la base de données
    ///////////////////////////////////////////////////////////////////////////*/
    void getInfosEquipements();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void constructionMachineEtat()
    // \brief Initialise la machine à état servant à l'exécution des tests
    ///////////////////////////////////////////////////////////////////////////*/
    void constructionMachineEtat();

public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn ExecutionTest(QPointer<Test> test)
    // \brief Constructeur
    //
    // \param test Pointeur vers une instance de la classe Test
    // \param idAnalyseurs Liste des identifiants des analyseurs testés et de leur interface
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
    // \brief Teste les différents périphériques
    //
    // La communication et les alarmes des périphériques sont testés. En cas d'erreur,
    // les variables m_erreurPeripherique et m_messageErreur sont initialisées.
    ///////////////////////////////////////////////////////////////////////////*/
    void testerPeripheriques();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void decrementerNbCyclesPhasesRestants()
    // \brief Décrémente le compteur du nombre de cycles de phases restants à exécuter
    ///////////////////////////////////////////////////////////////////////////*/
    void decrementerNbCyclesPhasesRestants();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void decrementerNbPhasesRestantes()
    // \brief Décrémente le compteur du nombre de phase restant à exécuter
    ///////////////////////////////////////////////////////////////////////////*/
    inline void decrementerNbPhasesRestantes() {m_nbPhasesRestantes -= 1;testerPhasesRestantes();}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void decrementerNbCyclesMesureRestants()
    // \brief Décrémente le compteur du nombre de cycles de mesure restant à exécuter
    ///////////////////////////////////////////////////////////////////////////*/
    inline void decrementerNbCyclesMesureRestants() {m_NbCyclesMesuresRestants -= 1;testerCyclesMesures();}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void IncrementerNoPhasesSuivant()
    // \brief Incrémente le numéro de phase à exécuter
    //
    // Incrémente le numéro de phase à exécuter
    ///////////////////////////////////////////////////////////////////////////*/
    inline void IncrementerNoPhasesSuivant() {m_noPhaseSuivante += 1;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void initTransitionTempsMaxPhase()
    // \brief Initialise la transition testant le temps maximum de phase
    ///////////////////////////////////////////////////////////////////////////*/
    inline void recupererTempsMaxPhase() {m_machine.data()->postEvent(new ev_TempsMaxPhase(m_noPhaseSuivante));}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void testerTempsMaxTest()
    // \brief Teste si un temps maximum de phase est défini
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
    // \brief Initialise les paramètres en début de phase
    ///////////////////////////////////////////////////////////////////////////*/
    void initialiserPhase();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void envoiConcentration()
    // \brief Envoi les concentrations définies au système étalon
    ///////////////////////////////////////////////////////////////////////////*/
    void envoiConcentration();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void enregistrerMoyenneMesures()
    // \brief Enregistrer la moyenne des mesures des analyseurs
    ///////////////////////////////////////////////////////////////////////////*/
    void enregistrerMoyenneMesures();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void verifierCritereArret()
    // \brief Vérifier qu'un critère d'arrêt a été défini pour la phase en cours
    ///////////////////////////////////////////////////////////////////////////*/
    inline void testerCritereArret() {qDebug()<<"Test du critère d'arrêt";m_NbCyclesMesuresRestants--;m_machine.data()->postEvent(new ev_TestCritereArret(this->m_noPhaseSuivante));}

    void testTermine();

    inline void erreurPeripheriqueRecue() {qDebug()<<"Erreur de périphérique";}

    void testerPhasesRestantes();

    void testerCyclesPhases();

    void testerCyclesMesures();

    void tempsStabilisationFini();

    void verifierCritereArret();

    void etatFinalTempsMaxPhase() {qDebug()<<"Temps max phase : etat final atteint";}

    void afficherTrame(const QString trame) {emit(traceTest(trame,2));}

    void messageBdHandler(const QString trace);

    void finPhase() {qDebug("Phase terminée");emit(this->phaseTerminee());}

public Q_SLOTS:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn void run()
    // \brief Lance l'exécution du test
    ///////////////////////////////////////////////////////////////////////////*/
    void run();

    void standByCalibrateur(const bool emitSignalFinTest = true) {m_protocoleCalibrateur.data()->standBy();if(emitSignalFinTest) emit(testMetroTermine());}

Q_SIGNALS:

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void erreurPeripherique()
    // \brief Signal envoyé si erreur produite lors de testerPeripheriques()
    ///////////////////////////////////////////////////////////////////////////*/
    void erreurPeripherique();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void peripheriquesOk()
    // \brief Signal envoyé si aucune erreur lors de testerPeripheriques()
    ///////////////////////////////////////////////////////////////////////////*/
    void peripheriquesOk();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void testMetroTermine()
    // \brief Signal envoyé lors de la fin du test métrologique
    ///////////////////////////////////////////////////////////////////////////*/
    void testMetroTermine();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void testMetroTermine()
    // \brief Signal envoyé lors de la fin d'une phase du test métrologique
    ///////////////////////////////////////////////////////////////////////////*/
    void phaseTerminee();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void tempsMaxPhaseAtteint()
    // \brief Signal envoyé si le temps maximum imparti pour une phase du test est dépassé
    ///////////////////////////////////////////////////////////////////////////*/
    void tempsMaxPhaseAtteint();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void mesuresEffectuees()
    // \brief Signal envoyé à la fin des mesures
    ///////////////////////////////////////////////////////////////////////////*/
    void mesuresEffectuees();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void mesuresEnregistrees()
    // \brief Signal envoyé à la fin de l'enregistrement des mesures
    ///////////////////////////////////////////////////////////////////////////*/
    void mesuresEnregistrees();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void initialisationPhaseTerminee()
    // \brief Signal envoyé lorsque l'initialisation de la phase est terminée
    ///////////////////////////////////////////////////////////////////////////*/
    void initialisationPhaseTerminee();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void concentrationEnvoyee()
    // \brief Signal envoyé lorsque les concentrations définies ont été envoyées
    ///////////////////////////////////////////////////////////////////////////*/
    void concentrationEnvoyee();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void moyenneMesuresEnregistree()
    // \brief Signal envoyé lorsque la moyenne des mesures a été enregistrée
    ///////////////////////////////////////////////////////////////////////////*/
    void moyenneMesuresEnregistree();

    void critereArretAtteint();

    void critereArretNonAtteint();

    void exitTest();

    void traceTest(const QString trace,const ushort niveau);

    void killMeAndMyThread(const short id);
};

#endif // EXECUTIONTEST_H
