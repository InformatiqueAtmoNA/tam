#include "tei_49c.h"

Tei_49c::Tei_49c(QString adressePeriph, TypePeripherique typePeriph, TypePolluant typePolluant,OptionTpg optionTpg)
    :Tei(adressePeriph, typePeriph,typePolluant,optionTpg) {}

// Commande au diluteur de se mettre au point de gaz zero
void Tei_49c::commandeSpanZero() {
    this->commandeSpanO3();
}

// Commande au diluteur de se mettre à un certain point de gaz O3
void Tei_49c::commandeSpanO3(ushort point03) {
    QString trame = "set O3 conc " + QString::number(point03);
    QString cmd = *(this->creerTrameCommande(trame));
    this->transaction(cmd);
}
