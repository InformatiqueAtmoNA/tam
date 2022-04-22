/*////////////////////////////////////////////////////
// \file Tei.cpp
// \brief Gère l'envoi et la reception de commande Tei
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

#include "tei.h"

Tei::Tei(const QString & adressePeriph,const TypePeripherique & typePeriph,const OptionTpg & optionTpg) {
    bool conversionOK;
    ushort adresseCodeAscii = adressePeriph.toUShort(&conversionOK,10);
    if(conversionOK) {
        char adresseChar = adresseCodeAscii;
        this->adresse = adresseChar;
    }
    else
        this->adresse = adressePeriph;

    this->typePeripherique = typePeriph;
    this->optionTpg = optionTpg;
}

// Cree une trame au format TEI
QString* Tei::creerTrameCommande(const QString & commande) {
    QString* trame = new QString();
    trame->append(this->adresse);
    trame->append(commande);
    trame->append(QChar(char(CR)));
    return trame;
}

// Enclenche le controle a distance
bool Tei::setModeRemote() {
    QString cmd = *(this->creerTrameCommande("set mode remote"));
    QString reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return false;
    if(reponse.contains("set mode remote ok"))
        return true;
    return false;
}

// Parametrage du format de trame
bool Tei::setFormat() {
    QString cmd = *(this->creerTrameCommande("set format 00"));
    QString reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return false;
    if(reponse.contains("set mode remote ok"))
        return true;
    return false;
}


// Initialisation des parametres du peripherique
bool Tei::init() {
    this->setModeRemote();
    this->setFormat();
    return true;
}

// Regle l'appareil sur son mode de fonctionnement par defaut
bool Tei::parDefault() {
    this->setModeRemote();
    return true;
}

// Renvoie l'offsetdu peripherique
short Tei::offset() {
    return 0;
}

// Renvoi le float contenu dans la trame de mesure
float Tei::getFloatFromMesureString(QString reponse) {
    reponse.remove(0,reponse.indexOf(" ")+1);
    reponse.remove(reponse.indexOf(" "),reponse.length()-reponse.indexOf(" "));
    return reponse.toFloat();
}

// Demande de mesure immediate pour le NOX
QPointer<MesureIndividuelle> Tei::demandeMesureNox() {
    QString cmd;
    QPointer<MesureIndividuelle> tabMesures(new MesureIndividuelle());

    for(int i=0;i<3;i++) {
        switch(i) {
        case 0:
            cmd = *(this->creerTrameCommande("no"));
            break;
        case 1:
            cmd = *(this->creerTrameCommande("nox"));
            break;
        case 2:
            cmd = *(this->creerTrameCommande("no2"));
            break;
        }

        QString reponse = this->transaction(cmd);
        if(reponse.isEmpty())
            return tabMesures;
        if(!reponse.contains("inf"))
            tabMesures.data()->append(this->getFloatFromMesureString(reponse));
    }
    return tabMesures;
}

// Demande de mesure immediate
QPointer<MesureIndividuelle> Tei::demandeMesure() {
    QString cmd;

    switch(this->polluantAssocie) {
    case CO :
        cmd = *(this->creerTrameCommande("co"));
        break;
    case O3 :
        cmd = *(this->creerTrameCommande("o3"));
        break;
    case SO2 :
        cmd = *(this->creerTrameCommande("so2"));
        break;
    case NO:
    case NO2:
    case NOX:
        return this->demandeMesureNox();
        break;
    default:
       emit(this->erreurCommande());
    }

    QString reponse = this->transaction(cmd);
    QPointer<MesureIndividuelle> tabMesures(new MesureIndividuelle());

    if(reponse.isEmpty())
        return tabMesures;

    tabMesures.data()->append(this->getFloatFromMesureString(reponse));

    return tabMesures;
}

// Demande d'alarme
ushort Tei::demandeAlarme() {
    QString cmd = *(this->creerTrameCommande("flags"));
    QString reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return 0;

    if(reponse.contains("*"))
        reponse.remove("*");
    reponse.remove(reponse.length()-1,1);
    QString flagsAlarme = reponse.right(4);

    for(int i=0;i<4;i++) {
        if(flagsAlarme.at(i)!='0') {
            qDebug()<<"Alarme generale : "<<flagsAlarme;
            emit(this->alarmeGenerale());
            return 1;
        }
    }

    return 0;
}

// Demande d'heure
QDateTime* Tei::demandeDateHeure() {
    QString cmd = *(this->creerTrameCommande("date"));
    QString date = this->transaction(cmd);
    if(date.isEmpty())
        return NULL;
    cmd = *(this->creerTrameCommande("time"));
    QString time = this->transaction(cmd);
    if(time.isEmpty())
        return NULL;

    QDateTime* dateTimeConfig;
    QTime timeConfig;
    QDate dateConfig;

    dateConfig.setDate((date.mid(5,2).toInt()),
                       (date.mid(8,2).toInt()),
                       (date.mid(11,2).toInt()));
    timeConfig.setHMS(time.mid(5,2).toInt(),
                      time.mid(8,2).toInt(),
                      time.mid(11,2).toInt());
    dateTimeConfig = new QDateTime();
    dateTimeConfig->setDate(dateConfig);
    dateTimeConfig->setTime(timeConfig);
    return dateTimeConfig;
}

// Demande de passage en mode zero
void Tei::passageZero() {
    QString cmd = *(this->creerTrameCommande("set zero"));
    this->transaction(cmd);
}

// Demande de passage en mode etalon
void Tei::passageEtalon() {
    QString cmd = *(this->creerTrameCommande("set span"));
    this->transaction(cmd);
}

// Demande de passage en mode mesure
void Tei::passageMesure() {
    QString cmd = *(this->creerTrameCommande("set sample"));
    this->transaction(cmd);
}

// mise en stand-by du peripherique
bool Tei::standBy() {
    QString cmd = *(this->creerTrameCommande("set gas off"));
    QString reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return true;
    cmd = *(this->creerTrameCommande("set ozon off"));
    this->transaction(cmd);
    return true;
}

// Renvoie la liste des commandes autorisees par le protocole
QVector<Commandes> const* Tei::getListeCommandes() {
    QVector<Commandes>* commandesAutorisees = new QVector<Commandes>(6);
    (*commandesAutorisees)[0] = DATE_HEURE;
    (*commandesAutorisees)[1] = MESURES;
    (*commandesAutorisees)[2] = ALARME;
    (*commandesAutorisees)[3] = MODE_ZERO;
    (*commandesAutorisees)[4] = MODE_ETALON;
    (*commandesAutorisees)[5] = MODE_MESURE;

    return commandesAutorisees;
}

// Renvoie une instance de SpanHandler contenant les infos
// sur les arguments de la commande
SpanHandler* Tei::getSpanHandler(Commandes commandeSpan) {
    SpanHandler* infosCommandeSpan = new SpanHandler();
    QVector<bool>* argumentsUtiles = new QVector<bool>(3);
    switch(commandeSpan) {
     default:
        argumentsUtiles->fill(false);
    }
    infosCommandeSpan->setTabArgumentsSpan(*argumentsUtiles);
    return infosCommandeSpan;
}
