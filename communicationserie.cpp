/*////////////////////////////////////////////////////
// \file communicationserie.cpp
// \brief Gestion de la communication RS232
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

#include "communicationserie.h"

CommunicationSerie::CommunicationSerie()
{
    this->device = new AbstractSerial(this);
    // Permet l'envoi de signaux de statuts
    device->enableEmitStatus(true);
}

CommunicationSerie::CommunicationSerie(int numCom)
{
    this->device= new AbstractSerial(this);
    this->setNumeroCom(numCom);
    if(this->device->isOpen())
        this->device->close();
    // Permet l'envoi de signaux de statuts
    device->enableEmitStatus(true);
}

CommunicationSerie::CommunicationSerie(QString deviceName)
{
    this->device= new AbstractSerial(this);
    qDebug()<<"Changement du nom de périphérique"<< deviceName;
    this->setDeviceName(deviceName);
    if(this->device->isOpen())
        this->device->close();
    device->enableEmitStatus(true);
}

CommunicationSerie::~CommunicationSerie() {
    if(this->device->isOpen())
        this->device->close();
    this->device->deleteLater();
}

//////////////////////////////////////////////////
// Accesseurs
//////////////////////////////////////////////////
// Obtenir numero du port Com
int CommunicationSerie::getNumeroCom() {
    return this->numeroCom;
}

// Obtenir Nom de l'appareil
QString CommunicationSerie::getDeviceName() {
    return this->deviceName;
}

// Obtenir Taux de transmission
QString CommunicationSerie::getBaudRate() {
    return this->baudRate;
}

// Obtenir Nombre de bits de transmission
QString CommunicationSerie::getDataBits() {
    return this->dataBits;
}

// Obtenir Parité
QString CommunicationSerie::getParity() {
    return this->parity;
}

// Obtenir Bits de stop
QString CommunicationSerie::getStopBits() {
    return this->stopBits;
}

// Obtenir Controle de flux
QString CommunicationSerie::getFlowControl() {
    return this->flowControl;
}

// Obtenir Intervalle de timeout en ms
int CommunicationSerie::getCharTimeOut() {
    return this->charTimeOut;
}

//////////////////////////////////////////////////
// Mutateurs
//////////////////////////////////////////////////

// Changer numero du port Com
bool CommunicationSerie::setNumeroCom(int newNumCom) {
    QString newDeviceName = "/dev/ttyS" + newNumCom;
    if(!this->setDeviceName(newDeviceName)) {
        qDebug() << "Erreur lors de changement de numéro de port com" <<  newNumCom;
        return false;
    }
    this->numeroCom = newNumCom;
    return true;
}

// Changer Nom de l'appareil
bool CommunicationSerie::setDeviceName(QString newDeviceName) {
    if (this->device->isOpen()) {
        qDebug() << "Erreur lors de changement de nom :" <<  newDeviceName;
        qDebug() << "Le périphérique est ouvert";
        return false;
    }
    this->device->setDeviceName(newDeviceName);
    this->deviceName = newDeviceName;

    // connection du signal et du slot de lecture
    if(!connect(device,SIGNAL(readyRead()),this,SLOT(slotRead()))) {
        qDebug() << "Problème lors de la connection du signal de lecture";
    }
    // connection du signal de changement de statut
    if(!connect(device,SIGNAL(signalStatus(QString,QDateTime)),this,SLOT(statusSignal(QString,QDateTime)))) {
        qDebug() << "Problème lors de la connection du signal de changement de statut";
    }
    // connection du signal de changement de statut
    if(!connect(device,SIGNAL(signalStatus(QString,QDateTime)),this,SLOT(statusSignal(QString,QDateTime)))) {
        qDebug() << "Problème lors de la connection du signal de changement de statut";
    }
    return true;
}

// Changer Taux de transmission
bool CommunicationSerie::setBaudRate(AbstractSerial::BaudRate newBaudRate) {
    if (!this->device->setBaudRate(newBaudRate)) {
        qDebug() << "Erreur lors de changement de taux de transmission " << newBaudRate;
        return false;
    }

    this->baudRate = newBaudRate;
    return true;
}

// Changer Nombre de bits de transmission
bool CommunicationSerie::setDataBits(AbstractSerial::DataBits newDataBits) {
    if (!this->device->setDataBits(newDataBits)) {
        qDebug() << "Erreur lors de changement de nombre de bits de transmission" << newDataBits;
        return false;
    }

    this->dataBits = newDataBits;
    return true;
}

// Changer la parité
bool CommunicationSerie::setParity(AbstractSerial::Parity newParity) {
    if (!this->device->setParity(newParity)) {
        qDebug() << "Erreur lors de changement de la parité" << newParity;
        return false;
    }

    this->parity = newParity;
    return true;
}

// Changer Bits de stop
bool CommunicationSerie::setStopBits(AbstractSerial::StopBits newStopBits) {
    if (!this->device->setStopBits(newStopBits)) {
        qDebug() << "Erreur lors de changement des bits de stop" << newStopBits;
        return false;
    }

    this->stopBits = newStopBits;
    return true;
}

// Changer Controle de flux
bool CommunicationSerie::setFlowControl(AbstractSerial::Flow newFlowControl) {
    if (!this->device->setFlowControl(newFlowControl)) {
        qDebug() << "Erreur lors de changement du controle de flux" << newFlowControl;
        return false;
    }

    this->flowControl = newFlowControl;
    return true;
}

// Changer Intervalle de timeout en ms
bool CommunicationSerie::setCharTimeOut(int newCharTimeOut) {
    if (!this->device->setCharIntervalTimeout(newCharTimeOut)) {
        qDebug() << "Erreur lors de changement de l'intervalle de timeout en ms" << newCharTimeOut;
        return false;
    }

    this->charTimeOut = newCharTimeOut;
    return true;
}

/////////////////////////////
// Fonctions de communication
/////////////////////////////

// Ouvrir le Périphérique déja configuré
bool CommunicationSerie::open(AbstractSerial::OpenMode openMode){
    if(!this->device->open(openMode)) {
        qDebug() << "Erreur lors de l'ouverture du périphérique " << this->deviceName;
        return false;
    }
    return true;
}

// Ouvrir le Périphérique en spécifiant le numéro de port com
bool CommunicationSerie::open(int numCom,AbstractSerial::OpenMode openMode) {
    if(!this->setNumeroCom(numCom))
        return false;
    return this->open(openMode);
}

// Ouvrir le Périphérique en spécifiant le nom
bool CommunicationSerie::open(QString deviceName,AbstractSerial::OpenMode openMode) {
    if(!this->setDeviceName(deviceName))
        return false;
    return this->open(openMode);
}

// Retourne un booléen indiquant si la communication avec le périphérique est ouverte
bool CommunicationSerie::isOpen() {
    return this->device->isOpen();
}

// Fermer la communication
void CommunicationSerie::close() {
    this->device->close();
}

void CommunicationSerie::write(QString data) {
    QByteArray baData = data.toAscii();
    if(this->device->write(baData)<0) {
        qDebug() << "Problème lors de l'envoi de la trame" << baData;
        emit(this->transmissionError());
    }
}

/////////////////
// Slots privés
/////////////////

// Lecture sur le port COM configuré
void CommunicationSerie::slotRead() {
     QByteArray data = this->device->readAll();
     qDebug() << "Readed is : " << data.size() << " bytes";
     QString stringData = data;
     emit(dataReaded(stringData));
}

// En cas de changement de statut du périphérique
void CommunicationSerie::statusSignal(QString status,QDateTime dateTime) {
    qDebug() << dateTime.time().toString() << " : " << status << "\n";
}

void CommunicationSerie::bytesWrittenSlot(qint64 bytes) {
    qDebug() << "Ecrit : " << bytes << " octets";
}

/////////////////
// Slots publics
/////////////////
void CommunicationSerie::sendData(QString data) {
    this->write(data);
}
