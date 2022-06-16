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
        /*
         * Définition du port de connexion IP
        */
        void setPort(quint16 newPort);

        /*
         * Définition de l'adresse IP
        */
        void setAddr(QString newAddr);

        /*
         * Définition du type de socket TCP/UDP
        */
        void setTypeSocket(QString newTypeSocket);

        /*
         * Connexion entre deux appareils
        */
        void bindToHost();

        /*
         * Vérification que le canal de communication est ouvert
        */
        bool isOpen();

        /*
         * Fermeture du canal de communication
        */
        void close();

        /*
         * Retourne l'état de la connexion
        */
        bool getEtatConnexion() const;

public Q_SLOTS :

        /*
         * Envoie d'une trame
        */
        void send_Trame(QString trame);

        /*
         * Confirme la connexion
        */
        void connexion_OK();

        /*
         * Prévient d'une perte de connexion
        */
        void connexion_perdue();

        /*
         * Lecture des données reçues
        */
        void socketRead();

    signals :

        /*
         * Prévient que les données ont été reçues
        */
        void dataReceived(QString trame);

};

#endif // COMMUNICATIONIP_H
