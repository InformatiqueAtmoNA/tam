/*////////////////////////////////////////////////////
// \file protocole.h
// \brief Interface pour les protocoles
// \author FOUQUART Christophe
// \version 0.1
// \date 09/02/2011
////////////////////////////////////////////////////*/

#ifndef PROTOCOLE_H
#define PROTOCOLE_H

#include <QTimer>
#include <QThread>
#include <QtCore>
#include <QVector>
#include <qfile.h>
#include "threadcomhandler.h"

// \def ETAT_REPOS Pas de communication en cours
#define ETAT_REPOS 0
// \def ETAT_ATTENTE Attente d'une r�ception de trame
#define ETAT_ATTENTE 1
// \def ETAT_LECTURE R�ception d'une trame effectu�e
#define ETAT_LECTURE 2
// \def ETAT_LECTURE R�ception d'une trame effectu�e
#define ETAT_ERREUR 3
// \def STX Caract�re ASCII Start Of Text
#define STX 0x02
// \def STX Caract�re ASCII End Of Text
#define ETX 0x03
// \def ACK Caract�re ASCII d'acquitement
#define ACK 0x06
// \def NACK Caract�re ASCII de non acquitement
#define NAK 0x15
// \def SEP Caract�re ASCII de s�paration
#define SEP 0x20
// \def CR Caract�re ASCII de retour chariot
#define CR  0x0D
// \enum TypePeripherique Type d'appareil
enum TypePeripherique {ANALYSEUR,DILUTEUR};
// \enum TypePolluant Polluant associ� � l'appareil
enum TypePolluant {CO,O3,SO2,NOX,DIL};
// \enum OptionTpg Option d'�talonnage permettant la TPG
enum OptionTpg {AUCUNE,OZONISEUR,PHOTOMETRE};
// \enum Protocoles Protocoles de communication support�s
// A MODIFIER LORS DE L'AJOUT DE NOUVEAUX PROTOCOLES
enum Protocoles {MODE4_ANA,MODE4_SX6000_17,MODE4_SX3022,TEI_ANA,TEI_146i,TEI_146c,TEI_49c,API_ANA,API_DIL};

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
    ThreadComHandler* threadCommunication; // G�re la communication RS232
    ushort flagEtatCom; // Flag de gestion de l'�tat de la communication
    ushort timeout; // Temps maximum d'attente lors d'une communication en ms
    bool timerFini; // Signifie la fin du timeout de communication
    QString trame;  // trame de r�ponse
    TypePeripherique typePeripherique; // Analyseur ou diluteur
    TypePolluant polluantAssocie; // Polluant associ�
    OptionTpg optionTpg;

public:
    Protocole();
    virtual ~Protocole();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setThreadComHandler(CommunicationSerie* newThreadComHandler)
    // \brief Affecte une nouvelle instance de la classe ThreadComHandler
    //
    // \param newThreadComHandler Nouvelle instance de la classe ThreadComHandler
    ///////////////////////////////////////////////////////////////////////////*/
    void setThreadComHandler(ThreadComHandler* newThreadComHandler);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ThreadComHandler* getThreadComHandler()
    // \brief Retourne l'instance de la classe ThreadComHandler
    //
    // \return ThreadComHandler* Pointeur sur l'instance de la classe ThreadComHandler
    ///////////////////////////////////////////////////////////////////////////*/
    ThreadComHandler* getThreadComHandler();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void setTimeOut(ushort timeout)
    // \brief Affecte un nouveau timeout en ms � la communication
    //
    // \param newTimeOut Nouvelle valeur de timeout en ms
    ///////////////////////////////////////////////////////////////////////////*/
    void setTimeOut(ushort newTimeOut);

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
    void changementEtat(ushort newEtatCom);

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
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void init();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void parDefault()
    // \brief R�gle l'appareil sur son mode de fonctionnement par d�faut
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void parDefault();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual short offset()
    // \brief Renvoie l'offsetdu p�riph�rique
    //
    // \return short Offsetdu p�riph�rique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual short offset();

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
    // \fn virtual void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre � un certain point de gaz
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpan(ushort canal,ushort point=0,ushort concO3=0);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre � un certain point de gaz
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpan_146c(QString canal,ushort point);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre au point de gaz zero
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanZero(ushort canal);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpanZero()
    // \brief Commande au diluteur de se mettre au point de gaz zero
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanZero();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre � un certain point de gaz
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanTpg(ushort canalNox,ushort point,ushort concO3);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre � un certain point de gaz
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanTpg(QString canalNox,ushort point,ushort concO3);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpan(ushort canal,ushort point)
    // \brief Commande au diluteur de se mettre � un certain point de gaz
    //
    // \param pointO3 Point de consigne ozone
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanTpg(ushort pointO3);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpan(ushort point,ushort concO3)
    // \brief Commande au diluteur de se mettre � un certain point de gaz
    //
    // \param pointNO Point de consigne NO
    // \param concO3  Point de consigne ozone
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanTpg(ushort pointNO,ushort concO3);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void commandeSpan(ushort canal,ushort point
    // \brief Commande au diluteur de se mettre � un certain point de gaz O3
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanO3(ushort canal,ushort point03);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpanO3(ushort point03=0)
    // \brief Commande au diluteur de se mettre � un certain point de gaz O3
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanO3(ushort point03=0);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpan(QString canal,ushort point=0,ushort concO3=0)
    // \brief Commande au diluteur de se mettre � un certain point de gaz
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpan(QString canal,ushort point=0,ushort concO3=0);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeSpan(QString canal,ushort point=0,ushort concO3=0)
    // \brief Commande au diluteur de se mettre � un certain point de gaz
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeSpanZero(QString canal);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void reset()
    // \brief Reset du p�riph�rique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void reset();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void standBy()
    // \brief mise en stand-by du p�riph�rique
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void standBy();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString transaction(QString commande)
    // \brief Execute une transaction avec le p�riph�rique (ecriture/attente r�ponse)
    //
    // \param commande Trame de commande
    // \return QString reponse du p�riph�rique
    ///////////////////////////////////////////////////////////////////////////*/
    QString transaction(QString commande);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn virtual void commandeEvent()
    // \brief Connaitre les valeurs actuelles de gaz du diluteur
    ///////////////////////////////////////////////////////////////////////////*/
    virtual void commandeEvent();

signals:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn void envoiTrame(QString data)
    // \brief Envoi d'une trame par RS232
    //
    // \param data Trame de commande
    ///////////////////////////////////////////////////////////////////////////*/
    void envoiTrame(QString data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void afficheTrame(QString data)
    // \brief Envoi d'une trame pour affichage
    //
    // \param data Trame � afficher
    ///////////////////////////////////////////////////////////////////////////*/
    void afficheTrame(QString data);

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
    void lectureTrame(QString data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void timeoutCom()
    // \brief Slot appele lorsque le timer de communication est fini
    ///////////////////////////////////////////////////////////////////////////*/
    void timeoutCom();
};

#endif // PROTOCOLE_H
