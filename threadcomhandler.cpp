/*////////////////////////////////////////////////////
// \file threadcomhandler.cpp
// \brief Classe implementant un thread gerant la communication RS232
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

#include "threadcomhandler.h"

ThreadComHandler::ThreadComHandler()
{
    flagStop = false;
}

ThreadComHandler::~ThreadComHandler() {
    if(this->comRS232->isOpen())
        this->comRS232->close();
    this->comRS232->deleteLater();
    QCoreApplication::sendPostedEvents(); // modifie
    QCoreApplication::processEvents();
}

void ThreadComHandler::run() {
    exec();
    qDebug()<<"Fin du thread de communication";
    this->comRS232->close();
}

// Configure la liaison RS232
void ThreadComHandler::configureRS232(const QString deviceName) {

    this->comRS232 = new CommunicationSerie(deviceName);

    if(!this->comRS232->open(QSerialPort::ReadWrite)) {
        qDebug() << "Problème lors de l'ouverture du peripherique " << deviceName;
        emit(this->ouverturePort(false));
        return;
    }
    else {
        qDebug() << "Ouverture du peripherique " << deviceName << " reussie";
        emit(this->ouverturePort(true));
    }
    if(!comRS232->setBaudRate(QSerialPort::Baud9600)) {
        this->comRS232->close();
        emit(this->ouverturePort(false));
        return;
    }
    if(!comRS232->setDataBits(QSerialPort::Data8)) {
        this->comRS232->close();
        emit(this->ouverturePort(false));
        return;
    }
    if(!comRS232->setParity(QSerialPort::NoParity)) {
        this->comRS232->close();
        emit(this->ouverturePort(false));
        return;
    }
    if(!comRS232->setStopBits(QSerialPort::OneStop)) {
        this->comRS232->close();
        emit(this->ouverturePort(false));
        return;
    }
    if(!comRS232->setFlowControl(QSerialPort::NoFlowControl)) {
        this->comRS232->close();
        emit(this->ouverturePort(false));
        return;
    }
    if(!comRS232->setCharTimeOut(200)) {
        this->comRS232->close();
        emit(this->ouverturePort(false));
        return;
    }

    // Connection signaux/slots de lecture/ecriture
    connect(this,SIGNAL(envoiTrame(QString)),this->comRS232,SLOT(sendData(QString)));
    connect(this->comRS232,SIGNAL(dataReaded(QString)),this,SIGNAL(receptionTrame(QString)));
}

void ThreadComHandler::stop() {
    this->flagStop = true;
    if(this->comRS232->isOpen())
        this->comRS232->close();
}
