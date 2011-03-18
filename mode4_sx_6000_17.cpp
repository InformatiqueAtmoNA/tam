#include "mode4_sx_6000_17.h"

Mode4_SX_6000_17::Mode4_SX_6000_17(QString adressePeriph, TypePeripherique typePeriph, bool accepteFloat)
    : Mode4(adressePeriph,typePeriph,accepteFloat)
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
void Mode4_SX_6000_17::commandeSpan(ushort canal,ushort point, ushort concO3) {
    if(concO3 > 0) {
        this->commandeSpanO3(canal,concO3);
        return;
    }
    QString strPoint = *this->getStrForNumber(point,2);

    QString data = QString::number(canal);
    if(canal<10)
        data.prepend('0');

    data.append(strPoint);

    QString cmd = *(this->creerTrameCommande("03",data));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre au point de gaz zero
void Mode4_SX_6000_17::commandeSpanZero(ushort canal) {
    QString cmd;
    QString data = QString::number(canal);
    if(canal<10)
        data.prepend('0');

    cmd = *(this->creerTrameCommande("02",data));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre à un certain point de gaz
void Mode4_SX_6000_17::commandeSpanTpg(ushort canalNox,ushort point,ushort concO3) {
    QString strPoint = *this->getStrForNumber(point,2);
    QString strO3 = *this->getStrForNumber(concO3,3);

    QString data = strPoint+strO3;

    QString cmd = *(this->creerTrameCommande("04",data));
    this->transaction(cmd);
}

void Mode4_SX_6000_17::commandeSpanO3(ushort canal,ushort concO3) {
    QString cmd;
    if(concO3==0)
        cmd = *(this->creerTrameCommande("06"));
    else {
        QString strPoint = *this->getStrForNumber(concO3,3);
        cmd = *(this->creerTrameCommande("07",strPoint));
    }
    this->transaction(cmd);
}



