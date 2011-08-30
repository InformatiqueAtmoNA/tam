/*////////////////////////////////////////////////////
// \file threadcomhandler.cpp
// \brief Classe implémentant un thread gérant la communication RS232
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

#include "threadcomhandler.h"

ThreadComHandler::ThreadComHandler()
{
    flagStop = false;
}

ThreadComHandler::~ThreadComHandler() {
    if(this->comRS232->isOpen())
        this->comRS232->close();
    this->comRS232->deleteLater();
}

void ThreadComHandler::run() {
    while(!flagStop) {
        QCoreApplication::processEvents();
    }
    this->comRS232->close();
}

// Configure la liaison RS232
void ThreadComHandler::configureRS232(const QString deviceName) {
    this->comRS232 = new CommunicationSerie(deviceName);

    if(!this->comRS232->open(AbstractSerial::ReadWrite)) {
        qDebug() << "Problème lors de l'ouverture du périphérique " << deviceName;
        emit(this->ouverturePort(false));
        return;
    }
    else {
        qDebug() << "Ouverture du périphérique " << deviceName << " réussie";
        emit(this->ouverturePort(true));
    }
    if(!comRS232->setBaudRate(AbstractSerial::BaudRate9600)) {
        this->comRS232->close();
        emit(this->ouverturePort(false));
        return;
    }
    if(!comRS232->setDataBits(AbstractSerial::DataBits8)) {
        this->comRS232->close();
        emit(this->ouverturePort(false));
        return;
    }
    if(!comRS232->setParity(AbstractSerial::ParityNone)) {
        this->comRS232->close();
        emit(this->ouverturePort(false));
        return;
    }
    if(!comRS232->setStopBits(AbstractSerial::StopBits1)) {
        this->comRS232->close();
        emit(this->ouverturePort(false));
        return;
    }
    if(!comRS232->setFlowControl(AbstractSerial::FlowControlOff)) {
        this->comRS232->close();
        emit(this->ouverturePort(false));
        return;
    }
    if(!comRS232->setCharTimeOut(200)) {
        this->comRS232->close();
        emit(this->ouverturePort(false));
        return;
    }

    // Connection signaux/slots de lecture/écriture
    connect(this,SIGNAL(envoiTrame(QString)),this->comRS232,SLOT(sendData(QString)));
    connect(this->comRS232,SIGNAL(dataReaded(QString)),this,SIGNAL(receptionTrame(QString)));
}

void ThreadComHandler::stop() {
    this->flagStop = true;
    if(this->comRS232->isOpen())
        this->comRS232->close();
}
