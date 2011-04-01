#include "tei_146c.h"

Tei_146c::Tei_146c(QString const & adressePeriph, TypePeripherique const & typePeriph, TypePolluant const & typePolluant,OptionTpg const & optionTpg)
    :Tei(adressePeriph, typePeriph,typePolluant,optionTpg) {}

// Mise en stand-by du p�riph�rique
bool Tei_146c::standBy() {
    QString cmd = *(this->creerTrameCommande("set flow mode 0"));
    this->transaction(cmd);
    return true;
}

// Commande au diluteur de se mettre � un certain point de gaz
void Tei_146c::commandeSpan(SpanHandler const & spanData) {
    QString cmd;
    QString canal = spanData.getCanal();
    if(canal.isEmpty())
        return;
    trame = "set flow mode ";

    switch(canal.at(0).toAscii()) {
    case 'a':
        trame.append("2");
        break;
    case 'b':
        trame.append("3");
        break;
    case 'c':
        trame.append("4");
        break;
    default:
        return;
    }

    cmd = *(this->creerTrameCommande(trame));
    QString reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return;

    trame = "set gas dil ";
    trame.append(spanData.getCanal());
    trame.append(" ");
    trame.append(QString::number(spanData.getPoint()));

    cmd = *(this->creerTrameCommande(trame));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre au point de gaz zero
void Tei_146c::commandeSpanZero(QString const & canal) {
    QString cmd = canal+*(this->creerTrameCommande("set flow mode 1"));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre � un certain point de gaz
void Tei_146c::commandeSpanTpg(SpanHandler const & spanTpgData) {
    QString cmd = *(this->creerTrameCommande("set flow mode 7"));
    QString reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return;
    QString trame = "set ozone level " + QString::number(spanTpgData.getConcO3());
    cmd = *(this->creerTrameCommande(trame));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre � un certain point de gaz O3
void Tei_146c::commandeSpanO3(SpanHandler const & spanO3Data) {
    QString cmd = *(this->creerTrameCommande("set flow mode 6"));
    QString reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return;
    QString trame = "set ozone level " + QString::number(spanO3Data.getConcO3());
    cmd = *(this->creerTrameCommande(trame));
    this->transaction(cmd);
}

// Renvoie la liste des commandes autoris�es par le protocole
QVector<Commandes> const* Tei_146c::getListeCommandes() {
    QVector<Commandes>* commandesAutorisees = new QVector<Commandes>(6);

    (*commandesAutorisees)[0] = STAND_BY;
    (*commandesAutorisees)[1] = ALARME;
    (*commandesAutorisees)[2] = SPAN;
    (*commandesAutorisees)[3] = SPAN_ZERO;
    (*commandesAutorisees)[4] = SPAN_TPG;
    (*commandesAutorisees)[5] = SPAN_O3;

    return commandesAutorisees;
}

// Renvoie une instance de SpanHandler contenant les infos
// sur les arguments de la commande
SpanHandler* Tei_146c::getSpanHandler(Commandes commandeSpan) {
    SpanHandler* infosCommandeSpan = new SpanHandler();
    QVector<bool>* argumentsUtiles = new QVector<bool>(3);
    argumentsUtiles->fill(false);
    switch(commandeSpan) {
    case SPAN:
        (*argumentsUtiles)[CANAL]=true;
        (*argumentsUtiles)[POINT]=true;
        break;
    case SPAN_TPG:
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
