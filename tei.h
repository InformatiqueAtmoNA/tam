/*////////////////////////////////////////////////////
// \file Tei.h
// \brief Gère l'envoi et la réception de commande Tei
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

#ifndef TEI_H
#define TEI_H

#include <iostream>
#include "protocole.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Tei
// \brief Gère l'envoi et la réception de commande TEI
//
// Cette classe définie les membres et méthodes
// héritées de la classe Protocole.
// Cette classe peut être dérivée en cas de changement de certaines fonctions
////////////////////////////////////////////////////////////////////////////////////*/
class Tei : public Protocole
{
protected:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn setModeRemote()
    // \brief Enclenche le controle a distance
    //
    // \return bool Vrai si réussite
    ///////////////////////////////////////////////////////////////////////////*/
    bool setModeRemote();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn setFormat()
    // \brief Paramétrage du format de trame
    //
    // \return bool Vrai si réussite
    ///////////////////////////////////////////////////////////////////////////*/
    bool setFormat();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn setGasUnit()
    // \brief Paramétrage de l'unité de mesure
    //
    // \return bool Vrai si réussite
    ///////////////////////////////////////////////////////////////////////////*/
    bool setGasUnit();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString CreerTrameCommande(QString commande)
    // \brief Crée une trame au format TEI
    //
    // \param commande Commande TEI
    // \param QString* Trame de commande
    ///////////////////////////////////////////////////////////////////////////*/
    QString* creerTrameCommande(const QString & commande);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn float getFloatFromMesureString(QString mesure)
    // \brief Renvoi le float contenu dans la trame de mesure
    //
    // \param mesure Trame de mesure
    // \return float Mesure au format flottant
    ///////////////////////////////////////////////////////////////////////////*/
    float getFloatFromMesureString(QString mesure);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void demandeMesureNox()
    // \brief Demande de mesure immédiate pour le NOX
    //
    // \return QVector<float> Tableau des mesures
    ///////////////////////////////////////////////////////////////////////////*/
    QWeakPointer<MesureIndividuelle> demandeMesureNox();

public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn Tei(QString adressePeriph, TypePeripherique typePeriph, bool accepteFloat)
    // \brief Constructeur
    //
    // \param adressePeriph Adresse du périphérique
    // \param typePeriph Analyseur ou Diluteur
    // \param typePolluant Type de polluant associé à l'appareil
    // \param optionTpg Paramètre définissant si l'appareil dispose d'un ozoniseur ou d'un photometre
    ///////////////////////////////////////////////////////////////////////////*/
    Tei(const QString & adressePeriph, const TypePeripherique & typePeriph,const OptionTpg & optionTpg=AUCUNE);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void init()
    // \brief Initialisation des parametres du périphérique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual bool init();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void parDefault()
    // \brief Règle l'appareil sur son mode de fonctionnement par défaut
    ///////////////////////////////////////////////////////////////////////////*/
    virtual bool parDefault();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual ushort offset()
    // \brief Renvoie l'offsetdu périphérique
    //
    // \return int Offsetdu périphérique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual short offset();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void standBy()
    // \brief mise en stand-by du périphérique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual bool standBy();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void demandeMesure()
    // \brief Demande de mesure immédiate
    //
    // \return MesureIndividuelle Tableau des mesures
    ///////////////////////////////////////////////////////////////////////////*/
    virtual QWeakPointer<MesureIndividuelle> demandeMesure();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void demandeAlarme()
    // \brief Demande d'alarme
    //
    // \return ushort Valeur de l'alarme
    ///////////////////////////////////////////////////////////////////////////*/
    virtual ushort demandeAlarme();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void demandeDateHeure()
    // \brief Demande la date et l'heure
    //
    // \return QDateTime* Date et heure du périphérique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual QDateTime* demandeDateHeure();

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

#endif // TEI_H
