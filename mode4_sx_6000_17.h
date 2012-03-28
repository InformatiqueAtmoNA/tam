/*////////////////////////////////////////////////////
// \file mode4_sx_6000_17.h
// \brief Classe dérivée de Mode4 pour les diluteurs SX6000 V1.7
// \author FOUQUART Christophe
// \version 1.0
// \date 31/03/2011
//
// TAM - Tests Automatiques Métrologiques
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
////////////////////////////////////////////////////*/

#ifndef MODE4_SX_6000_17_H
#define MODE4_SX_6000_17_H

#include "mode4.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Mode4_SX_6000_17
// \brief Classe dérivée de Mode4 pour les diluteurs SX6000 V1.7
//
// Cette classe redéfinie certaines méthodes
// héritées de la classe Mode4.
////////////////////////////////////////////////////////////////////////////////////*/

class Mode4_SX_6000_17 : public Mode4
{

public:
    Mode4_SX_6000_17(const QString & adressePeriph, const TypePeripherique & typePeriph, const bool accepteFloat);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void demandeAlarme()
    // \brief Demande d'alarme
    //
    // \return ushort Valeur de l'alarme
    ///////////////////////////////////////////////////////////////////////////*/
    ushort demandeAlarme();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan(const SpanHandler & spanData)
    // \brief Commande au diluteur de se mettre à un certain point de gaz
    //
    // \param spanData Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpan(const SpanHandler & spanData);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpanZero(const QString & canal="")
    // \brief Commande au diluteur de se mettre au point de gaz zero
    //
    // \param canal Canal associé au polluant voulu
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanZero(const QString & canal="");

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpanTpg(SpanHandler & spanTpgData)
    // \brief Commande au diluteur de se mettre à un certain point de gaz
    //  pour effectuer un Titrage en Phase Gazeuse
    //
    // \param spanTpgData Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanTpg(const SpanHandler & spanTpgData);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan03(SpanHandler & spanO3Data)
    // \brief Commande au diluteur de se mettre à un certain point de gaz O3
    //
    // \param spanO3Data Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    void commandeSpanO3(const SpanHandler & spanO3Data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual QVector<Commandes> const* getListeCommandes()
    // \brief Renvoie la liste des commandes autorisées par le protocole
    //
    // \return QVector<Commandes> const* Liste des commandes autorisées par le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    virtual QVector<Commandes> const* getListeCommandes();

};

#endif // MODE4_SX_6000_17_H
