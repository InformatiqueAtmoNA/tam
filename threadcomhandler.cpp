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

ThreadComHandler::ThreadComHandler(QString typeConnexion)
{
    flagStop = false;
    this->m_typeConnexion=typeConnexion;
}

ThreadComHandler::~ThreadComHandler() {
    if(m_typeConnexion=="RS232"){
        if(this->comRS232->isOpen())
            this->comRS232->close();
        this->comRS232->deleteLater();
    }
    else if(m_typeConnexion=="IP"){
        if(this->comIP->isOpen())
            this->comIP->close();
        this->comIP->deleteLater();
    }
    QCoreApplication::sendPostedEvents();
    QCoreApplication::processEvents();
}

void ThreadComHandler::run() {
    exec();
    qDebug()<<"Fin du thread de communication";
    if(m_typeConnexion=="RS232"){
         this->comRS232->close();
    }
    else if(m_typeConnexion=="IP"){
            this->comIP->close();
    }
}

void ThreadComHandler::configureRS232(const QString deviceName) {
    qDebug() << "Connexion en RS232";

    this->comRS232 = new CommunicationSerie(deviceName);

    if(!this->comRS232->open(QSerialPort::ReadWrite)) {
        qDebug() << "Problème lors de l'ouverture du périphérique " << deviceName;
        emit(this->ouverturePort(false));
        return;
    }
    else {
        qDebug() << "Ouverture du périphérique " << deviceName << " réussie";
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

    // Connection signaux/slots de lecture/écriture
    connect(this,SIGNAL(envoiTrame(QString)),this->comRS232,SLOT(sendData(QString)));
    connect(this->comRS232,SIGNAL(dataReaded(QString)),this,SIGNAL(receptionTrame(QString)));
}

void ThreadComHandler::configureIP(QString IP, quint16 port, QString typeSocket){
    qDebug() << "Connexion en IP";
    this->comIP = new CommunicationIP();
    this->comIP->setPort(port);
    this->comIP->setAddr(IP);
    this->comIP->setTypeSocket(typeSocket);
    this->comIP->bindToHost();

    if(!this->comIP->getEtatConnexion()) {
        qDebug() << "Problème lors de l'ouverture du périphérique " << IP;
        emit(this->ouverturePort(false));
        return;
    }
    else {
        qDebug() << "Ouverture du peripherique " << IP << " réussie";
        emit(this->ouverturePort(true));
    }

    // Connection signaux/slots de lecture/écriture
    connect(this,SIGNAL(envoiTrame(QString)),this->comIP,SLOT(send_Trame(QString)));
    connect(this->comIP,SIGNAL(dataReceived(QString)),this,SIGNAL(receptionTrame(QString)));
}



void ThreadComHandler::stop() {
    this->flagStop = true;
    if(m_typeConnexion=="RS232"){
        if(this->comRS232->isOpen())
            this->comRS232->close();
    }
    else if(m_typeConnexion=="IP"){
        if(this->comIP->isOpen())
            this->comIP->close();
    }
}
