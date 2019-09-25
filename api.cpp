/*////////////////////////////////////////////////////
// \file Api.cpp
// \brief Gère l'envoi et la réception de commande API
// \author FOUQUART Christophe
// \version 1.0
// \date 31/03/2011
//
// TAM - Tests Automatiques Métrologiques
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

#include "api.h"

Api::Api(const QString & adressePeriph, const TypePeripherique & typePeriph, const DesignationProtocole & protocoleUtilise)
{
    this->adresse = adressePeriph;
    this->typePeripherique = typePeriph;
    this->versionProtocole = protocoleUtilise;
}

// \brief Crée une trame en fonction du numéro de commande API
QString* Api::creerTrameCommande(const QString & typeCommande,const QString & data) {
    QString* trame = new QString();
    trame->append(typeCommande);
    trame->append(" ");
    trame->append(this->adresse);
    trame->append(" ");
    trame->append(data);
    trame->append(CR);
    return trame;
}

float Api::getFloatFromMesureString(const QString & mesure) {
    int indexDebutMesure = mesure.indexOf("=")+1;
    int indexFinMesure = mesure.indexOf(" ",indexDebutMesure);

    return mesure.mid(indexDebutMesure,indexFinMesure-indexDebutMesure).toFloat();
}

QWeakPointer<MesureIndividuelle> Api::demandeMesureNox() {
    QString cmd;
    QSharedPointer<MesureIndividuelle> ms(new MesureIndividuelle());
    QWeakPointer<MesureIndividuelle> mesures = ms;
    ms.clear();

    for(int i=0;i<3;i++) {
        switch(i) {
        case 0:
            cmd = *(this->creerTrameCommande("T","NO"));
            break;
        case 1:
            cmd = *(this->creerTrameCommande("T","NOX"));
            break;
        case 2:
            cmd = *(this->creerTrameCommande("T","NO2"));
            break;
        }

        QString reponse = this->transaction(cmd);
        if(reponse.isEmpty())
            return mesures;
        if(!reponse.contains("inf"))
            mesures.data()->append(this->getFloatFromMesureString(reponse));
    }
    return mesures;
}

// Demande de mesure immédiate
QWeakPointer<MesureIndividuelle> Api::demandeMesure() {
    QString cmd;

    switch(this->polluantAssocie) {
    case CO :
        cmd = *(this->creerTrameCommande("T","CO"));
        break;
    case O3 :
        cmd = *(this->creerTrameCommande("T","O3"));
        break;
    case SO2 :
        cmd = *(this->creerTrameCommande("T","SO2"));
        break;
    case NO:
    case NO2:
    case NOX:
        return this->demandeMesureNox();
        break;
    case H2S:
        cmd = *(this->creerTrameCommande("T","H2S"));
        break;
    default:
       emit(this->erreurCommande());
    }
    QString reponse = this->transaction(cmd);

    QSharedPointer<MesureIndividuelle> ms(new MesureIndividuelle());
    QWeakPointer<MesureIndividuelle> mesures = ms;
    ms.clear();

    if(reponse.isEmpty())
        return mesures;

    mesures.data()->append(this->getFloatFromMesureString(reponse));

    return mesures;
}

// Demande d'alarme
ushort Api::demandeAlarme() {
    QString cmd = *(this->creerTrameCommande("W","LIST ALL HEX"));

    QString reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return 0;

    reponse.remove(0,reponse.indexOf("0x")+2);

    bool conversionOK;
    int codeAlarme = reponse.toInt(&conversionOK,16);
    if(!conversionOK) {
        emit(this->erreurCommande());
        return 1;
    }
    if(codeAlarme>0)
        emit(this->alarmeGenerale());
    return codeAlarme;
}

// Demande de passage en mode zero
void Api::passageZero() {
    QString cmd = *(this->creerTrameCommande("C","ZERO"));
    this->transaction(cmd);
}

// Demande de passage en mode étalon
void Api::passageEtalon() {
    QString cmd = *(this->creerTrameCommande("C","SPAN"));
    this->transaction(cmd);
}

// Demande de passage en mode mesure
void Api::passageMesure() {
    QString cmd = *(this->creerTrameCommande("C","ABORT"));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre à un certain point de gaz
void Api::commandeSpan(const SpanHandler & spanData) {
    QString data = "GENERATE "+ QString::number(spanData.getPoint()) + " PPB " + spanData.getCanal();
    QString cmd = *(this->creerTrameCommande("C",data));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre au point de gaz zero
void Api::commandeSpanZero(const QString & canal) {
    //SpanHandler* spanData = new SpanHandler();
    //spanData->setSpanArguments(canal,0,0);
    //this->commandeSpan(*spanData);
    //delete spanData;
    QString cmd = *(this->creerTrameCommande("C","GENERATE ZERO"));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre à un certain point de gaz
void Api::commandeSpanTpg(const SpanHandler & spanData) {
    QString data = "GPT "+ QString::number(spanData.getPoint()) + " PPB " + QString::number(spanData.getConcO3()) + " PPB";
    QString cmd = *(this->creerTrameCommande("C",data));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre à un certain point de gaz O3
void Api::commandeSpanO3(const SpanHandler & spanData) {
    QString data = "GENERATE "+ QString::number(spanData.getConcO3()) + " PPB O3";
    QString cmd = *(this->creerTrameCommande("C",data));
    this->transaction(cmd);
}

// Mise en stand-by du périphérique
bool Api::standBy() {
    QString cmd = *(this->creerTrameCommande("C","STANDBY"));
    this->transaction(cmd);
    return true;
}

// Renvoie la liste des commandes autorisées par le protocole
QVector<Commandes> const* Api::getListeCommandes() {
    QVector<Commandes>* commandesAutorisees;

    if(this->versionProtocole == API_ANA) {
        commandesAutorisees = new QVector<Commandes>(5);
        (*commandesAutorisees)[0] = MESURES;
        (*commandesAutorisees)[1] = ALARME;
        (*commandesAutorisees)[2] = MODE_ZERO;
        (*commandesAutorisees)[3] = MODE_ETALON;
        (*commandesAutorisees)[4] = MODE_MESURE;

    }
    else {
        commandesAutorisees = new QVector<Commandes>(6);
        (*commandesAutorisees)[0] = STAND_BY;
        (*commandesAutorisees)[1] = ALARME;
        (*commandesAutorisees)[2] = SPAN;
        (*commandesAutorisees)[3] = SPAN_ZERO;
        (*commandesAutorisees)[4] = SPAN_TPG;
        (*commandesAutorisees)[5] = SPAN_O3;
    }
    return commandesAutorisees;
}

// Renvoie une instance de SpanHandler contenant les infos
// sur les arguments de la commande
SpanHandler* Api::getSpanHandler(Commandes commandeSpan) {
    SpanHandler* infosCommandeSpan = new SpanHandler();
    QVector<bool>* argumentsUtiles = new QVector<bool>(3);
    argumentsUtiles->fill(false);
    switch(commandeSpan) {
    case SPAN:
        (*argumentsUtiles)[CANAL]=true;
        (*argumentsUtiles)[POINT]=true;
        break;
    case SPAN_ZERO:
        (*argumentsUtiles)[CANAL]=true;
        break;
    case SPAN_TPG:
        (*argumentsUtiles)[POINT]=true;
        (*argumentsUtiles)[CONCO3]=true;
        break;
    case SPAN_O3:
        (*argumentsUtiles)[CONCO3]=true;
        break;
    default:
        break;
    }
    infosCommandeSpan->setTabArgumentsSpan(*argumentsUtiles);
    return infosCommandeSpan;
}
