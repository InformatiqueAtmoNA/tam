/*////////////////////////////////////////////////////////////
// \file Equipement.h
// \brief Représente un equipement lors d'un test
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

#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H

#include "definitions_globales.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Equipement
// \brief Repr�sente un equipement lors d'un test
//
// Classe de transition vers le format XML
////////////////////////////////////////////////////////////////////////////////////*/

class Equipement : public QObject
{
    TypePeripherique m_type; // Type de p�riph�rique : ANALYSEUR ou ETALON
    QStringList m_polluants; // Polluants associ�s au test

public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn Equipement()
    // \brief Constructeur par d�faut
    ///////////////////////////////////////////////////////////////////////////*/
    Equipement();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn Equipement(Equipement const & equipement)
    // \brief Constructeur de copie
    //
    // \param equipement R�f�rence vers l'objet Equipement �  copier
    ///////////////////////////////////////////////////////////////////////////*/
    Equipement(Equipement const & equipement);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QStringList const & getPolluants()
    // \brief Renvoi la liste des polluants associ�s au périphérique
    //
    // \return QStringList Liste des polluants associés au périphérique
    ///////////////////////////////////////////////////////////////////////////*/
    inline QStringList const & getPolluants() {return this->m_polluants;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn TypePeripherique getTypePeripherique()
    // \brief Renvoi le type de périphérique
    //
    // \return QString Type de périphérique
    ///////////////////////////////////////////////////////////////////////////*/
    inline TypePeripherique getTypePeripherique() {return this->m_type;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void ajouterPolluant(QString const & polluant)
    // \brief Ajoute un polluant �  la liste des polluants associés au périphérique
    //
    // \param polluant Polluant �  ajouter �  la liste des polluants associés au périphérique
    ///////////////////////////////////////////////////////////////////////////*/
    inline void ajouterPolluant(QString const & polluant) {this->m_polluants.append(polluant);}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTypePeripherique(TypePeripherique const & typePeriph)
    // \brief Affecte un nouveau type de périphérique
    //
    // \param typePeriph Nouveau type de périphérique
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTypePeripherique(TypePeripherique const & typePeriph) {this->m_type = typePeriph;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn Equipement& operator=(const Equipement&)
    // \brief Surchage de l'opérateur =
    //
    // \param equipement Référence vers l'objet Equipement �  copier
    // \return Equipement& Nouvelle instance de la classe Equipement
    ///////////////////////////////////////////////////////////////////////////*/
    Equipement& operator=(const Equipement& equipement);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QDomElement exportToXml(QDomDocument & xmlTest)
    // \brief Exporte les données membres de l'instance de la classe sous forme d'élément XML
    //
    // \param xmlTest Document XML servant �  l'export
    // \return QDomElement Elément XML contenant les données membres de la classe
    ///////////////////////////////////////////////////////////////////////////*/
    QDomElement exportToXml(QDomDocument & xmlTest);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn static QPointer<Equipement> importFromXml(QDomElement const & domEquipement)
    // \brief Construit une nouvelle instance de la classe Equipement �  partir d'un élément XML
    //
    // \param domEquipement Elément XML représentant un périphérique
    // \return QPointer<Equipement> Pointeur vers la nouvelle instance de la classe Equipement
    ///////////////////////////////////////////////////////////////////////////*/
    static QPointer<Equipement> importFromXml(QDomElement const & domEquipement);
};

#endif // EQUIPEMENT_H
