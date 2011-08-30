/*////////////////////////////////////////////////////////////
// \file Phase.cpp
// \brief Représente une phase lors du déroulement d'un test
// \author FOUQUART Christophe
// \version 1.0
// \date 25/03/2011
//
// TAM - Tests Automatiques Métrologiques
// Copyright (C) 2011 FOUQUART Christophe
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
////////////////////////////////////////////////////////////*/

#include "phase.h"

Phase::Phase()
{
    this->m_noPhase = 0;
    this->m_nbCyclesMesures = 0;
    this->m_idMolecule = 0;
    this->m_idConcentration = 0;
    this->m_commandeDebutPhase= NO_CMD;
    this->m_critereArretPrevu = false;
    this->m_criteresArret.reserve(2);
    this->m_criteresArret.append(0);
    this->m_criteresArret.append(0);
    this->m_criteresArret.append(0);
}

Phase::Phase(const Phase & phase) : QObject() {
    this->m_noPhase = phase.m_noPhase;
    this->m_tempsMaxPhase = phase.m_tempsMaxPhase;
    this->m_nbCyclesMesures = phase.m_nbCyclesMesures;
    this->m_idConcentration = phase.m_idConcentration;
    this->m_idMolecule = phase.m_idMolecule;
    this->m_tempsMaxPhase = phase.m_tempsMaxPhase;
    this->m_tempsStabilisation = phase.m_tempsStabilisation;
    this->m_tempsMoyennageMesure = phase.m_tempsMoyennageMesure;
    this->m_tempsAttenteEntreMesure= phase.m_tempsAttenteEntreMesure;
    this->m_critereArretPrevu = phase.m_critereArretPrevu;
    this->m_criteresArret = phase.m_criteresArret;
    this->m_commandeDebutPhase = phase.m_commandeDebutPhase;
}

Phase& Phase::operator=(const Phase& newPhase) {
    this->m_noPhase = newPhase.m_noPhase;
    this->m_tempsMaxPhase = newPhase.m_tempsMaxPhase;
    this->m_nbCyclesMesures = newPhase.m_nbCyclesMesures;
    this->m_idConcentration = newPhase.m_idConcentration;
    this->m_idMolecule = newPhase.m_idMolecule;
    this->m_tempsMaxPhase = newPhase.m_tempsMaxPhase;
    this->m_tempsStabilisation = newPhase.m_tempsStabilisation;
    this->m_tempsMoyennageMesure = newPhase.m_tempsMoyennageMesure;
    this->m_tempsAttenteEntreMesure= newPhase.m_tempsAttenteEntreMesure;
    this->m_critereArretPrevu = newPhase.m_critereArretPrevu;
    this->m_criteresArret = newPhase.m_criteresArret;
    this->m_commandeDebutPhase = newPhase.m_commandeDebutPhase;

    return *this;
}

QDomElement Phase::exportToXml(QDomDocument & xmlTest) {
    // Création de l'élément et ajout des attibuts s'ils existent
    QDomElement newPhase = xmlTest.createElement("phase");
    newPhase.setAttribute("no_phase",this->m_noPhase);
    if(!this->m_tempsMaxPhase.isNull())
        newPhase.setAttribute("tps_max_phase",this->m_tempsMaxPhase.toString("hh:mm:ss"));
    if(this->m_nbCyclesMesures>0)
       newPhase.setAttribute("nb_cycles_mesures",this->m_nbCyclesMesures);
    if(!this->m_tempsStabilisation.isNull())
        newPhase.setAttribute("tps_stabilisation",this->m_tempsStabilisation.toString("hh:mm:ss"));
    if(!this->m_tempsMoyennageMesure.isNull())
        newPhase.setAttribute("tps_moyennage_mesure",this->m_tempsMoyennageMesure.toString("hh:mm:ss"));
    if(!this->m_tempsAttenteEntreMesure.isNull())
        newPhase.setAttribute("tps_attente_entre_mesure",this->m_tempsAttenteEntreMesure.toString("hh:mm:ss"));

    // Création et ajout de l'élément XML polluant
    QDomElement polluant = xmlTest.createElement("polluant");
    polluant.setAttribute("type",m_idMolecule);
    polluant.setAttribute("id_concentration",m_idConcentration);
    newPhase.appendChild(polluant);

    // Création et ajout de l'élément XML critere_arret s'il existe
    if(this->m_critereArretPrevu) {
        QDomElement el_critereArret = xmlTest.createElement("critere_arret");
        el_critereArret.setAttribute("nb_cycle_mesure",this->m_criteresArret.at(CRITERE_ARRET_NB_CYCLES_MESURES));
        el_critereArret.setAttribute("pourcentage_stabilisation",this->m_criteresArret.at(CRITERE_ARRET_POURCENTAGE_STABILISATION));
        if(this->m_criteresArret.at(CRITERE_ARRET_UNITE)==POURCENTAGE)
            el_critereArret.setAttribute("unite","POURCENTAGE");
        else
            el_critereArret.setAttribute("unite","PPB");
        newPhase.appendChild(el_critereArret);
    }

    // Création et ajout de l'élément XML commande_debut_phase s'il existe
    if(this->m_commandeDebutPhase != NO_CMD) {
        QDomElement el_commandeDebutPhase = xmlTest.createElement("commande_debut_phase");
        QDomText str_cmdFinTs = xmlTest.createTextNode(commandesToString(this->m_commandeDebutPhase));
        el_commandeDebutPhase.appendChild(str_cmdFinTs);
        newPhase.appendChild(el_commandeDebutPhase);
    }

    return newPhase;
}

QPointer<Phase> Phase::importFromXml(const QDomElement & domPhase) {
    QPointer<Phase> newPhase = new Phase();

    newPhase->setNoPhase(domPhase.attribute("no_phase").toUShort());
    newPhase->setTpsMaxPhase(QTime::fromString(domPhase.attribute("tps_max_phase"),"hh:mm:ss"));
    newPhase->setNbCyclesMesures(domPhase.attribute("nb_cycles_mesures").toUShort());
    newPhase->setTpsStabilisation(QTime::fromString(domPhase.attribute("tps_stabilisation"),"hh:mm:ss"));
    newPhase->setTpsMoyennageMesure(QTime::fromString(domPhase.attribute("tps_moyennage_mesure"),"hh:mm:ss"));
    newPhase->setTpsAttenteEntreMesure(QTime::fromString(domPhase.attribute("tps_attente_entre_mesure"),"hh:mm:ss"));

    QDomElement el_critereArret = domPhase.firstChildElement("critere_arret");
    if(!el_critereArret.isNull()) {
        ushort nbCyclesMesures = el_critereArret.attribute("nb_cycle_mesure").toUShort();
        ushort pourcentageStabilisation = el_critereArret.attribute("pourcentage_stabilisation").toUShort();
        QString unite = el_critereArret.attribute("unite");
        newPhase->setCritereArretPrevu(true);
        newPhase->setCritereArret_NbCyclesMesures(nbCyclesMesures);
        newPhase->setCritereArret_PourcentageStabilisation(pourcentageStabilisation);
        if(unite.contains("POURCENTAGE"))
            newPhase->setCritereArret_Unite(POURCENTAGE);
        else
            newPhase->setCritereArret_Unite(PPB);
    }

    QDomElement polluant = domPhase.firstChildElement("polluant");
    newPhase->ajouterPolluant(polluant.attribute("type").toUShort(),polluant.attribute("id_concentration").toUShort());

    QDomElement el_commandeDebutPhase = domPhase.firstChildElement("commande_debut_phase");
    if(!el_commandeDebutPhase.isNull())
        newPhase->setCmdDebutPhase(stringToCommande(el_commandeDebutPhase.text()));
    else
        newPhase->setCmdDebutPhase(NO_CMD);

    return newPhase;
}

QPointer<Phase> Phase::getPhaseFromConf(const PhaseConfig & config) {
    QPointer<Phase> newPhase = new Phase();

    newPhase->setNbCyclesMesures(config.nbCyclesMesures);
    newPhase->setTpsAttenteEntreMesure(config.tempsAttenteEntreMesure);
    newPhase->setTpsMoyennageMesure(config.tempsMoyennageMesure);
    newPhase->setTpsStabilisation(config.tempsStabilisation);

    return newPhase;
}

QDebug operator<<(QDebug dbg, const Phase & phase) {
    dbg.nospace() << "Phase n° " << phase.getNoPhase()<<"\n";
    dbg.nospace() << "Identifiant Molecule " << phase.getIdMolecule()<<"\n";
    dbg.nospace() << "Identifiant Concentration " << phase.getIdConcentration()<<"\n";

    return dbg.space();
}
