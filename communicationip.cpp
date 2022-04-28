#include "communicationip.h"
#include <QDebug>





CommunicationIP::CommunicationIP()
{
    tcp_socket = new QTcpSocket;
    udp_socket = new QUdpSocket;
    connect(this->tcp_socket,SIGNAL(connected()),this,SLOT(connexion_OK()));
    connect(this->udp_socket,SIGNAL(connected()),this,SLOT(connexion_OK()));
}

void CommunicationIP::setSocketType(const QString &newSocketType)
{
    socketType = newSocketType;
}

void CommunicationIP::setPort(quint16 newPort)
{
    port = newPort;
}

void CommunicationIP::setAddr(QString newAddr)
{
    addr = new QHostAddress(newAddr) ;
}


void CommunicationIP::bindToHost()
{
    if(socketType=="UDP"){
        udp_socket->connectToHost(*addr,port);
    }
    else if(socketType=="TCP"){
        tcp_socket->connectToHost(*addr,port);
    }
}

//slot
void CommunicationIP::send_Trame(QString &trame)
{
    if(socketType=="UDP"){
        udp_socket->writeDatagram(trame.toLatin1(),*addr,port);
    }
    else if(socketType=="TCP"){
        tcp_socket->write(trame.toLatin1());
        tcp_socket->flush();
    }
}

void CommunicationIP::connexion_OK()
{
    qDebug() << "Connecté" << "\n";
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
        QByteArray buffer;
        buffer=tcp_socket->readAll();
        QString trameRecue = buffer.data();
        emit(dataReceived(trameRecue));
    }

}


