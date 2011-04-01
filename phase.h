/*////////////////////////////////////////////////////////////
// \file Phase.h
// \brief Représente une phase lors du déroulement d'un test
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

#ifndef PHASE_H
#define PHASE_H

#include "definitions_globales.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Phase
// \brief Représente une phase lors du déroulement d'un test
//
// Classe de transition vers le format XML
////////////////////////////////////////////////////////////////////////////////////*/

class Phase : public QObject
{
    ushort m_noPhase; // Numéro de la phase
    QTime m_tempsMaxPhase; // Temps maximum de la phase
    ushort m_nbCyclesMesures; // Nombre de cycles de mesures �  effectuer après le temps de stabilisation
    QTime m_tempsStabilisation; // Temps de stabilisation pendant lequel aucune mesure n'est effectuée
    QTime m_tempsMoyennageMesure; // Temps correspondant �  un cycle sur lequel les mesures seront moyennées
    QVector<ushort> m_criteresArret; // Critère d'arrêt : m_criteresArret[0]=m_nbCyclesMesures,  m_criteresArret[1]=pourcentage de stabilisation
    Commandes m_commandeFinTs; // Commande optionnelle �  effectuer �  la fin du temps de stabilisation
    QMap<TypePolluant,ushort> m_listePolluants; // Liste des polluants et de leurs points de gaz associés

public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn Phase();
    // \brief Constructeur par défaut
    ///////////////////////////////////////////////////////////////////////////*/
    Phase();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn Phase();
    // \brief Constructeur de copie
    //
    // \param phase Référence vers l'objet Phase �  copier
    ///////////////////////////////////////////////////////////////////////////*/
    Phase(Phase const & phase);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ushort getNoPhase()
    // \brief Renvoi le numéro de phase
    //
    // \return ushort Numéro de phase
    ///////////////////////////////////////////////////////////////////////////*/
    inline ushort getNoPhase() {return this->m_noPhase;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime const getTpsMaxPhase()
    // \brief Renvoi le temps maximum de durée de la phase
    //
    // \return QTime Temps maximum de durée de la phase
    ///////////////////////////////////////////////////////////////////////////*/
    inline QTime getTpsMaxPhase() {return this->m_tempsMaxPhase;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime const getNbCyclesMesures()
    // \brief Renvoi le nombre de cycles de mesures avant la fin de la phase
    //
    // \return ushort Nombre de cycles de mesures avant la fin de la phase
    ///////////////////////////////////////////////////////////////////////////*/
    inline ushort getNbCyclesMesures() {return this->m_nbCyclesMesures;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime const getTpsStabilisation()
    // \brief Renvoi le temps de stabilisation
    //
    // \return QTime Temps de stabilisation
    ///////////////////////////////////////////////////////////////////////////*/
    inline QTime getTpsStabilisation() {return this->m_tempsStabilisation;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime const getTpsMoyennageMesure()
    // \brief Renvoi le temps de moyennage des mesures pour un cycle
    //
    // \return QTime Temps de moyennage des mesures pour un cycle
    ///////////////////////////////////////////////////////////////////////////*/
    inline QTime getTpsMoyennageMesure() {return this->m_tempsMoyennageMesure;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime const getCmdFinTs()
    // \brief Renvoi la commande optionnelle de fin de tamps de stabilisation
    //
    // \return Commandes Commande optionnelle de fin de tamps de stabilisation
    ///////////////////////////////////////////////////////////////////////////*/
    inline Commandes getCmdFinTs() {return this->m_commandeFinTs;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime const getListePolluants()
    // \brief Renvoi la liste des polluants et de leurs points de gaz associés
    //
    // \return QMap<TypePolluant,ushort> Liste des polluants et de leurs points de gaz associés
    ///////////////////////////////////////////////////////////////////////////*/
    inline QMap<TypePolluant,ushort> getListePolluants() {return m_listePolluants;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setNoPhase(ushort const & numPhase)
    // \brief Affecte un nouveau numéro de phase
    //
    // \param numPhase Nouveau numéro de phase
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setNoPhase(ushort const & numPhase) {this->m_noPhase = numPhase;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTpsMaxPhase(QTime const & tempsMaxPhase)
    // \brief Affecte un nouveau temps maximum de durée de la phase
    //
    // \param tempsMaxPhase Nouveau temps maximum de durée de la phase
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTpsMaxPhase(QTime const & tempsMaxPhase) {this->m_tempsMaxPhase = tempsMaxPhase;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setNbCyclesMesures(ushort const & nbCyclesMesures)
    // \brief Affecte un nouveau nombre de cycles de mesures avant la fin de la phase
    //
    // \param nbCyclesMesures Nouveau nombre de cycles de mesures avant la fin de la phase
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setNbCyclesMesures(ushort const & nbCyclesMesures) {this->m_nbCyclesMesures = nbCyclesMesures;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTpsStabilisation(QTime const & tempsStabilisation)
    // \brief Affecte un nouveau temps de stabilisation
    //
    // \param tempsStabilisation Nouveau temps de stabilisation
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTpsStabilisation(QTime const & tempsStabilisation) {this->m_tempsStabilisation = tempsStabilisation;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTpsMoyennageMesure(QTime const & tempsMoyennageMesure)
    // \brief Affecte un nouveau temps de moyennage des mesures
    //
    // \param tempsMoyennageMesure Nouveau temps de moyennage des mesures
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTpsMoyennageMesure(QTime const & tempsMoyennageMesure) {this->m_tempsMoyennageMesure = tempsMoyennageMesure;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setCmdFinTs(Commandes const & commandeFinTS)
    // \brief Affecte une nouvelle commande fin de temps de stabilisation
    //
    // \param commandeFinTS Nouvelle commande fin de temps de stabilisation
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setCmdFinTs(Commandes const & commandeFinTS) {this->m_commandeFinTs = commandeFinTS;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void ajouterPolluant(TypePolluant const & polluant, ushort pointGaz)
    // \brief Ajoute un nouveau polluant �  la phase avec son point de gaz associé
    //
    // \param polluant Nouveau polluant
    // \param pointGaz Nouveau point de gaz associé
    ///////////////////////////////////////////////////////////////////////////*/
    inline void ajouterPolluant(TypePolluant const & polluant, ushort pointGaz) {this->m_listePolluants.insert(polluant,pointGaz);}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void ajouterCritereArret(ushort const & nbCycleMesure, ushort const & pourcentageStabilisation)
    // \brief Ajoute un critère d'arrêt de la phase
    //
    // \param nbCycleMesure Nombre cycles de mesures pendant lequel on vérifiera la stabilité
    // \param pourcentageStabilisation Pourcentage de stabilité provoquant l'arrêt de la phase
    ///////////////////////////////////////////////////////////////////////////*/
    void ajouterCritereArret(ushort const & nbCycleMesure, ushort const & pourcentageStabilisation);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn Phase& operator=(const Phase&)
    // \brief Surcharge de l'opérateur =
    //
    // \param Phase Référence sur l'objet Phase �  copier
    // \return Phase& Référence sur le nouvel objet Phase
    ///////////////////////////////////////////////////////////////////////////*/
    Phase& operator=(const Phase&);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QDomElement exportToXml(QDomDocument & xmlTest)
    // \brief Exporte les données membres de l'instance de la classe sous forme d'élément XML
    //
    // \param xmlTest Document XML servant �  l'export
    // \return QDomElement Elément XML contenant les données membres de la classe
    ///////////////////////////////////////////////////////////////////////////*/
    QDomElement exportToXml(QDomDocument & xmlTest);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn static QPointer<Phase> importFromXml(QDomElement const & domPhase)
    // \brief Construit une nouvelle instance de la classe Phase �  partir d'un élément XML
    //
    // \param domPhase Elément XML représentant une phase
    // \return QPointer<Phase> Pointeur vers la nouvelle instance de la classe Phase
    ///////////////////////////////////////////////////////////////////////////*/
    static QPointer<Phase> importFromXml(QDomElement const & domPhase);
};

#endif // PHASE_H
