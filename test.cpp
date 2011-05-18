/*////////////////////////////////////////////////////////////
// \file test.cpp
// \brief Représente un un test
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


#include "test.h"

Test::Test()
{
    this->m_idTest = 0;
    this->m_nbCyclesMesureParPhase = 1;
    this->m_nbCyclesDePhase = 1;
    this->m_idSystemeEtalon = 0;
}

Test::Test(const Test& test) : QObject() {
    this->m_idTest = test.m_idTest;
    this->m_tempsMaxTest = test.m_tempsMaxTest;
    this->m_tempsAcquisition = test.m_tempsAcquisition;
    this->m_tempsStabilisation = test.m_tempsStabilisation;
    this->m_tempsMoyennageMesure = test.m_tempsMoyennageMesure;
    this->m_nbCyclesMesureParPhase = test.m_nbCyclesMesureParPhase;
    this->m_nbCyclesDePhase = test.m_nbCyclesDePhase;
    this->m_listePhases = test.m_listePhases;
}

void Test::ajouterPhase(const Phase & newPhase) {
    this->m_listePhases.insert(newPhase.getNoPhase(),newPhase);
}

void Test::ajouterPhase(ushort const noPhase, Phase & newPhase) {
    newPhase.setNoPhase(noPhase);
    this->m_listePhases.insert(noPhase, newPhase);
}

void Test::phaseInseree(ushort noPhase) {
    QMap<ushort,Phase> newListePhases;

    QMapIterator<ushort,Phase> iterator(this->m_listePhases);
    while(iterator.hasNext()) {
        iterator.next();
        if(iterator.key()<noPhase)
            newListePhases.insert(iterator.key(),iterator.value());
        else {
            Phase phase = iterator.value();
            phase.setNoPhase(iterator.key()+1);
            newListePhases.insert(iterator.key()+1,phase);
        }
    }
    this->m_listePhases = newListePhases;
}

void Test::supprimerPhase(const ushort noPhase) {
    QMap<ushort,Phase> newListePhases;

    QMapIterator<ushort,Phase> iterator(this->m_listePhases);
    while(iterator.hasNext()) {
        iterator.next();
        if(iterator.key()==noPhase)
            continue;
        if(iterator.key()<noPhase)
            newListePhases.insert(iterator.key(),iterator.value());
        else {
            Phase phase = iterator.value();
            phase.setNoPhase(iterator.key()-1);
            newListePhases.insert(iterator.key()-1,phase);
        }
    }
    this->m_listePhases = newListePhases;
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

    QDomElement el_etalon = testXml.createElement("etalon");
    el_etalon.setAttribute("id",this->m_idSystemeEtalon);
    el_etalon.setAttribute("type",typeTestToString(this->m_typeTest));

    el_listeEquipement.appendChild(el_etalon);

    for(int i=0;i < this->m_polluants.size();i++) {
        QDomElement polluant = testXml.createElement("polluant");
        polluant.setAttribute("type",this->m_polluants.at(i));
        el_listeEquipement.appendChild(polluant);
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
    if(!this->m_tempsAttenteEntreMesure.isNull())
        el_deroulement.setAttribute("tps_attente_entre_mesure_par_phase",this->m_tempsAttenteEntreMesure.toString("hh:mm:ss"));
    if(this->m_nbCyclesMesureParPhase>0)
        el_deroulement.setAttribute("nb_cycles_mesures_par_phase",QString::number(this->m_nbCyclesMesureParPhase));
    if(this->m_nbCyclesDePhase>0)
        el_deroulement.setAttribute("nb_cycles_de_phases",QString::number(this->m_nbCyclesDePhase));

    // Pour chaque phase, on crée un élement XML
    // que l'on rajoute à l'élément <deroulement>
    QMapIterator<ushort, Phase> iterator(this->m_listePhases);
    while (iterator.hasNext()) {
        iterator.next();
        Phase phase = iterator.value();
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
    p_test->setIdTest(el_test.attribute("id").toInt());
    // On affecte le type de test
    p_test->setTypeTest(el_test.attribute("type"));

    // Récupère la liste des élément enfants de l'élément XML equipement
    QDomNodeList nl_listeEquipement = el_test.firstChildElement("equipement").childNodes();

    // Pour chaque élément enfant, on l'ajoute à la liste des équipements
    for(int i=0;i<nl_listeEquipement.size();i++) {
        if(nl_listeEquipement.at(i).nodeName()=="etalon")
        {
            QDomElement el_etalon = nl_listeEquipement.at(i).toElement();
            p_test->setSystemeEtalon(el_etalon.attribute("id").toUInt());
        }
        else {
            QDomElement polluant = nl_listeEquipement.at(i).toElement();
            p_test->ajouterPolluant(polluant.attribute("type"));
        }
    }

    // Récupération de l'élément XML déroulement et de ses attributs
    QDomElement el_deroulement = el_test.firstChildElement("deroulement").toElement();

    QString tpsMaxTest = el_deroulement.attribute("tps_max_test");
    QString tpsAcquisition = el_deroulement.attribute("tps_acquisition");
    QString tpsStabilisation = el_deroulement.attribute("tps_stabilisation");
    QString tpsMoyennageMesure = el_deroulement.attribute("tps_moyennage_mesure");
    QString nbCyclesMesureParPhase = el_deroulement.attribute("nb_cycles_mesures_par_phase");
    QString nbCyclesDePhases= el_deroulement.attribute("nb_cycles_de_phases");
    QString tpsAttenteEntreMesure = el_deroulement.attribute("tps_attente_entre_mesure_par_phase");

    p_test->setTempsMaxTest(QTime::fromString(tpsMaxTest,"hh:mm:ss"));
    p_test->setTempsAcquisition(tpsAcquisition.toUShort());
    p_test->setTempsStabilisation(QTime::fromString(tpsStabilisation,"hh:mm:ss"));
    p_test->setTempsMoyennageMesure(QTime::fromString(tpsMoyennageMesure,"hh:mm:ss"));
    p_test->setTempsAttenteEntreMesure(QTime::fromString(tpsAttenteEntreMesure,"hh:mm:ss"));
    p_test->setNbCyclesMesureParPhase(nbCyclesMesureParPhase.toUShort());
    p_test->setNbCyclesDePhase(nbCyclesDePhases.toUShort());

    // Récupère la liste des élément enfants de l'élément XML déroulement
    QDomNodeList nl_listePhases = el_deroulement.childNodes();
    // Pour chaque élément enfant, on l'ajoute à la liste des phases
    for(int i=0;i<nl_listePhases.size();i++) {
        QDomElement phase = nl_listePhases.at(i).toElement();
        Phase newPhase = *(Phase::importFromXml(phase).data());
        p_test->ajouterPhase(newPhase);
    }

    return p_test;
}
