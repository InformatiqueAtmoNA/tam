/*////////////////////////////////////////////////////
// \file protocole.cpp
// \brief Interface pour les protocoles
// \author FOUQUART Christophe
// \version 0.1
// \date 09/02/2011
////////////////////////////////////////////////////*/

#include "protocole.h"
#include "mode4_sx_6000_17.h"
#include "mode4_sx_6000_1808.h"
#include "tei_146c.h"
#include "tei_146c_v2.h"
#include "tei_146i.h"
#include "tei_49c.h"
#include "tei_49ps.h"
#include "api.h"
#include "modbus.h"
#include "horiba.h"

Protocole::Protocole() {
    m_avorterTransaction = false;
}

Protocole::~Protocole() {
//    if(threadCommunication->isRunning())
//        threadCommunication->stop();
    threadCommunication->deleteLater();
    while(threadCommunication->isRunning()) {
        QCoreApplication::processEvents();
        QCoreApplication::sendPostedEvents(); // modifie
    }
}

void Protocole::quitter() {
    m_avorterTransaction=true;
}

// Affecte une nouvelle instance de la classe CommunicationSerie
void Protocole::setThreadComHandler(const QPointer<ThreadComHandler> newThreadComHandler) {
    this->threadCommunication = newThreadComHandler;
    connect(this,SIGNAL(envoiTrame(QString)),this->threadCommunication,SIGNAL(envoiTrame(QString)));
    connect(this->threadCommunication,SIGNAL(receptionTrame(QString)),this,SLOT(lectureTrame(QString)));
}

// Retourne l'instance de la classe CommunicationSerie
QPointer<ThreadComHandler> Protocole::getThreadComHandler() {
    return this->threadCommunication;
}

// Affecte un nouveau timeout en ms à la communication
void Protocole::setTimeOut(const ushort newTimeOut) {
    this->timeout = newTimeOut;
}

// Retourne le timeout à la communication
ushort Protocole::getTimeOut() {
    return this->timeout;
}

// Affecte un nouvel etat de la communication RS232
void Protocole::changementEtat(const ushort newEtatCom) {
    this->flagEtatCom = newEtatCom;
}

// Retourne l'etat de la communication RS232
ushort Protocole::getEtatCom() {
    return this->flagEtatCom;
}

// Execute une transaction avec le peripherique (ecriture/attente reponse)
QString Protocole::transaction(const QString & commande) {
    this->timerFini = false;

    QFile logTrames("logTrames.txt");
    logTrames.open(QFile::WriteOnly | QFile::Append);
    int compteur=1;
    do{
        qDebug()<< "Essai " << compteur << " : ";

    emit(this->envoiTrame(commande));
    this->flagEtatCom = ETAT_ATTENTE;
    QTimer timerCommunication;
        logTrames.write(commande.toLatin1());
        logTrames.write("\n");

        qDebug()<< "Trame envoyee : " << commande.toLatin1();
        timerCommunication.start(this->timeout);
        connect(&timerCommunication,SIGNAL(timeout()),this,SLOT(timeoutCom()));
        // On attend timeout ms de recevoir la reponse
        while(!this->timerFini) {
            if(m_avorterTransaction)
                return NULL;
            QCoreApplication::processEvents();
        }
        compteur++;
    }while(this->flagEtatCom == ETAT_ATTENTE && compteur<=3);
    // Si pas de reponse
    if(this->flagEtatCom == ETAT_ATTENTE) {
        emit(this->erreurTransmission());
        return NULL;
    }
    this->flagEtatCom = ETAT_REPOS;

    // Si erreur de commande
    // CONDITION A MODIFIER LORS DE L'AJOUT DE NOUVEAUX PROTOCOLES
    if(this->trame.at(1)==QString::number(NAK,16) || this->trame.contains("bad command")) { // modifie
        emit(this->erreurCommande());
        qDebug()<<"Erreur commande";
        return NULL;
    }
    logTrames.write(trame.toLatin1());
    logTrames.close();

    emit(this->afficheTrame(this->trame));
    return this->trame;
}

// Slot de lecture d'une trame
void Protocole::lectureTrame(const QString & data) {
    qDebug()<<"Trame reçue : " << data.toLatin1();
    this->trame=data;
    this->flagEtatCom = ETAT_LECTURE;
}

// Slot appele lorsque le timer de communication est fini
void Protocole::timeoutCom() {
    this->timerFini = true;
}

QPointer<Protocole> Protocole::getProtocoleObject(const DesignationProtocole & designationProtocole, const QString & adresse)
{
    QPointer<Protocole> protocole;

    switch(designationProtocole) {
    case MODE4_ANA_CMD04:
        protocole = new Mode4(adresse,ANALYSEUR,MODE4_ANA_CMD04,false);
        break;
    case MODE4_ANA_CMD16:
        protocole = new Mode4(adresse,ANALYSEUR,MODE4_ANA_CMD16,true);
        break;
    case MODE4_LNI:
        protocole = new Mode4(adresse,ETALON,MODE4_LNI,true);
        break;
    case MODE4_SX6000_17:
        protocole = new Mode4_SX_6000_17(adresse,ETALON,true);
        break;
    case MODE4_SX6000_1808:
        protocole = new Mode4_SX_6000_1808(adresse,ETALON,true);
        break;
    case TEI_ANA:
        protocole = new Tei(adresse,ANALYSEUR,AUCUNE);
        break;
    case TEI_146i:
        protocole = new Tei_146i(adresse,ETALON,AUCUNE);
        break;
    case TEI_146i_OZONISEUR:
        protocole = new Tei_146i(adresse,ETALON,OZONISEUR);
        break;
    case TEI_146i_PHOTOMETRE:
        protocole = new Tei_146i(adresse,ETALON,PHOTOMETRE);
        break;
    case TEI_146c:
        protocole = new Tei_146c(adresse,ETALON,AUCUNE);
        break;
    case TEI_146c_V2 :
        protocole = new Tei_146c_v2(adresse,ETALON,AUCUNE);
        break;
    case TEI_49ps:
        protocole = new Tei_49ps(adresse,ETALON,AUCUNE);
        break;
    case API_ANA:
        protocole = new Api(adresse,ANALYSEUR,API_ANA);
        break;
    case API_DIL:
        protocole = new Api(adresse,ETALON,API_DIL);
        break;
    case MODBUS_RTU_TXXXX :
        protocole = new Modbus(adresse,SONDE,MODBUS_RTU_TXXXX);
    case HORIBA_APXX:
        protocole = new horiba(adresse,ANALYSEUR);
        break;
    default:
        break;
    }

    return protocole;
}

// Commande au diluteur de se mettre à un certain point de gaz
void Protocole::commandeSpan(const SpanHandler & spanData) {
    qDebug()<<"commandeSpan("<<spanData.getCanal()<<","<<spanData.getPoint()<<")";
    qDebug()<<"Cette commande doit être reimplementee dans une classe derivee";
}

// Commande au diluteur de se mettre au point de gaz zero
void Protocole::commandeSpanZero(const QString & canal) {
    qDebug()<<"commandeSpanZero("<<canal<<")";
    qDebug()<<"Cette commande doit être reimplementee dans une classe derivee";
}

// Commande au diluteur de se mettre à un certain point de gaz
void Protocole::commandeSpanTpg(const SpanHandler & spanTpgData) {
    qDebug()<<"commandeSpan("<<spanTpgData.getCanal()<<","<<spanTpgData.getPoint()<<","<<spanTpgData.getConcO3()<<")";
    qDebug()<<"Cette commande doit être reimplementee dans une classe derivee";
}

// Commande au diluteur de se mettre à un certain point de gaz O3
void Protocole::commandeSpanO3(const SpanHandler & spanO3Data) {
    qDebug()<<"commandeSpan("<<spanO3Data.getCanal()<<","<<spanO3Data.getPoint()<<")";
    qDebug()<<"Cette commande doit être reimplementee dans une classe derivee";
}
