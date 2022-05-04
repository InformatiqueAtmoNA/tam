/*////////////////////////////////////////////////////////////
// \file definitions_globale.h
// \brief Macros, énumérations et fonctions globales au programme
// \author FOUQUART Christophe
// \version 1.0
// \date 25/03/2011
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

#ifndef DEFINITIONS_GLOBALES_H
#define DEFINITIONS_GLOBALES_H

#include <QObject>
#include <QtWidgets/QGridLayout>
#include <QDebug>
#include <QStringList>
#include <QtXml>
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QtCore>
#include <QVector>
#include <QList>
#include <QtWidgets/QListWidgetItem>
#include <QFile>
#include <QMapIterator>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QtWidgets/QMessageBox>
#include <QSqlRecord>
#include <QSqlRelationalDelegate>
#include <QStateMachine>
#include <QErrorMessage>
#include "MesureIndividuelle.h"
#include <QStandardItemModel>

// \def ETAT_REPOS Pas de communication en cours
#define ETAT_REPOS 0
// \def ETAT_ATTENTE Attente d'une réception de trame
#define ETAT_ATTENTE 1
// \def ETAT_LECTURE Réception d'une trame effectuée
#define ETAT_LECTURE 2
// \def ETAT_LECTURE Réception d'une trame effectuée
#define ETAT_ERREUR 3
// \def STX Caractère ASCII Start Of Text
#define STX 0x02
// \def STX Caractère ASCII End Of Text
#define ETX 0x03
// \def ACK Caractère ASCII d'acquitement
#define ACK 0x06
// \def NACK Caractère ASCII de non acquitement
#define NAK 0x15
// \def SEP Caractère ASCII de séparation
#define SEP 0x20
// \def CR Caractère ASCII de retour chariot
#define CR  0x0D
// \def SH_CANAL Index du canal dans les données membres de la classe SpanHandler
#define SH_CANAL  0
// \def SH_POINT Index du point dans les données membres de la classe SpanHandler
#define SH_POINT  1
// \def SH_CONCO3 Index de la concentration d'O3 dans les données membres de la classe SpanHandler
#define SH_CONCO3 2

// \enum DesignationProtocole DesignationProtocole de communication supportés
// A MODIFIER LORS DE L'AJOUT DE NOUVEAUX DesignationProtocole
enum DesignationProtocole {MODE4_ANA_CMD04,MODE4_ANA_CMD16,MODE4_SX6000_17,MODE4_SX6000_1808,MODE4_LNI,TEI_ANA,TEI_146i,TEI_146i_OZONISEUR,
                 TEI_146i_PHOTOMETRE,TEI_146c,TEI_146c_V2,TEI_49ps,API_ANA,API_DIL,PROTOCOLE_INCONNU};

// \enum TypePeripherique Type d'appareil
enum TypePeripherique {ANALYSEUR,ETALON,DILUTEUR,BOUTEILLE,GO3,GZERO};
// \enum TypePolluant Polluant associé à  l'appareil
enum TypePolluant {CO,O3,SO2,NO,NO2,NOX,H2S,DIL};
// \enum TypeTest Type de test
enum TypeTest {REPETABILITE,LINEARITE,TEMPS_REPONSE,RENDEMENT_FOUR,PERSO};
// \enum OptionTpg Option d'étalonnage permettant la TPG
enum OptionTpg {AUCUNE,OZONISEUR,PHOTOMETRE};
// \enum Commandes Listes des commandes
enum Commandes {OFFSET,MESURES,ALARME,DATE_HEURE,MODE_ZERO,MODE_ETALON,MODE_MESURE,
                SPAN,SPAN_ZERO,SPAN_TPG,SPAN_O3,RESET,STAND_BY,EVENT,NO_CMD};
// \enum Arguments de commande de span
enum SpanArguments {CANAL,POINT,CONCO3};
// \enum Commandes disponible en début de phase de test
enum commandes_debut_phase {CMD_DEBUT_PHASE_NOCMD,CMD_DEBUT_PHASE_MODE_MESURE,CMD_DEBUT_PHASE_MODE_ETALON};
// \enum CritereArretUnite Unité du critère d'arrêt
enum CritereArretUnite {POURCENTAGE=0,PPB=1};
// \enum Champs du QVector contenant un critère d'arrêt d'une phase
enum critere_arret {CRITERE_ARRET_NB_CYCLES_MESURES,CRITERE_ARRET_POURCENTAGE_STABILISATION,CRITERE_ARRET_UNITE};

// \enum Colonnes de la table Molecule
enum table_molecule {MOLECULE_ID,MOLECULE_CODE,MOLECULE_NOM,MOLECULE_FORMULE};
// \enum Colonnes de la table Protocole
enum table_protocole {PROTOCOLE_ID,PROTOCOLE_DESIGNATION};
// \enum Colonnes de la table Marque_Equipement
enum table_marque {MARQUE_ID,MARQUE_DESIGNATION};
// \enum Colonnes de la table Taux_Transmission
enum table_tx_transmission {TX_TRANSMISSION_ID,TX_TRANSMISSION_DESIGNATION};
// \enum Colonnes de la table Port_Serie
enum table_port_serie {PORT_SERIE_NO,PORT_SERIE_DESIGNATION};
// \enum Colonnes de la table Modele_Equipement
enum table_modele {MODELE_ID,MODELE_ID_MARQUE,MODELE_ID_PROTOCOLE,MODELE_DESIGNATION,MODELE_TYPE};
// \enum Colonnes de la table Equipement
enum table_equipement {EQUIPEMENT_ID,EQUIPEMENT_ID_MODELE,EQUIPEMENT_NO_SERIE,
                       EQUIPEMENT_EN_SERVICE,EQUIPEMENT_MIN_GAMME,
                       EQUIPEMENT_MAX_GAMME,EQUIPEMENT_OFFSET,EQUIPEMENT_ID_TX_TRANSMISSION,
                       EQUIPEMENT_ADRESSE,EQUIPEMENT_NB_BITS_TRANSMISSION, EQUIPEMENT_NB_BITS_STOP,
                       EQUIPEMENT_CONTROLE_FLUX,EQUIPEMENT_PARITE,EQUIPEMENT_PORT};
// \enum Colonnes de la table Systeme_Etalon
enum table_systeme_etalon {SYS_ETALON_ID,SYS_ETALON_DILUTEUR,SYS_ETALON_BOUTEILLE,SYS_ETALON_GZERO,SYS_CANAL_DEFAUT};
// \enum Colonnes de la table Concentration
enum table_concentration {CONCENTRATION_ID,CONCENTRATION_SYS_ETALON,CONCENTRATION_ID_MOLECULE,
                          CONCENTRATION_POINT,CONCENTRATION_REELLE,CONCENTRATION_OZONE};
// \enum Colonnes de la table Concentration_Associee
enum table_concentration_associee {CONC_ASSOCIEE_ID,CONC_ASSOCIEE_ID_CONCENTRATION,CONC_ASSOCIEE_FORMULE,
                                   CONC_ASSOCIEE_CONCENTRATION};
// \enum Colonnes de la table Polluant_Associe
enum table_polluant_associe {POLLUANT_ASSOCIE_ID,POLLUANT_ASSOCIE_ID_EQUIPEMENT,POLLUANT_ASSOCIE_ID_MOLECULE};
// \enum Colonnes de la table Test_Metro
enum table_test_metro {TEST_METRO_ID,TEST_METRO__TYPE_TEST,TEST_METRO_ID_OPERATEUR,TEST_METRO_ID_SYSTEME_ETALON,TEST_METRO_ID_LIEU,
                     TEST_METRO_PRESSION,TEST_METRO_TEMPERATURE,TEST_METRO_DATE_DEBUT,TEST_METRO_DATE_FIN,TEST_METRO_TPS_ACQUISITION,
                      TEST_METRO_CRITERE_1,TEST_METRO_CRITERE_2};
// \enum Colonnes de la table Test_XML
enum table_test_xml {TEST_XML_ID,TEST_XML_NOM_FICHIER,TEST_XML_TYPE_TEST,TEST_XML_ID_SYSTEME_ETALON,TEST_XML_EST_FAVORI};
// \enum Colonnes de la table Operateur
enum table_operateur {OPERATEUR_ID,OPERATEUR_NOM,OPERATEUR_PRENOM,OPERATEUR_MDP,OPERATEUR_ADMIN};
// \enum Colonnes de la table Lieu
enum table_lieu {LIEU_ID,LIEU_DESIGNATION};
// \enum Colonnes de la table Mesure
enum table_mesure {MESURE_ID,MESURE_ID_TEST,MESURE_ID_EQUIPEMENT,MESURE_NO_CYCLE_PHASE,MESURE_NO_PHASE,
                   MESURE_NO_CYCLE_MESURE,MESURE_MESURE};
// \enum Colonnes de la table Liste_Analyseurs_Test
enum table_liste_analyseurs_test {LISTE_ANA_TEST_ID_TEST,LISTE_ANA_ID_EQUIPEMENT};
// \enum Colonnes de la table Concentration_Test_Metro
enum table_Concentration_Test_Metro {CONC_TEST_METRO_ID,CONC_TEST_METRO_ID_TEST,CONC_TEST_METRO_NO_PHASE,
                                     CONC_TEST_METRO_ID_MOLECULE,CONC_TEST_METRO_CONCENTRATION,CONC_TEST_METRO_NBRE_ACQUISITION};
// \enum Colonnes du modèle PolluantByIdSystemeEtalon
enum modele_polluant_by_systeme_etalon {POLLUANT_BY_SYS_ETALON_ID,POLLUANT_BY_SYS_ETALON_FORMULE};

// \enum Colonnes du TableWidget Polluant de ct_phasewidget
enum phasewidget_tablewidget_polluant {PHASEW_TABLEW_POLLUANTS_IDMOLECULE,PHASEW_TABLEW_POLLUANTS_CODE,
                                       PHASEW_TABLEW_POLLUANTS_FORMULE,PHASEW_TABLEW_POLLUANTS_POINT,
                                       PHASEW_TABLEW_POLLUANTS_REELLE,PHASEW_TABLEW_POLLUANTS_OZONE};

// \enum Colonnes du TableWidget TestsXml de HomeWidget
enum homewidget_tablewidget_test {HOMEW_TABLEW_TEST_ID_TEST,HOMEW_TABLEW_TEST_FICHIER,HOMEW_TABLEW_TEST_TYPE,
                                  HOMEW_TABLEW_TEST_DILUTEUR,HOMEW_TABLEW_TEST_BOUTEILLE,HOMEW_TABLEW_TEST_GZERO,HOMEW_TABLEW_TEST_EST_FAVORI};

// \enum Colonnes du TableWiew TestsRapport de HomWidget
enum homewidget_tableview_testrappport{HOMEW_TABVIEW_TEST_ID_TEST,HOMEW_TABVIEW_TEST_ID_EQUIP,HOMEW_TABVIEW_TEST_NO_EQUIP,
                                       HOMEW_TABVIEW_TEST_TYPE_TEST,HOMEW_TABVIEW_TEST_DATE};
// \enum Colonnes du modèle relationnel de la table Equipement
enum model_relationnel_equipement {REL_EQUIPEMENT_ID,REL_EQUIPEMENT_NUM_SERIE,REL_EQUIPEMENT_MODELE,REL_EQUIPEMENT_MARQUE,
                                   REL_EQUIPEMENT_MIN_GAMME,REL_EQUIPEMENT_MAX_GAMME,REL_EQUIPEMENT_OFFSET,REL_EQUIPEMENT_ADRESSE,REL_EQUIPEMENT_PORTSERIE};

// \enum Colonnes de la requete getModeleAnalyseur
enum requete_getModeleAnalyseur {REQ_MODELE_EQUIP_ID,REQ_MODELE_EQUIP_DESIGNATION};

// \enum Colonnes du TableWidget Analyseurs de et_interfaceExecutionTest
enum et_interfaceExecutionTest_tablewidget_Analyseurs
   {ET_TABLEW_ANALYSEURS_ID_EQUIPEMENT,ET_TABLEW_ANALYSEURS_NUM_SERIE,ET_TABLEW_ANALYSEURS_MODELE,
    ET_TABLEW_ANALYSEURS_MARQUE,ET_TABLEW_ANALYSEURS_MIN_GAMME,ET_TABLEW_ANALYSEURS_MAX_GAMME,
    ET_TABLEW_ANALYSEURS_OFFSET,ET_TABLEW_ANALYSEURS_ADRESSE,ET_TABLEW_ANALYSEURS_PORTSERIE};

// \enum Colonnes du TableWidget Communications de et_interfaceExecutionTest
enum et_interfaceExecutionTest_tablewidget_Communication
   {ET_TABLEW_COMMUNICATION_ID_EQUIPEMENT,ET_TABLEW_COMMUNICATION_NUM_SERIE,
    ET_TABLEW_COMMUNICATION_INTERFACE,ET_TABLEW_COMMUNICATION_ETAT_COM};

// \enum Colonnes du TableWidget TestsEnAttente de et_listeAttenteTest
enum et_listeAttenteTest_tablewidget_TestsEnAttente
   {ET_TABLEW_TEST_ATTENTE_ID_TEST,ET_TABLEW_TEST_ATTENTE_NOM_TEST,ET_TABLEW_TEST_ATTENTE_DATE_HEURE_DEBUT,
    ET_TABLEW_TEST_ATTENTE_ETAT,ET_TABLEW_TEST_ATTENTE_INTERFACE_CALIBRATEUR,
    ET_TABLEW_TEST_ATTENTE_CANAL_CALIBRATEUR};

// \enum C


// \enum Codes d'erreurs de l'exécution de test
enum erreur_execution_test {AUCUNE_ERREUR,ERREUR_COMMUNICATION,ERREUR_ALARME};

// \enum Choix de début de test
enum choix_debut_test {DEBUT_TEST_IMMEDIAT,DEBUT_TEST_DATE_HEURE};

// \struct PhaseConfig Informations de configuration de ct_phasewidget
struct PhaseConfig {
    ushort nbCyclesMesures; // Nombre de cycles de mesures à effectuer après le temps de stabilisation
    QTime tempsStabilisation; // Temps de stabilisation pendant lequel aucune mesure n'est effectuée
    QTime tempsMoyennageMesure; // Temps correspondant à un cycle sur lequel les mesures seront moyennées
    QTime tempsAttenteEntreMesure; // Temps d'attente entre chaque cycle de mesure
};

// \struct MesureInfo Informations de mesure
struct MesureInfo {
    ushort idTest;
    ushort idEquipement;
    ushort noCyclePhase;
    ushort noPhase;
    ushort noCycleMesure;
    QPointer<MesureIndividuelle> mesure;
};

/*///////////////////////////////////////////////////////////////////////////
// \fn extern DesignationProtocole stringToTypePolluant(QString protocole)
// \brief Renvoi le DesignationProtocole contenu dans la chaine en paramètre
//
// \param DesignationProtocole Chaine de caractères représentant le protocole
// \return protocole Type de polluant
///////////////////////////////////////////////////////////////////////////*/
extern DesignationProtocole stringToProtocole(QString protocole);

/*///////////////////////////////////////////////////////////////////////////
// \fn extern TypePolluant stringToTypePolluant(QString type)
// \brief Renvoi le TypePolluant contenu dans la chaine en paramètre
//
// \param type Chaine de caractères représentant le type de polluant
// \return TypePolluant Type de polluant
///////////////////////////////////////////////////////////////////////////*/
extern TypePolluant stringToTypePolluant(QString type);

/*///////////////////////////////////////////////////////////////////////////
// \fn extern QString typePolluantToString(TypePolluant type)
// \brief Renvoi la Chaine de caractères représentant le type de polluant
//
// \param type Type de polluant
// \return QString Chaine de caractères représentant le type de polluant
///////////////////////////////////////////////////////////////////////////*/
extern QString typePolluantToString(TypePolluant type);

/*///////////////////////////////////////////////////////////////////////////
// \fn extern TypePolluant stringToTypePolluant(QString type)
// \brief Renvoi le TypePolluant contenu dans la chaine en paramètre
//
// \param type Chaine de caractères représentant le type de polluant
// \return TypePolluant Type de polluant
///////////////////////////////////////////////////////////////////////////*/
extern TypeTest stringToTypeTest(QString type);

/*///////////////////////////////////////////////////////////////////////////
// \fn extern QString typePolluantToString(TypePolluant type)
// \brief Renvoi la Chaine de caractères représentant le type de polluant
//
// \param type Type de polluant
// \return QString Chaine de caractères représentant le type de polluant
///////////////////////////////////////////////////////////////////////////*/
extern QString typeTestToString(TypeTest type);

/*///////////////////////////////////////////////////////////////////////////
// \fn extern Commandes stringToCommande(QString cmd)
// \brief Renvoi la commande contenue dans la chaine en paramètre
//
// \param cmd Chaine de caractères représentant la commande
// \return Commandes Commande
///////////////////////////////////////////////////////////////////////////*/
extern Commandes stringToCommande(QString cmd);

/*///////////////////////////////////////////////////////////////////////////
// \fn extern QString commandesToString(Commandes cmd)
// \brief Renvoi la chaine en paramètre représentant la commande
//
// \param cmd Commande
// \return Commandes Chaine de caractères représentant
///////////////////////////////////////////////////////////////////////////*/
extern QString commandesToString(Commandes cmd);

/*///////////////////////////////////////////////////////////////////////////
// \fn extern QVariant getParam(QString const & key)
// \brief Renvoi un paramètre stocké dans param.ini
//
// \param key Nom du paramètre demandé
// \return QVariant Paramètre demandé
///////////////////////////////////////////////////////////////////////////*/
extern QVariant getParam(QString const & key);

/*///////////////////////////////////////////////////////////////////////////
// \fn extern void setParam(QString const & key, QVariant const & param)
// \brief Stocke un paramètre dans param.ini
//
// \param key Nom du paramètre demandé
// \param param Paramètre à sauvegarder
///////////////////////////////////////////////////////////////////////////*/
extern void setParam(QString const & key, QVariant const & param);

#endif // DEFINITIONS_GLOBALES_H
