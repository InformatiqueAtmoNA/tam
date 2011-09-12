/*////////////////////////////////////////////////////
// \file mode4_sx_6000_17.h
// \brief Classe dérivée de Mode4 pour les diluteurs SX6000 V1.7
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
////////////////////////////////////////////////////*/

#include "mode4_sx_6000_17.h"

Mode4_SX_6000_17::Mode4_SX_6000_17(const QString & adressePeriph, const TypePeripherique & typePeriph, const bool accepteFloat)
    : Mode4(adressePeriph,typePeriph,MODE4_SX6000_17,accepteFloat)
{
}

// Demande d'alarme
ushort Mode4_SX_6000_17::demandeAlarme() {
    QString cmd = *(this->creerTrameCommande("12","\0"));
    QString reponse = this->transaction(cmd);
    ushort codeAlarme = reponse.mid(7,2).toUShort();
    if(codeAlarme > 0)
        emit(this->alarmeGenerale());
    return codeAlarme;
}

// Commande au diluteur de se mettre à un certain point de gaz
void Mode4_SX_6000_17::commandeSpan(const SpanHandler & spanData) {
    QString strPoint = *this->getStrForNumber(spanData.getPoint(),2);

    QString data = spanData.getCanal();
    if(spanData.getCanal().toUShort()<10)
        data.prepend('0');

    data.append(strPoint);

    QString cmd = *(this->creerTrameCommande("03",data));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre au point de gaz zero
void Mode4_SX_6000_17::commandeSpanZero(const QString & canal) {
    QString cmd;
    QString data = canal;
    if(canal.toUShort()<10)
        data.prepend('0');

    cmd = *(this->creerTrameCommande("02",data));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre à un certain point de gaz
void Mode4_SX_6000_17::commandeSpanTpg(const SpanHandler & spanTpgData) {
    QString strPoint = *this->getStrForNumber(spanTpgData.getPoint(),2);
    QString strO3 = *this->getStrForNumber(spanTpgData.getConcO3(),3);

    QString data = strPoint+strO3;

    QString cmd = *(this->creerTrameCommande("04",data));
    this->transaction(cmd);
}

void Mode4_SX_6000_17::commandeSpanO3(const SpanHandler & spanO3Data) {
    QString cmd;

    ushort concO3 = spanO3Data.getConcO3();

    if(concO3==0)
        cmd = *(this->creerTrameCommande("06"));
    else {
        QString strPoint = *this->getStrForNumber(concO3,3);
        cmd = *(this->creerTrameCommande("07",strPoint));
    }
    this->transaction(cmd);
}

// Renvoie la liste des commandes autorisées par le protocole
QVector<Commandes> const* Mode4_SX_6000_17::getListeCommandes() {
    QVector<Commandes>* commandesAutorisees = new QVector<Commandes>(7);

    (*commandesAutorisees)[0] = RESET;
    (*commandesAutorisees)[1] = STAND_BY;
    (*commandesAutorisees)[2] = ALARME;
    (*commandesAutorisees)[3] = SPAN;
    (*commandesAutorisees)[4] = SPAN_ZERO;
    (*commandesAutorisees)[5] = SPAN_TPG;
    (*commandesAutorisees)[6] = SPAN_O3;

    return commandesAutorisees;
}

