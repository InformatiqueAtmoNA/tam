#include "api.h"

Api::Api(QString const & adressePeriph, TypePeripherique const & typePeriph, Protocoles const & protocoleUtilise, TypePolluant const & typePolluant)
{
    this->adresse = adressePeriph;
    this->typePeripherique = typePeriph;
    this->polluantAssocie = typePolluant;
    this->versionProtocole = protocoleUtilise;
}

// \brief Cr�e une trame en fonction du num�ro de commande API
QString* Api::creerTrameCommande(QString const & typeCommande,QString const & data) {
    QString* trame = new QString();
    trame->append(typeCommande);
    trame->append(" ");
    trame->append(this->adresse);
    trame->append(" ");
    trame->append(data);
    trame->append(CR);
    return trame;
}

float Api::getFloatFromMesureString(QString const & mesure) {
    int indexDebutMesure = mesure.indexOf("=")+1;
    int indexFinMesure = mesure.indexOf(" ",indexDebutMesure);

    return mesure.mid(indexDebutMesure,indexFinMesure-indexDebutMesure).toFloat();
}

QVector<float>* Api::demandeMesureNox() {
    QString cmd;
    QVector<float>* tabMesures = new QVector<float>(3);

    for(int i=0;i<3;i++) {
        switch(i) {
        case 0:
            cmd = *(this->creerTrameCommande("T","NO"));
            break;
        case 1:
            cmd = *(this->creerTrameCommande("T","NO2"));
            break;
        case 2:
            cmd = *(this->creerTrameCommande("T","NOX"));
            break;
        }

        QString reponse = this->transaction(cmd);
        if(reponse.isEmpty())
            return NULL;

        (*tabMesures)[i]=this->getFloatFromMesureString(reponse);
    }
    return tabMesures;
}

// Demande de mesure imm�diate
QVector<float>* Api::demandeMesure() {
    QString cmd;

    switch(this->polluantAssocie) {
    case CO :
        cmd = *(this->creerTrameCommande("T","CO"));
        break;
    case O3 :
        cmd = *(this->creerTrameCommande("T","O3"));
        break;
    case SO2 :
        cmd = *(this->creerTrameCommande("T","SO2"));
        break;
    case NOX:
        return this->demandeMesureNox();
        break;
    default:
       emit(this->erreurCommande());
    }
    QString reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return NULL;

    QVector<float>* mesures = new QVector<float>(1);

    (*mesures)[0] = this->getFloatFromMesureString(reponse);

    return mesures;
}

// Demande d'alarme
ushort Api::demandeAlarme() {
    QString cmd = *(this->creerTrameCommande("W","LIST ALL HEX"));

    QString reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return NULL;

    reponse.remove(0,reponse.indexOf("0x")+2);

    bool conversionOK;
    int codeAlarme = reponse.toInt(&conversionOK,16);
    if(!conversionOK) {
        emit(this->erreurCommande());
        return 1;
    }
    if(codeAlarme>0)
        emit(this->alarmeGenerale());
    return codeAlarme;
}

// Demande de passage en mode zero
void Api::passageZero() {
    QString cmd = *(this->creerTrameCommande("C","ZERO"));
    this->transaction(cmd);
}

// Demande de passage en mode �talon
void Api::passageEtalon() {
    QString cmd = *(this->creerTrameCommande("C","SPAN"));
    this->transaction(cmd);
}

// Demande de passage en mode mesure
void Api::passageMesure() {
    QString cmd = *(this->creerTrameCommande("C","ABORT"));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre � un certain point de gaz
void Api::commandeSpan(SpanHandler const & spanData) {
    QString data = "GENERATE "+ QString::number(spanData.getPoint()) + " PPB " + spanData.getCanal();
    QString cmd = *(this->creerTrameCommande("C",data));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre au point de gaz zero
void Api::commandeSpanZero(QString const & canal) {
    SpanHandler* spanData = new SpanHandler();
    spanData->setSpanArguments(canal,0,0);
    this->commandeSpan(*spanData);
    delete spanData;
}

// Commande au diluteur de se mettre � un certain point de gaz
void Api::commandeSpanTpg(SpanHandler const & spanData) {
    QString data = "GPT "+ QString::number(spanData.getPoint()) + " PPB " + QString::number(spanData.getConcO3()) + " PPB";
    QString cmd = *(this->creerTrameCommande("C",data));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre � un certain point de gaz O3
void Api::commandeSpanO3(SpanHandler const & spanData) {
    QString data = "GENERATE "+ QString::number(spanData.getConcO3()) + " PPB O3";
    QString cmd = *(this->creerTrameCommande("C",data));
    this->transaction(cmd);
}

// Mise en stand-by du p�riph�rique
bool Api::standBy() {
    QString cmd = *(this->creerTrameCommande("C","STANDBY"));
    this->transaction(cmd);
    return true;
}

// Renvoie la liste des commandes autoris�es par le protocole
QVector<Commandes> const* Api::getListeCommandes() {
    QVector<Commandes>* commandesAutorisees;

    if(this->versionProtocole == API_ANA) {
        commandesAutorisees = new QVector<Commandes>(5);
        (*commandesAutorisees)[0] = MESURES;
        (*commandesAutorisees)[1] = ALARME;
        (*commandesAutorisees)[2] = MODE_ZERO;
        (*commandesAutorisees)[3] = MODE_ETALON;
        (*commandesAutorisees)[4] = MODE_MESURE;

    }
    else {
        commandesAutorisees = new QVector<Commandes>(6);
        (*commandesAutorisees)[0] = STAND_BY;
        (*commandesAutorisees)[1] = ALARME;
        (*commandesAutorisees)[2] = SPAN;
        (*commandesAutorisees)[3] = SPAN_ZERO;
        (*commandesAutorisees)[4] = SPAN_TPG;
        (*commandesAutorisees)[5] = SPAN_O3;
    }
    return commandesAutorisees;
}

// Renvoie une instance de SpanHandler contenant les infos
// sur les arguments de la commande
SpanHandler* Api::getSpanHandler(Commandes commandeSpan) {
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
