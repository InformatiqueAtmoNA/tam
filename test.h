/*////////////////////////////////////////////////////////////
// \file test.h
// \brief Repr�sente un un test
// \author FOUQUART Christophe
// \version 1.0
// \date 25/03/2011
//
// TAM - Tests Automatiques M�trologiques
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

#ifndef TEST_H
#define TEST_H

#include "phase.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Test
// \brief Repr�sente un test
//
// Classe de transition vers le format XML
////////////////////////////////////////////////////////////////////////////////////*/

class Test : public QObject
{
    Q_OBJECT
    int m_idTest; // Identifiant du test
    TypeTest m_typeTest;
    ushort m_tempsAcquisition; // Temps d'attente entre deux mesures en secondes.
    QTime m_tempsStabilisation; // Temps de stabilisation pendant lequel aucune mesure n'est effectu�e
    QTime m_tempsMoyennageMesure; // Temps correspondant � un cycle sur lequel les mesures seront moyenn�es
    QTime m_tempsAttenteEntreMesure; // Temps d'attente entre chaque cycle de mesure
    ushort m_nbCyclesMesureParPhase; // Nombre de cycles de mesures � effectuer apr�s le temps de stabilisation
    ushort m_nbCyclesDePhase; // Nombre de cycles de phase
    uint m_idSystemeEtalon; // Id du syst�me d'�talonnage
    QStringList m_polluants;
    QMap<ushort,Phase> m_listePhases; // Liste des diff�rentes phases du test

public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn Test();
    // \brief Constructeur par d�faut
    ///////////////////////////////////////////////////////////////////////////*/
    Test();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn Test();
    // \brief Constructeur de copie
    //
    // \param test R�f�rence vers l'objet Test � copier
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
    // \fn TypeTest getTypeTest()
    // \brief Renvoi le type de test
    //
    // \return TypeTest Type de test
    ///////////////////////////////////////////////////////////////////////////*/
    inline TypeTest getTypeTest() {return this->m_typeTest;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ushort getTempsAcquisition()
    // \brief Renvoi le temps d'attente entre deux mesures en secondes
    //
    // \return ushort Temps d'attente entre deux mesures en secondes
    ///////////////////////////////////////////////////////////////////////////*/
    inline ushort getTempsAcquisition() {return this->m_tempsAcquisition;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime getTempsStabilisation()
    // \brief Renvoi le temps de stabilisation
    //
    // \return QTime Temps de stabilisation
    ///////////////////////////////////////////////////////////////////////////*/
    inline QTime getTempsStabilisation() {return this->m_tempsStabilisation;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime getTempsMoyennageMesure()
    // \brief Renvoi le temps d'un cycle de mesures
    //
    // \return QTime temps d'un cycle de mesures
    ///////////////////////////////////////////////////////////////////////////*/
    inline QTime getTempsMoyennageMesure() {return this->m_tempsMoyennageMesure;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QTime getTempsAttenteEntreMesure()
    // \brief Renvoi le temps d'attente entre chaque cycle de mesures
    //
    // \return QTime temps d'un cycle de mesures
    ///////////////////////////////////////////////////////////////////////////*/
    inline QTime getTempsAttenteEntreMesure() {return this->m_tempsAttenteEntreMesure;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ushort getNbCyclesMesureParPhase()
    // \brief Renvoi le nombre de cycles de mesures � effectuer apr�s le temps de stabilisation
    //
    // \return ushort Nombre de cycles de mesures � effectuer apr�s le temps de stabilisation
    ///////////////////////////////////////////////////////////////////////////*/
    inline ushort getNbCyclesMesureParPhase() {return this->m_nbCyclesMesureParPhase;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ushort getNbCyclesDePhases()
    // \brief Renvoi le nombre de cycles de phases
    //
    // \return ushort Nombre de cycles de phases
    ///////////////////////////////////////////////////////////////////////////*/
    inline ushort getNbCyclesDePhases() {return this->m_nbCyclesDePhase;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn uint getIdSystemeEtalon()
    // \brief Renvoi l'identifiant du systeme �talon
    //
    // \return ushort Identifiant du systeme �talon
    ///////////////////////////////////////////////////////////////////////////*/
    inline uint getIdSystemeEtalon() {return this->m_idSystemeEtalon;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ushort getNbPhases()
    // \brief Renvoi le nombre de phases
    //
    // \return ushort Nombre de phases
    ///////////////////////////////////////////////////////////////////////////*/
    inline ushort getNbPhases() {return this->m_listePhases.count();}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QStringList getPolluants()
    // \brief Renvoi la liste des polluants
    //
    // \return QStringList Liste des polluants
    ///////////////////////////////////////////////////////////////////////////*/
    inline QStringList getPolluants() {return this->m_polluants;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn Phase getPhase(const ushort noPhase)
    // \brief Renvoi la phase correspondant au num�ro de phase pass� en param�tre
    //
    // \param noPhase Num�ro de la phase
    // \return Phase Phase correspondant
    ///////////////////////////////////////////////////////////////////////////*/
    inline Phase getPhase(const ushort noPhase) {return this->m_listePhases.value(noPhase);}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool isListePhasesEmpty()
    // \brief Teste si la liste de phases est vide
    //
    // \return bool Vrai si la liste de phases est vide. Faux sinon
    ///////////////////////////////////////////////////////////////////////////*/
    inline bool isListePhasesEmpty() {return this->m_listePhases.isEmpty();}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool phaseExiste(const ushort noPhase)
    // \brief Teste si une phase num�ro noPhase existe
    //
    // \param noPhase Num�ro de la phase
    // \return bool Vrai si la phase existe, faux sinon
    ///////////////////////////////////////////////////////////////////////////*/
    inline bool phaseExiste(const ushort noPhase) {return this->m_listePhases.contains(noPhase);}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool isTempsMaxTestDefined()
    // \brief Teste si un temps maximum de test a �t� d�fini
    //
    // \return bool Vrai si tempsMaxTest d�fini. Faux sinon
    ///////////////////////////////////////////////////////////////////////////*/
    bool isTempsMaxTestDefined();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QMap<ushort,QTime> getListeTempsMaxPhase()
    // \brief Renvoie la liste des temps max de phase d�finis pour chaque phase
    //
    // \return QMap<ushort,QTime> Liste des temps max de phase d�finis pour chaque phase
    ///////////////////////////////////////////////////////////////////////////*/
    QMap<ushort,QTime> getListeTempsMaxPhase();

    /*//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // \fn QVector<ushort> getListeCommandeDebutPhaseDefinies()
    // \brief Renvoie la liste des num�ros de phases pour lesquelles une commande
    //        de d�but de phase est d�finie
    //
    // \return QVector<ushort> des num�ros de phases pour lesquelles une commande de d�but de phase est d�finie
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
    QVector<ushort> getListeCommandeDebutPhaseDefinies();

    /*//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // \fn QVector<ushort> getListeCritereArretDefinis()
    // \brief Renvoie la liste des num�ros de phases pour lesquelles un crit�re d'arr�t est d�fini
    //
    // \return QVector<ushort> des num�ros de phases pour lesquelles un crit�re d'arr�t est d�fini
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
    QVector<ushort> getListeCritereArretDefinis();

////// Mutateurs /////////////

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setIdTest(int const & idTest)
    // \brief Affecte un identifiant au test
    //
    // \param idTest Nouvel identifiant
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setIdTest(const int idTest) {this->m_idTest = idTest;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTypeTest(const int typeTest)
    // \brief Affecte un type au test
    //
    // \param typeTest Nouvel type de test
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTypeTest(const int typeTest) {this->m_typeTest = (TypeTest)typeTest;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTypeTest(const TypeTest typeTest)
    // \brief Affecte un type au test
    //
    // \param typeTest Nouvel type de test
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTypeTest(const TypeTest typeTest) {this->m_typeTest = typeTest;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTypeTest(const QString & typeTest)
    // \brief Affecte un type au test
    //
    // \param typeTest Nouvel type de test
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTypeTest(const QString & typeTest) {this->m_typeTest = stringToTypeTest(typeTest);}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTempsAcquisition(ushort const & tpsAcquisition)
    // \brief Ajoute un nouveau temps d'attente entre deux mesures
    //
    // \param tpsAcquisition Nouveau temps d'attente entre deux mesures
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTempsAcquisition(const ushort & tpsAcquisition) {this->m_tempsAcquisition = tpsAcquisition;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTempsStabilisation(QTime const & tpsStabilisation)
    // \brief Ajoute un nouveau temps de stabilisation
    //
    // \param tpsStabilisation Nouveau temps de stabilisation
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTempsStabilisation(const QTime & tpsStabilisation) {this->m_tempsStabilisation = tpsStabilisation;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTempsMoyennageMesure(QTime const & tpsMoyennageMesure)
    // \brief Ajoute un nouveau temps d'un cycle de mesures
    //
    // \param tpsMoyennageMesure Nouveau temps d'un cycle de mesures
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTempsMoyennageMesure(const QTime & tpsMoyennageMesure) {this->m_tempsMoyennageMesure = tpsMoyennageMesure;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTempsMoyennageMesure(QTime const & tpsMoyennageMesure)
    // \brief Ajoute un nouveau temps d'un cycle de mesures
    //
    // \param tpsMoyennageMesure Nouveau temps d'attente entre chaque cycle de mesures
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTempsAttenteEntreMesure(const QTime & tpsAttenteEntreMesure) {this->m_tempsAttenteEntreMesure = tpsAttenteEntreMesure;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setNbCyclesMesureParPhase(ushort const & nbCyclesMesureParPhase)
    // \brief Ajoute un nouveau Nombre de cycles de mesures � effectuer apr�s le temps de stabilisation
    //
    // \param nbCyclesMesureParPhase Nouveau nombre de cycles de mesures � effectuer apr�s le temps de stabilisation
    // \param pointGaz Nouveau point de gaz associ�
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setNbCyclesMesureParPhase(const ushort nbCyclesMesureParPhase) {this->m_nbCyclesMesureParPhase = nbCyclesMesureParPhase;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setNbCyclesMesureParPhase(ushort const & nbCyclesMesureParPhase)
    // \brief Ajoute un nouveau Nombre de cycles de mesures � effectuer apr�s le temps de stabilisation
    //
    // \param nbCyclesDePhase Nouveau nombre de cycles de phases
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setNbCyclesDePhase(const ushort nbCyclesDePhase) {this->m_nbCyclesDePhase = nbCyclesDePhase;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setSystemeEtalon(const uint idSystemeEtalon)
    // \brief Ajoute un identifiant de syst�me d'�talonnage
    //
    // \param idSystemeEtalon Identifiant du syst�me d'�talonnage
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setSystemeEtalon(const uint idSystemeEtalon) {this->m_idSystemeEtalon = idSystemeEtalon; }

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void ajouterPolluant(const TypePolluant polluant)
    // \brief Ajoute un polluant � la liste de polluants du test
    //
    // \param polluant Polluant � ajouter � la liste de polluants du test
    ///////////////////////////////////////////////////////////////////////////*/
    inline void ajouterPolluant(const uint molecule) {this->m_polluants.append(QString::number(molecule)); }

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void ajouterPolluant(const QString polluant)
    // \brief Ajoute un polluant � la liste de polluants du test
    //
    // \param polluant Polluant � ajouter � la liste de polluants du test
    ///////////////////////////////////////////////////////////////////////////*/
    inline void ajouterPolluant(const QString polluant) {this->m_polluants.append(polluant); }

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void ajouterPhase(const Phase newPhase)
    // \brief Ajoute une nouvelle phase au d�roulement du test
    //
    // \param newPhase Nouvelle phase � ajouter au d�roulement du test
    ///////////////////////////////////////////////////////////////////////////*/
    inline void viderListePhase() {this->m_listePhases.clear();}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void ajouterPhase(const Phase newPhase)
    // \brief Ajoute une nouvelle phase au d�roulement du test
    //
    // \param newPhase Nouvelle phase � ajouter au d�roulement du test
    ///////////////////////////////////////////////////////////////////////////*/
    void ajouterPhase(const Phase & newPhase);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void ajouterPhase(ushort const & noPhase, Phase newPhase)
    // \brief Ajoute une nouvelle phase au d�roulement du test avec un num�ro de phase associ�
    //
    // \param noPhase Num�ro de phase
    // \param newPhase Nouvelle phase � ajouter au d�roulement du test
    ///////////////////////////////////////////////////////////////////////////*/
    void ajouterPhase(const ushort noPhase, Phase & newPhase);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void supprimerPhase(const ushort & noPhase)
    // \brief Supprime la phase correspondant au num�ro en param�tre
    //
    // \param noPhase Num�ro de phase � supprimer
    ///////////////////////////////////////////////////////////////////////////*/
    void supprimerPhase(const ushort noPhase);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void phaseInseree(ushort noPhase)
    // \brief D�cale les num�ros de phase � partir de celui fourni en param�tre
    //
    // \param noPhase Num�ro de la phase qui sera ins�r�e
    ///////////////////////////////////////////////////////////////////////////*/
    void phaseInseree(const ushort noPhase);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn Test& operator=(const Test&)
    // \brief Surcharge de l'op�rateur =
    //
    // \param Test R�f�rence sur l'objet Test � copier
    // \return Test& R�f�rence sur le nouvel objet Test
    ///////////////////////////////////////////////////////////////////////////*/
    Test& operator=(const Test&);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QDomElement exportToXml(QDomDocument & xmlTest)
    // \brief Exporte les donn�es membres de l'instance de la classe sous forme d'�l�ment XML
    //
    // \param nomFichier Nom du fichier XML servant � l'export
    // \return bool Faux si erreur. Vrai sinon
    ///////////////////////////////////////////////////////////////////////////*/
    bool exportToXml(const QString & nomFichier);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn static QPointer<Test> importFromXml(QString const & nomFichier);
    // \brief Construit une nouvelle instance de la classe Phase � partir d'un �l�ment XML
    //
    // \param nomFichier Nom du fichier XML servant � l'import
    // \return QPointer<Test> Pointeur vers la nouvelle instance de la classe Test
    ///////////////////////////////////////////////////////////////////////////*/
    static QPointer<Test> importFromXml(const QString & nomFichier);

};

#endif // TEST_H
