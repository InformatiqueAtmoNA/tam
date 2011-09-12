/*////////////////////////////////////////////////////
// \file protocole.cpp
// \brief Interface pour les protocoles
// \author FOUQUART Christophe
// \version 0.1
// \date 09/02/2011
////////////////////////////////////////////////////*/

#include "protocole.h"
#include "mode4_sx_6000_17.h"
#include "tei_146c.h"
#include "tei_146c_v2.h"
#include "tei_146i.h"
#include "tei_49c.h"
#include "tei_49ps.h"
#include "api.h"

Protocole::Protocole() {
    m_avorterTransaction = false;
}

Protocole::~Protocole() {
//    if(threadCommunication->isRunning())
//        threadCommunication->stop();
    threadCommunication->deleteLater();
    while(threadCommunication->isRunning()) {
        QCoreApplication::processEvents();
        QCoreApplication::flush();
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

// Affecte un nouvel état de la communication RS232
void Protocole::changementEtat(const ushort newEtatCom) {
    this->flagEtatCom = newEtatCom;
}

// Retourne l'état de la communication RS232
ushort Protocole::getEtatCom() {
    return this->flagEtatCom;
}

// Execute une transaction avec le périphérique (ecriture/attente réponse)
QString Protocole::transaction(const QString & commande) {
    this->timerFini = false;

    QFile logTrames("logTrames.txt");
    logTrames.open(QFile::WriteOnly | QFile::Append);
    logTrames.write(commande.toAscii());
    logTrames.write("\n");

    qDebug()<< "Trame envoyee : " << commande.toAscii();

    emit(this->envoiTrame(commande));
    this->flagEtatCom = ETAT_ATTENTE;
    QTimer timerCommunication;

    timerCommunication.start(this->timeout);
    connect(&timerCommunication,SIGNAL(timeout()),this,SLOT(timeoutCom()));
    // On attend timeout ms de recevoir la réponse
    while(!this->timerFini) {
        if(m_avorterTransaction)
            return NULL;
        QCoreApplication::processEvents();
    }
    // Si pas de réponse
    if(this->flagEtatCom == ETAT_ATTENTE) {
        emit(this->erreurTransmission());
        return NULL;
    }
    this->flagEtatCom = ETAT_REPOS;

    // Si erreur de commande
    // CONDITION A MODIFIER LORS DE L'AJOUT DE NOUVEAUX PROTOCOLES
    if(this->trame.at(1)==NAK || this->trame.contains("bad command")) {
        emit(this->erreurCommande());
        qDebug()<<"Erreur commande";
        return NULL;
    }
    logTrames.write(trame.toAscii());
    logTrames.close();

    emit(this->afficheTrame(this->trame));
    return this->trame;
}

// Slot de lecture d'une trame
void Protocole::lectureTrame(const QString & data) {
    qDebug()<<"Trame reçue : " << data.toAscii();
    this->trame = data;
    this->flagEtatCom = ETAT_LECTURE;
}

// Slot appelé lorsque le timer de communication est fini
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
    default:
        break;
    }

    return protocole;
}

// Commande au diluteur de se mettre à un certain point de gaz
void Protocole::commandeSpan(const SpanHandler & spanData) {
    qDebug()<<"commandeSpan("<<spanData.getCanal()<<","<<spanData.getPoint()<<")";
    qDebug()<<"Cette commande doit être réimplémentée dans une classe dérivée";
}

// Commande au diluteur de se mettre au point de gaz zero
void Protocole::commandeSpanZero(const QString & canal) {
    qDebug()<<"commandeSpanZero("<<canal<<")";
    qDebug()<<"Cette commande doit être réimplémentée dans une classe dérivée";
}

// Commande au diluteur de se mettre à un certain point de gaz
void Protocole::commandeSpanTpg(const SpanHandler & spanTpgData) {
    qDebug()<<"commandeSpan("<<spanTpgData.getCanal()<<","<<spanTpgData.getPoint()<<","<<spanTpgData.getConcO3()<<")";
    qDebug()<<"Cette commande doit être réimplémentée dans une classe dérivée";
}

// Commande au diluteur de se mettre à un certain point de gaz O3
void Protocole::commandeSpanO3(const SpanHandler & spanO3Data) {
    qDebug()<<"commandeSpan("<<spanO3Data.getCanal()<<","<<spanO3Data.getPoint()<<")";
    qDebug()<<"Cette commande doit être réimplémentée dans une classe dérivée";
}
