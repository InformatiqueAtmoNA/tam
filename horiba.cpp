#include "horiba.h"


horiba::horiba(const QString & adressePeriph,const TypePeripherique & typePeriph)
{
    this->adresse=adressePeriph;
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
    if(reponse.isEmpty() or reponse.length()){
        return mesures;
    }

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
    QString *trame = creerTrameCommande("A29","0");
    QString reponse = transaction(*trame);
}

void horiba::passageEtalon()
{

}

void horiba::passageMesure()
{

}

const QVector<Commandes> *horiba::getListeCommandes()
{

}

SpanHandler *horiba::getSpanHandler(Commandes commandeSpan)
{

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
