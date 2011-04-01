/*    Copyright 2011 FOUQUART Christophe
//
//    This file is part of Test_Automatiques_Metrologiques.
//
//    Test_Automatiques_Metrologiques is free software:
//    you can redistribute it and/or modify it under the terms of the
//    GNU General Public License as published by the Free Software Foundation,
//    either version 3 of the License, or (at your option) any later version.
//
//    Test_Automatiques_Metrologiques is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Test_Automatiques_Metrologiques.
//    If not, see <http://www.gnu.org/licenses/>.
*/

#include "phase.h"

Phase::Phase()
{
    this->m_noPhase = 0;
    this->m_nbCyclesMesures = 0;
}

Phase::Phase(Phase const & phase) : QObject() {
    this->m_noPhase = phase.m_noPhase;
    this->m_tempsMaxPhase = phase.m_tempsMaxPhase;
    this->m_nbCyclesMesures = phase.m_nbCyclesMesures;
    this->m_listePolluants = phase.m_listePolluants;
    this->m_tempsMaxPhase = phase.m_tempsMaxPhase;
    this->m_tempsStabilisation = phase.m_tempsStabilisation;
    this->m_tempsMoyennageMesure = phase.m_tempsMoyennageMesure;
    this->m_criteresArret = phase.m_criteresArret;
    this->m_commandeFinTs = phase.m_commandeFinTs;
}

Phase& Phase::operator=(const Phase& newPhase) {
    this->m_noPhase = newPhase.m_noPhase;
    this->m_tempsMaxPhase = newPhase.m_tempsMaxPhase;
    this->m_nbCyclesMesures = newPhase.m_nbCyclesMesures;
    this->m_listePolluants = newPhase.m_listePolluants;
    this->m_tempsMaxPhase = newPhase.m_tempsMaxPhase;
    this->m_tempsStabilisation = newPhase.m_tempsStabilisation;
    this->m_tempsMoyennageMesure = newPhase.m_tempsMoyennageMesure;
    this->m_criteresArret = newPhase.m_criteresArret;
    this->m_commandeFinTs = newPhase.m_commandeFinTs;

    return *this;
}

void Phase::ajouterCritereArret(ushort const & nbCycleMesure, ushort const & pourcentageStabilisation) {
    this->m_criteresArret.reserve(2);
    this->m_criteresArret.append(nbCycleMesure);
    this->m_criteresArret.append(pourcentageStabilisation);
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

    // Création et ajout des éléments XML polluant
    QMapIterator<TypePolluant,ushort> i(m_listePolluants);
    while (i.hasNext()) {
        i.next();
        QDomElement polluant = xmlTest.createElement("polluant");
        polluant.setAttribute("type",i.key());
        polluant.setAttribute("point_gaz",QString::number(i.value()));
        newPhase.appendChild(polluant);
    }

    // Création et ajout de l'élément XML critere_arret s'il existe
    if(this->m_criteresArret.size()>0) {
        QDomElement el_critereArret = xmlTest.createElement("critere_arret");
        el_critereArret.setAttribute("nb_cycle_mesure",this->m_criteresArret.at(0));
        el_critereArret.setAttribute("pourcentage_stabilisation",this->m_criteresArret.at(1));
        newPhase.appendChild(el_critereArret);
    }

    // Création et ajout de l'élément XML commande_fin_ts s'il existe
    if(this->m_commandeFinTs != NO_CMD) {
        QDomElement el_commandeFinTs = xmlTest.createElement("commande_fin_ts");
        el_commandeFinTs.setNodeValue(commandesToString(this->m_commandeFinTs));
        newPhase.appendChild(el_commandeFinTs);
    }

    return newPhase;
}

QPointer<Phase> Phase::importFromXml(QDomElement const & domPhase) {
    QPointer<Phase> newPhase = new Phase();

    QString str_attribut = domPhase.attribute("no_phase");
    newPhase.data()->setNoPhase(str_attribut.toUShort());

    str_attribut = domPhase.attribute("tps_max_phase");
    QTime qtime_attribut = QTime::fromString(str_attribut,"hh:mm:ss");
    newPhase.data()->setTpsMaxPhase(qtime_attribut);

    str_attribut = domPhase.attribute("nb_cycles_mesures");
    newPhase.data()->setNbCyclesMesures(str_attribut.toUShort());

    str_attribut = domPhase.attribute("tps_stabilisation");
    qtime_attribut = QTime::fromString(str_attribut,"hh:mm:ss");
    newPhase.data()->setTpsStabilisation(qtime_attribut);

    str_attribut = domPhase.attribute("tps_moyennage_mesure");
    qtime_attribut = QTime::fromString(str_attribut,"hh:mm:ss");
    newPhase.data()->setTpsMoyennageMesure(qtime_attribut);

    QDomNodeList nodeList_critereArret = domPhase.elementsByTagName("critere_arret");
    if(!nodeList_critereArret.isEmpty()) {
        QDomElement el_critereArret = nodeList_critereArret.at(0).toElement();
        ushort nbCyclesMesures = el_critereArret.attribute("nb_cycle_mesure").toUShort();
        ushort pourcentageStabilisation = el_critereArret.attribute("pourcentage_stabilisation").toUShort();
        newPhase.data()->ajouterCritereArret(nbCyclesMesures,pourcentageStabilisation);
    }

    QDomNodeList nodeList_polluant = domPhase.elementsByTagName("polluant");
    for(int i=0;i<nodeList_polluant.count();i++) {
        QDomElement polluant = nodeList_polluant.at(i).toElement();
        QString str_type = polluant.attribute("type");
        newPhase.data()->ajouterPolluant(stringToTypePolluant(str_type),polluant.attribute("point_gaz").toUShort());
    }

    QDomNodeList nodeList_commandeFinTs = domPhase.elementsByTagName("critere_arret");
    if(!nodeList_commandeFinTs.isEmpty()) {
        QDomElement el_commandeFinTs = nodeList_commandeFinTs.at(0).toElement();
        newPhase.data()->setCmdFinTs(stringToCommande(el_commandeFinTs.text()));
    }
    else
        newPhase.data()->setCmdFinTs(NO_CMD);
    return newPhase;
}
