#include "mode4_sx_6000_1808.h"

Mode4_SX_6000_1808::Mode4_SX_6000_1808(QString const & adressePeriph, TypePeripherique const & typePeriph, bool const & accepteFloat)
    : Mode4(adressePeriph,typePeriph,MODE4_SX6000_1808,accepteFloat)
{
}


// Commande au diluteur de se mettre à un certain point de gaz
void Mode4_SX_6000_1808::commandeSpan(SpanHandler const & spanData) {
    ushort point = spanData.getPoint();
    ushort concO3 = spanData.getConcO3();
    QString strPoint = *(this->getStrForNumber(point,8));
    QString strO3 = *(this->getStrForNumber(concO3,8));

    QString data = spanData.getCanal();
    if(spanData.getCanal().toUShort()<10)
        data.prepend('0');

    data.append(strPoint);
    data.append(strO3);

    QString cmd = *(this->creerTrameCommande("03",data));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre au point de gaz zero
void Mode4_SX_6000_1808::commandeSpanZero(QString const & canal) {
    QString cmd;
    QString data = canal;
    if(canal.toUShort()<10)
        data.prepend('0');

    cmd = *(this->creerTrameCommande("02",data));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre à un certain point de gaz O3
void Mode4_SX_6000_1808::commandeSpanO3(SpanHandler const & spanO3Data) {
    this->commandeSpan(spanO3Data);
}

// Commande au diluteur de se mettre à un certain point de gaz
void Mode4_SX_6000_1808::commandeSpanTpg(SpanHandler const & spanTpgData) {
    this->commandeSpan(spanTpgData);
}

// Renvoie la liste des commandes autorisées par le protocole
QVector<Commandes> const* Mode4_SX_6000_1808::getListeCommandes() {
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
