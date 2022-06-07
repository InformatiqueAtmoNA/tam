#include "horiba.h"


horiba::horiba(const QString & adressePeriph,const TypePeripherique & typePeriph)
{
    this->adresse=adressePeriph;
}

bool horiba::init()
{
    this->passageMesure();
    return true;
}

bool horiba::parDefault()
{
    this->passageMesure();
    return true;
}

QString *horiba::creerTrameCommande(const QString &codeCommande,const QString & paramCommande)
{
    QString *trame= new QString;
    trame->append(char(0x01));
    trame->append("FF");
    trame->append(adresse);
    trame->append("01");
    trame->append(codeCommande);
    trame->append(char(STX));
    trame->append(paramCommande);
    trame->append(*calculerFCS(*trame));
    trame->append(char(ETX));

    return trame;
}

QString *horiba::calculerFCS(const QString &trame)
{
    char fcs = 0x00;
    for(int i=0;i<trame.length();i++) {
             fcs ^= trame[i].toLatin1();
    }
    QString* strFcs = new QString();
    int ibcc = static_cast<int>(fcs);
    strFcs->append((QString::number(ibcc,16)).toUpper());
    if(strFcs->length()==1)
        strFcs->prepend("0");
    return  strFcs;
}


QPointer<MesureIndividuelle> horiba::demandeMesure()
{
    QPointer<MesureIndividuelle> mesures(new MesureIndividuelle());

    QString *trame = creerTrameCommande("R001","");
    QString reponse = transaction(*trame);
    if(reponse.isEmpty() or getInformationFromResponse(reponse)["error"]!="00"){
        return mesures;
    }
    QString mesure1 = getInformationFromResponse(reponse)["valeur1"];
    mesures.data()->append(mesure1.toFloat());
    QString mesure2 = getInformationFromResponse(reponse)["valeur2"];
    mesures.data()->append(mesure2.toFloat());
    QString mesure3 = getInformationFromResponse(reponse)["valeur3"];
    mesures.data()->append(mesure3.toFloat());

    return mesures;
}

ushort horiba::demandeAlarme()
{
    ushort alarme=0;
    QString *trame = creerTrameCommande("R001","");
    QString reponse = transaction(*trame);

    if(getInformationFromResponse(reponse)["alarme"].contains('1')){
        emit(this->alarmeGenerale());
        alarme=1;
    }
    return alarme;
}

void horiba::passageZero()
{
    QString *trame = creerTrameCommande("A024","1");
    QString reponse = transaction(*trame);
}

void horiba::passageEtalon()
{
    QString *trame = creerTrameCommande("A024","2");
    QString reponse = transaction(*trame);
}

void horiba::passageMesure()
{
    QString *trame = creerTrameCommande("A024","0");
    QString reponse = transaction(*trame);
}

const QVector<Commandes> *horiba::getListeCommandes()
{
    QVector<Commandes>* commandesAutorisees;
    /*if(this->typePeripherique == ANALYSEUR){
        commandesAutorisees = new QVector<Commandes>(5);
        (*commandesAutorisees)[0] = MESURES;
        (*commandesAutorisees)[1] = ALARME;
        (*commandesAutorisees)[2] = MODE_ZERO;
        (*commandesAutorisees)[3] = MODE_ETALON;
        (*commandesAutorisees)[4] = MODE_MESURE;
    }
    else{
        commandesAutorisees = new QVector<Commandes>(6);
        (*commandesAutorisees)[0] = STAND_BY;
        (*commandesAutorisees)[1] = ALARME;
        (*commandesAutorisees)[2] = SPAN;
        (*commandesAutorisees)[3] = SPAN_ZERO;
        (*commandesAutorisees)[4] = SPAN_TPG;
        (*commandesAutorisees)[5] = SPAN_O3;
    }*/

    return commandesAutorisees;
}

SpanHandler *horiba::getSpanHandler(Commandes commandeSpan)
{
    SpanHandler* infosCommandeSpan = new SpanHandler();
    QVector<bool>* argumentsUtiles = new QVector<bool>(3);
    argumentsUtiles->fill(false);
    /*switch(commandeSpan) {
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
    }*/
    infosCommandeSpan->setTabArgumentsSpan(*argumentsUtiles);
    return infosCommandeSpan;
}

QDateTime *horiba::demandeDateHeure()
{
    QDateTime* date = new QDateTime;

    QString *trame = creerTrameCommande("R001","");
    QString reponse = transaction(*trame);
    QString dateHeure= getInformationFromResponse(reponse)["date"];

    QString annee = dateHeure.mid(0,4);
    QString mois = dateHeure.mid(4,2);
    QString jour = dateHeure.mid(6,2);
    QString heure = dateHeure.mid(8,2);
    QString minute = dateHeure.mid(10,2);
    QString seconde = dateHeure.mid(12,2);

    dateHeure = QString(annee+'-'+mois+'-'+jour+'T'+heure+':'+minute+':'+seconde);
    *date=QDateTime::fromString(dateHeure,Qt::ISODate);
    return date;
}

QMap<QString,QString> horiba::getInformationFromResponse(QString reponse)
{
    QMap<QString,QString> map;

    reponse.remove(' ');
    QStringList liste;
    if(reponse.contains(',')){
        liste = reponse.split(',');

        QString header = liste[0].mid(0,12);
        header.remove(char(0x01));
        header.remove(char(0x02));
        QString error = liste[0].mid(12,14);
        QString date = liste[1];

        QString valeur1 = liste[2];
        QString valeur2 = liste[3];
        QString valeur3 = liste[4];

        for(int i=0; i<5 ; i++){
            valeur1.remove(0,1);
            valeur2.remove(0,1);
            valeur3.remove(0,1);
        }


        QString stat = liste[5];
        QString caution = liste[6];
        QString alarme = liste[7];
        alarme.remove(char(0x03));
        alarme.remove(alarme.back());
        alarme.remove(alarme.back());

        map["header"]=header;
        map["error"]=error;
        map["date"]=date;
        map["valeur1"]=valeur1;
        map["valeur2"]=valeur2;
        map["valeur3"]=valeur3;
        map["stat"]=stat;
        map["caution"]=caution;
        map["alarme"]=alarme;
    }
    else{
        QString header = reponse.mid(1,10);
        QString error =  reponse.mid(12,2);
        map["header"]=header;
        map["error"]=error;
    }
    return map;
}
