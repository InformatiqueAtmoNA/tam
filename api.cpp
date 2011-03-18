#include "api.h"

Api::Api(QString adressePeriph, TypePeripherique typePeriph, TypePolluant typePolluant)
{
    this->adresse = adressePeriph;
    this->typePeripherique = typePeriph;
    this->polluantAssocie = typePolluant;
}

// \brief Crée une trame en fonction du numéro de commande API
QString* Api::creerTrameCommande(QString typeCommande,QString data) {
    QString* trame = new QString();
    trame->append(typeCommande);
    trame->append(" ");
    trame->append(this->adresse);
    trame->append(" ");
    trame->append(data);
    trame->append(CR);
    return trame;
}

float Api::getFloatFromMesureString(QString mesure) {
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

// Demande de mesure immédiate
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

// Commande au diluteur de se mettre à un certain point de gaz
void Api::commandeSpan(QString canal,ushort point,ushort concO3) {
    if(concO3>0) {
        if(point>0)
            this->commandeSpanTpg(point,concO3);
        else
            this->commandeSpanO3(concO3);
        return;
    }
    QString data = "GENERATE "+ QString::number(point) + " PPB " + canal;
    QString cmd = *(this->creerTrameCommande("C",data));
    this->transaction(cmd);
}

// Demande de passage en mode zero
void Api::passageZero() {
    QString cmd = *(this->creerTrameCommande("C","ZERO"));
    this->transaction(cmd);
}

// Demande de passage en mode étalon
void Api::passageEtalon() {
    QString cmd = *(this->creerTrameCommande("C","SPAN"));
    this->transaction(cmd);
}

// Demande de passage en mode mesure
void Api::passageMesure() {
    QString cmd = *(this->creerTrameCommande("C","ABORT"));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre à un certain point de gaz
void Api::commandeSpanTpg(ushort pointNO,ushort concO3) {
    QString data = "GPT "+ QString::number(pointNO) + " PPB " + QString::number(concO3) + " PPB";
    QString cmd = *(this->creerTrameCommande("C",data));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre à un certain point de gaz O3
void Api::commandeSpanO3(ushort point03) {
    QString data = "GENERATE "+ QString::number(point03) + " PPB O3";
    QString cmd = *(this->creerTrameCommande("C",data));
    this->transaction(cmd);
}

// Mise en stand-by du périphérique
void Api::standBy() {
    QString cmd = *(this->creerTrameCommande("C","STANDBY"));
    this->transaction(cmd);
}

void Api::commandeSpanZero(QString canal) {
    QString data = "GENERATE 0 PPB " + canal;
    QString cmd = *(this->creerTrameCommande("C",data));
    this->transaction(cmd);
}
