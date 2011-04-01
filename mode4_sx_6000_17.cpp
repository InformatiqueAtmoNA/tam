#include "mode4_sx_6000_17.h"

Mode4_SX_6000_17::Mode4_SX_6000_17(QString const & adressePeriph, TypePeripherique const & typePeriph, bool const & accepteFloat)
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
void Mode4_SX_6000_17::commandeSpan(SpanHandler const & spanData) {
    QString strPoint = *this->getStrForNumber(spanData.getPoint(),2);

    QString data = spanData.getCanal();
    if(spanData.getCanal().toUShort()<10)
        data.prepend('0');

    data.append(strPoint);

    QString cmd = *(this->creerTrameCommande("03",data));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre au point de gaz zero
void Mode4_SX_6000_17::commandeSpanZero(QString const & canal) {
    QString cmd;
    QString data = canal;
    if(canal.toUShort()<10)
        data.prepend('0');

    cmd = *(this->creerTrameCommande("02",data));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre à un certain point de gaz
void Mode4_SX_6000_17::commandeSpanTpg(SpanHandler const & spanTpgData) {
    QString strPoint = *this->getStrForNumber(spanTpgData.getPoint(),2);
    QString strO3 = *this->getStrForNumber(spanTpgData.getConcO3(),3);

    QString data = strPoint+strO3;

    QString cmd = *(this->creerTrameCommande("04",data));
    this->transaction(cmd);
}

void Mode4_SX_6000_17::commandeSpanO3(SpanHandler const & spanO3Data) {
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

