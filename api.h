/*////////////////////////////////////////////////////
// \file Api.h
// \brief Gère l'envoi et la réception de commande API
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

#ifndef API_H
#define API_H

#include "protocole.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Api
// \brief Gère l'envoi et la réception de commande API
//
// Cette classe définie les membres et méthodes
// héritées de la classe Protocole.
// Cette classe peut être dérivée en cas de changement de certaines fonctions
////////////////////////////////////////////////////////////////////////////////////*/

class Api : public Protocole
{
protected:

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString* creerTrameCommande(TypeCommandesApi noCommande,QString data="")
    // \brief Crée une trame en fonction du numéro de commande API
    //
    // \param TypeCommandesApi Type de commande API
    // \param data Données facultatives
    ///////////////////////////////////////////////////////////////////////////*/
    QString* creerTrameCommande(QString const & typeCommande,QString const & data="");

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void demandeMesureNox()
    // \brief Demande de mesure immédiate pour le NOX
    //
    // \return QVector<float> Tableau des mesures
    ///////////////////////////////////////////////////////////////////////////*/
    QVector<float>* demandeMesureNox();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn float getFloatFromMesureString(QString mesure)
    // \brief Renvoi le float contenu dans la trame de mesure
    //
    // \return float Mesure au format flottant
    ///////////////////////////////////////////////////////////////////////////*/
    float getFloatFromMesureString(QString const & mesure);

public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn Api(QString adressePeriph, TypePeripherique typePeriph, Protocoles const & protocoleUtilise, TypePolluant typePolluant)
    // \brief Constructeur
    //
    // \param adressePeriph Adresse du périphérique
    // \param TypePeripherique Analyseur ou Diluteur
    // \param TypePolluant Polluant associé au périphérique
    // \param protocoleUtilise Version de protocole utilisée
    ///////////////////////////////////////////////////////////////////////////*/
    Api(QString const & adressePeriph, TypePeripherique const & typePeriph, Protocoles const & protocoleUtilise, TypePolluant const & typePolluant);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void demandeMesure()
    // \brief Demande de mesure immédiate
    //
    // \return QVector<float> Tableau des mesures
    ///////////////////////////////////////////////////////////////////////////*/
    virtual QVector<float>* demandeMesure();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void demandeAlarme()
    // \brief Demande d'alarme
    //
    // \return ushort Valeur de l'alarme
    ///////////////////////////////////////////////////////////////////////////*/
    virtual ushort demandeAlarme();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void passageZero()
    // \brief Demande de passage en mode zero
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void passageZero();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void passageEtalon()
    // \brief Demande de passage en mode étalon
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void passageEtalon();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void passageMesure()
    // \brief Demande de passage en mode mesure
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void passageMesure();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void void commandeSpan(SpanHandler const & spanData)
    // \brief Commande au diluteur de se mettre à un certain point de gaz
    //
    // \param spanData Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpan(SpanHandler const & spanData);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpanZero(QString const & canal="")
    // \brief Commande au diluteur de se mettre au point de gaz zero
    //
    // \param canal Canal associé au polluant voulu
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanZero(QString const & canal="");

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpanTpg(SpanHandler const & spanTpgData)
    // \brief Commande au diluteur de se mettre à un certain point de gaz
    //  pour effectuer un Titrage en Phase Gazeuse
    //
    // \param spanTpgData Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanTpg(SpanHandler const & spanTpgData);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpan03(SpanHandler const & spanO3Data)
    // \brief Commande au diluteur de se mettre à un certain point de gaz O3
    //
    // \param spanO3Data Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanO3(SpanHandler const & spanO3Data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void standBy()
    // \brief mise en stand-by du périphérique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual bool standBy();

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

#endif // API_H
