/*////////////////////////////////////////////////////
// \file protocole.h
// \brief Interface pour les DesignationProtocole
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

#ifndef PROTOCOLE_H
#define PROTOCOLE_H

#include "definitions_globales.h"
#include "threadcomhandler.h"
#include "spanhandler.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class Protocole
// \brief classe abstraite définissant les méthodes d'un protocole
//
// Cette classe sert d'interface pour définir les membres et méthodes
// communes à tous les DesignationProtocole.
// Les méthodes de cette classe doivent être réimplémentées dans les classes filles
////////////////////////////////////////////////////////////////////////////////////*/

class Protocole : public QObject
{
    Q_OBJECT
protected:
    QString adresse; // Adresse du périphérique
    QPointer<ThreadComHandler> threadCommunication; // Gère la communication RS232
    ushort flagEtatCom; // Flag de gestion de l'état de la communication
    ushort timeout; // Temps maximum d'attente lors d'une communication en ms
    bool timerFini; // Signifie la fin du timeout de communication
    QString trame;  // trame de réponse
    TypePeripherique typePeripherique; // Analyseur ou diluteur
    QList<TypePolluant> polluantAssocie; // Polluant associé
    OptionTpg optionTpg; // Option pour le tirtrage en phase gazeuse
    DesignationProtocole versionProtocole; // Version de protocole
    bool m_avorterTransaction;
    QTimer m_timerFinTransaction;

public:
    Protocole();
    virtual ~Protocole();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setThreadComHandler(QPointer<ThreadComHandler> newThreadComHandler)
    // \brief Affecte une nouvelle instance de la classe ThreadComHandler
    //
    // \param newThreadComHandler Nouvelle instance de la classe ThreadComHandler
    ///////////////////////////////////////////////////////////////////////////*/
    void setThreadComHandler(const QPointer<ThreadComHandler> newThreadComHandler);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QPointer<ThreadComHandler> getThreadComHandler()
    // \brief Retourne l'instance de la classe ThreadComHandler
    //
    // \return QPointer<ThreadComHandler> Pointeur sur l'instance de la classe ThreadComHandler
    ///////////////////////////////////////////////////////////////////////////*/
    QPointer<ThreadComHandler> getThreadComHandler();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTimeOut(ushort timeout)
    // \brief Affecte un nouveau timeout en ms à la communication
    //
    // \param newTimeOut Nouvelle valeur de timeout en ms
    ///////////////////////////////////////////////////////////////////////////*/
    void setTimeOut(const ushort newTimeOut);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTypePolluant(const TypePolluant & typePolluant)
    // \brief Initialise le type de polluant associé à l'appareil
    //
    // \param typePolluant Type de polluant associé à l'appareil
    ///////////////////////////////////////////////////////////////////////////*/
    inline void setTypePolluant(QList<TypePolluant> typePolluant) {this->polluantAssocie=typePolluant;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ushort getTimeOut()
    // \brief Retourne le timeout à la communication
    //
    // \return ushort Timeout à la communication en ms
    ///////////////////////////////////////////////////////////////////////////*/
    ushort getTimeOut();

    /*/////////////////////ushort canal,ushort point//////////////////////////////////////////////////////
    // \fn void changementEtat(ushort newEtatCom)
    // \brief Affecte un nouvel état de la communication RS232
    ///////////////////////////////////////////////////////////////////////////*/
    void changementEtat(const ushort newEtatCom);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ushort getEtatCom()
    // \brief Retourne l'état de la communication RS232
    //
    // \return ushort Etat de la communication RS232
    ///////////////////////////////////////////////////////////////////////////*/
    ushort getEtatCom();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void init()
    // \brief Initialisation des parametres de fonctionnement du périphérique
    //
    // \return bool faux si non disponible pour le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    inline virtual bool init() {return false;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void parDefault()
    // \brief Règle l'appareil sur son mode de fonctionnement par défaut
    //
    // \return bool faux si non disponible pour le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    inline virtual bool parDefault() {return false;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual short offset()
    // \brief Renvoie l'offsetdu périphérique
    //
    // \return short 0 par défaut si non disponible pour le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    inline virtual short offset() {return 0;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void reset()
    // \brief Reset du périphérique
    //
    // \return bool faux si non disponible pour le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    inline virtual bool reset() {return false;}

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void standBy()
    // \brief mise en stand-by du périphérique
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
    // \fn virtual QPointer<MesureIndividuelle> demandeMesure()
    // \brief Demande de mesure immédiate
    //
    // \return QPointer<MesureIndividuelle> Tableau des mesures
    ///////////////////////////////////////////////////////////////////////////*/
    virtual QPointer<MesureIndividuelle> demandeMesure()=0;

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
    // \return QDateTime* Date et heure du périphérique
    ///////////////////////////////////////////////////////////////////////////*/
    inline virtual QDateTime* demandeDateHeure() { return NULL; }

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void passageZero()
    // \brief Demande de passage en mode zero
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void passageZero()=0;

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void passageEtalon()
    // \brief Demande de passage en mode étalon
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void passageEtalon()=0;

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void passageMesure()
    // \brief Demande de passage en mode mesure
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void passageMesure()=0;

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void void commandeSpan(SpanHandler const & spanData)
    // \brief Commande au diluteur de se mettre à un certain point de gaz
    //
    // \param spanData Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpan(const SpanHandler & spanData);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpanZero(QString const & canal="")
    // \brief Commande au diluteur de se mettre au point de gaz zero
    //
    // \param canal Canal associé au polluant voulu
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanZero(const QString & canal=0);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpanTpg(SpanHandler const & spanTpgData)
    // \brief Commande au diluteur de se mettre à un certain point de gaz
    //  pour effectuer un Titrage en Phase Gazeuse
    //
    // \param spanTpgData Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanTpg(const SpanHandler & spanTpgData);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpan03(SpanHandler const & spanO3Data)
    // \brief Commande au diluteur de se mettre à un certain point de gaz O3
    //
    // \param spanO3Data Instance de la classe SpanHandler contenant les infos de span
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanO3(const SpanHandler & spanO3Data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString transaction(QString commande)
    // \brief Execute une transaction avec le périphérique (ecriture/attente réponse)
    //
    // \param commande Trame de commande
    // \return QString reponse du périphérique
    ///////////////////////////////////////////////////////////////////////////*/
    QString transaction(const QString & commande);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual QVector<Commandes> const* getListeCommandes()
    // \brief Renvoie la liste des commandes autorisées par le protocole
    //
    // \return QVector<Commandes> const* Liste des commandes autorisées par le protocole
    ///////////////////////////////////////////////////////////////////////////*/
    virtual QVector<Commandes> const* getListeCommandes()=0;

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual SpanHandler* getSpanHandler(Commandes commandeSpan)=0
    // \brief Renvoie une instance de SpanHandler contenant les infos
    //        sur les arguments de la commande
    //
    // \param commandeSpan Commande dont on veut connaitre les arguments à utiliser
    // \return SpanHandler* Instance de SpanHandler contenant les infos sur les arguments de la commande
    ///////////////////////////////////////////////////////////////////////////*/
    virtual SpanHandler* getSpanHandler(Commandes commandeSpan)=0;

    /*///////////////////////////////////////////////////////////////////////////
    // \fn static QPointer<Protocole> getProtocoleObject(const DesignationProtocole & designationProtocole, const QString & adresse)
    // \brief Retourne un objet protocole en fonction des paramètres
    //
    // \param protocole Identifiant du protocole de l'appareil
    // \param adresse Adresse de l'appareil
    // \return SpanHandler* Instance de SpanHandler contenant les infos sur les arguments de la commande
    ///////////////////////////////////////////////////////////////////////////*/
    static QPointer<Protocole> getProtocoleObject(const DesignationProtocole & designationProtocole, const QString & adresse);

    void setVersionProtocole(DesignationProtocole newVersionProtocole);

public Q_SLOTS:

    void quitter();

Q_SIGNALS:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn void envoiTrame(QString data)
    // \brief Envoi d'une trame par RS232
    //
    // \param data Trame de commande
    ///////////////////////////////////////////////////////////////////////////*/
    void envoiTrame(const QString & data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void afficheTrame(QString data)
    // \brief Envoi d'une trame pour affichage
    //
    // \param data Trame à afficher
    ///////////////////////////////////////////////////////////////////////////*/
    void afficheTrame(const QString & data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void afficheTrame(QString data)
    // \brief Envoi d'une trame pour affichage
    //
    // \param data Trame à afficher
    ///////////////////////////////////////////////////////////////////////////*/
    void afficheTrame(const QStringList* data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void erreurTransmission()
    // \brief Envoi un signal lors d'une erreur de transmission
    ///////////////////////////////////////////////////////////////////////////*/
    void erreurTransmission();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void erreurCommande()
    // \brief Envoi un signal si le message reçu est un message d'erreur
    ///////////////////////////////////////////////////////////////////////////*/
    void erreurCommande();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void alarmeGenerale()
    // \brief Envoi un signal d'alarme générale
    ///////////////////////////////////////////////////////////////////////////*/
    void alarmeGenerale();

protected Q_SLOTS:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn void lectureTrame(QString data)
    // \brief Slot de lecture d'une trame
    //
    // \param data Trame reçue
    ///////////////////////////////////////////////////////////////////////////*/
    void lectureTrame(const QString & data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void timeoutCom()
    // \brief Slot appele lorsque le timer de communication est fini
    ///////////////////////////////////////////////////////////////////////////*/
    void timeoutCom();
};

#endif // PROTOCOLE_H
