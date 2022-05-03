#ifndef COMMUNICATIONIP_H
#define COMMUNICATIONIP_H

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>


class CommunicationIP: public QObject
{
    Q_OBJECT

    private :

        quint16 port;
        QHostAddress * addr;
        QUdpSocket * udp_socket;
        QTcpSocket * tcp_socket;
        QString socketType;
        bool EtatConnexion;

    public :

        CommunicationIP();
        void setPort(quint16 newPort);
        void setAddr(QString newAddr);
        void setTypeSocket(QString newTypeSocket);
        void bindToHost();
        bool isOpen();
        void close();    
        bool getEtatConnexion() const;

public Q_SLOTS :

        void send_Trame(QString trame);
        void connexion_OK();
        void connexion_perdue();
        void socketRead();

    signals :

        void dataReceived(QString trame);

};

#endif // COMMUNICATIONIP_H
