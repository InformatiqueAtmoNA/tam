#include "tei_146c_v2.h"

Tei_146c_v2::Tei_146c_v2(const QString & adressePeriph, const TypePeripherique & typePeriph, const OptionTpg & optionTpg)
        :Tei(adressePeriph, typePeriph,optionTpg) {}

// Commande au diluteur de se mettre à un certain point de gaz
void Tei_146c_v2::commandeSpan(const SpanHandler & spanData) {
    QString cmd;
    QString reponse;
    cmd = *(this->creerTrameCommande("sel ozon perm off"));
    reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return;
    QString trame = "sel gas ";
    trame.append(spanData.getCanal());
    trame.append(" span ");
    trame.append(QString::number(spanData.getPoint()));

    cmd = *(this->creerTrameCommande(trame));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre au point de gaz zero
void Tei_146c_v2::commandeSpanZero(const QString & canal) {
    SpanHandler* spanData = new SpanHandler();
    spanData->setSpanArguments(canal,0,0);
    this->commandeSpan(*spanData);
    delete spanData;
}

// Commande au diluteur de se mettre à un certain point de gaz
void Tei_146c_v2::commandeSpanTpg(const SpanHandler & spanTpgData) {
    QString trame = "sel gas ";
    trame.append(spanTpgData.getCanal());
    trame.append(" span ");
    trame.append(QString::number(spanTpgData.getPoint()));

    QString cmd = *(this->creerTrameCommande(trame));
    QString reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return;

    this->commandeSpanO3(spanTpgData);
}

// Commande au diluteur de se mettre à un certain point de gaz O3
void Tei_146c_v2::commandeSpanO3(const SpanHandler & spanO3Data) {
    switch(this->optionTpg) {
    case AUCUNE:
        return;
    default:
        trame = "sel ozon ";
        break;
    }
    trame.append(QString::number(spanO3Data.getConcO3()));
    QString cmd = *(this->creerTrameCommande(trame));
    this->transaction(cmd);
}

// Renvoie la liste des commandes autorisées par le protocole
QVector<Commandes> const* Tei_146c_v2::getListeCommandes() {
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
SpanHandler* Tei_146c_v2::getSpanHandler(Commandes commandeSpan) {
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
        (*argumentsUtiles)[CANAL]=true;
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
