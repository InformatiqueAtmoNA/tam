/*////////////////////////////////////////////////////
// \file communicationserie.cpp
// \brief Gestion de la communication RS232
// \author FOUQUART Christophe
// \version 1.0
// \date 31/03/2011
//
// TAM - Tests Automatiques Metrologiques
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

#include "communicationserie.h"

CommunicationSerie::CommunicationSerie()
{
    this->device = new QSerialPort(this);
    // Permet l'envoi de signaux de statuts
    //device->enableEmitStatus(true);
}

CommunicationSerie::CommunicationSerie(const int numCom)
{
    this->device= new QSerialPort(this);
    this->setNumeroCom(numCom);
    if(this->device->isOpen())
        this->device->close();
    // Permet l'envoi de signaux de statuts
    //device->enableEmitStatus(true);
}

CommunicationSerie::CommunicationSerie(const QString deviceName)
{
    this->device= new QSerialPort(this);
    qDebug()<<"Changement du nom de peripherique"<< deviceName;
    this->setDeviceName(deviceName);
    if(this->device->isOpen())
        this->device->close();
    //device->enableEmitStatus(true);
}

CommunicationSerie::~CommunicationSerie() {
    if(this->device->isOpen())
        this->device->close();
    this->device->deleteLater();
    QCoreApplication::processEvents();
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

// Obtenir Parite
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
bool CommunicationSerie::setNumeroCom(const int newNumCom) {
    QString newDeviceName = &"/dev/ttyS" [ newNumCom];
    if(!this->setDeviceName(newDeviceName)) {
        qDebug() << "Erreur lors de changement de numero de port com" <<  newNumCom;
        return false;
    }
    this->numeroCom = newNumCom;
    return true;
}

// Changer Nom de l'appareil
bool CommunicationSerie::setDeviceName(const QString newDeviceName) {
    if (this->device->isOpen()) {
        qDebug() << "Erreur lors de changement de nom :" <<  newDeviceName;
        qDebug() << "Le peripherique est ouvert";
        return false;
    }
    this->device->setPortName(newDeviceName);
    this->deviceName = newDeviceName;
     connect(device,SIGNAL(readyRead()),this,SLOT(slotRead()));

//    // connection du signal de changement de statut
//    if(!connect(device,SIGNAL(signalStatus(QString,QDateTime)),this,SLOT(statusSignal(QString,QDateTime)))) {
//        qDebug() << "Problème lors de la connection du signal de changement de statut";
//    }
//    // connection du signal de changement de statut
//    if(!connect(device,SIGNAL(signalStatus(QString,QDateTime)),this,SLOT(statusSignal(QString,QDateTime)))) {
//        qDebug() << "Problème lors de la connection du signal de changement de statut";
//    }
    return true;
}

// Changer Taux de transmission
bool CommunicationSerie::setBaudRate(const QSerialPort::BaudRate newBaudRate) {
    if (!this->device->setBaudRate(newBaudRate)) {
        qDebug() << "Erreur lors de changement de taux de transmission " << newBaudRate;
        return false;
    }

    this->baudRate = QString::number(newBaudRate); // modifie
    return true;
}

// Changer Nombre de bits de transmission
bool CommunicationSerie::setDataBits(const QSerialPort::DataBits newDataBits) {
    if (!this->device->setDataBits(newDataBits)) {
        qDebug() << "Erreur lors de changement de nombre de bits de transmission" << newDataBits;
        return false;
    }

    this->dataBits = QString::number(newDataBits); // modifie
    return true;
}

// Changer la parite
bool CommunicationSerie::setParity(const QSerialPort::Parity newParity) {
    if (!this->device->setParity(newParity)) {
        qDebug() << "Erreur lors de changement de la parite" << newParity;
        return false;
    }

    this->parity = QString::number(newParity); // modifie
    return true;
}

// Changer Bits de stop
bool CommunicationSerie::setStopBits(const QSerialPort::StopBits newStopBits) {
    if (!this->device->setStopBits(newStopBits)) {
        qDebug() << "Erreur lors de changement des bits de stop" << newStopBits;
        return false;
    }

    this->stopBits = QString::number(newStopBits); // modifie
    return true;
}

// Changer Controle de flux
bool CommunicationSerie::setFlowControl(const QSerialPort::FlowControl newFlowControl) {
    if (!this->device->setFlowControl(newFlowControl)) {
        qDebug() << "Erreur lors de changement du controle de flux" << newFlowControl;
        return false;
    }


    this->flowControl = QString::number(newFlowControl); // modifie
    return true;
}

// Changer Intervalle de timeout en ms
bool CommunicationSerie::setCharTimeOut(const int newCharTimeOut) {
    //this->device->setCharIntervalTimeout(newCharTimeOut);
    this->charTimeOut = newCharTimeOut;
    return true;
}

/////////////////////////////
// Fonctions de communication
/////////////////////////////

// Ouvrir le Peripherique deja configure
bool CommunicationSerie::open(QSerialPort::OpenMode openMode){
    if(!this->device->open(openMode)) {
        qDebug() << "Erreur lors de l'ouverture du peripherique " << this->deviceName;
        return false;
    }
    return true;
}

// Ouvrir le Peripherique en specifiant le numero de port com
bool CommunicationSerie::open(const int numCom,const QSerialPort::OpenMode openMode) {
    if(!this->setNumeroCom(numCom))
        return false;
    return this->open(openMode);
}

// Ouvrir le Peripherique en specifiant le nom
bool CommunicationSerie::open(const QString deviceName,const QSerialPort::OpenMode openMode) {
    if(!this->setDeviceName(deviceName))
        return false;
    return this->open(openMode);
}

// Retourne un booleen indiquant si la communication avec le peripherique est ouverte
bool CommunicationSerie::isOpen() {
    return this->device->isOpen();
}

// Fermer la communication
void CommunicationSerie::close() {
    this->device->close();
}

void CommunicationSerie::write(const QString data) {
    QByteArray baData = data.toLatin1();
    if(this->device->write(baData)<0) {
        qDebug() << "Problème lors de l'envoi de la trame" << baData;
        emit(this->transmissionError());        
    }
    bool a =device->waitForBytesWritten();
}

/////////////////
// Slots prives
/////////////////

// Lecture sur le port COM configure
void CommunicationSerie::slotRead() {
     QByteArray data = this->device->readAll();
     qDebug() << "Readed is : " << data.size() << " bytes";
     QString stringData;
     for(int i=0 ; i<data.length() ; i++){
         stringData.append(data[i]);
     }
     emit(dataReaded(stringData));
}

// En cas de changement de statut du peripherique
void CommunicationSerie::statusSignal(const QString status,const QDateTime dateTime) {
    qDebug() << dateTime.time().toString() << " : " << status << "\n";
}

void CommunicationSerie::bytesWrittenSlot(qint64 bytes) {
    qDebug() << "Ecrit : " << bytes << " octets";
}

/////////////////
// Slots publics
/////////////////
void CommunicationSerie::sendData(const QString data) {
    this->write(data);
}
