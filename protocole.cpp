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
void Protocole::setThreadComHandler(ThreadComHandler const & newThreadComHandler) {
    this->threadCommunication = &newThreadComHandler;
    connect(this,SIGNAL(envoiTrame(QString)),this->threadCommunication,SIGNAL(envoiTrame(QString)));
    connect(this->threadCommunication,SIGNAL(receptionTrame(QString)),this,SLOT(lectureTrame(QString)));
}

// Retourne l'instance de la classe CommunicationSerie
ThreadComHandler const* Protocole::getThreadComHandler() {
    return this->threadCommunication;
}

// Affecte un nouveau timeout en ms � la communication
void Protocole::setTimeOut(ushort const & newTimeOut) {
    this->timeout = newTimeOut;
}

// Retourne le timeout � la communication
ushort Protocole::getTimeOut() {
    return this->timeout;
}

// Affecte un nouvel �tat de la communication RS232
void Protocole::changementEtat(ushort const & newEtatCom) {
    this->flagEtatCom = newEtatCom;
}

// Retourne l'�tat de la communication RS232
ushort Protocole::getEtatCom() {
    return this->flagEtatCom;
}

// Execute une transaction avec le p�riph�rique (ecriture/attente r�ponse)
QString Protocole::transaction(QString const & commande) {
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
    // On attend timeout ms de recevoir la r�ponse
    while(!this->timerFini) {
        QCoreApplication::processEvents();
    }
    // Si pas de r�ponse
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
void Protocole::lectureTrame(QString const & data) {
    qDebug()<<"Trame re�ue : " << data.toAscii();
    this->trame = data;
    this->flagEtatCom = ETAT_LECTURE;
}

// Slot appel� lorsque le timer de communication est fini
void Protocole::timeoutCom() {
    this->timerFini = true;
}

// Commande au diluteur de se mettre � un certain point de gaz
void Protocole::commandeSpan(SpanHandler const & spanData) {
    qDebug()<<"commandeSpan("<<spanData.getCanal()<<","<<spanData.getPoint()<<")";
    qDebug()<<"Cette commande doit �tre r�impl�ment�e dans une classe d�riv�e";
}

// Commande au diluteur de se mettre au point de gaz zero
void Protocole::commandeSpanZero(QString const & canal) {
    qDebug()<<"commandeSpanZero("<<canal<<")";
    qDebug()<<"Cette commande doit �tre r�impl�ment�e dans une classe d�riv�e";
}

// Commande au diluteur de se mettre � un certain point de gaz
void Protocole::commandeSpanTpg(SpanHandler const & spanTpgData) {
    qDebug()<<"commandeSpan("<<spanTpgData.getCanal()<<","<<spanTpgData.getPoint()<<","<<spanTpgData.getConcO3()<<")";
    qDebug()<<"Cette commande doit �tre r�impl�ment�e dans une classe d�riv�e";
}

// Commande au diluteur de se mettre � un certain point de gaz O3
void Protocole::commandeSpanO3(SpanHandler const & spanO3Data) {
    qDebug()<<"commandeSpan("<<spanO3Data.getCanal()<<","<<spanO3Data.getPoint()<<")";
    qDebug()<<"Cette commande doit �tre r�impl�ment�e dans une classe d�riv�e";
}
