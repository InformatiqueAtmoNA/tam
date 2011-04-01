/*////////////////////////////////////////////////////
// \file tei_146c.h
// \brief Classe dérivée de Tei pour les diluteur 146c
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

#ifndef TEI_146C_H
#define TEI_146C_H

#include "tei.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Tei_146c
// \brief Classe dérivée de Tei pour les diluteur 146i
//
// Cette classe redéfinie certaines méthodes
// héritées de la classe Tei.
////////////////////////////////////////////////////////////////////////////////////*/
class Tei_146c : public Tei
{

public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn Tei_146c(QString adressePeriph, TypePeripherique typePeriph, bool accepteFloat)
    // \brief Constructeur
    //
    // \param adressePeriph Adresse du périphérique
    // \param typePeriph Analyseur ou Diluteur
    // \param typePolluant Type de polluant associé à l'appareil
    // \param optionTpg Paramètre définissant si l'appareil dispose d'un ozoniseur ou d'un photometre
    ///////////////////////////////////////////////////////////////////////////*/
    Tei_146c(QString const & adressePeriph, TypePeripherique const & typePeriph, TypePolluant const & typePolluant,OptionTpg const & optionTpg=AUCUNE);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void standBy()
    // \brief mise en stand-by du périphérique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual bool standBy();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void void commandeSpan(SpanHandler const & spanData)
    // \brief Commande au diluteur de se mettre à un certain point de gaz
    //
    // \param spanData Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpan(SpanHandler const & spanData);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpanZero(QString const & canal="")
    // \brief Commande au diluteur de se mettre au point de gaz zero
    //
    // \param canal Canal associé au polluant voulu
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanZero(QString const & canal="");

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpanTpg(SpanHandler const & spanTpgData)
    // \brief Commande au diluteur de se mettre à un certain point de gaz
    //  pour effectuer un Titrage en Phase Gazeuse
    //
    // \param spanTpgData Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanTpg(SpanHandler const & spanTpgData);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan03(SpanHandler const & spanO3Data)
    // \brief Commande au diluteur de se mettre à un certain point de gaz O3
    //
    // \param spanO3Data Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanO3(SpanHandler const & spanO3Data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual QVector<Commandes> const* getListeCommandes()
    // \brief Renvoie la liste des commandes autorisées par le protocole
    //
    // \return QVector<Commandes> const* Liste des commandes autorisées par le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    virtual QVector<Commandes> const* getListeCommandes();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual SpanHandler* getSpanHandler(Commandes commandeSpan)
    // \brief Renvoie une instance de SpanHandler contenant les infos
    //        sur les arguments de la commande
    //
    // \param commandeSpan Commande dont on veut connaitre les arguments à utiliser
    // \return SpanHandler* Instance de SpanHandler contenant les infos sur les arguments de la commande
    ///////////////////////////////////////////////////////////////////////////*/
    virtual SpanHandler* getSpanHandler(Commandes commandeSpan);
};

#endif // TEI_146C_H
