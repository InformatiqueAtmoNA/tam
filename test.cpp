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

#include "test.h"

Test::Test()
{
    this->m_etalonPresent=false;
    this->m_idTest = 0;
    this->m_nbCyclesMesureParPhase = 0;
}

Test::Test(const Test& test) : QObject() {
    this->m_idTest = test.m_idTest;
    this->m_tempsMaxTest = test.m_tempsMaxTest;
    this->m_tempsAcquisition = test.m_tempsAcquisition;
    this->m_tempsStabilisation = test.m_tempsStabilisation;
    this->m_tempsMoyennageMesure = test.m_tempsMoyennageMesure;
    this->m_nbCyclesMesureParPhase = test.m_nbCyclesMesureParPhase;
    this->m_listeEquipement = test.m_listeEquipement;
    this->m_etalonPresent = test.m_etalonPresent;
    this->m_listePhases = test.m_listePhases;
}

bool Test::ajouterEquipement(Equipement peripherique) {
    // Si peripherqie est un étalon
    if(peripherique.getTypePeripherique()==ETALON) {
        // Si un étalon a déja été configuré, on retourne faux
        if(this->m_etalonPresent)
            return false;
        else // Sinon on indique la présence d'un étalon dans la configuration
            m_etalonPresent = true;
    }
    // Sinon on ajoute le périphérique à la liste des équipements
    this->m_listeEquipement.append(peripherique);
    return true;
}

void Test::ajouterPhase(Phase newPhase) {
    this->m_listePhases.append(newPhase);
}

void Test::ajouterPhase(ushort const & noPhase, Phase newPhase) {
    newPhase.setNoPhase(noPhase);
    this->m_listePhases.insert(noPhase-1, newPhase);
}

bool Test::exportToXml(QString const & nomFichier) {
    QDomDocument testXml;
    QFile file;
    QTextStream out;
    QDomImplementation impl;

    // Ouverture du nouveau fichier XML
    file.setFileName(nomFichier);
    if (!file.open(QIODevice::WriteOnly))
        return false;
    out.setDevice(&file);

    // On crée l'élément XML test, et on lui ajoute son id en attribut
    QDomElement el_test = testXml.createElement("test");
    el_test.setAttribute("id",QString::number(this->m_idTest));

    // insertion en début de document de <?xml version="1.0" ?>
    QDomNode noeud = testXml.createProcessingInstruction("xml","version=\"1.0\" encoding=\"ISO-8859-1\"");
    testXml.insertBefore(noeud,testXml.firstChild());
    QDomDocumentType type = impl.createDocumentType("test",0, "StructureTest.dtd");
    testXml.appendChild(type);

    // Pour chaque élément d'équipement, on crée un élement XML
    // que l'on rajoute à l'élément <equipement>
    QDomElement el_listeEquipement = testXml.createElement("equipement");

    for(int i=0;i < this->m_listeEquipement.size();i++) {
        Equipement equipement = this->m_listeEquipement.at(i);
        QDomElement el_equipement = equipement.exportToXml(testXml);
        el_listeEquipement.appendChild(el_equipement);
    }
    el_test.appendChild(el_listeEquipement);

    // Création de l'élément XML deroulement et ajout des paramètres s'ils existent
    QDomElement el_deroulement = testXml.createElement("deroulement");
    if(!this->m_tempsMaxTest.isNull())
        el_deroulement.setAttribute("tps_max_test",this->m_tempsMaxTest.toString("hh:mm:ss"));
    if(this->m_tempsAcquisition>0)
        el_deroulement.setAttribute("tps_acquisition",QString::number(this->m_tempsAcquisition));
    if(!this->m_tempsStabilisation.isNull())
        el_deroulement.setAttribute("tps_stabilisation",this->m_tempsStabilisation.toString("hh:mm:ss"));
    if(!this->m_tempsMoyennageMesure.isNull())
        el_deroulement.setAttribute("tps_moyennage_mesure",this->m_tempsMoyennageMesure.toString("hh:mm:ss"));
    if(this->m_nbCyclesMesureParPhase>0)
        el_deroulement.setAttribute("nb_cycles_mesures_par_phase",QString::number(this->m_nbCyclesMesureParPhase));

    // Pour chaque phase, on crée un élement XML
    // que l'on rajoute à l'élément <deroulement>
    for(int i=0;i < this->m_listePhases.size();i++) {
        Phase phase = this->m_listePhases.at(i);
        QDomElement el_phase = phase.exportToXml(testXml);
        el_deroulement.appendChild(el_phase);
    }
    el_test.appendChild(el_deroulement);

    testXml.appendChild(el_test);

    // Ecriture du fichier xml
    testXml.save(out,3);
    file.close();

    return true;
}

QPointer<Test> Test::importFromXml(QString const & nomFichier) {
    QDomDocument testXml;
    QFile file(nomFichier);
    QPointer<Test> p_test = new Test();

    // ouverture du fichier XML
    if (!file.open(QIODevice::ReadOnly))
        return 0;
    // Etablit le document XML à
    // partir des données du fichier (hiérarchie, etc)
    if (!testXml.setContent(&file)) {
        file.close();
        return 0;
    }
    file.close();

    // On récupère l'élément racine du fichier xml
    QDomElement el_test = testXml.documentElement();
    // On affecte l'id de test
    p_test.data()->setIdTest(el_test.attribute("id").toInt());

    // Récupère la liste des élément enfants de l'élément XML equipement
    QDomNodeList nl_listeEquipement = el_test.firstChildElement("equipement").childNodes();
    // Pour chaque élément enfant, on l'ajoute à la liste des équipements
    for(int i=0;i<nl_listeEquipement.size();i++) {
        QDomElement equipement = nl_listeEquipement.at(i).toElement();
        Equipement newEquipement = *(Equipement::importFromXml(equipement).data());
        p_test.data()->ajouterEquipement(newEquipement);
    }

    // Récupération de l'élément XML déroulement et de ses attributs
    QDomElement el_deroulement = el_test.firstChildElement("deroulement").toElement();

    QString tpsMaxTest = el_deroulement.attribute("tps_max_test");
    QString tpsAcquisition = el_deroulement.attribute("tps_acquisition");
    QString tpsStabilisation = el_deroulement.attribute("tps_stabilisation");
    QString tpsMoyennageMesure = el_deroulement.attribute("tps_moyennage_mesure");
    QString nbCyclesMesureParPhase = el_deroulement.attribute("nb_cycles_mesures_par_phase");

    p_test.data()->setTpsMaxTest(QTime::fromString(tpsMaxTest,"hh:mm:ss"));
    p_test.data()->setTempsAcquisition(tpsAcquisition.toUShort());
    p_test.data()->setTempsStabilisation(QTime::fromString(tpsStabilisation,"hh:mm:ss"));
    p_test.data()->setTempsMoyennageMesure(QTime::fromString(tpsMoyennageMesure,"hh:mm:ss"));
    p_test.data()->setNbCyclesMesureParPhase(nbCyclesMesureParPhase.toUShort());

    // Récupère la liste des élément enfants de l'élément XML déroulement
    QDomNodeList nl_listePhases = el_deroulement.childNodes();
    // Pour chaque élément enfant, on l'ajoute à la liste des phases
    for(int i=0;i<nl_listePhases.size();i++) {
        QDomElement phase = nl_listePhases.at(i).toElement();
        Phase newPhase = *(Phase::importFromXml(phase).data());
        p_test.data()->ajouterPhase(newPhase);
    }

    return p_test;
}
