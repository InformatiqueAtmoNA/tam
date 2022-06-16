#include "communicationip.h"
#include <QDebug>


CommunicationIP::CommunicationIP()
{
    EtatConnexion=false;
}

// retourne vrai si le logiciel est connecté à une machine distante
bool CommunicationIP::getEtatConnexion() const
{
    return EtatConnexion;
}

// définition du numero de port de la machine distante
void CommunicationIP::setPort(quint16 newPort)
{
    port = newPort;
}

// définition de l'adresse IP de la machine distante
void CommunicationIP::setAddr(QString newAddr)
{
    addr = new QHostAddress(newAddr) ;
}

// définition du type de socket utilisé pour la communication IP
void CommunicationIP::setTypeSocket(QString newTypeSocket){

    socketType=newTypeSocket;

    if(socketType=="UDP"){
        udp_socket = new QUdpSocket;
        connect(this->udp_socket,SIGNAL(connected()),this,SLOT(connexion_OK()));
        connect(this->udp_socket, SIGNAL(readyRead()), this, SLOT(socketRead()));

    }
    else if(socketType=="TCP"){
        tcp_socket = new QTcpSocket;
        connect(this->tcp_socket, SIGNAL(readyRead()), this, SLOT(socketRead()));
        connect(this->tcp_socket,SIGNAL(connected()),this,SLOT(connexion_OK()));
    }

}

/*
 * liaison de l'application avec une machine distant en utilisant son adresse IP
 * et son numéro de port
*/
void CommunicationIP::bindToHost()
{
    if(socketType=="UDP"){
        udp_socket->connectToHost(*addr,port, QIODevice::ReadWrite);
        udp_socket->waitForConnected(10000);
    }
    else if(socketType=="TCP"){
        tcp_socket->connectToHost(*addr,port, QIODevice::ReadWrite);
        tcp_socket->waitForConnected(10000);
    }
}


// retourne vrai si on peut lire et écrire sur la machine
bool CommunicationIP::isOpen() {
    bool isOPen = false;

    if(socketType=="UDP"){
        isOPen= this->udp_socket->isOpen();
    }
    else if(socketType=="TCP"){
        isOPen= this->tcp_socket->isOpen();
    }
    return isOPen;
}

// arrete la connexion
void CommunicationIP::close() {
    if(socketType=="UDP"){
        return this->udp_socket->close();
    }
    else if(socketType=="TCP"){
        return this->tcp_socket->close();
    }
}




////////////////
///slots
////////////////
void CommunicationIP::send_Trame(QString trame)
{
    QByteArray dataSent = trame.toLatin1();
    if(socketType=="UDP"){
        udp_socket->writeDatagram(dataSent,*addr,port);
    }
    else if(socketType=="TCP"){
        QDataStream l_vStream(&dataSent, QIODevice::WriteOnly);
        tcp_socket->write(trame.toLatin1());
    }
}

void CommunicationIP::connexion_OK()
{
    EtatConnexion = true;
    qDebug() << "Connecté en IP" << "\n";
}

void CommunicationIP::connexion_perdue(){
    EtatConnexion = true;
    qDebug() << "Connexion IP perdue" << "\n";
}

void CommunicationIP::socketRead()
{
    if(socketType=="UDP"){
        QByteArray buffer;
        buffer.resize(udp_socket->pendingDatagramSize());
        this->udp_socket->readDatagram(buffer.data(),buffer.size(),addr,&port);
        QString trameRecue = buffer.data();
        emit(dataReceived(trameRecue));
    }
    else if(socketType=="TCP"){
        QByteArray buffer = tcp_socket->readAll();
        QString trameRecue = buffer.data();
        emit(dataReceived(trameRecue));
    }

}


