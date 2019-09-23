/*////////////////////////////////////////////////////
// \file communicationserie.h
// \brief Gestion de la communication RS232
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

#ifndef COMMUNICATIONSERIE_H
#define COMMUNICATIONSERIE_H

#include <QtCore>
#include <iostream>
#include <string>
#include <QtCore/qstring.h>
#include <QtSerialPort/QSerialPort>

/*/////////////////////////////////////////////////////
// \class CommunicationSerie
// \brief classe gérant la communication RS232
/////////////////////////////////////////////////////*/
class CommunicationSerie : public QObject
{
    Q_OBJECT
private:

    // Périphérique relié par rs232
    QSerialPort *device;

///////////////////////////////////////////////
// Paramètres de configuration du périphérique
///////////////////////////////////////////////
    int numeroCom; // Numero du port Com
    QString deviceName; // Nom de Périphérique
    QString baudRate; // Taux de transmission
    QString dataBits; // Nombre de bits de transmission
    QString parity; // Type de parité
    QString stopBits; // Bits de stop
    QString flowControl; // Type de controle de flux
    int charTimeOut; // Intervalle de timeout en ms

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void connectSignalsSlots()
    // \brief Effectue les connection entre signaux/slots
    ///////////////////////////////////////////////////////////////////////////*/
    void connectSignalsSlots();

private Q_SLOTS:

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void slotRead()
    // \brief Effectue la lecture sur le port COM configuré
    ///////////////////////////////////////////////////////////////////////////*/
    void slotRead();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void statusSignal(QString status,QDateTime dateTime)
    // \brief Signale un changement de statut du périphérique
    //
    // \param status Changement de statut
    // \param dateTime Date et heure du changement de statut
    ///////////////////////////////////////////////////////////////////////////*/
    void statusSignal(const QString status,const QDateTime dateTime);

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
    // \param numCom Numéro de port X (défini par /dev/ttyX ou COMX)
    ///////////////////////////////////////////////////////////////////////////*/
    CommunicationSerie(int numCom);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn CommunicationSerie()
    // \brief Constructeur de la classe CommunicationSerie
    //
    // \param deviceName Identifiant du port COM (défini par /dev/ttyX ou COMX)
    ///////////////////////////////////////////////////////////////////////////*/
    CommunicationSerie(const QString deviceName);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn CommunicationSerie()
    // \brief Destructeur de la classe CommunicationSerie
    ///////////////////////////////////////////////////////////////////////////*/
    ~CommunicationSerie();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn int getNumeroCom()
    // \brief Retourne le numéro de port COM du périphérique
    //
    // \return int Numéro de port COM
    ///////////////////////////////////////////////////////////////////////////*/
    int getNumeroCom();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn QString getDeviceName()
    // \brief Retourne l'identifiant du port COM du périphérique
    //
    // \return QString Identifiant du port COM du périphérique
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
    // \brief Retourne le type de parité
    //
    // \return QString Type de parité
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
    // \brief Change le numéro de port COM
    //
    // \param newNumCom Nouveau numéro de port COM
    // \return bool Vrai si changement effectué. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool setNumeroCom(const int newNumCom);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool setDeviceName(QString newDeviceName)
    // \brief Change l'identifiant du périphérique
    //
    // \param newDeviceName Nouvel identifiant du périphérique
    // \return bool Vrai si changement effectué. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool setDeviceName(const QString newDeviceName);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool setBaudRate(QSerialPort::BaudRate newBaudRate)
    // \brief Change le taux de transmission
    //
    // \param newBaudRate Nouveau taux de transmission
    // \return bool Vrai si changement effectué. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool setBaudRate(const QSerialPort::BaudRate newBaudRate);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool setDataBits(QSerialPort::DataBits newDataBits)
    // \brief Change le nombre de bits de transmission
    //
    // \param newDataBits Nouveau nombre de bits de transmission
    // \return bool Vrai si changement effectué. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool setDataBits(const QSerialPort::DataBits newDataBits);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool setParity(QSerialPort::Parity newParity)
    // \brief Change le type de parité
    //
    // \param newParity Nouveau type de parité
    // \return bool Vrai si changement effectué. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool setParity(const QSerialPort::Parity newParity);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn CommunicationSerie();
    // \brief Change le nombre de bits de stop
    //
    // \param newStopBits Nouveau nombre de bits de stop
    // \return bool Vrai si changement effectué. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool setStopBits(const QSerialPort::StopBits newStopBits);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool setFlowControl(QSerialPort::Flow newFlowControl)
    // \brief Change le type de controle de flux
    //
    // \param newFlowControl Nouveau type de controle de flux
    // \return bool Vrai si changement effectué. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool setFlowControl(const QSerialPort::FlowControl newFlowControl);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool setCharTimeOut(int newCharTimeOut)
    // \brief Change l'Intervalle de timeout en ms
    //
    // \param newCharTimeOut Nouvel intervalle de timeout en ms
    // \return bool Vrai si changement effectué. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    // Changer Intervalle de timeout en ms
    bool setCharTimeOut(const int newCharTimeOut);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool open(QSerialPort::OpenMode openMode)
    // \brief Ouvrir le Périphérique préconfiguré
    //
    // \param openMode Mode d'ouverture du périphérique
    // \return bool Vrai si changement effectué. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool open(const QSerialPort::OpenMode openMode);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool open(int numCom,QSerialPort::OpenMode openMode)
    // \brief Ouvrir le Périphérique en spécifiant le numéro de port com
    //
    // \param numCom Numéro de port COM
    // \param openMode Mode d'ouverture du périphérique
    // \return bool Vrai si changement effectué. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool open(const int numCom,const QSerialPort::OpenMode openMode);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool open(QString deviceName,QSerialPort::OpenMode openMode)
    // \brief Ouvrir le Périphérique en spécifiant l'identifiant
    //
    // \param deviceName identifiant du périphérique
    // \param openMode Mode d'ouverture du périphérique
    // \return bool Vrai si changement effectué. Faux si erreur
    ///////////////////////////////////////////////////////////////////////////*/
    bool open(const QString deviceName,const QSerialPort::OpenMode openMode);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn bool isOpen()
    // \brief Retourne un booléen indiquant si la communication avec le périphérique est ouverte
    //
    // \return bool Vrai si le périphérique est ouvert. Faux sinon.
    ///////////////////////////////////////////////////////////////////////////*/
    bool isOpen();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void close()
    // \brief Fermer la communication
    ///////////////////////////////////////////////////////////////////////////*/
    void close();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void write(QString data)
    // \brief Ecrire sur le port com configuré
    //
    // \param data Trame à tansmettre
    ///////////////////////////////////////////////////////////////////////////*/
    void write(const QString data);

public Q_SLOTS:

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void sendData(QString data)
    // \brief Slot lancant l'écriture sur le port com configuré
    //
    // \param data Trame à tansmettre
    ///////////////////////////////////////////////////////////////////////////*/
    void sendData(const QString data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void bytesWrittenSlot(qint64 bytes)
    // \brief Slot traçant le nombre d'octets écrits
    //
    // \param bytes Nombre d'octets écrits
    ///////////////////////////////////////////////////////////////////////////*/
    void bytesWrittenSlot(const qint64 bytes);

Q_SIGNALS:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn void dataReaded(QString data)
    // \brief Signal émis lors de la lecture de données sur le périphérique
    //
    // \param data Données lues
    ///////////////////////////////////////////////////////////////////////////*/
    void dataReaded(const QString data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void transmissionError()
    // \brief Signal émis lorsqu'une erreur de transmission se produit
    ///////////////////////////////////////////////////////////////////////////*/
    void transmissionError();

};

#endif // COMMUNICATIONSERIE_H
