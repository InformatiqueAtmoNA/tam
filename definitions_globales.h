/*////////////////////////////////////////////////////////////
// \file definitions_globale.h
// \brief Macros, énumérations et fonctions globales au programme
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

#include <QDebug>
#include <QStringList>
#include <QtXml>
#include <QTime>
#include <QTimer>
#include <QThread>
#include <QtCore>
#include <QVector>
#include <QFile>
#include <QMapIterator>

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
// \def SH_CANAL Index du canal dans les donn�es membres de la classe SpanHandler
#define SH_CANAL  0
// \def SH_POINT Index du point dans les donn�es membres de la classe SpanHandler
#define SH_POINT  1
// \def SH_CONCO3 Index de la concentration d'O3 dans les donn�es membres de la classe SpanHandler
#define SH_CONCO3 2

// \enum TypePeripherique Type d'appareil
enum TypePeripherique {ANALYSEUR,ETALON};
// \enum TypePolluant Polluant associé �  l'appareil
enum TypePolluant {CO,O3,SO2,NO,NO2,NOX,DIL};
// \enum OptionTpg Option d'étalonnage permettant la TPG
enum OptionTpg {AUCUNE,OZONISEUR,PHOTOMETRE};
// \enum Protocoles Protocoles de communication supportés
// A MODIFIER LORS DE L'AJOUT DE NOUVEAUX PROTOCOLES
enum Protocoles {MODE4_ANA,MODE4_SX6000_17,MODE4_SX3022,TEI_ANA,TEI_146i,TEI_146c,TEI_49ps,API_ANA,API_DIL};
// \enum Commandes Listes des commandes
enum Commandes {OFFSET,MESURES,ALARME,DATE_HEURE,MODE_ZERO,MODE_ETALON,MODE_MESURE,
                SPAN,SPAN_ZERO,SPAN_TPG,SPAN_O3,RESET,STAND_BY,EVENT,NO_CMD};
// \enum Arguments de commande de span
enum SpanArguments {CANAL,POINT,CONCO3};

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

#endif // DEFINITIONS_GLOBALES_H
