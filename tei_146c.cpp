#include "tei_146c.h"


Tei_146c::Tei_146c(QString adressePeriph, TypePeripherique typePeriph, TypePolluant typePolluant,OptionTpg optionTpg)
    :Tei(adressePeriph, typePeriph,typePolluant,optionTpg) {}

// Commande au diluteur de se mettre à un certain point de gaz
void Tei_146c::commandeSpan_146c(QString canal,ushort point) {
    QString cmd;
    QString reponse;
    cmd = *(this->creerTrameCommande("set ozon off"));
    reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return;

    trame = "set flow mode ";
    trame.append(canal);
    cmd = *(this->creerTrameCommande(trame));
    reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return;

    trame = "set gas dil ";
    trame.append(canal);
    trame.append(QString::number(point));

    cmd = *(this->creerTrameCommande(trame));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre au point de gaz zero
void Tei_146c::commandeSpanZero() {
    QString cmd = *(this->creerTrameCommande("set flow mode 1"));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre à un certain point de gaz
void Tei_146c::commandeSpanTpg(ushort pointO3) {
    QString cmd = *(this->creerTrameCommande("set flow mode 7"));
    QString reponse = this->transaction(cmd);
    this->commandeSpanO3(pointO3);
}

// Commande au diluteur de se mettre à un certain point de gaz O3
void Tei_146c::commandeSpanO3(ushort point03) {
    QString trame = "set ozon level " + QString::number(point03);
    QString cmd = *(this->creerTrameCommande(trame));
    this->transaction(cmd);
}
