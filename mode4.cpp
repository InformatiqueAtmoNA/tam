/*////////////////////////////////////////////////////
// \file mode4.cpp
// \brief Gère l'envoi et la reception de commande MODE4
// \author FOUQUART Christophe
// \version 1.0
// \date 31/03/2011
//
// TAM - Tests Automatiques Metrologiques
// Copyright (C) 2011-2012 TAM Team
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//
////////////////////////////////////////////////////*/

#include "mode4.h"

Mode4::Mode4() {

}

Mode4::Mode4(const QString & adressePeriph, const TypePeripherique & typePeriph, const DesignationProtocole & protocoleUtilise, const bool accepteFloat) {
    this->adresse = adressePeriph;
    this->typePeripherique = typePeriph;
    this->accepteMesureFloat = accepteFloat;
    this->flagEtatCom = ETAT_REPOS;
    this->versionProtocole = protocoleUtilise;
}

Mode4::~Mode4() {

}

// Cree une trame en fonction du numero de commande MODE4
QString* Mode4::creerTrameCommande(const QString & noCommande, const QString & data) {
    QString* trame = new QString();

    char StartOfText = STX;
    char EndOfText = ETX;
    trame->append(QChar(StartOfText));  // modifie
    trame->append(this->adresse);
    trame->append(noCommande);
    trame->append(data);
    trame->append(calculerBCC(*trame)->data()); // modifie
    trame->append(QChar(EndOfText)); //modifie
    return trame;
}

// Calcule du BCC (Block Check Character)
// Le BCC est obtenu en effectuant un OU exclusif (XOR)
// sur tous les octets dà partir de STX exclu jusqu'au BCC exclu
QString* Mode4::calculerBCC(const QString & trame) {
    char bcc = 0x00;
    for(int i=1;i<trame.length();i++) {
             bcc ^= trame[i].toLatin1();
// Ajout de la condition suivante pour gerer les trames avec un nombre impair de caractère car sinon le BCC ne se calcul pas bien
// Correction pour SX6000 ancien
             if (i == trame.length()-1 && i%2 == 1)
                 bcc ^= '0';
    }
    QString* strBcc = new QString();
    int ibcc = static_cast<int>(bcc);
    strBcc->append((QString::number(ibcc,16)).toUpper());
    if(strBcc->length()==1)
        strBcc->prepend("0");
    return  strBcc;
}

// Analyse la trame et renvoie les infos de config dans une structure
void* Mode4::parseConfig(const QString & trameConfig) {

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
        confAnalyseur->infoConfig->append("offset : "+ QString::number(confAnalyseur->offset));

        // Recherche des alarmes
        QString alarmes = trameConfig.mid(positionData+20,2);
        confAnalyseur->alarme = alarmes.toUShort();
        confAnalyseur->infoConfig->append("alarmes : "+QString::number(confAnalyseur->alarme));
        // Teste si le chiffre representant l'alarme est impair
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
        confDiluteur->infoConfig->append(&"Pression air : "[confDiluteur->pressionAir]);
        confDiluteur->pressionDil = trameConfig.mid(positionData+4,4).toUShort();
        confDiluteur->infoConfig->append(&"Pression diluteur : "[confDiluteur->pressionDil]);
        confDiluteur->tempOzoneur = trameConfig.mid(positionData+8,4).toFloat();
        confDiluteur->infoConfig->append("Temperature ozoneur : "+QString::number(confDiluteur->tempOzoneur));
        confDiluteur->uvMesuree = trameConfig.mid(positionData+12,4).toUShort();
        confDiluteur->infoConfig->append(&"Lumiere UV mesuree : "[confDiluteur->uvMesuree]);
        confDiluteur->uvCalculee = trameConfig.mid(positionData+16,4).toUShort();
        confDiluteur->infoConfig->append(&"Lumiere UV calculee : "[confDiluteur->uvCalculee]);
        confDiluteur->pressionAtm = trameConfig.mid(positionData+20,4).toUShort();
        confDiluteur->infoConfig->append(&"Pression atmospherique : "[confDiluteur->pressionAtm]);
        confDiluteur->tempInterne = trameConfig.mid(positionData+24,4).toFloat();
        confDiluteur->infoConfig->append("Temperature interne : "+QString::number(confDiluteur->tempInterne));
        confDiluteur->tempFour = trameConfig.mid(positionData+28,3).toFloat();
        confDiluteur->infoConfig->append("Temperature du four : "+QString::number(confDiluteur->tempFour));
        confDiluteur->urdm1 = trameConfig.at(positionData+28).toLatin1();
        confDiluteur->infoConfig->append(&"U RDM1 : "[confDiluteur->urdm1]);
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
QString* Mode4::getStrForNumber(const ushort & nombre,const ushort & nbDigit) {
    QString* strNombre = new QString();
    strNombre->fill('0',nbDigit);
    if(nombre==0) return strNombre;
    QString strNbTemp = QString::number(nombre,10);
    strNombre->replace((nbDigit-strNbTemp.length()),strNbTemp.length(),strNbTemp);
    return strNombre;
}

// Initialisation des parametres du peripherique
bool Mode4::init() {
    this->passageMesure();
    return true;
}

// Regle l'appareil sur son mode de fonctionnement par defaut
bool Mode4::parDefault() {
    this->passageMesure();
    return true;
}

// Renvoie l'offsetdu peripherique
short Mode4::offset() {
    Mode4_AnalyseurConfig* config = (Mode4_AnalyseurConfig*)commandeConfig();
    if(config==NULL)
        return 0;
    return config->offset;
}

// Renvoie le flottant represente dans la chaine passe en paramètre
float Mode4::getMesureFromString(const QString & mesure) {
    for(ushort i=0;i<mesure.length();i++)
        if(mesure.at(i)!='0') {
            return mesure.right(mesure.length()-i).toFloat();
        }
    return 0;
}

// Demande de mesure immediate
QPointer<MesureIndividuelle> Mode4::demandeMesure() {
    QString cmd;
    QPointer<MesureIndividuelle> mesures(new MesureIndividuelle());

    if(this->accepteMesureFloat)
        cmd = *(this->creerTrameCommande("16","\0"));
    else
        cmd = *(this->creerTrameCommande("04","\0"));
    QString reponse = this->transaction(cmd);
    if(reponse.isEmpty())
        return mesures;

    if(this->accepteMesureFloat) {
        // On enleve la partie de la trame jusqu'au separateur inclu
        reponse.remove(0,reponse.indexOf(" ")+1);
        // Le nouveau debut de trame est la voie 1
        mesures.data()->append(reponse.left(reponse.indexOf(" ")).toFloat());
        // On enleve la partie de la trame correspondant à la voie 1
        reponse.remove(0,reponse.indexOf(" ")+1);
        // Le nouveau debut de trame est la voie 2
        QString mesure = reponse.left(reponse.indexOf(" "));
        if(!mesure.contains("inf")) {
            mesures.data()->append(mesure.toFloat());
        }
        // Et on recommence pour avoir la voie 3
        reponse.remove(0,reponse.indexOf(" ")+1);
        QString mesure2 = reponse.left(reponse.indexOf(" "));
        if(!mesure.contains("inf")) {
            mesures.data()->append(mesure2.toFloat());
        }
    }
    else {
        qDebug()<<"Mesure 1: "<<reponse.mid(16,8);
        QString mesure = reponse.mid(16,8);
        mesures.data()->append(this->getMesureFromString(mesure));
        mesure = reponse.mid(24,8);
        if(!mesure.contains("inf")) {
            qDebug()<<"Mesure 2: "<<reponse.mid(24,8);
            mesures.data()->append(this->getMesureFromString(mesure));
        }
        mesure = reponse.mid(32,8);
        if(!mesure.contains("inf")) {
            qDebug()<<"Mesure 3: "<<reponse.mid(32,8);
            mesures.data()->append(this->getMesureFromString(mesure));
        }
    }
    return mesures;
}

// Demande d'alarme
ushort Mode4::demandeAlarme() {
    if(this->typePeripherique == ANALYSEUR) {
        Mode4_AnalyseurConfig* config = (Mode4_AnalyseurConfig*)commandeConfig();
        if(config==NULL)
            return 0;
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

// Demande de passage en mode etalon
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
void Mode4::commandeSpan(const SpanHandler & spanData) {
    ushort point = spanData.getPoint();
    ushort concO3 = spanData.getConcO3();
    QString strPoint = *(this->getStrForNumber(point,8));
    QString strO3 = *(this->getStrForNumber(concO3,8));

    QString data = spanData.getCanal();
    if(spanData.getCanal().toUShort()<10)
        data.prepend('0');

    data.append(strPoint);
    data.append(strO3);

    QString cmd = *(this->creerTrameCommande("08",data));
    this->transaction(cmd);
}

// Commande au diluteur de se mettre au point de gaz zero
void Mode4::commandeSpanZero(const QString & canal) {
    SpanHandler* spanData = new SpanHandler();
    spanData->setSpanArguments(canal,0,0);
    this->commandeSpan(*spanData);
    delete spanData;
}

// Commande au diluteur de se mettre à un certain point de gaz O3
void Mode4::commandeSpanO3(const SpanHandler & spanO3Data) {
    this->commandeSpan(spanO3Data);
}

// Commande au diluteur de se mettre à un certain point de gaz
void Mode4::commandeSpanTpg(const SpanHandler & spanTpgData) {
    this->commandeSpan(spanTpgData);
}

// Reset du peripherique
bool Mode4::reset() {
    QString cmd;
    if(this->typePeripherique==ANALYSEUR)
        cmd = *(this->creerTrameCommande("1Z","\0"));
    else
        cmd = *(this->creerTrameCommande("00","\0"));

    emit(this->envoiTrame(cmd));
    qDebug()<<"Trame reset envoyee : "<<cmd;
    return true;
}

// mise en stand-by du peripherique
bool Mode4::standBy() {
    QString cmd;
    if(this->typePeripherique==ANALYSEUR)
        cmd = *(this->creerTrameCommande("14","\0"));
    else
        cmd = *(this->creerTrameCommande("01","\0"));

    this->transaction(cmd);
    return true;
}

// Connaitre les valeurs actuelles de gaz du diluteur
bool Mode4::commandeEvent() {
    QString cmd = *(this->creerTrameCommande("12","\0"));
    this->transaction(cmd);
    return true;
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

// Renvoie la liste des commandes autorisees par le protocole
QVector<Commandes> const* Mode4::getListeCommandes() {
    QVector<Commandes>* commandesAutorisees;

    if(this->versionProtocole == MODE4_ANA_CMD04 || this->versionProtocole == MODE4_ANA_CMD16) {
        commandesAutorisees = new QVector<Commandes>(9);
        (*commandesAutorisees)[0] = RESET;
        (*commandesAutorisees)[1] = OFFSET;
        (*commandesAutorisees)[2] = STAND_BY;
        (*commandesAutorisees)[3] = DATE_HEURE;
        (*commandesAutorisees)[4] = MESURES;
        (*commandesAutorisees)[5] = ALARME;
        (*commandesAutorisees)[6] = MODE_ZERO;
        (*commandesAutorisees)[7] = MODE_ETALON;
        (*commandesAutorisees)[8] = MODE_MESURE;
    }
    else {
        commandesAutorisees = new QVector<Commandes>(8);
        (*commandesAutorisees)[0] = RESET;
        (*commandesAutorisees)[1] = EVENT;
        (*commandesAutorisees)[2] = STAND_BY;
        (*commandesAutorisees)[3] = ALARME;
        (*commandesAutorisees)[4] = SPAN;
        (*commandesAutorisees)[5] = SPAN_ZERO;
        (*commandesAutorisees)[6] = SPAN_TPG;
        (*commandesAutorisees)[7] = SPAN_O3;
    }
    return commandesAutorisees;
}

// Renvoie une instance de SpanHandler contenant les infos
// sur les arguments de la commande
SpanHandler* Mode4::getSpanHandler(Commandes commandeSpan) {
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
        (*argumentsUtiles)[CANAL]=true;
        (*argumentsUtiles)[CONCO3]=true;
        break;
    default:
        break;
    }
    infosCommandeSpan->setTabArgumentsSpan(*argumentsUtiles);
    return infosCommandeSpan;
}
