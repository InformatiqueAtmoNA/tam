/*//////////////////////////////////////////////////////////////////////
// \file tei_49ps.h
// \brief Classe dérivée de Tei pour les générateurs d'ozone SONIMIX 49ps
// \author FOUQUART Christophe
// \version 1.0
// \date 31/03/2011
//
// TAM - Tests Automatiques Métrologiques
// Copyright (C) 2011 FOUQUART Christophe
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
//////////////////////////////////////////////////////////////////////*/

#include "tei_49ps.h"

Tei_49ps::Tei_49ps(QString const & adressePeriph, TypePeripherique const & typePeriph, TypePolluant const & typePolluant,OptionTpg const & optionTpg)
    :Tei(adressePeriph, typePeriph,typePolluant,optionTpg) {}

// Commande au diluteur de se mettre à un certain point de gaz
void Tei_49ps::commandeSpan(SpanHandler const & spanData) {
    qDebug()<<"TEI-49ps : commande non supportée :";
    qDebug()<<"CommandeSpan("<<spanData.getCanal()<<","<<spanData.getPoint()<<","<<spanData.getConcO3()<<")";
}

// Commande au diluteur de se mettre au point de gaz zero
void Tei_49ps::commandeSpanZero(QString const & canal) {
    SpanHandler* spanO3Data = new SpanHandler();
    spanO3Data->setSpanArguments(canal,0,0);
    this->commandeSpanO3(*spanO3Data);
    delete spanO3Data;
}

// Commande au diluteur de se mettre à un certain point de gaz
// Commande non supportée par l'appareil
void Tei_49ps::commandeSpanTpg(SpanHandler const & spanTpgData) {
    qDebug()<<"TEI-49ps : commande non supportée :";
    qDebug()<<"CommandeSpan("<<spanTpgData.getCanal()<<","<<spanTpgData.getPoint()<<","<<spanTpgData.getConcO3()<<")";
}

// Commande au diluteur de se mettre à un certain point de gaz O3
void Tei_49ps::commandeSpanO3(SpanHandler const & spanO3Data) {
    QString trame = "set O3 conc " + QString::number(spanO3Data.getConcO3());
    QString cmd = *(this->creerTrameCommande(trame));
    QString reponse = this->transaction(cmd);
    // Si la réponse n'est pas vide alors la commande a réussie
    if(!reponse.isEmpty())
        return;
    // Sinon on essaie la deuxième manière
    trame = "set z/span " + QString::number(spanO3Data.getConcO3());
    cmd = *(this->creerTrameCommande(trame));
    this->transaction(cmd);
}

// Renvoie la liste des commandes autorisées par le protocole
QVector<Commandes> const* Tei_49ps::getListeCommandes() {
    QVector<Commandes>* commandesAutorisees = new QVector<Commandes>(3);

    (*commandesAutorisees)[0] = ALARME;
    (*commandesAutorisees)[1] = SPAN_ZERO;
    (*commandesAutorisees)[2] = SPAN_O3;

    return commandesAutorisees;
}

// Renvoie une instance de SpanHandler contenant les infos
// sur les arguments de la commande
SpanHandler* Tei_49ps::getSpanHandler(Commandes commandeSpan) {
    SpanHandler* infosCommandeSpan = new SpanHandler();
    QVector<bool>* argumentsUtiles = new QVector<bool>(3);
    argumentsUtiles->fill(false);
    switch(commandeSpan) {
    case SPAN_O3:
        (*argumentsUtiles)[CONCO3]=true;
        break;
    default:
        break;
    }
    infosCommandeSpan->setTabArgumentsSpan(*argumentsUtiles);
    return infosCommandeSpan;
}
