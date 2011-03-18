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
}

// Configure la liaison RS232
void ThreadComHandler::configureRS232(QString deviceName) {
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
    if(!comRS232->setCharTimeOut(300)) {
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
