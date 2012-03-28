/*////////////////////////////////////////////////////
// \file mode4.h
// \brief G�re l'envoi et la r�ception de commande MODE4
// \author FOUQUART Christophe
// \version 1.0
// \date 31/03/2011
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
////////////////////////////////////////////////////*/

#ifndef MODE4_H
#define MODE4_H

#include <iostream>

#include "protocole.h"
#include "mode4StructConfig.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Mode4
// \brief G�re l'envoi et la r�ception de commande MODE4
//
// Cette classe d�finie les membres et m�thodes
// h�rit�es de la classe Protocole.
// Cette classe peut �tre d�riv�e en cas de changement de certaines fonctions
////////////////////////////////////////////////////////////////////////////////////*/
class Mode4 : public Protocole
{
protected:
    bool accepteMesureFloat;

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString* creerTrameCommande(QString & noCommande,QString & data="")
    // \brief Cr�e une trame en fonction du num�ro de commande MODE4
    //
    // \param noCommande Cum�ro de commande MODE4
    // \param data Donn�es facultatives
    ///////////////////////////////////////////////////////////////////////////*/
    QString* creerTrameCommande(const QString & noCommande,const QString & data="");

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString* calculerBCC(QString & trame)
    // \brief Calcule du BCC (Block Check Character)
    //
    // Le BCC est obtenu en effectuant un OU exclusif (XOR)
    // sur tous les octets d� partir de STX exclu jusqu'au BCC exclu
    //
    // \param trame Trame MODE4
    // \return QString* BCC
    ///////////////////////////////////////////////////////////////////////////*/
    QString* calculerBCC(const QString & trame);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn Mode4_AnalyseurConfig* parseConfig(QString & trameConfig)
    // \brief Analyse la trame et renvoie les infos de config dans une structure
    //
    // \param trameConfig Trame de r�ponse a la commande MODE4 02
    // \return void* Pointeur sur une structure contenant les infos de configuration
    ///////////////////////////////////////////////////////////////////////////*/
    void* parseConfig(const QString & trameConfig);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void getStrForNumber()
    // \brief Renvoie une chaine de 8 caract�res repr�sentant le nombre en param�tre
    //
    // \param nombre Nombre � transformer en chaine
    // \return QString* Chaine repr�sentant le nombre
    ///////////////////////////////////////////////////////////////////////////*/
    QString* getStrForNumber(const ushort & nombre, const ushort & nbDigit);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn float getMesureFromString(QString & mesure)
    // \brief Renvoie le flottant repr�sent� dans la chaine pass� en param�tre
    //
    // \param mesure Chaine contenant le flottant
    // \return QString* Chaine repr�sentant le nombre
    ///////////////////////////////////////////////////////////////////////////*/
    float getMesureFromString(const QString & mesure);

public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn Mode4()
    // \brief Constructeur
    ///////////////////////////////////////////////////////////////////////////*/
    Mode4();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn Mode4(QString adressePeriph, TypePeripherique typePeriph, bool accepteFloat)
    // \brief Constructeur
    //
    // \param adressePeriph Adresse du p�riph�rique
    // \param TypePeripherique Analyseur ou Diluteur
    // \param accepteFloat Vrai si accepte la commande 16 de mesure en flottant
    ///////////////////////////////////////////////////////////////////////////*/
    Mode4(const QString & adressePeriph, const TypePeripherique & typePeriph, const DesignationProtocole & protocoleUtilise, const bool accepteFloat);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual Mode4()
    // \brief Destructeur
    ///////////////////////////////////////////////////////////////////////////*/
    ~Mode4();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void init()
    // \brief Initialisation des parametres du p�riph�rique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual bool init();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void parDefault()
    // \brief R�gle l'appareil sur son mode de fonctionnement par d�faut
    ///////////////////////////////////////////////////////////////////////////*/
    virtual bool parDefault();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual short offset()
    // \brief Renvoie l'offsetdu p�riph�rique
    //
    // \return short Offsetdu p�riph�rique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual short offset();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void demandeMesure()
    // \brief Demande de mesure imm�diate
    //
    // \return QVector<float> Tableau des mesures
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
    // \fn virtual void demandeHeure()
    // \brief Demande d'heure
    //
    // \return QDateTime* Date et heure du p�riph�rique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual QDateTime* demandeDateHeure();

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
    // \fn virtual void reset()
    // \brief Reset du p�riph�rique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual bool reset();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void standBy()
    // \brief mise en stand-by du p�riph�rique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual bool standBy();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeEvent()
    // \brief Connaitre les valeurs actuelles de gaz du diluteur
    ///////////////////////////////////////////////////////////////////////////*/
    virtual bool commandeEvent();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeConfig()
    // \brief Connaitre la configuration actuelle du diluteur
    //
    // \return void* Pointeur sur la structure de configuration
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void* commandeConfig();

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

#endif // MODE4_H
