/*////////////////////////////////////////////////////////////
// \file definitions_globale.h
// \brief Macros, �num�rations et fonctions globales au programme
// \author FOUQUART Christophe
// \version 1.0
// \date 25/03/2011
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

#ifndef DEFINITIONS_GLOBALES_H
#define DEFINITIONS_GLOBALES_H

#include <QObject>
#include <QDebug>
#include <QStringList>
#include <QtXml>
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QtCore>
#include <QVector>
#include <QList>
#include <QListWidgetItem>
#include <QFile>
#include <QMapIterator>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlRelationalDelegate>

// \def ETAT_REPOS Pas de communication en cours
#define ETAT_REPOS 0
// \def ETAT_ATTENTE Attente d'une r�ception de trame
#define ETAT_ATTENTE 1
// \def ETAT_LECTURE R�ception d'une trame effectu�e
#define ETAT_LECTURE 2
// \def ETAT_LECTURE R�ception d'une trame effectu�e
#define ETAT_ERREUR 3
// \def STX Caract�re ASCII Start Of Text
#define STX 0x02
// \def STX Caract�re ASCII End Of Text
#define ETX 0x03
// \def ACK Caract�re ASCII d'acquitement
#define ACK 0x06
// \def NACK Caract�re ASCII de non acquitement
#define NAK 0x15
// \def SEP Caract�re ASCII de s�paration
#define SEP 0x20
// \def CR Caract�re ASCII de retour chariot
#define CR  0x0D
// \def SH_CANAL Index du canal dans les donn�es membres de la classe SpanHandler
#define SH_CANAL  0
// \def SH_POINT Index du point dans les donn�es membres de la classe SpanHandler
#define SH_POINT  1
// \def SH_CONCO3 Index de la concentration d'O3 dans les donn�es membres de la classe SpanHandler
#define SH_CONCO3 2

// \enum TypePeripherique Type d'appareil
enum TypePeripherique {ANALYSEUR,ETALON,DILUTEUR,BOUTEILLE,GO3,GZERO};
// \enum TypePolluant Polluant associ� �  l'appareil
enum TypePolluant {CO,O3,SO2,NO,NO2,NOX,DIL};
// \enum TypeTest Type de test
enum TypeTest {REPETABILITE_1,REPETABILITE_2,LINEARITE,TEMP_REPONSE,RENDEMENT_FOUR,PERSO};
// \enum OptionTpg Option d'�talonnage permettant la TPG
enum OptionTpg {AUCUNE,OZONISEUR,PHOTOMETRE};
// \enum Protocoles Protocoles de communication support�s
// A MODIFIER LORS DE L'AJOUT DE NOUVEAUX PROTOCOLES
enum Protocoles {MODE4_ANA,MODE4_SX6000_17,MODE4_SX3022,TEI_ANA,TEI_146i,TEI_146c,TEI_49ps,API_ANA,API_DIL};
// \enum Commandes Listes des commandes
enum Commandes {OFFSET,MESURES,ALARME,DATE_HEURE,MODE_ZERO,MODE_ETALON,MODE_MESURE,
                SPAN,SPAN_ZERO,SPAN_TPG,SPAN_O3,RESET,STAND_BY,EVENT,NO_CMD};
// \enum Arguments de commande de span
enum SpanArguments {CANAL,POINT,CONCO3};
// \enum Commandes disponible en d�but de phase de test
enum commandes_debut_phase {CMD_DEBUT_PHASE_NOCMD,CMD_DEBUT_PHASE_MODE_MESURE,CMD_DEBUT_PHASE_MODE_ETALON};
// \enum Champs du QVector contenant un crit�re d'arr�t d'une phase
enum critere_arret {CRITERE_ARRET_NB_CYCLES_MESURES,CRITERE_ARRET_POURENTAGE_STABILISATION};

// \enum Colonnes de la table Molecule
enum table_molecule {MOLECULE_ID,MOLECULE_CODE,MOLECULE_NOM,MOLECULE_FORMULE};
// \enum Colonnes de la table Protocole
enum table_protocole {PROTOCOLE_ID,PROTOCOLE_DESIGNATION};
// \enum Colonnes de la table Marque_Equipement
enum table_marque {MARQUE_ID,MARQUE_DESIGNATION};
// \enum Colonnes de la table Taux_Transmission
enum table_tx_transmission {TX_TRANSMISSION_ID,TX_TRANSMISSION_DESIGNATION};
// \enum Colonnes de la table Modele_Equipement
enum table_modele {MODELE_ID,MODELE_ID_MARQUE,MODELE_ID_PROTOCOLE,MODELE_DESIGNATION};
// \enum Colonnes de la table Equipement
enum table_equipement {EQUIPEMENT_ID,EQUIPEMENT_ID_MODELE,EQUIPEMENT_NO_SERIE,
                       EQUIPEMENT_TYPE,EQUIPEMENT_EN_SERVICE,EQUIPEMENT_MIN_GAMME,
                       EQUIPEMENT_MAX_GAMME,EQUIPEMENT_OFFSET,EQUIPEMENT_ID_TX_TRANSMISSION,
                       EQUIPEMENT_ADRESSE,EQUIPEMENT_NB_BITS_TRANSMISSION, EQUIPEMENT_NB_BITS_STOP,
                       EQUIPEMENT_CONTROLE_FLUX,EQUIPEMENT_PARITE};
// \enum Colonnes de la table Systeme_Etalon
enum table_systeme_etalon {SYS_ETALON_ID,SYS_ETALON_DILUTEUR,SYS_ETALON_BOUTEILLE,SYS_ETALON_GZERO};
// \enum Colonnes de la table Concentration
enum table_concentration {CONCENTRATION_ID,CONCENTRATION_SYS_ETALON,CONCENTRATION_ID_MOLECULE,
                          CONCENTRATION_POINT,CONCENTRATION_REELLE,CONCENTRATION_OZONE};
// \enum Colonnes de la table Polluant_Associe
enum table_polluant_associe {POLLUANT_ASSOCIE_ID,POLLUANT_ASSOCIE_ID_EQUIPEMENT,POLLUANT_ASSOCIE_ID_MOLECULE};
// \enum Colonnes du mod�le PolluantByIdSystemeEtalon
enum modele_polluant_by_systeme_etalon {POLLUANT_BY_SYS_ETALON_ID,POLLUANT_BY_SYS_ETALON_FORMULE};
// \enum Colonnes du TableWidget Polluant de ct_phasewidget
enum phasewidget_tablewidget_polluant {PHASEW_TABLEW_POLLUANTS_IDMOLECULE,PHASEW_TABLEW_POLLUANTS_CODE,
                                       PHASEW_TABLEW_POLLUANTS_FORMULE,PHASEW_TABLEW_POLLUANTS_POINT,
                                       PHASEW_TABLEW_POLLUANTS_REELLE,PHASEW_TABLEW_POLLUANTS_OZONE};

// \struct PhaseConfig Informations de configration de ct_phasewidget
struct PhaseConfig {
    ushort nbCyclesMesures; // Nombre de cycles de mesures � effectuer apr�s le temps de stabilisation
    QTime tempsStabilisation; // Temps de stabilisation pendant lequel aucune mesure n'est effectu�e
    QTime tempsMoyennageMesure; // Temps correspondant � un cycle sur lequel les mesures seront moyenn�es
    QTime tempsAttenteEntreMesure; // Temps d'attente entre chaque cycle de mesure
};

/*///////////////////////////////////////////////////////////////////////////
// \fn extern TypePolluant stringToTypePolluant(QString type)
// \brief Renvoi le TypePolluant contenu dans la chaine en param�tre
//
// \param type Chaine de caract�res repr�sentant le type de polluant
// \return TypePolluant Type de polluant
///////////////////////////////////////////////////////////////////////////*/
extern TypePolluant stringToTypePolluant(QString type);

/*///////////////////////////////////////////////////////////////////////////
// \fn extern QString typePolluantToString(TypePolluant type)
// \brief Renvoi la Chaine de caract�res repr�sentant le type de polluant
//
// \param type Type de polluant
// \return QString Chaine de caract�res repr�sentant le type de polluant
///////////////////////////////////////////////////////////////////////////*/
extern QString typePolluantToString(TypePolluant type);

/*///////////////////////////////////////////////////////////////////////////
// \fn extern TypePolluant stringToTypePolluant(QString type)
// \brief Renvoi le TypePolluant contenu dans la chaine en param�tre
//
// \param type Chaine de caract�res repr�sentant le type de polluant
// \return TypePolluant Type de polluant
///////////////////////////////////////////////////////////////////////////*/
extern TypeTest stringToTypeTest(QString type);

/*///////////////////////////////////////////////////////////////////////////
// \fn extern QString typePolluantToString(TypePolluant type)
// \brief Renvoi la Chaine de caract�res repr�sentant le type de polluant
//
// \param type Type de polluant
// \return QString Chaine de caract�res repr�sentant le type de polluant
///////////////////////////////////////////////////////////////////////////*/
extern QString typeTestToString(TypeTest type);

/*///////////////////////////////////////////////////////////////////////////
// \fn extern Commandes stringToCommande(QString cmd)
// \brief Renvoi la commande contenue dans la chaine en param�tre
//
// \param cmd Chaine de caract�res repr�sentant la commande
// \return Commandes Commande
///////////////////////////////////////////////////////////////////////////*/
extern Commandes stringToCommande(QString cmd);

/*///////////////////////////////////////////////////////////////////////////
// \fn extern QString commandesToString(Commandes cmd)
// \brief Renvoi la chaine en param�tre repr�sentant la commande
//
// \param cmd Commande
// \return Commandes Chaine de caract�res repr�sentant
///////////////////////////////////////////////////////////////////////////*/
extern QString commandesToString(Commandes cmd);

#endif // DEFINITIONS_GLOBALES_H
