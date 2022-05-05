/*////////////////////////////////////////////////////////////
// \file test.cpp
// \brief Represente un un test
// \author FOUQUART Christophe
// \version 1.0
// \date 25/03/2011
//
// TAM - Tests Automatiques Metrologiques
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
////////////////////////////////////////////////////////////*/

#include "test.h"

Test::Test()
{
    this->m_idTest = 0;
    this->m_nbCyclesMesureParPhase = 1;
    this->m_nbCyclesDePhase = 1;
    this->m_idSystemeEtalon = 0;
    this->m_tempsAcquisition = 0;
}

Test::Test(const Test& test) : QObject()
{
    this->m_idTest = test.m_idTest;
    this->m_typeTest = test.m_typeTest;
    this->m_tempsAcquisition = test.m_tempsAcquisition;
    this->m_tempsStabilisation = test.m_tempsStabilisation;
    this->m_tempsMoyennageMesure = test.m_tempsMoyennageMesure;
    this->m_tempsAttenteEntreMesure = test.m_tempsAttenteEntreMesure;
    this->m_nbCyclesMesureParPhase = test.m_nbCyclesMesureParPhase;
    this->m_nbCyclesDePhase = test.m_nbCyclesDePhase;
    this->m_idSystemeEtalon = test.m_idSystemeEtalon;
    this->m_polluants = test.m_polluants;
    this->m_listePhases = test.m_listePhases;
    this->m_critere1 = test.m_critere1;
    this->m_critere2 = test.m_critere2;
    this->m_critere3 = test.m_critere3;
}

Test& Test::operator =(const Test& test)
{
    this->m_idTest = test.m_idTest;
    this->m_typeTest = test.m_typeTest;
    this->m_tempsAcquisition = test.m_tempsAcquisition;
    this->m_tempsStabilisation = test.m_tempsStabilisation;
    this->m_tempsMoyennageMesure = test.m_tempsMoyennageMesure;
    this->m_tempsAttenteEntreMesure = test.m_tempsAttenteEntreMesure;
    this->m_nbCyclesMesureParPhase = test.m_nbCyclesMesureParPhase;
    this->m_nbCyclesDePhase = test.m_nbCyclesDePhase;
    this->m_idSystemeEtalon = test.m_idSystemeEtalon;
    this->m_polluants = test.m_polluants;
    this->m_listePhases = test.m_listePhases;
    this->m_critere1 = test.m_critere1;
    this->m_critere2 = test.m_critere2;
    this->m_critere3 = test.m_critere3;

    return *this;
}

QMap<ushort,QTime> Test::getListeTempsMaxPhase()
{
    QMap<ushort,QTime> listeTempsMaxPhaseDefinis;

    for(ushort i=1;i<=this->getNbPhases();i++)
    {
        Phase phase = this->getPhase(i);
        QTime tempsMaxPhase = phase.getTpsMaxPhase();
        listeTempsMaxPhaseDefinis.insert(i,tempsMaxPhase);
    }

    return listeTempsMaxPhaseDefinis;
}

QVector<ushort> Test::getListeCommandeDebutPhaseDefinies()
{
    QVector<ushort> listeCommandeDebutPhaseDefinies;

    for(ushort i=1;i<=this->getNbPhases();i++)
    {
        Phase phase = this->getPhase(i);
        if(phase.getCmdDebutPhase()!=NO_CMD)
            listeCommandeDebutPhaseDefinies.append(i);
    }

    return listeCommandeDebutPhaseDefinies;
}

QVector<ushort> Test::getListeCritereArretDefinis()
{
    QVector<ushort> listeCritereArretDefinis;

    for(ushort i=1;i<=this->getNbPhases();i++)
    {
        if(this->getPhase(i).critereArretExiste())
            listeCritereArretDefinis.append(i);
    }

    return listeCritereArretDefinis;
}


void Test::ajouterPhase(const Phase & newPhase)
{
    this->m_listePhases.insert(newPhase.getNoPhase(),newPhase);
}

void Test::ajouterPhase(ushort const noPhase, Phase & newPhase)
{
    newPhase.setNoPhase(noPhase);
    this->m_listePhases.insert(noPhase, newPhase);
}

void Test::phaseInseree(ushort noPhase)
{
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

void Test::supprimerPhase(const ushort noPhase)
{
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

bool Test::exportToXml(QString const & nomFichier)
{
    QDomDocument testXml;
    QFile file;
    QTextStream out;
    QDomImplementation impl;

    // Si le fichier existe dejà
    if(QFile::exists(nomFichier)) {
        QMessageBox msgBox;
        msgBox.setText(QLatin1String("Le fichier existe dejà"));
        msgBox.setInformativeText(QLatin1String("Voulez-vous le remplacer ?"));
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();

        if(ret==QMessageBox::Cancel)
            return false;
    }

    // Ouverture du nouveau fichier XML
    file.setFileName(nomFichier);
    if (!file.open(QIODevice::WriteOnly))
        return false;
    out.setDevice(&file);

    // On cree l'element XML test, et on lui ajoute son id en attribut
    QDomElement el_test = testXml.createElement("test");
    el_test.setAttribute("id",QString::number(this->m_idTest));
    el_test.setAttribute("type",typeTestToString(this->m_typeTest));

    // insertion en debut de document de <?xml version="1.0" ?>
    QDomNode noeud = testXml.createProcessingInstruction("xml","version=\"1.0\" encoding=\"ISO-8859-1\"");
    testXml.insertBefore(noeud,testXml.firstChild());
    QDomDocumentType type = impl.createDocumentType("test",0, "StructureTest.dtd");
    testXml.appendChild(type);

    // Pour chaque element d'equipement, on cree un element XML
    // que l'on rajoute à l'element <equipement>
    QDomElement el_listeEquipement = testXml.createElement("equipement");

    QDomElement el_etalon = testXml.createElement("etalon");
    el_etalon.setAttribute("id",this->m_idSystemeEtalon);

    el_listeEquipement.appendChild(el_etalon);

    el_test.appendChild(el_listeEquipement);

    // Creation de l'element XML deroulement et ajout des paramètres s'ils existent
    QDomElement el_deroulement = testXml.createElement("deroulement");
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
    if(this->m_critere1>0)
        el_deroulement.setAttribute("critere_1",QString::number(this->m_critere1));
    if(this->m_critere2>0)
        el_deroulement.setAttribute("critere_2",QString::number(this->m_critere2));
    if(this->m_critere3>0)
        el_deroulement.setAttribute("critere_3",QString::number(this->m_critere3));

    // Pour chaque phase, on cree un element XML
    // que l'on rajoute à l'element <deroulement>
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

QPointer<Test> Test::importFromXml(QString const & nomFichier)
{
    QDomDocument testXml;
    QFile file(nomFichier);
    QPointer<Test> p_test = new Test();

    // ouverture du fichier XML
    if (!file.open(QIODevice::ReadOnly))
        return 0;
    // Etablit le document XML à
    // partir des donnees du fichier (hierarchie, etc)
    if (!testXml.setContent(&file)) {
        file.close();
        return 0;
    }
    file.close();

    // On recupère l'element racine du fichier xml
    QDomElement el_test = testXml.documentElement();
    // On affecte l'id de test
    p_test->setIdTest(el_test.attribute("id").toInt());
    // On affecte le type de test
    QString typeTest = el_test.attribute("type");
    p_test->setTypeTest(typeTest);

    // Recupère la liste des element enfants de l'element XML equipement
    QDomNodeList nl_listeEquipement = el_test.firstChildElement("equipement").childNodes();

    // Pour chaque element enfant, on l'ajoute à la liste des equipements
    for(int i=0;i<nl_listeEquipement.size();i++) {
        QDomElement el_etalon = nl_listeEquipement.at(i).toElement();
        p_test->setSystemeEtalon(el_etalon.attribute("id").toUInt());
    }

//    // Pour chaque element enfant, on l'ajoute à la liste des equipements
//    for(int i=0;i<nl_listeEquipement.size();i++) {
//        if(nl_listeEquipement.at(i).nodeName()=="etalon")
//        {
//            QDomElement el_etalon = nl_listeEquipement.at(i).toElement();
//            p_test->setSystemeEtalon(el_etalon.attribute("id").toUInt());
//        }
//        else {
//            QDomElement polluant = nl_listeEquipement.at(i).toElement();
//            p_test->ajouterPolluant(polluant.attribute("type"));
//        }
//    }

    // Recuperation de l'element XML deroulement et de ses attributs
    QDomElement el_deroulement = el_test.firstChildElement("deroulement").toElement();

    QString tpsAcquisition = el_deroulement.attribute("tps_acquisition");
    QString tpsStabilisation = el_deroulement.attribute("tps_stabilisation");
    QString tpsMoyennageMesure = el_deroulement.attribute("tps_moyennage_mesure");
    QString nbCyclesMesureParPhase = el_deroulement.attribute("nb_cycles_mesures_par_phase");
    QString nbCyclesDePhases= el_deroulement.attribute("nb_cycles_de_phases");
    QString tpsAttenteEntreMesure = el_deroulement.attribute("tps_attente_entre_mesure_par_phase");
    QString critere1 = el_deroulement.attribute("critere_1");
    QString critere2 = el_deroulement.attribute("critere_2");
    QString critere3 = el_deroulement.attribute("critere_3");

    p_test->setTempsAcquisition(tpsAcquisition.toUShort());
    p_test->setTempsStabilisation(QTime::fromString(tpsStabilisation,"hh:mm:ss"));
    p_test->setTempsMoyennageMesure(QTime::fromString(tpsMoyennageMesure,"hh:mm:ss"));
    p_test->setTempsAttenteEntreMesure(QTime::fromString(tpsAttenteEntreMesure,"hh:mm:ss"));
    p_test->setNbCyclesMesureParPhase(nbCyclesMesureParPhase.toUShort());
    p_test->setNbCyclesDePhase(nbCyclesDePhases.toUShort());
    p_test->setCritere1(critere1.toDouble());
    p_test->setCritere2(critere2.toDouble());
    p_test->setCritere3(critere3.toDouble());

    // Recupère la liste des element enfants de l'element XML deroulement
    QDomNodeList nl_listePhases = el_deroulement.childNodes();
    // Pour chaque element enfant, on l'ajoute à la liste des phases
    for(int i=0;i<nl_listePhases.size();i++) {
        QDomElement phase = nl_listePhases.at(i).toElement();
        Phase newPhase = *(Phase::importFromXml(phase).data());
        p_test->ajouterPhase(newPhase);
    }

    return p_test;
}
