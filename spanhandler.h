/*////////////////////////////////////////////////////
// \file spanhandler.h
// \brief Classe gérant les paramètres de span
// \author FOUQUART Christophe
// \version 1.0
// \date 31/03/2011
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
////////////////////////////////////////////////////*/

#ifndef SPANHANDLER_H
#define SPANHANDLER_H

#include "definitions_globales.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class SpanHandler
// \brief Classe gérant les paramètres de span
//
// Cette classe gère les paramètres de span
////////////////////////////////////////////////////////////////////////////////////*/

class SpanHandler
{
    QVector<QVariant>* spanArguments; // Valeur des arguments pour les fonctions de span
    QVector<bool>* tabArgumentsUtiles; // Tableau des arguments utilisés
public:

    /*///////////////////////////////////////////////////////////////////////////
    // \fn SpanHandler()
    // \brief Constructeur par defaut
    ///////////////////////////////////////////////////////////////////////////*/
    SpanHandler();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTabArgumentsSpan(QVector<bool> argumentSpan)
    // \brief Affecte le tableau d'utilisation des arguments de span
    //
    // \param argumentSpan Tableau d'utilisation des arguments de span
    ///////////////////////////////////////////////////////////////////////////*/
    void setTabArgumentsSpan(QVector<bool> argumentSpan);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QVector<bool>* getTabArgumentsSpan()
    // \brief Renvoie le tableau d'utilisation des arguments de span
    //
    // \return QVector<bool>* Tableau d'utilisation des arguments de span
    ///////////////////////////////////////////////////////////////////////////*/
    QVector<bool>* getTabArgumentsSpan();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setSpanArguments(QString const & canal,ushort const & point, ushort const & concO3)
    // \brief Affecte les valeurs des arguments de span au tableau
    //
    // \param canal Canal de polluant
    // \param point Point de gaz à atteindre
    // \param concO3 Concentration d'ozone désirée
    ///////////////////////////////////////////////////////////////////////////*/
    void setSpanArguments(const QString canal,const ushort point, const ushort concO3);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString getCanal() const
    // \brief Renvoie le canal de polluant
    //
    // \return QString Canal de polluant
    ///////////////////////////////////////////////////////////////////////////*/
    QString getCanal() const;

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ushort getPoint() const
    // \brief Renvoie le point de gaz à atteindre
    //
    // \return ushort Point de gaz à atteindre
    ///////////////////////////////////////////////////////////////////////////*/
    ushort getPoint() const;

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ushort getConcO3() const
    // \brief Renvoie la concentration d'ozone désirée
    //
    // \return ushort Concentration d'ozone désirée
    ///////////////////////////////////////////////////////////////////////////*/
    ushort getConcO3() const;
};

#endif // SPANHANDLER_H
