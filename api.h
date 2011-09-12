/*////////////////////////////////////////////////////
// \file Api.h
// \brief G�re l'envoi et la r�ception de commande API
// \author FOUQUART Christophe
// \version 1.0
// \date 31/03/2011
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
////////////////////////////////////////////////////*/

#ifndef API_H
#define API_H

#include "protocole.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Api
// \brief G�re l'envoi et la r�ception de commande API
//
// Cette classe d�finie les membres et m�thodes
// h�rit�es de la classe Protocole.
// Cette classe peut �tre d�riv�e en cas de changement de certaines fonctions
////////////////////////////////////////////////////////////////////////////////////*/

class Api : public Protocole
{
protected:

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString* creerTrameCommande(TypeCommandesApi noCommande,QString data="")
    // \brief Cr�e une trame en fonction du num�ro de commande API
    //
    // \param TypeCommandesApi Type de commande API
    // \param data Donn�es facultatives
    ///////////////////////////////////////////////////////////////////////////*/
    QString* creerTrameCommande(const QString & typeCommande,const QString & data="");

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void demandeMesureNox()
    // \brief Demande de mesure imm�diate pour le NOX
    //
    // \return QWeakPointer<MesureIndividuelle> Tableau des mesures
    ///////////////////////////////////////////////////////////////////////////*/
    QWeakPointer<MesureIndividuelle> demandeMesureNox();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn float getFloatFromMesureString(QString mesure)
    // \brief Renvoi le float contenu dans la trame de mesure
    //
    // \return float Mesure au format flottant
    ///////////////////////////////////////////////////////////////////////////*/
    float getFloatFromMesureString(const QString & mesure);

public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn Api(QString adressePeriph, TypePeripherique typePeriph, DesignationProtocole & protocoleUtilise, TypePolluant typePolluant)
    // \brief Constructeur
    //
    // \param adressePeriph Adresse du p�riph�rique
    // \param TypePeripherique Analyseur ou Diluteur
    // \param TypePolluant Polluant associ� au p�riph�rique
    // \param protocoleUtilise Version de protocole utilis�e
    ///////////////////////////////////////////////////////////////////////////*/
    Api(const QString & adressePeriph,const TypePeripherique & typePeriph, const DesignationProtocole & protocoleUtilise);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void demandeMesure()
    // \brief Demande de mesure imm�diate
    //
    // \return QWeakPointer<MesureIndividuelle> Tableau des mesures
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
    // \fn virtual void passageZero()
    // \brief Demande de passage en mode zero
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void passageZero();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void passageEtalon()
    // \brief Demande de passage en mode �talon
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void passageEtalon();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void passageMesure()
    // \brief Demande de passage en mode mesure
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void passageMesure();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void void commandeSpan(const SpanHandler & spanData)
    // \brief Commande au diluteur de se mettre � un certain point de gaz
    //
    // \param spanData Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpan(const SpanHandler & spanData);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpanZero(const QString & canal="")
    // \brief Commande au diluteur de se mettre au point de gaz zero
    //
    // \param canal Canal associ� au polluant voulu
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanZero(const QString & canal="");

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpanTpg(const SpanHandler & spanTpgData)
    // \brief Commande au diluteur de se mettre � un certain point de gaz
    //  pour effectuer un Titrage en Phase Gazeuse
    //
    // \param spanTpgData Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanTpg(const SpanHandler & spanTpgData);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpan03(const SpanHandler & spanO3Data)
    // \brief Commande au diluteur de se mettre � un certain point de gaz O3
    //
    // \param spanO3Data Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanO3(const SpanHandler & spanO3Data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void standBy()
    // \brief mise en stand-by du p�riph�rique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual bool standBy();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual QVector<Commandes> const* getListeCommandes()
    // \brief Renvoie la liste des commandes autoris�es par le protocole
    //
    // \return QVector<Commandes> const* Liste des commandes autoris�es par le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    virtual QVector<Commandes> const* getListeCommandes();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual SpanHandler* getSpanHandler(Commandes commandeSpan)
    // \brief Renvoie une instance de SpanHandler contenant les infos
    //        sur les arguments de la commande
    //
    // \param commandeSpan Commande dont on veut connaitre les arguments � utiliser
    // \return SpanHandler* Instance de SpanHandler contenant les infos sur les arguments de la commande
    ///////////////////////////////////////////////////////////////////////////*/
    virtual SpanHandler* getSpanHandler(Commandes commandeSpan);
};

#endif // API_H
