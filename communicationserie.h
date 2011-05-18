/*////////////////////////////////////////////////////
// \file communicationserie.h
// \brief Gestion de la communication RS232
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

#ifndef COMMUNICATIONSERIE_H
#define COMMUNICATIONSERIE_H

#include <QtCore/QDebug>
#include <QtCore/QByteArray>
#include <iostream>
#include <string>
#include <QtCore/qstring.h>
#include "abstractserial.h"

/*/////////////////////////////////////////////////////
// \class CommunicationSerie
// \brief classe g�rant la communication RS232
/////////////////////////////////////////////////////*/
class CommunicationSerie : public QObject
{
    Q_OBJECT
private:

    // P�riph�rique reli� par rs232
    AbstractSerial *device;

///////////////////////////////////////////////
// Param�tres de configuration du p�riph�rique
///////////////////////////////////////////////
    int numeroCom; // Numero du port Com
    QString deviceName; // Nom de P�riph�rique
    QString baudRate; // Taux de transmission
    QString dataBits; // Nombre de bits de transmission
    QString parity; // Type de parit�
    QString stopBits; // Bits de stop
    QString flowControl; // Type de controle de flux
    int charTimeOut; // Intervalle de timeout en ms

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void connectSignalsSlots()
    // \brief Effectue les connection entre signaux/slots
    ///////////////////////////////////////////////////////////////////////////*/
    void connectSignalsSlots();

private slots:

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void slotRead()
    // \brief Effectue la lecture sur le port COM configur�
    ///////////////////////////////////////////////////////////////////////////*/
    void slotRead();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void statusSignal(QString status,QDateTime dateTime)
    // \brief Signale un changement de statut du p�riph�rique
    //
    // \param status Changement de statut
    // \param dateTime Date et heure du changement de statut
    ///////////////////////////////////////////////////////////////////////////*/
    void statusSignal(QString status,QDateTime dateTime);

public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn CommunicationSerie()
    // \brief Constructeur de la classe CommunicationSerie
    ///////////////////////////////////////////////////////////////////////////*/
    CommunicationSerie();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn CommunicationSerie()
    // \brief Constructeur de la classe CommunicationSerie
    //
    // \param numCom Num�ro de port X (d�fini par /dev/ttyX ou COMX)
    ///////////////////////////////////////////////////////////////////////////*/
    CommunicationSerie(int numCom);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn CommunicationSerie()
    // \brief Constructeur de la classe CommunicationSerie
    //
    // \param deviceName Identifiant du port COM (d�fini par /dev/ttyX ou COMX)
    ///////////////////////////////////////////////////////////////////////////*/
    CommunicationSerie(QString deviceName);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn CommunicationSerie()
    // \brief Destructeur de la classe CommunicationSerie
    ///////////////////////////////////////////////////////////////////////////*/
    ~CommunicationSerie();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn int getNumeroCom()
    // \brief Retourne le num�ro de port COM du p�riph�rique
    //
    // \return int Num�ro de port COM
    ///////////////////////////////////////////////////////////////////////////*/
    int getNumeroCom();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString getDeviceName()
    // \brief Retourne l'identifiant du port COM du p�riph�rique
    //
    // \return QString Identifiant du port COM du p�riph�rique
    ///////////////////////////////////////////////////////////////////////////*/
    QString getDeviceName();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString getBaudRate()
    // \brief Retourne le taux de transmission
    //
    // \return QString Taux de transmission
    ///////////////////////////////////////////////////////////////////////////*/
    // Obtenir Taux de transmission
    QString getBaudRate();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString getDataBits()
    // \brief Retourne le nombre de bits de transmission
    //
    // \return QString Nombre de bits de transmission
    ///////////////////////////////////////////////////////////////////////////*/
    QString getDataBits();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString getParity()
    // \brief Retourne le type de parit�
    //
    // \return QString Type de parit�
    ///////////////////////////////////////////////////////////////////////////*/
    QString getParity();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString getStopBits()
    // \brief Retourne le nombre de bits de stop
    //
    // \return QString Nombre de bits de stop
    ///////////////////////////////////////////////////////////////////////////*/
    QString getStopBits();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString getFlowControl()
    // \brief Retourne le type de controle de flux
    //
    // \return QString Type de controle de flux
    ///////////////////////////////////////////////////////////////////////////*/
    QString getFlowControl();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn CommunicationSerie();
    // \brief Retourne l'intervalle de timeout en ms
    //
    // \return int Intervalle de timeout en ms
    ///////////////////////////////////////////////////////////////////////////*/
    int getCharTimeOut();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool setNumeroCom(int newNumCom)
    // \brief Change le num�ro de port COM
    //
    // \param newNumCom Nouveau num�ro de port COM
    // \return bool Vrai si changement effectu�. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool setNumeroCom(int newNumCom);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool setDeviceName(QString newDeviceName)
    // \brief Change l'identifiant du p�riph�rique
    //
    // \param newDeviceName Nouvel identifiant du p�riph�rique
    // \return bool Vrai si changement effectu�. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool setDeviceName(QString newDeviceName);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool setBaudRate(AbstractSerial::BaudRate newBaudRate)
    // \brief Change le taux de transmission
    //
    // \param newBaudRate Nouveau taux de transmission
    // \return bool Vrai si changement effectu�. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool setBaudRate(AbstractSerial::BaudRate newBaudRate);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool setDataBits(AbstractSerial::DataBits newDataBits)
    // \brief Change le nombre de bits de transmission
    //
    // \param newDataBits Nouveau nombre de bits de transmission
    // \return bool Vrai si changement effectu�. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool setDataBits(AbstractSerial::DataBits newDataBits);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool setParity(AbstractSerial::Parity newParity)
    // \brief Change le type de parit�
    //
    // \param newParity Nouveau type de parit�
    // \return bool Vrai si changement effectu�. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool setParity(AbstractSerial::Parity newParity);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn CommunicationSerie();
    // \brief Change le nombre de bits de stop
    //
    // \param newStopBits Nouveau nombre de bits de stop
    // \return bool Vrai si changement effectu�. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool setStopBits(AbstractSerial::StopBits newStopBits);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool setFlowControl(AbstractSerial::Flow newFlowControl)
    // \brief Change le type de controle de flux
    //
    // \param newFlowControl Nouveau type de controle de flux
    // \return bool Vrai si changement effectu�. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool setFlowControl(AbstractSerial::Flow newFlowControl);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool setCharTimeOut(int newCharTimeOut)
    // \brief Change l'Intervalle de timeout en ms
    //
    // \param newCharTimeOut Nouvel intervalle de timeout en ms
    // \return bool Vrai si changement effectu�. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    // Changer Intervalle de timeout en ms
    bool setCharTimeOut(int newCharTimeOut);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool open(AbstractSerial::OpenMode openMode)
    // \brief Ouvrir le P�riph�rique pr�configur�
    //
    // \param openMode Mode d'ouverture du p�riph�rique
    // \return bool Vrai si changement effectu�. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool open(AbstractSerial::OpenMode openMode);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool open(int numCom,AbstractSerial::OpenMode openMode)
    // \brief Ouvrir le P�riph�rique en sp�cifiant le num�ro de port com
    //
    // \param numCom Num�ro de port COM
    // \param openMode Mode d'ouverture du p�riph�rique
    // \return bool Vrai si changement effectu�. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool open(int numCom,AbstractSerial::OpenMode openMode);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool open(QString deviceName,AbstractSerial::OpenMode openMode)
    // \brief Ouvrir le P�riph�rique en sp�cifiant l'identifiant
    //
    // \param deviceName identifiant du p�riph�rique
    // \param openMode Mode d'ouverture du p�riph�rique
    // \return bool Vrai si changement effectu�. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool open(QString deviceName,AbstractSerial::OpenMode openMode);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool isOpen()
    // \brief Retourne un bool�en indiquant si la communication avec le p�riph�rique est ouverte
    //
    // \return bool Vrai si le p�riph�rique est ouvert. Faux sinon.
    ///////////////////////////////////////////////////////////////////////////*/
    bool isOpen();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void close()
    // \brief Fermer la communication
    ///////////////////////////////////////////////////////////////////////////*/
    void close();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void write(QString data)
    // \brief Ecrire sur le port com configur�
    //
    // \param data Trame � tansmettre
    ///////////////////////////////////////////////////////////////////////////*/
    void write(QString data);

public slots:

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void sendData(QString data)
    // \brief Slot lancant l'�criture sur le port com configur�
    //
    // \param data Trame � tansmettre
    ///////////////////////////////////////////////////////////////////////////*/
    void sendData(QString data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void bytesWrittenSlot(qint64 bytes)
    // \brief Slot tra�ant le nombre d'octets �crits
    //
    // \param bytes Nombre d'octets �crits
    ///////////////////////////////////////////////////////////////////////////*/
    void bytesWrittenSlot(qint64 bytes);

signals:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn void dataReaded(QString data)
    // \brief Signal �mis lors de la lecture de donn�es sur le p�riph�rique
    //
    // \param data Donn�es lues
    ///////////////////////////////////////////////////////////////////////////*/
    void dataReaded(QString data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void transmissionError()
    // \brief Signal �mis lorsqu'une erreur de transmission se produit
    ///////////////////////////////////////////////////////////////////////////*/
    void transmissionError();

};

#endif // COMMUNICATIONSERIE_H
