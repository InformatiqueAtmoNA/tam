#include "tei_146i.h"

Tei_146i::Tei_146i(QString const & adressePeriph, TypePeripherique const & typePeriph, TypePolluant const & typePolluant,OptionTpg const & optionTpg)
        :Tei(adressePeriph, typePeriph,typePolluant,optionTpg) {}

// Commande au diluteur de se mettre à un certain point de gaz
void Tei_146i::commandeSpan(SpanHandler const & spanData) {
    QString cmd;
    QString reponse;
    cmd = *(this->creerTrameCommande("set ozon off"));
    reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return;
    QString trame = "set gas ";
    trame.append(spanData.getCanal());
    trame.append(" span ");
    trame.append(QString::number(spanData.getPoint()));

    cmd = *(this->creerTrameCommande(trame));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre au point de gaz zero
void Tei_146i::commandeSpanZero(QString const & canal) {
    SpanHandler* spanData = new SpanHandler();
    spanData->setSpanArguments(canal,0,0);
    this->commandeSpan(*spanData);
    delete spanData;
}

// Commande au diluteur de se mettre à un certain point de gaz
void Tei_146i::commandeSpanTpg(SpanHandler const & spanTpgData) {
    QString trame = "set gas ";
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
void Tei_146i::commandeSpanO3(SpanHandler const & spanO3Data) {
    switch(this->optionTpg) {
    case AUCUNE:
        return;
    case OZONISEUR:
        trame = "set ozon ";
        break;
    case PHOTOMETRE:
        trame = "set o3 conc ";
        break;
    }
    trame.append(QString::number(spanO3Data.getConcO3()));
    QString cmd = *(this->creerTrameCommande(trame));
    this->transaction(cmd);
}

// Renvoie la liste des commandes autorisées par le protocole
QVector<Commandes> const* Tei_146i::getListeCommandes() {
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
SpanHandler* Tei_146i::getSpanHandler(Commandes commandeSpan) {
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
