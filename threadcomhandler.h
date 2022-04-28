/*////////////////////////////////////////////////////
// \file threadcomhandler.h
// \brief Classe impl�mentant un thread g�rant la communication RS232
// \author FOUQUART Christophe
// \version 1.0
// \date 31/03/2011
//
// TAM - Tests Automatiques M�trologiques
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

#ifndef THREADCOMHANDLER_H
#define THREADCOMHANDLER_H

#include <QtCore>
#include <QThread>
#include "communicationserie.h"
#include "communicationip.h"

/*////////////////////////////////////////////////////////////////////////////////////
// \class ThreadComHandler
// \brief Classe impl�mentant un thread g�rant la communication RS232
////////////////////////////////////////////////////////////////////////////////////*/

class ThreadComHandler : public QThread
{
    Q_OBJECT
private:
    CommunicationSerie* comRS232; // G�re la communication RS232
    CommunicationIP* comIP; // G�re la communication IP
    bool flagStop; // Indique que le thread doit s'arr�ter

public:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn ThreadComHandler()
    // \brief Constructeur par d�faut
    ///////////////////////////////////////////////////////////////////////////*/
    ThreadComHandler();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn ~ThreadComHandler()
    // \brief Destructeur
    ///////////////////////////////////////////////////////////////////////////*/
    ~ThreadComHandler();

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void run()
    // \brief Ex�cute une boucle infinie permettant d'utiliser les signaux/slots
    ///////////////////////////////////////////////////////////////////////////*/
    void run();

public Q_SLOTS:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn void configureRS232()
    // \brief Configure la liaison RS232
    //
    // \param deviceName Identifiant du p�riph�rique (/dev/tty* ou COM*)
    // \return bool Faux si probleme de connexion
    ///////////////////////////////////////////////////////////////////////////*/
    void configureRS232(const QString deviceName);

    void configureIP(QString IP, quint16 port, QString socketType);
    /*///////////////////////////////////////////////////////////////////////////
    // \fn void stop()
    // \brief Stop le thread
    ///////////////////////////////////////////////////////////////////////////*/
    void stop();

Q_SIGNALS:
    /*///////////////////////////////////////////////////////////////////////////
    // \fn void envoiTrame(QString data)
    // \brief Envoi d'une trame par RS232
    //
    // \param data Trame de commande
    ///////////////////////////////////////////////////////////////////////////*/
    void envoiTrame(const QString data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void envoiTrame(QString data)
    // \brief R�ception d'une trame par RS232
    //
    // \param data Trame de r�ponse
    ///////////////////////////////////////////////////////////////////////////*/
    void receptionTrame(const QString data);

    /*///////////////////////////////////////////////////////////////////////////
    // \fn void ouverturePort(bool reussi)
    // \brief Informe de la r�ussite ou pas de l'ouverture du port s�rie
    //
    // \param reussi Vrai si ouverture r�ussie, faux sinon
    ///////////////////////////////////////////////////////////////////////////*/
    void ouverturePort(const bool reussi);
};

#endif // THREADCOMHANDLER_H
