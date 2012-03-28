/*////////////////////////////////////////////////////
// \file commandesliste.cpp
// \brief Classe gérant les commandes utilisables selon les protocoles et le matériel
// \author FOUQUART Christophe
// \version 1.0
// \date 31/03/2011
//
// TAM - Tests Automatiques Métrologiques
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
