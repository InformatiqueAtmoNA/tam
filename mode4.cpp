#include "mode4.h"

Mode4::Mode4() {

}

Mode4::Mode4(QString adressePeriph, TypePeripherique typePeriph, bool accepteFloat) {
    this->adresse = adressePeriph;
    this->typePeripherique = typePeriph;
    this->accepteMesureFloat = accepteFloat;
    this->flagEtatCom = ETAT_REPOS;
}

Mode4::~Mode4() {

}

// Cree une trame en fonction du numero de commande MODE4
QString* Mode4::creerTrameCommande(QString noCommande, QString data) {
    QString* trame = new QString();
    trame->append(STX);
    trame->append(this->adresse);
    trame->append(noCommande);
    trame->append(data);
    trame->append(calculerBCC(*trame));
    trame->append(ETX);
    return trame;
}

// Calcule du BCC (Block Check Character)
// Le BCC est obtenu en effectuant un OU exclusif (XOR)
// sur tous les octets dà partir de STX exclu jusqu'au BCC exclu
QString* Mode4::calculerBCC(QString trame) {
    char bcc = 0x00;
    for(int i=1;i<trame.length();i++)
        bcc ^= trame[i].toAscii();
    QString* strBcc = new QString();
    int ibcc = static_cast<int>(bcc);
    strBcc->append((QString::number(ibcc,16)).toUpper());
    if(strBcc->length()==1)
        strBcc->prepend("0");
    return  strBcc;
}

// Analyse la trame et renvoie les infos de config dans une structure
void* Mode4::parseConfig(QString trameConfig) {

    int positionData = 7;
    // Si ce n'est pas une trame de configuration
    if(trameConfig.length() < 10) {
        qDebug() << "Trame de configuration incorrecte\n"<<trameConfig;
        return NULL;
    }

    if(this->typePeripherique == ANALYSEUR) {
        Mode4_AnalyseurConfig* confAnalyseur = new Mode4_AnalyseurConfig;
        confAnalyseur->infoConfig = new QStringList();

        // Recherche de la date
        QDate dateConfig;
        QTime timeConfig;
        dateConfig.setDate((trameConfig.mid(positionData,2).toInt()),
                           (trameConfig.mid(positionData+2,2).toInt()),
                           (trameConfig.mid(positionData+4,2).toInt()));
        timeConfig.setHMS(trameConfig.mid(positionData+6,2).toInt(),
                          trameConfig.mid(positionData+8,2).toInt(),
                          trameConfig.mid(positionData+10,2).toInt());
        confAnalyseur->dateHeure.setDate(dateConfig);
        confAnalyseur->dateHeure.setTime(timeConfig);
        confAnalyseur->infoConfig->append("Configuration de l'appareil :");
        confAnalyseur->infoConfig->append("Date : "+confAnalyseur->dateHeure.toString("dd/MM/yy - hh:mm:ss"));

        // Recherche de l'offset
        confAnalyseur->offset = trameConfig.mid(positionData+12,4).toShort();
        confAnalyseur->infoConfig->append("offset : "+ confAnalyseur->offset);

        // Recherche des alarmes
        QString alarmes = trameConfig.mid(positionData+20,2);
        confAnalyseur->alarme = alarmes.toUShort();
        confAnalyseur->infoConfig->append("alarmes : "+confAnalyseur->alarme);
        // Teste si le chiffre représentant l'alarme est impair
        if(confAnalyseur->alarme % 2 > 0)
            emit(this->alarmeGenerale()); // On declenche le signal d'alarme

        // Recherche des cycles
        QString cycle = trameConfig.mid(positionData+22,2);
        confAnalyseur->periodeZeroRef = cycle.toUShort();
        confAnalyseur->infoConfig->append("Periode cycle zero-ref : "+cycle);
        cycle = trameConfig.mid(positionData+24,2);
        confAnalyseur->periodeZero = cycle.toUShort();
        confAnalyseur->infoConfig->append("Periode cycle zero : "+cycle);
        cycle = trameConfig.mid(positionData+26,2);
        confAnalyseur->periodeAutoCalib = cycle.toUShort();
        confAnalyseur->infoConfig->append("Periode cycle auto calibrage : "+cycle);
        cycle = trameConfig.mid(positionData+28,2);
        confAnalyseur->periodeCalib = cycle.toUShort();
        confAnalyseur->infoConfig->append("Periode cycle calibrage : "+cycle);
        cycle = trameConfig.mid(positionData+30,4);
        confAnalyseur->dureeCycleZero = cycle.toUShort();
        confAnalyseur->infoConfig->append("Periode cycle zero en sec. : "+cycle);
        cycle = trameConfig.mid(positionData+34,4);
        confAnalyseur->dureeCycleCalib = cycle.toUShort();
        confAnalyseur->infoConfig->append("Periode cycle calibrage en sec. : "+cycle);

        return confAnalyseur;
    }
    else {
        Mode4_DiluteurConfig* confDiluteur = new Mode4_DiluteurConfig;
        confDiluteur->pressionAir = trameConfig.mid(positionData,4).toUShort();
        confDiluteur->infoConfig->append("Pression air : "+confDiluteur->pressionAir);
        confDiluteur->pressionDil = trameConfig.mid(positionData+4,4).toUShort();
        confDiluteur->infoConfig->append("Pression diluteur : "+confDiluteur->pressionDil);
        confDiluteur->tempOzoneur = trameConfig.mid(positionData+8,4).toFloat();
        confDiluteur->infoConfig->append("Temperature ozoneur : "+QString::number(confDiluteur->tempOzoneur));
        confDiluteur->uvMesuree = trameConfig.mid(positionData+12,4).toUShort();
        confDiluteur->infoConfig->append("Lumiere UV mesuree : "+confDiluteur->uvMesuree);
        confDiluteur->uvCalculee = trameConfig.mid(positionData+16,4).toUShort();
        confDiluteur->infoConfig->append("Lumiere UV calculee : "+confDiluteur->uvCalculee);
        confDiluteur->pressionAtm = trameConfig.mid(positionData+20,4).toUShort();
        confDiluteur->infoConfig->append("Pression atmosphérique : "+confDiluteur->pressionAtm);
        confDiluteur->tempInterne = trameConfig.mid(positionData+24,4).toFloat();
        confDiluteur->infoConfig->append("Temperature interne : "+QString::number(confDiluteur->tempInterne));
        confDiluteur->tempFour = trameConfig.mid(positionData+28,3).toFloat();
        confDiluteur->infoConfig->append("Temperature du four : "+QString::number(confDiluteur->tempFour));
        confDiluteur->urdm1 = trameConfig.at(positionData+28).toAscii();
        confDiluteur->infoConfig->append("U RDM1 : "+confDiluteur->urdm1);
        confDiluteur->urdm2 = trameConfig.mid(positionData+29,4);
        confDiluteur->infoConfig->append("U RDM2 : "+confDiluteur->urdm2);
        confDiluteur->opt1 = trameConfig.mid(positionData+33,4);
        confDiluteur->infoConfig->append("Optionnel 1 : "+confDiluteur->opt1);
        confDiluteur->opt2 = trameConfig.mid(positionData+37,4);
        confDiluteur->infoConfig->append("Optionnel 2 : "+confDiluteur->opt2);
        confDiluteur->opt3 = trameConfig.mid(positionData+41,4);
        confDiluteur->infoConfig->append("Optionnel 3 : "+confDiluteur->opt3);
        confDiluteur->opt4 = trameConfig.mid(positionData+45,4);
        confDiluteur->infoConfig->append("Optionnel 4 : "+confDiluteur->opt4);

        return confDiluteur;
    }
}

// Renvoie une chaine de 8 caractères representant le nombre en parametre
QString* Mode4::getStrForNumber(ushort nombre,ushort nbDigit) {
    QString* strNombre = new QString();
    strNombre->fill('0',nbDigit);
    if(nombre==0) return strNombre;
    QString strNbTemp = QString::number(nombre,10);
    strNombre->replace((nbDigit-strNbTemp.length()),strNbTemp.length(),strNbTemp);
    return strNombre;
}

// Initialisation des parametres du périphérique
void Mode4::init() {
    this->passageMesure();
}

// Regle l'appareil sur son mode de fonctionnement par défaut
void Mode4::parDefault() {
    this->passageMesure();
}

// Renvoie l'offsetdu periphérique
short Mode4::offset() {
    Mode4_AnalyseurConfig* config = (Mode4_AnalyseurConfig*)commandeConfig();
    if(config==NULL)
        return NULL;
    return config->offset;
}

// Demande de mesure immediate
QVector<float>* Mode4::demandeMesure() {
    QString cmd;
    QVector<float>* mesures = new QVector<float>(3);

    if(this->accepteMesureFloat)
        cmd = *(this->creerTrameCommande("16","\0"));
    else
        cmd = *(this->creerTrameCommande("04","\0"));
    QString reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return NULL;

    if(this->accepteMesureFloat) {
        // On enleve la partie de la trame jusqu'au separateur inclu
        reponse.remove(0,reponse.indexOf(" ")+1);
        // Le nouveau début de trame est la voie 1
        (*mesures)[0] = reponse.left(reponse.indexOf(" ")).toFloat();
        // On enleve la partie de la trame correspondant à la voie 1
        reponse.remove(0,reponse.indexOf(" ")+1);
        // Le nouveau debut de trame est la voie 2
        (*mesures)[1] = reponse.left(reponse.indexOf(" ")).toFloat();
        // Et on recommence pour avoir la voie 3
        reponse.remove(0,reponse.indexOf(" ")+1);
        (*mesures)[2] = reponse.left(reponse.indexOf(" ")).toFloat();
    }
    else {
        (*mesures)[0] = reponse.mid(16,8).toFloat()/10;
        (*mesures)[1] = reponse.mid(24,8).toFloat()/10;
        (*mesures)[2] = reponse.mid(32,8).toFloat()/10;
    }
    return mesures;
}

// Demande d'alarme
ushort Mode4::demandeAlarme() {
    if(this->typePeripherique == ANALYSEUR) {
        Mode4_AnalyseurConfig* config = (Mode4_AnalyseurConfig*)commandeConfig();
        if(config==NULL)
            return NULL;
        return config->alarme;
    }
    // Si diluteur
    QString cmd = *(this->creerTrameCommande("15","\0"));
    QString reponse = this->transaction(cmd);
    ushort codeAlarme = reponse.mid(7,2).toUShort();
    if(codeAlarme > 0)
        emit(this->alarmeGenerale());
    return codeAlarme;
}

// Demande d'heure
QDateTime* Mode4::demandeDateHeure() {
    Mode4_AnalyseurConfig* config = (Mode4_AnalyseurConfig*)commandeConfig();
    if(config==NULL)
        return NULL;
    QDateTime* dateTimeConfig = new QDateTime(config->dateHeure);
    for(ushort i=0;i<config->infoConfig->size();i++) {
        qDebug()<<config->infoConfig->at(i);
    }
    emit(this->afficheTrame(config->infoConfig));
    return dateTimeConfig;
}

// Demande de passage en mode zero
void Mode4::passageZero() {
    QString cmd = *(this->creerTrameCommande("06","9999\0"));
    this->transaction(cmd);
}

// Demande de passage en mode étalon
void Mode4::passageEtalon() {
    QString cmd = *(this->creerTrameCommande("08","9999\0"));
    this->transaction(cmd);
}

// Demande de passage en mode mesure
void Mode4::passageMesure() {
    QString cmd = *(this->creerTrameCommande("09","\0"));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre à un certain point de gaz
void Mode4::commandeSpan(ushort canal,ushort point,ushort concO3) {
    QString strPoint = *this->getStrForNumber(point,8);
    QString strO3 = *this->getStrForNumber(concO3,8);

    QString data = QString::number(canal);
    if(canal<10)
        data.prepend('0');

    data.append(strPoint);
    data.append(strO3);

    QString cmd = *(this->creerTrameCommande("08",data));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre à un certain point de gaz O3
void Mode4::commandeSpanO3(ushort canal,ushort point03) {
    this->commandeSpan(canal,0,point03);
}


// Reset du périphérique
void Mode4::reset() {
    QString cmd;
    if(this->typePeripherique==ANALYSEUR)
        cmd = *(this->creerTrameCommande("1Z","\0"));
    else
        cmd = *(this->creerTrameCommande("00","\0"));

    emit(this->envoiTrame(cmd));
}

// mise en stand-by du périphérique
void Mode4::standBy() {
    QString cmd;
    if(this->typePeripherique==ANALYSEUR)
        cmd = *(this->creerTrameCommande("14","\0"));
    else
        cmd = *(this->creerTrameCommande("01","\0"));

    this->transaction(cmd);
}

// Connaitre les valeurs actuelles de gaz du diluteur
void Mode4::commandeEvent() {
    QString cmd = *(this->creerTrameCommande("12","\0"));
    this->transaction(cmd);
}

// Connaitre la configuration actuelle de l'appareil
void* Mode4::commandeConfig() {
    QString cmd;
    if(this->typePeripherique==ANALYSEUR)
        cmd = *(this->creerTrameCommande("02","\0"));
    else
        cmd = *(this->creerTrameCommande("14","\0"));

    QString reponse = this->transaction(cmd);

    if(reponse.isEmpty())
        return NULL;

    void* conf = parseConfig(reponse);

    return conf;
}
