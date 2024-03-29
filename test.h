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
    double m_critere1; // critere de performance utilis� pour la g�n�ration des rapports
    double m_critere2; // critere de performance utilis� pour la g�n�ration des rapports
    double m_critere3; // critere de performance utilis� pour la g�n�ration des rapports
    double m_critere_temp_min; // critere de performance utilis� pour la g�n�ration des rapports
    double m_critere_temp_max; // critere de performance utilis� pour la g�n�ration des rapports
    double m_critere_variation; // critere de performance utilis� pour la g�n�ration des rapports

    float m_temp_min=0;
    float m_temp_max=0;
    float m_temp_moyenne=0;

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
    // \fn double getCritere1()
    // \brief Renvoi le critere de performance 1
    //
    // \return double critere de performance 1
    ///////////////////////////////////////////////////////////////////////////*/
    inline double getCritere1() {return this->m_critere1;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn double getCritere2()
    // \brief Renvoi le critere de performance 2
    //
    // \return double critere de performance 2
    ///////////////////////////////////////////////////////////////////////////*/
    inline double getCritere2() {return this->m_critere2;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn double getCritere2()
    // \brief Renvoi le critere de performance 3
    //
    // \return double critere de performance 3
    ///////////////////////////////////////////////////////////////////////////*/
    inline double getCritere3() {return this->m_critere3;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn double getCritere2()
    // \brief Renvoi le critere de temp�rature minimum
    //
    // \return double critere de temp�rature minimum
    ///////////////////////////////////////////////////////////////////////////*/
    inline double getCritere_Temp_min() {return this->m_critere_temp_min;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn double getCritere2()
    // \brief Renvoi le critere de temp�rature maximum
    //
    // \return double critere de temp�rature maximum
    ///////////////////////////////////////////////////////////////////////////*/
    inline double getCritere_Temp_max() {return this->m_critere_temp_max;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn double getCritere2()
    // \brief Renvoi le critere de variation de temp�rature
    //
    // \return double critere de variation de temp�rature
    ///////////////////////////////////////////////////////////////////////////*/
    inline double getCritere_Variation() {return this->m_critere_variation;}


    /*///////////////////////////////////////////////////////////////////////////
    // \fn float getTempMin()
    // \brief Renvoi la temp�rature minimum
    //
    // \return float temp�rature minimum
    ///////////////////////////////////////////////////////////////////////////*/
    inline float getTempMin() {return this->m_temp_min;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn float getTempMax()
    // \brief Renvoi la temp�rature maximum
    //
    // \return float temp�rature maximum
    ///////////////////////////////////////////////////////////////////////////*/
    inline float getTempMax() {return this->m_temp_max;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn float getTempMoyenne()
    // \brief Renvoi la temp�rature moyenne
    //
    // \return float temp�rature moyenne
    ///////////////////////////////////////////////////////////////////////////*/
    inline float getTempMoyenne() {return this->m_temp_moyenne;}

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
    // \fn void setCritere1(const double & critere1)
    // \brief Ajoute un critere de performance
    //
    // \param Critere de performance 1
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setCritere1(const double & critere1) {this->m_critere1 = critere1;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setCritere2(const double & critere2)
    // \brief Ajoute un critere de performance
    //
    // \param Critere de performance 2
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setCritere2(const float & critere2) {this->m_critere2 = critere2;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setCritere3(const double & criter3)
    // \brief Ajoute un critere de performance
    //
    // \param Critere de performance 3
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setCritere3(const double & critere3) {this->m_critere3 = critere3;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setCritere_Temp_min(const double & critere_temp_min)
    // \brief Ajoute un critere de temp�rature minimum
    //
    // \param critere de temp�rature minimum
    ///////////////////////////////////////////////////////////////////////////*/
    inline double setCritere_Temp_min(const double & critere_temp_min) {this->m_critere_temp_min  = critere_temp_min;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setCritere_Temp_max(const double & critere_temp_max)
    // \brief Ajoute un critere de temp�rature maximum
    //
    // \param critere de temp�rature maximum
    ///////////////////////////////////////////////////////////////////////////*/
    inline double setCritere_Temp_max(const double & critere_temp_max) {this->m_critere_temp_max = critere_temp_max;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setCritere_Variation(const double & critere_variation)
    // \brief Ajoute un critere de variation de temp�rature
    //
    // \param critere de variation de temp�rature
    ///////////////////////////////////////////////////////////////////////////*/
    inline double setCritere_Variation(const double & critere_variation) {this->m_critere_variation = critere_variation;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTempMin(const float & TempMin)
    // \brief Ajoute une temp�rature minimum
    //
    // \param temp�rature minimum
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTempMin(const float & TempMin) {this->m_temp_min =TempMin ;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTempMax(const float & TempMax)
    // \brief Ajoute une temp�rature maximum
    //
    // \param temp�rature maximum
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTempMax(const float & TempMax) {this->m_temp_max =TempMax ;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTempMoyenne(const float & TempMoyenne)
    // \brief Ajoute une temp�rature moyenne
    //
    // \param temp�rature moyenne
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTempMoyenne(const float & TempMoyenne) {this->m_temp_moyenne =TempMoyenne ;}

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
