/*////////////////////////////////////////////////////////////
// \file Phase.h
// \brief Représente une phase lors du déroulement d'un test
// \author FOUQUART Christophe
// \version 1.0
// \date 25/03/2011
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
    ushort m_nbCyclesMesures; // Nombre de cycles de mesures à effectuer après le temps de stabilisation
    QTime m_tempsStabilisation; // Temps de stabilisation pendant lequel aucune mesure n'est effectuée
    QTime m_tempsMoyennageMesure; // Temps correspondant à un cycle sur lequel les mesures seront moyennées
    QTime m_tempsAttenteEntreMesure; // Temps d'attente entre chaque cycle de mesure
    bool m_critereArretPrevu; // Indique si un critère d'arrêt est prévu
    QVector<ushort> m_criteresArret; // Critère d'arrêt : voir enum dans definitions_globales.h
    Commandes m_commandeDebutPhase; // Commande optionnelle à effectuer à la fin du temps de stabilisation
    QMap<ushort,uint> m_listePolluants; // Liste des polluants et de leurs points de gaz associés

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
    // \param phase Référence vers l'objet Phase à copier
    ///////////////////////////////////////////////////////////////////////////*/
    Phase(const Phase & phase);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool isEmpty() const
    // \brief Teste si la phase est vide
    //
    // \return bool Vrai si le numéro de phase est à 0. Faux sinon
    ///////////////////////////////////////////////////////////////////////////*/
    inline bool isEmpty() const {return (this->m_noPhase==0);}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool polluantPresent(ushort idMolecule) const
    // \brief Renvoi si un polluant est déjà programé
    //
    // \param idMolecule Id de la table Molecule
    // \return bool Vrai si le polluant est déjà programé. Faux sinon
    ///////////////////////////////////////////////////////////////////////////*/
    inline bool polluantPresent(ushort idMolecule) const {return m_listePolluants.contains(idMolecule);}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ushort getNoPhase()
    // \brief Renvoi le numéro de phase
    //
    // \return ushort Numéro de phase
    ///////////////////////////////////////////////////////////////////////////*/
    inline ushort getNoPhase() const {return this->m_noPhase;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime const getTpsMaxPhase() const
    // \brief Renvoi le temps maximum de durée de la phase
    //
    // \return QTime Temps maximum de durée de la phase
    ///////////////////////////////////////////////////////////////////////////*/
    inline QTime getTpsMaxPhase() const {return this->m_tempsMaxPhase;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime const getNbCyclesMesures() const
    // \brief Renvoi le nombre de cycles de mesures avant la fin de la phase
    //
    // \return ushort Nombre de cycles de mesures avant la fin de la phase
    ///////////////////////////////////////////////////////////////////////////*/
    inline ushort getNbCyclesMesures() const {return this->m_nbCyclesMesures;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ushort getCritereArret_NbCyclesMesures() const
    // \brief Renvoi le nombre de cycles de mesures sur lequel s'applique le critère d'arrêt
    //
    // \return ushort Nombre de cycles de mesures sur lequel s'applique le critère d'arrêt
    ///////////////////////////////////////////////////////////////////////////*/
    inline ushort getCritereArret_NbCyclesMesures() const {return this->m_criteresArret[CRITERE_ARRET_NB_CYCLES_MESURES];}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ushort getCritereArret_PourcentageStabilisation() const
    // \brief Renvoi le pourcentage de variation en dessous duquel l'arrêt s'effectue
    //
    // \return ushort pourcentage de variation en dessous duquel l'arrêt s'effectue
    ///////////////////////////////////////////////////////////////////////////*/
    inline ushort getCritereArret_PourcentageStabilisation() const {return this->m_criteresArret[CRITERE_ARRET_POURENTAGE_STABILISATION];}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool critereArretExiste() const
    // \brief Teste si un critère d'arrêt est programmé
    //
    // \return bool Vrai si critère d'arrêt programmé, Faux sinon
    ///////////////////////////////////////////////////////////////////////////*/
    inline bool critereArretExiste() const {return m_critereArretPrevu;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime const getTpsStabilisation() const
    // \brief Renvoi le temps de stabilisation
    //
    // \return QTime Temps de stabilisation
    ///////////////////////////////////////////////////////////////////////////*/
    inline QTime getTpsStabilisation() const {return this->m_tempsStabilisation;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime const getTpsMoyennageMesure() const
    // \brief Renvoi le temps de moyennage des mesures pour un cycle
    //
    // \return QTime Temps de moyennage des mesures pour un cycle
    ///////////////////////////////////////////////////////////////////////////*/
    inline QTime getTpsMoyennageMesure() const {return this->m_tempsMoyennageMesure;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime getTpsAttenteEntreMesure() const
    // \brief Renvoi le temps d'attente entre chaque cycle de mesures
    //
    // \return QTime temps d'un cycle de mesures
    ///////////////////////////////////////////////////////////////////////////*/
    inline QTime getTpsAttenteEntreMesure() const {return this->m_tempsAttenteEntreMesure;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime const getCmdDebutPhase() const
    // \brief Renvoi la commande optionnelle de fin de tamps de stabilisation
    //
    // \return Commandes Commande optionnelle de fin de tamps de stabilisation
    ///////////////////////////////////////////////////////////////////////////*/
    inline Commandes getCmdDebutPhase() const {return this->m_commandeDebutPhase;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime const getListePolluants() const
    // \brief Renvoi la liste des identifiants de la table Concentration de la phase
    //
    // \return QMap<ushort,uint> Liste des polluants et de leurs points de gaz associés
    ///////////////////////////////////////////////////////////////////////////*/
    inline QMap<ushort,uint> getListePolluants() const {return m_listePolluants;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setNoPhase(ushort const & numPhase)
    // \brief Affecte un nouveau numéro de phase
    //
    // \param numPhase Nouveau numéro de phase
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setNoPhase(const ushort & numPhase) {this->m_noPhase = numPhase;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTpsMaxPhase(QTime const & tempsMaxPhase)
    // \brief Affecte un nouveau temps maximum de durée de la phase
    //
    // \param tempsMaxPhase Nouveau temps maximum de durée de la phase
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTpsMaxPhase(const QTime & tempsMaxPhase) {this->m_tempsMaxPhase = tempsMaxPhase;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setNbCyclesMesures(ushort const & nbCyclesMesures)
    // \brief Affecte un nouveau nombre de cycles de mesures avant la fin de la phase
    //
    // \param nbCyclesMesures Nouveau nombre de cycles de mesures avant la fin de la phase
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setNbCyclesMesures(const ushort & nbCyclesMesures) {this->m_nbCyclesMesures = nbCyclesMesures;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTpsStabilisation(QTime const & tempsStabilisation)
    // \brief Affecte un nouveau temps de stabilisation
    //
    // \param tempsStabilisation Nouveau temps de stabilisation
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTpsStabilisation(const QTime & tempsStabilisation) {this->m_tempsStabilisation = tempsStabilisation;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTpsMoyennageMesure(QTime const & tempsMoyennageMesure)
    // \brief Affecte un nouveau temps de moyennage des mesures
    //
    // \param tempsMoyennageMesure Nouveau temps de moyennage des mesures
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTpsMoyennageMesure(const QTime & tempsMoyennageMesure) {this->m_tempsMoyennageMesure = tempsMoyennageMesure;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTempsMoyennageMesure(QTime const & tpsMoyennageMesure)
    // \brief Ajoute un nouveau temps d'un cycle de mesures
    //
    // \param tpsMoyennageMesure Nouveau temps d'attente entre chaque cycle de mesures
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTpsAttenteEntreMesure(const QTime & tpsAttenteEntreMesure) {this->m_tempsAttenteEntreMesure = tpsAttenteEntreMesure;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setCmdFinTs(Commandes const & commandeFinTS)
    // \brief Affecte une nouvelle commande fin de temps de stabilisation
    //
    // \param commandeFinTS Nouvelle commande fin de temps de stabilisation
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setCmdFinTs(const Commandes & commandeFinTS) {this->m_commandeDebutPhase = commandeFinTS;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void ajouterPolluant(uint idConcentration)
    // \brief Ajoute un nouveau polluant à la phase grâce à son identifant de la table Concentration
    //
    // \param polluant Nouveau polluant
    // \param pointGaz Nouveau point de gaz associé
    ///////////////////////////////////////////////////////////////////////////*/
    inline void ajouterPolluant(const uint polluant, const uint idConcentration) {this->m_listePolluants.insert(polluant,idConcentration);}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void supprimerPolluant(const uint polluant)
    // \brief Supprime un nouveau polluant de la phase
    //
    // \param polluant Polluant à supprimer
    ///////////////////////////////////////////////////////////////////////////*/
    inline void supprimerPolluant(const uint polluant) {this->m_listePolluants.remove(polluant);}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setCritereArretPrevu(bool critereArretPrevu)
    // \brief Ajoute un critère d'arrêt de la phase
    //
    // \param critereArretPrevu Indique si un critère d'arrêt est spécifié
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setCritereArretPrevu(bool critereArretPrevu) {this->m_critereArretPrevu=critereArretPrevu;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setCritereArret_NbCyclesMesures(const ushort value) const
    // \brief Initialise le nombre de cycles de mesures sur lequel s'applique le critère d'arrêt
    //
    // \param value Nombre de cycles de mesures sur lequel s'applique le critère d'arrêt
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setCritereArret_NbCyclesMesures(const ushort value) {this->m_criteresArret.replace(CRITERE_ARRET_NB_CYCLES_MESURES,value);}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void getCritereArret_PourcentageStabilisation(const ushort value) const
    // \brief Initialise le pourcentage de variation en dessous duquel l'arrêt s'effectue
    //
    // \return value Pourcentage de variation en dessous duquel l'arrêt s'effectue
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setCritereArret_PourcentageStabilisation(const ushort value) {this->m_criteresArret.replace(CRITERE_ARRET_POURENTAGE_STABILISATION,value);}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn Phase& operator=(const Phase&)
    // \brief Surcharge de l'opérateur =
    //
    // \param Phase Référence sur l'objet Phase à copier
    // \return Phase& Référence sur le nouvel objet Phase
    ///////////////////////////////////////////////////////////////////////////*/
    Phase& operator=(const Phase&);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QDomElement exportToXml(QDomDocument & xmlTest)
    // \brief Exporte les données membres de l'instance de la classe sous forme d'élément XML
    //
    // \param xmlTest Document XML servant à l'export
    // \return QDomElement Elément XML contenant les données membres de la classe
    ///////////////////////////////////////////////////////////////////////////*/
    QDomElement exportToXml(QDomDocument & xmlTest);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn static QPointer<Phase> importFromXml(QDomElement const & domPhase)
    // \brief Construit une nouvelle instance de la classe Phase à partir d'un élément XML
    //
    // \param domPhase Elément XML représentant une phase
    // \return QPointer<Phase> Pointeur vers la nouvelle instance de la classe Phase
    ///////////////////////////////////////////////////////////////////////////*/
    static QPointer<Phase> importFromXml(const QDomElement & domPhase);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn static QPointer<Phase> getPhaseFromConf(const PhaseConfig & config)
    // \brief Construit une nouvelle instance de la classe Phase à partir d'une structure de configuration
    //
    // \param config Configuration de la phase : voir definition_globales.h
    // \return QPointer<Phase> Pointeur vers la nouvelle instance de la classe Phase
    ///////////////////////////////////////////////////////////////////////////*/
    static QPointer<Phase> getPhaseFromConf(const PhaseConfig & config);
};

#endif // PHASE_H

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QDebug operator<<(QDebug dbg, const Phase & Phase)
    // \brief Redéfinition de l'opérateur de flux pour l'affichage en mode debug
    //
    // \param dbg Flux de sortie
    // \param phase Référence vers l'instance de la classe Phase à afficher
    // \return QDebug Flux de sortie
    ///////////////////////////////////////////////////////////////////////////*/
    extern QDebug operator<<(QDebug dbg, const Phase & phase);
