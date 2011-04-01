/*////////////////////////////////////////////////////////////
// \file test.h
// \brief Représente un un test
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

#ifndef TEST_H
#define TEST_H

#include "equipement.h"
#include "phase.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Test
// \brief Représente un test
//
// Classe de transition vers le format XML
////////////////////////////////////////////////////////////////////////////////////*/

class Test : public QObject
{
    int m_idTest; // Identifiant du test
    QTime m_tempsMaxTest; // Temps maximum de test au del�  duquel le test est arrêté automatiquement
    ushort m_tempsAcquisition; // Temps d'attente entre deux mesures en secondes.
    QTime m_tempsStabilisation; // Temps de stabilisation pendant lequel aucune mesure n'est effectuée
    QTime m_tempsMoyennageMesure; // Temps correspondant �  un cycle sur lequel les mesures seront moyennées
    ushort m_nbCyclesMesureParPhase; // Nombre de cycles de mesures �  effectuer après le temps de stabilisation
    QList<Equipement> m_listeEquipement; // Liste des périphériques nécessaires au test
    bool m_etalonPresent; // Indique si un étalon a déja été configurer pour ce test
    QVector<Phase> m_listePhases; // Liste des différentes phases du test

public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn Test();
    // \brief Constructeur par défaut
    ///////////////////////////////////////////////////////////////////////////*/
    Test();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn Test();
    // \brief Constructeur de copie
    //
    // \param test Référence vers l'objet Test �  copier
    ///////////////////////////////////////////////////////////////////////////*/
    Test(const Test& test);

////// Accesseurs /////////////

    /*///////////////////////////////////////////////////////////////////////////
    // \fn int getIdTest()
    // \brief Renvoi l'identifiant de test
    //
    // \return int Identifiant de test
    ///////////////////////////////////////////////////////////////////////////*/
    inline int getIdTest() {return this->m_idTest;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime getTpsMaxTest()
    // \brief Renvoi le temps maximum de test
    //
    // \return QTime Temps maximum de test
    ///////////////////////////////////////////////////////////////////////////*/
    inline QTime getTpsMaxTest() {return this->m_tempsMaxTest;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ushort getTpsAcquisition()
    // \brief Renvoi le temps d'attente entre deux mesures en secondes
    //
    // \return ushort Temps d'attente entre deux mesures en secondes
    ///////////////////////////////////////////////////////////////////////////*/
    inline ushort getTpsAcquisition() {return this->m_tempsAcquisition;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime getTpsStabilisation()
    // \brief Renvoi le temps de stabilisation
    //
    // \return QTime Temps de stabilisation
    ///////////////////////////////////////////////////////////////////////////*/
    inline QTime getTpsStabilisation() {return this->m_tempsStabilisation;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime getTpsMoyennageMesure()
    // \brief Renvoi le temps d'un cycle de mesures
    //
    // \return QTime temps d'un cycle de mesures
    ///////////////////////////////////////////////////////////////////////////*/
    inline QTime getTpsMoyennageMesure() {return this->m_tempsMoyennageMesure;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ushort getNbCyclesMesureParPhase()
    // \brief Renvoi le nombre de cycles de mesures �  effectuer après le temps de stabilisation
    //
    // \return ushort Nombre de cycles de mesures �  effectuer après le temps de stabilisation
    ///////////////////////////////////////////////////////////////////////////*/
    inline ushort getNbCyclesMesureParPhase() {return this->m_nbCyclesMesureParPhase;}

////// Mutateurs /////////////

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setIdTest(int const & idTest)
    // \brief Affecte un identifiant au test
    //
    // \param idTest Nouvel identifiant
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setIdTest(int const & idTest) {this->m_idTest = idTest;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTpsMaxTest(QTime const & tpsMaxTest)
    // \brief Ajoute un nouveau temps maximum de durée du test
    //
    // \param tpsMaxTest Nouveau temps maximum de durée du test
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTpsMaxTest(QTime const & tpsMaxTest) {this->m_tempsMaxTest = tpsMaxTest;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTempsAcquisition(ushort const & tpsAcquisition)
    // \brief Ajoute un nouveau temps d'attente entre deux mesures
    //
    // \param tpsAcquisition Nouveau temps d'attente entre deux mesures
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTempsAcquisition(ushort const & tpsAcquisition) {this->m_tempsAcquisition = tpsAcquisition;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTempsStabilisation(QTime const & tpsStabilisation)
    // \brief Ajoute un nouveau temps de stabilisation
    //
    // \param tpsStabilisation Nouveau temps de stabilisation
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTempsStabilisation(QTime const & tpsStabilisation) {this->m_tempsStabilisation = tpsStabilisation;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTempsMoyennageMesure(QTime const & tpsMoyennageMesure)
    // \brief Ajoute un nouveau temps d'un cycle de mesures
    //
    // \param tpsMoyennageMesure Nouveau temps d'un cycle de mesures
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTempsMoyennageMesure(QTime const & tpsMoyennageMesure) {this->m_tempsMoyennageMesure = tpsMoyennageMesure;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setNbCyclesMesureParPhase(ushort const & nbCyclesMesureParPhase)
    // \brief Ajoute un nouveau Nombre de cycles de mesures �  effectuer après le temps de stabilisation
    //
    // \param nbCyclesMesureParPhase Nouveau nombre de cycles de mesures �  effectuer après le temps de stabilisation
    // \param pointGaz Nouveau point de gaz associé
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setNbCyclesMesureParPhase(ushort const & nbCyclesMesureParPhase) {this->m_nbCyclesMesureParPhase = nbCyclesMesureParPhase;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool ajouterEquipement(Equipement peripherique)
    // \brief Ajoute un nouveau périphérique �  la liste des équipements
    //
    // \param peripherique Nouveau périphérique �  ajouter �  la liste des équipements
    // \return Faux si il y a déja un étalon de configuré et que l'on essaie d'en ajouter un autre. Sinon vrai
    ///////////////////////////////////////////////////////////////////////////*/
    bool ajouterEquipement(Equipement peripherique);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void ajouterPhase(Phase newPhase)
    // \brief Ajoute une nouvelle phase au déroulement du test
    //
    // \param newPhase Nouvelle phase �  ajouter au déroulement du test
    ///////////////////////////////////////////////////////////////////////////*/
    void ajouterPhase(Phase newPhase);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void ajouterPhase(ushort const & noPhase, Phase newPhase)
    // \brief Ajoute une nouvelle phase au déroulement du test avec un numéro de phase associé
    //
    // \param noPhase Numéro de phase
    // \param newPhase Nouvelle phase �  ajouter au déroulement du test
    ///////////////////////////////////////////////////////////////////////////*/
    void ajouterPhase(ushort const & noPhase, Phase newPhase);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QDomElement exportToXml(QDomDocument & xmlTest)
    // \brief Exporte les données membres de l'instance de la classe sous forme d'élément XML
    //
    // \param nomFichier Nom du fichier XML servant �  l'export
    // \return bool Faux si erreur. Vrai sinon
    ///////////////////////////////////////////////////////////////////////////*/
    bool exportToXml(QString const & nomFichier);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn static QPointer<Test> importFromXml(QString const & nomFichier);
    // \brief Construit une nouvelle instance de la classe Phase �  partir d'un élément XML
    //
    // \param nomFichier Nom du fichier XML servant �  l'import
    // \return QPointer<Test> Pointeur vers la nouvelle instance de la classe Test
    ///////////////////////////////////////////////////////////////////////////*/
    static QPointer<Test> importFromXml(QString const & nomFichier);
};

#endif // TEST_H
