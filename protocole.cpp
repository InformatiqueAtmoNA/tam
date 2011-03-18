/*////////////////////////////////////////////////////
// \file protocole.cpp
// \brief Interface pour les protocoles
// \author FOUQUART Christophe
// \version 0.1
// \date 09/02/2011
////////////////////////////////////////////////////*/

#include "protocole.h"

Protocole::Protocole() {
}

Protocole::~Protocole() {
}

// Affecte une nouvelle instance de la classe CommunicationSerie
void Protocole::setThreadComHandler(ThreadComHandler* newThreadComHandler) {
    this->threadCommunication = newThreadComHandler;
    connect(this,SIGNAL(envoiTrame(QString)),this->threadCommunication,SIGNAL(envoiTrame(QString)));
    connect(this->threadCommunication,SIGNAL(receptionTrame(QString)),this,SLOT(lectureTrame(QString)));
}

// Retourne l'instance de la classe CommunicationSerie
ThreadComHandler* Protocole::getThreadComHandler() {
    return this->threadCommunication;
}

// Affecte un nouveau timeout en ms à la communication
void Protocole::setTimeOut(ushort newTimeOut) {
    this->timeout = newTimeOut;
}

// Retourne le timeout à la communication
ushort Protocole::getTimeOut() {
    return this->timeout;
}

// Affecte un nouvel état de la communication RS232
void Protocole::changementEtat(ushort newEtatCom) {
    this->flagEtatCom = newEtatCom;
}

// Retourne l'état de la communication RS232
ushort Protocole::getEtatCom() {
    return this->flagEtatCom;
}

// Execute une transaction avec le périphérique (ecriture/attente réponse)
QString Protocole::transaction(QString commande) {
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

// Commande au diluteur de se mettre au point de gaz zero
void Protocole::commandeSpanZero(ushort canal) {
    this->commandeSpan(canal);
}

// Commande au diluteur de se mettre au point de gaz zero
void Protocole::commandeSpanZero(QString canal) {
    this->commandeSpan(canal);
}

// Commande au diluteur de se mettre à un certain point de gaz
void Protocole::commandeSpanTpg(ushort canalNox,ushort point,ushort concO3 ) {
    this->commandeSpan(canalNox,point,concO3);
}

// Commande au diluteur de se mettre à un certain point de gaz
void Protocole::commandeSpanTpg(QString canalNox,ushort point,ushort concO3 ) {
    this->commandeSpan(canalNox,point,concO3);
}

// Slot de lecture d'une trame
void Protocole::lectureTrame(QString data) {
    qDebug()<<"Trame reçue : " << data.toAscii();
    this->trame = data;
    this->flagEtatCom = ETAT_LECTURE;
}

// Slot appelé lorsque le timer de communication est fini
void Protocole::timeoutCom() {
    this->timerFini = true;
}

/////////////////////////////////////////////////////////////////////////
// Les fonctions suivantes sont là car la classe principale du programme
// ne connait que la classe Protocole et non ses classes dérivées
// A MODIFIER LORS DE L'AJOUT D'UN PROTOCOLE
// SI UNE FONCTION SUPPLEMENTAIRE EST IMPLEMENTEE
/////////////////////////////////////////////////////////////////////////

void Protocole::init() {
    qDebug()<<"Protocole::init()";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
}

void Protocole::parDefault() {
    qDebug()<<"Protocole::parDefault()";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
}

short Protocole::offset() {
    qDebug()<<"Protocole::offset()";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
    return 0;
}

QDateTime* Protocole::demandeDateHeure() {
    qDebug()<<"Protocole::demandeDateHeure()";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
    return NULL;
}

void Protocole::passageZero() {
    qDebug()<<"Protocole::passageZero()";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
}

void Protocole::passageEtalon() {
    qDebug()<<"Protocole::passageEtalon()";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
}

void Protocole::passageMesure() {
    qDebug()<<"Protocole::passageMesure()";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
}

void Protocole::reset() {
    qDebug()<<"Protocole::reset()";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
}

void Protocole::commandeEvent() {
    qDebug()<<"Protocole::commandeEvent()";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
}

void Protocole::commandeSpanZero() {
    qDebug()<<"Protocole::commandeSpanZero()";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
}

void Protocole::commandeSpan_146c(QString canal,ushort point) {
    qDebug()<<"Protocole::commandeSpan("<<canal<<","<<point<<")";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
}

void Protocole::commandeSpanO3(ushort canal, ushort pointO3) {
    qDebug()<<"Protocole::commandeSpanO3("<<canal<<","<<pointO3<<")";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
}

void Protocole::commandeSpanO3(ushort pointO3) {
    qDebug()<<"Protocole::commandeSpanO3("<<pointO3<<")";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
}

void Protocole::commandeSpanTpg(ushort pointO3) {
    qDebug()<<"Protocole::commandeSpanTpg("<<pointO3<<")";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
}

void Protocole::commandeSpanTpg(ushort pointNO, ushort pointO3) {
    qDebug()<<"Protocole::commandeSpanTpg("<<pointNO<<","<<pointO3<<")";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
}

void Protocole::standBy() {
    qDebug()<<"Protocole::standBy()";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
}

void Protocole::commandeSpan(QString canal,ushort point,ushort concO3) {
    qDebug()<<"Protocole::commandeSpan("<<canal<<","<<point<<","<<concO3<<")";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
}

void Protocole::commandeSpan(ushort canal,ushort point,ushort concO3) {
    qDebug()<<"Protocole::commandeSpan("<<canal<<","<<point<<","<<concO3<<")";
    qDebug()<<"La fonction ne doit pas être appelée de cette classe ";
    qDebug()<<"Et doit être redéfinie dans une classe dérivée";
}
