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

    public :
        CommunicationIP();

        void setPort(quint16 newPort);

        quint16 getPort() const;

        void setAddr(QString newAddr);

        QHostAddress *getAddr() const;

        void setUdp_socket(QUdpSocket *newUdp_socket);

        QUdpSocket *getUdp_socket() const;

        void setTcp_socket(QTcpSocket *newTcp_socket);

        QTcpSocket *getTcp_socket() const;

        void readSocketType();

        void bindToHost();

        void setSocketType(const QString &newSocketType);

        const QString &getSocketType() const;

    public Q_SLOTS :

        void send_Trame(QString &trame);

    private Q_SLOTS :

        void connexion_OK();
        void socketRead();

    signals :
        void dataReceived(QString);


};

#endif // COMMUNICATIONIP_H
