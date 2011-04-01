#include "commandesliste.h"

CommandesListe::CommandesListe()
{
    this->m_IntitulesCommandes.insert(RESET,"Reset");
    this->m_IntitulesCommandes.insert(OFFSET,"Offset");
    this->m_IntitulesCommandes.insert(MESURES,"Mesures");
    this->m_IntitulesCommandes.insert(ALARME,"Demande alarme");
    this->m_IntitulesCommandes.insert(DATE_HEURE,"Demande d'infos");
    this->m_IntitulesCommandes.insert(MODE_ZERO,"Passage mode zéro");
    this->m_IntitulesCommandes.insert(MODE_ETALON,"Passage mode étalon");
    this->m_IntitulesCommandes.insert(MODE_MESURE,"Passage mode mesure");
    this->m_IntitulesCommandes.insert(SPAN,"Commande span");
    this->m_IntitulesCommandes.insert(SPAN_ZERO,"Commande span zéro");
    this->m_IntitulesCommandes.insert(SPAN_TPG,"Commande span TPG");
    this->m_IntitulesCommandes.insert(SPAN_O3,"Commande span O3");
    this->m_IntitulesCommandes.insert(STAND_BY,"Stand-by");
    this->m_IntitulesCommandes.insert(EVENT,"Event");
}

QStringList CommandesListe::getIntitulesCommandes(QVector<Commandes> const & commandesAutorisees) {
    this->m_IndexCommandesListe.clear();
    QStringList intitulesCommandes;

    for(int i=0;i<commandesAutorisees.size();i++) {
        intitulesCommandes.append(this->m_IntitulesCommandes.value(commandesAutorisees[i]));
        this->m_IndexCommandesListe.insert(i,commandesAutorisees[i]);
    }
    return intitulesCommandes;
}

ushort CommandesListe::getIdCommande(ushort indexListe) {
    return m_IndexCommandesListe.value(indexListe);
}
