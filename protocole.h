/*////////////////////////////////////////////////////
// \file protocole.h
// \brief Interface pour les protocoles
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

#ifndef PROTOCOLE_H
#define PROTOCOLE_H

#include "definitions_globales.h"
#include "threadcomhandler.h"
#include "spanhandler.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Protocole
// \brief classe abstraite d�finissant les m�thodes d'un protocole
//
// Cette classe sert d'interface pour d�finir les membres et m�thodes
// communes � tous les protocoles.
// Les m�thodes de cette classe doivent �tre r�impl�ment�es dans les classes filles
////////////////////////////////////////////////////////////////////////////////////*/

class Protocole : public QObject
{
    Q_OBJECT
protected:
    QString adresse; // Adresse du p�riph�rique
    ThreadComHandler const* threadCommunication; // G�re la communication RS232
    ushort flagEtatCom; // Flag de gestion de l'�tat de la communication
    ushort timeout; // Temps maximum d'attente lors d'une communication en ms
    bool timerFini; // Signifie la fin du timeout de communication
    QString trame;  // trame de r�ponse
    TypePeripherique typePeripherique; // Analyseur ou diluteur
    TypePolluant polluantAssocie; // Polluant associ�
    OptionTpg optionTpg; // Option pour le tirtrage en phase gazeuse
    Protocoles versionProtocole; // Version de protocole


public:
    Protocole();
    virtual ~Protocole();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setThreadComHandler(CommunicationSerie* newThreadComHandler)
    // \brief Affecte une nouvelle instance de la classe ThreadComHandler
    //
    // \param newThreadComHandler Nouvelle instance de la classe ThreadComHandler
    ///////////////////////////////////////////////////////////////////////////*/
    void setThreadComHandler(ThreadComHandler const & newThreadComHandler);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ThreadComHandler* getThreadComHandler()
    // \brief Retourne l'instance de la classe ThreadComHandler
    //
    // \return ThreadComHandler* Pointeur sur l'instance de la classe ThreadComHandler
    ///////////////////////////////////////////////////////////////////////////*/
    ThreadComHandler const* getThreadComHandler();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTimeOut(ushort timeout)
    // \brief Affecte un nouveau timeout en ms � la communication
    //
    // \param newTimeOut Nouvelle valeur de timeout en ms
    ///////////////////////////////////////////////////////////////////////////*/
    void setTimeOut(ushort const & newTimeOut);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ushort getTimeOut()
    // \brief Retourne le timeout � la communication
    //
    // \return ushort Timeout � la communication en ms
    ///////////////////////////////////////////////////////////////////////////*/
    ushort getTimeOut();

    /*/////////////////////ushort canal,ushort point//////////////////////////////////////////////////////
    // \fn void changementEtat(ushort newEtatCom)
    // \brief Affecte un nouvel �tat de la communication RS232
    ///////////////////////////////////////////////////////////////////////////*/
    void changementEtat(ushort const & newEtatCom);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ushort getEtatCom()
    // \brief Retourne l'�tat de la communication RS232
    //
    // \return ushort Etat de la communication RS232
    ///////////////////////////////////////////////////////////////////////////*/
    ushort getEtatCom();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void init()
    // \brief Initialisation des parametres de fonctionnement du p�riph�rique
    //
    // \return bool faux si non disponible pour le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    inline virtual bool init() {return false;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void parDefault()
    // \brief R�gle l'appareil sur son mode de fonctionnement par d�faut
    //
    // \return bool faux si non disponible pour le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    inline virtual bool parDefault() {return false;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual short offset()
    // \brief Renvoie l'offsetdu p�riph�rique
    //
    // \return short 0 par d�faut si non disponible pour le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    inline virtual short offset() {return 0;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void reset()
    // \brief Reset du p�riph�rique
    //
    // \return bool faux si non disponible pour le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    inline virtual bool reset() {return false;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void standBy()
    // \brief mise en stand-by du p�riph�rique
    //
    // \return bool faux si non disponible pour le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    inline virtual bool standBy() {return false;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeEvent()
    // \brief Connaitre les valeurs actuelles de gaz du diluteur
    //
    // \return bool faux si non disponible pour le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    inline virtual bool commandeEvent() {return false;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual QVector<float> demandeMesure()
    // \brief Demande de mesure imm�diate
    //
    // \return QVector<float> Tableau des mesures
    ///////////////////////////////////////////////////////////////////////////*/
    virtual QVector<float>* demandeMesure()=0;

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void demandeAlarme()
    // \brief Demande d'alarme
    //
    // \return ushort Valeur de l'alarme
    ///////////////////////////////////////////////////////////////////////////*/
    virtual ushort demandeAlarme()=0;

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void demandeHeure()
    // \brief Demande d'heure
    //
    // \return QDateTime* Date et heure du p�riph�rique
    ///////////////////////////////////////////////////////////////////////////*/
    inline virtual QDateTime* demandeDateHeure() { return NULL; }

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void passageZero()
    // \brief Demande de passage en mode zero
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void passageZero()=0;

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void passageEtalon()
    // \brief Demande de passage en mode �talon
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void passageEtalon()=0;

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void passageMesure()
    // \brief Demande de passage en mode mesure
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void passageMesure()=0;

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void void commandeSpan(SpanHandler const & spanData)
    // \brief Commande au diluteur de se mettre � un certain point de gaz
    //
    // \param spanData Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpan(SpanHandler const & spanData);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpanZero(QString const & canal="")
    // \brief Commande au diluteur de se mettre au point de gaz zero
    //
    // \param canal Canal associ� au polluant voulu
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanZero(QString const & canal=0);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpanTpg(SpanHandler const & spanTpgData)
    // \brief Commande au diluteur de se mettre � un certain point de gaz
    //  pour effectuer un Titrage en Phase Gazeuse
    //
    // \param spanTpgData Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanTpg(SpanHandler const & spanTpgData);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpan03(SpanHandler const & spanO3Data)
    // \brief Commande au diluteur de se mettre � un certain point de gaz O3
    //
    // \param spanO3Data Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanO3(SpanHandler const & spanO3Data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString transaction(QString commande)
    // \brief Execute une transaction avec le p�riph�rique (ecriture/attente r�ponse)
    //
    // \param commande Trame de commande
    // \return QString reponse du p�riph�rique
    ///////////////////////////////////////////////////////////////////////////*/
    QString transaction(QString const & commande);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual QVector<Commandes> const* getListeCommandes()
    // \brief Renvoie la liste des commandes autoris�es par le protocole
    //
    // \return QVector<Commandes> const* Liste des commandes autoris�es par le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    virtual QVector<Commandes> const* getListeCommandes()=0;

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual SpanHandler* getSpanHandler(Commandes commandeSpan)=0
    // \brief Renvoie une instance de SpanHandler contenant les infos
    //        sur les arguments de la commande
    //
    // \param commandeSpan Commande dont on veut connaitre les arguments � utiliser
    // \return SpanHandler* Instance de SpanHandler contenant les infos sur les arguments de la commande
    ///////////////////////////////////////////////////////////////////////////*/
    virtual SpanHandler* getSpanHandler(Commandes commandeSpan)=0;

signals:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn void envoiTrame(QString data)
    // \brief Envoi d'une trame par RS232
    //
    // \param data Trame de commande
    ///////////////////////////////////////////////////////////////////////////*/
    void envoiTrame(QString const & data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void afficheTrame(QString data)
    // \brief Envoi d'une trame pour affichage
    //
    // \param data Trame � afficher
    ///////////////////////////////////////////////////////////////////////////*/
    void afficheTrame(QString const & data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void afficheTrame(QString data)
    // \brief Envoi d'une trame pour affichage
    //
    // \param data Trame � afficher
    ///////////////////////////////////////////////////////////////////////////*/
    void afficheTrame(QStringList* data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void erreurTransmission()
    // \brief Envoi un signal lors d'une erreur de transmission
    ///////////////////////////////////////////////////////////////////////////*/
    void erreurTransmission();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void erreurCommande()
    // \brief Envoi un signal si le message re�u est un message d'erreur
    ///////////////////////////////////////////////////////////////////////////*/
    void erreurCommande();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void alarmeGenerale()
    // \brief Envoi un signal d'alarme g�n�rale
    ///////////////////////////////////////////////////////////////////////////*/
    void alarmeGenerale();

protected slots:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn void lectureTrame(QString data)
    // \brief Slot de lecture d'une trame
    //
    // \param data Trame re�ue
    ///////////////////////////////////////////////////////////////////////////*/
    void lectureTrame(QString const & data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void timeoutCom()
    // \brief Slot appele lorsque le timer de communication est fini
    ///////////////////////////////////////////////////////////////////////////*/
    void timeoutCom();
};

#endif // PROTOCOLE_H
