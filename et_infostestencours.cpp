/*////////////////////////////////////////////////////////////
// \file et_infostestencours.cpp
// \brief Classe regroupant les infos d'exécution d'un test
// \author FOUQUART Christophe
// \version 1.0
// \date 11/08/2011
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

#include "et_infostestencours.h"
#include "ui_et_infostestencours.h"

et_InfosTestEnCours::et_InfosTestEnCours(QPointer<BdHandler> bdHandler,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::et_InfosTestEnCours)
{
    ui->setupUi(this);

    m_bdHandler = bdHandler;
    m_niveauInfos = this->ui->comboBox_NiveauInfos->currentIndex();

    connect(ui->comboBox_NiveauInfos,SIGNAL(currentIndexChanged(int)),this,SLOT(comboBoxNiveauInfosIndexChanged(int)));
}

et_InfosTestEnCours::~et_InfosTestEnCours()
{
    delete ui;
}

void et_InfosTestEnCours::afficherTraceTest(const QString trace,const ushort niveau)
{
    if(niveau>m_niveauInfos)
        return;

    QColor couleurTexte;

    switch(niveau) {
    case 0:
        ui->textEdit_TraceTest->setTextColor(couleurTexte.black());
        break;
    case 1:
        ui->textEdit_TraceTest->setTextColor(couleurTexte.blue());
        break;
    case 2:
        ui->textEdit_TraceTest->setTextColor(couleurTexte.red());
        break;
    }

    ui->textEdit_TraceTest->append(trace);
    qDebug()<<trace;
}

void et_InfosTestEnCours::afficherParamsTest(QPointer<et_ParamsTest> paramsTest)
{
    bool remplirFichierCSV = false;
    if(!paramsTest->m_fichierCSV.isNull() && paramsTest->m_fichierCSV->isOpen())
        remplirFichierCSV = true;

    QString trace = "---------------------\n";
    ui->textEdit_ParametresTest->append(trace);

    trace = QString::number(paramsTest->m_test->getIdTest());
    ui->textEdit_ParametresTest->append("N° de test : "+trace);
    if(remplirFichierCSV)
        paramsTest->m_fichierCSV->write(QString("N° de test :;"+trace+"\n").toAscii());

    trace = paramsTest->m_nomTest;
    ui->textEdit_ParametresTest->append("Nom du test : "+trace);
    if(remplirFichierCSV)
        paramsTest->m_fichierCSV->write(QString("Nom du test :;"+trace+"\n").toAscii());

    trace = typeTestToString(paramsTest->m_test->getTypeTest());
    ui->textEdit_ParametresTest->append("Type de test : "+trace);
    if(remplirFichierCSV)
        paramsTest->m_fichierCSV->write(QString("Type de test :;"+trace+"\n").toAscii());

    trace = paramsTest->m_nomOperateur;
    ui->textEdit_ParametresTest->append("Opérateur : "+trace);
    trace.append("\n");
    if(remplirFichierCSV)
        paramsTest->m_fichierCSV->write(QString("Opérateur :;"+trace).toAscii());

    trace = paramsTest->m_designationLieu;
    ui->textEdit_ParametresTest->append("Lieu : "+trace);
    if(remplirFichierCSV)
        paramsTest->m_fichierCSV->write(QString("Lieu :;"+trace+"\n").toAscii());

    trace = "\n----Equipement----";
    ui->textEdit_ParametresTest->append(trace);
    if(remplirFichierCSV)
        paramsTest->m_fichierCSV->write(QString(trace+"\n").toAscii());

    trace = "Système d'étalonnage :";
    ui->textEdit_ParametresTest->append(trace);
    if(remplirFichierCSV)
        paramsTest->m_fichierCSV->write(QString(trace+"\n").toAscii());

    qDebug()<<paramsTest->m_test->getIdSystemeEtalon();
    QSqlRecord* systemeEtalonRow = m_bdHandler->getSystemeEtalonRow(paramsTest->m_test->getIdSystemeEtalon());
    ushort idCalibrateur = systemeEtalonRow->value(SYS_ETALON_DILUTEUR).toUInt();
    ushort idBouteille = systemeEtalonRow->value(SYS_ETALON_BOUTEILLE).toUInt();
    ushort idGZero = systemeEtalonRow->value(SYS_ETALON_GZERO).toUInt();

    delete systemeEtalonRow;

    trace = m_bdHandler->getEquipementNumSerie(idCalibrateur);
    ui->textEdit_ParametresTest->append("Calibrateur : "+trace);
    if(remplirFichierCSV)
        paramsTest->m_fichierCSV->write(QString("Calibrateur :;"+trace+"\n").toAscii());

    trace = m_bdHandler->getEquipementNumSerie(idBouteille);
    ui->textEdit_ParametresTest->append("Bouteille : "+trace);
    if(remplirFichierCSV)
        paramsTest->m_fichierCSV->write(QString("Bouteille :;"+trace+"\n").toAscii());

    trace = m_bdHandler->getEquipementNumSerie(idGZero);
    ui->textEdit_ParametresTest->append("Générateur d'air zéro : "+trace);
    if(remplirFichierCSV)
        paramsTest->m_fichierCSV->write(QString("Générateur d'air zéro :;"+trace+"\n").toAscii());

    trace = "\nAnalyseurs :";
    ui->textEdit_ParametresTest->append(trace);
    if(remplirFichierCSV)
        paramsTest->m_fichierCSV->write(QString(trace+"\n").toAscii());

    QMapIterator<ushort,QString> it_listeNumSerieAna(paramsTest->m_listeNumSerieAnalyseurs);

    ushort cptAna=1;
    while(it_listeNumSerieAna.hasNext()){
        it_listeNumSerieAna.next();

        ui->textEdit_ParametresTest->append("ANA "+QString::number(cptAna)+":"+it_listeNumSerieAna.value());
        if(remplirFichierCSV)
            paramsTest->m_fichierCSV->write(QString("ANA "+QString::number(cptAna)+": ;"+it_listeNumSerieAna.value()+"\n").toAscii());
        cptAna++;
    }

    trace.append("\n");
    if(remplirFichierCSV)
        paramsTest->m_fichierCSV->write(trace.toAscii());

    trace = "----Paramètres----";
    ui->textEdit_ParametresTest->append(trace);
    if(remplirFichierCSV)
        paramsTest->m_fichierCSV->write(QString(trace+"\n").toAscii());

    trace = QString::number(paramsTest->m_test->getTempsAcquisition());
    ui->textEdit_ParametresTest->append("Temps d'acquisition : "+trace);
    if(remplirFichierCSV)
        paramsTest->m_fichierCSV->write(QString("Temps d'acquisition :;"+trace+"\n").toAscii());

    trace = QString::number(paramsTest->m_test->getNbCyclesDePhases());
    ui->textEdit_ParametresTest->append("Nombre de cycles de phases : "+trace);
    if(remplirFichierCSV)
        paramsTest->m_fichierCSV->write(QString("Nombre de cycles de phases :;"+trace+"\n").toAscii());

    trace = "--Déroulement--";
    ui->textEdit_ParametresTest->append(trace);
    paramsTest->m_fichierCSV->write(QString(trace+"\n").toAscii());

    for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
        trace = "Numéro de phase : ";
        trace.append(QString::number(i));
        ui->textEdit_ParametresTest->append(trace);

        trace = "Nombre de cycles de mesures : ";
        trace.append(QString::number(paramsTest->m_test->getPhase(i).getNbCyclesMesures()));
        ui->textEdit_ParametresTest->append(trace);

        trace = "Temps de stabilisation : ";
        trace.append(paramsTest->m_test->getPhase(i).getTpsStabilisation().toString("hh:mm:ss")+";");
        ui->textEdit_ParametresTest->append(trace);

        trace = "Temps de moyennage des mesures : ";
        trace.append(paramsTest->m_test->getPhase(i).getTpsMoyennageMesure().toString("hh:mm:ss"));
        ui->textEdit_ParametresTest->append(trace);

        trace = "Temps d'attente entre chaque cycle de mesures : ";
        QString tpsAttenteEntreCycleMesures = paramsTest->m_test->getPhase(i).getTpsAttenteEntreMesure().toString("hh:mm:ss");
        if(tpsAttenteEntreCycleMesures.isEmpty())
            trace.append("--");
        else
            trace.append(tpsAttenteEntreCycleMesures);
        ui->textEdit_ParametresTest->append(trace);

        trace = "Commande de début de phase : ";
        QString cmdDebutPhase = commandesToString(paramsTest->m_test->getPhase(i).getCmdDebutPhase());
        if(cmdDebutPhase == "NO_CMD")
            trace.append("--");
        else
            trace.append(cmdDebutPhase);
        ui->textEdit_ParametresTest->append(trace);

        trace = "Critère d'arrêt : ";
        if(paramsTest->m_test->getPhase(i).critereArretExiste())
            trace.append("OUI;");
        else
            trace.append("NON;");
        ui->textEdit_ParametresTest->append(trace);

        trace = "Variation : ";
        if(paramsTest->m_test->getPhase(i).critereArretExiste())
            trace.append(QString::number(paramsTest->m_test->getPhase(i).getCritereArret_PourcentageStabilisation()));
        else
            trace.append("-");
        ui->textEdit_ParametresTest->append(trace);

        trace = "Pendant N cycles de mesure : ";
        if(paramsTest->m_test->getPhase(i).critereArretExiste())
            trace.append(QString::number(paramsTest->m_test->getPhase(i).getCritereArret_NbCyclesMesures()));
        else
            trace.append("-");
        ui->textEdit_ParametresTest->append(trace);


        trace = "Temps maximum de phase : ";
        QTime tpsMaxPhase = paramsTest->m_test->getPhase(i).getTpsMaxPhase();
        if(tpsMaxPhase.isNull() || !tpsMaxPhase.isValid() || (tpsMaxPhase.hour()==0 && tpsMaxPhase.minute()==0 && tpsMaxPhase.second()==0))
            trace.append("-");
        else
            trace.append(tpsMaxPhase.toString("hh:mm:ss"));
        ui->textEdit_ParametresTest->append(trace);

        trace = "Polluant : ";
        QSqlRecord* record = m_bdHandler->getMoleculeRow(paramsTest->m_test->getPhase(i).getIdMolecule());
        trace.append(record->value(MOLECULE_FORMULE).toString());
        delete record;
        ui->textEdit_ParametresTest->append(trace);

        trace = "Point de consigne : ";
        record = m_bdHandler->getConcentrationRow(paramsTest->m_test->getPhase(i).getIdConcentration());
        trace.append(record->value(CONCENTRATION_POINT).toString());
        delete record;
        ui->textEdit_ParametresTest->append(trace);

        trace = "Consigne Ozone : ";
        record = m_bdHandler->getConcentrationRow(paramsTest->m_test->getPhase(i).getIdConcentration());
        trace.append(record->value(CONCENTRATION_OZONE).toString());
        delete record;
        ui->textEdit_ParametresTest->append(trace);

        trace = "Concentration réelle : ";
        record = m_bdHandler->getConcentrationRow(paramsTest->m_test->getPhase(i).getIdConcentration());
        trace.append(record->value(CONCENTRATION_REELLE).toString());
        delete record;
        ui->textEdit_ParametresTest->append(trace);

        QMap<ushort, QPointer<QSqlRelationalTableModel> > mapConcentrationsAssociee;
        ushort nbMaxConcentrationAssociees=0;

        QPointer<QSqlRelationalTableModel> model = m_bdHandler->getConcentrationAssocieeModel(paramsTest->m_test->getPhase(i).getIdConcentration());
        if(model->rowCount()>nbMaxConcentrationAssociees)
            nbMaxConcentrationAssociees = model->rowCount();
        mapConcentrationsAssociee.insert(i,model);

        for(int j=0;j<nbMaxConcentrationAssociees;j++) {
            trace = "Polluant associe " + QString::number(j+1)+" : ";
            QString trace2 = "Concentration associée " + QString::number(j+1)+" : ";

            QPointer<QSqlRelationalTableModel> model = mapConcentrationsAssociee.value(i);

            QSqlRecord record = model->record(j);
            if(!record.isEmpty()) {
                trace.append(record.value(CONC_ASSOCIEE_FORMULE).toString());
                trace2.append(record.value(CONC_ASSOCIEE_CONCENTRATION).toString());
                ui->textEdit_ParametresTest->append(trace);
                ui->textEdit_ParametresTest->append(trace2);
            }
        }
        ui->textEdit_ParametresTest->append("\n");
    }

    if(remplirFichierCSV) {
        trace = "Numéro de phase :;";
        for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
            trace.append(QString::number(i)+";");
        }
        trace.append("\n");
        paramsTest->m_fichierCSV->write(trace.toAscii());

        trace = "Nombre de cycles de mesures :;";
        for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
            trace.append(QString::number(paramsTest->m_test->getPhase(i).getNbCyclesMesures())+";");
        }
        trace.append("\n");
        paramsTest->m_fichierCSV->write(trace.toAscii());

        trace = "Temps de stabilisation :;";
        for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
            trace.append(paramsTest->m_test->getPhase(i).getTpsStabilisation().toString("hh:mm:ss")+";");
        }
        trace.append("\n");
        paramsTest->m_fichierCSV->write(trace.toAscii());

        trace = "Temps de moyennage des mesures :;";
        for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
            trace.append(paramsTest->m_test->getPhase(i).getTpsMoyennageMesure().toString("hh:mm:ss")+";");
        }
        trace.append("\n");
        paramsTest->m_fichierCSV->write(trace.toAscii());

        trace = "Temps d'attente entre chaque cycle de mesures :;";
        for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
            QString tpsAttenteEntreCycleMesures = paramsTest->m_test->getPhase(i).getTpsAttenteEntreMesure().toString("hh:mm:ss");
            if(tpsAttenteEntreCycleMesures.isEmpty())
                trace.append("--;");
            else
                trace.append(tpsAttenteEntreCycleMesures+";");
        }
        trace.append("\n");
        paramsTest->m_fichierCSV->write(trace.toAscii());

        trace = "Commande de début de phase :;";
        for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
            QString cmdDebutPhase = commandesToString(paramsTest->m_test->getPhase(i).getCmdDebutPhase());
            if(cmdDebutPhase == "NO_CMD")
                trace.append("--;");
            else
                trace.append(cmdDebutPhase+";");
        }
        trace.append("\n");
        paramsTest->m_fichierCSV->write(trace.toAscii());

        trace = "Critère d'arrêt :;";
        for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
            if(paramsTest->m_test->getPhase(i).critereArretExiste())
                trace.append("OUI;");
            else
                trace.append("NON;");
        }
        trace.append("\n");
        paramsTest->m_fichierCSV->write(trace.toAscii());

        trace = "Variation :;";
        for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
            if(paramsTest->m_test->getPhase(i).critereArretExiste())
                trace.append(QString::number(paramsTest->m_test->getPhase(i).getCritereArret_PourcentageStabilisation())+";");
            else
                trace.append("-;");
        }
        trace.append("\n");
        paramsTest->m_fichierCSV->write(trace.toAscii());

        trace = "Pendant N cycles de mesure :;";
        for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
            if(paramsTest->m_test->getPhase(i).critereArretExiste())
                trace.append(QString::number(paramsTest->m_test->getPhase(i).getCritereArret_NbCyclesMesures())+";");
            else
                trace.append("-;");
        }
        trace.append("\n");
        paramsTest->m_fichierCSV->write(trace.toAscii());

        trace = "Temps maximum de phase :;";
        for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
            QTime tpsMaxPhase = paramsTest->m_test->getPhase(i).getTpsMaxPhase();
            if(tpsMaxPhase.isNull() || !tpsMaxPhase.isValid() || (tpsMaxPhase.hour()==0 && tpsMaxPhase.minute()==0 && tpsMaxPhase.second()==0))
                trace.append("-;");
            else
                trace.append(tpsMaxPhase.toString("hh:mm:ss")+";");
        }

        trace.append("\n");
        paramsTest->m_fichierCSV->write(trace.toAscii());

        trace = "Polluant :;";
        for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
            QSqlRecord* record = m_bdHandler->getMoleculeRow(paramsTest->m_test->getPhase(i).getIdMolecule());
            trace.append(record->value(MOLECULE_FORMULE).toString()+";");
            delete record;
        }
        trace.append("\n");
        paramsTest->m_fichierCSV->write(trace.toAscii());

        trace = "Point de consigne :;";
        for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
            QSqlRecord* record = m_bdHandler->getConcentrationRow(paramsTest->m_test->getPhase(i).getIdConcentration());
            trace.append(record->value(CONCENTRATION_POINT).toString()+";");
            delete record;
        }
        trace.append("\n");
        paramsTest->m_fichierCSV->write(trace.toAscii());

        trace = "Consigne Ozone :;";
        for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
            QSqlRecord* record = m_bdHandler->getConcentrationRow(paramsTest->m_test->getPhase(i).getIdConcentration());
            trace.append(record->value(CONCENTRATION_OZONE).toString()+";");
            delete record;
        }
        trace.append("\n");
        paramsTest->m_fichierCSV->write(trace.toAscii());

        trace = "Concentration réelle :;";
        for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
            QSqlRecord* record = m_bdHandler->getConcentrationRow(paramsTest->m_test->getPhase(i).getIdConcentration());
            trace.append(record->value(CONCENTRATION_REELLE).toString()+";");
            delete record;
        }
        trace.append("\n");
        paramsTest->m_fichierCSV->write(trace.toAscii());

        QMap<ushort, QPointer<QSqlRelationalTableModel> > mapConcentrationsAssociee;
        ushort nbMaxConcentrationAssociees=0;

        for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
            QPointer<QSqlRelationalTableModel> model = m_bdHandler->getConcentrationAssocieeModel(paramsTest->m_test->getPhase(i).getIdConcentration());
            if(model->rowCount()>nbMaxConcentrationAssociees)
                nbMaxConcentrationAssociees = model->rowCount();
            mapConcentrationsAssociee.insert(i,model);
        }

        for(int i=0;i<nbMaxConcentrationAssociees;i++) {
            trace = "Polluant associe "+QString::number(i+1)+" :;";
            QString trace2 = "Concentration associée "+QString::number(i+1)+" :;";

            for(int j=1;j<=paramsTest->m_test->getNbPhases();j++) {
                QPointer<QSqlRelationalTableModel> model = mapConcentrationsAssociee.value(j);

                QSqlRecord record = model->record(i);
                if(record.isEmpty()) {
                    trace.append("-;");
                    trace2.append("-;");
                }
                else {
                    trace.append(record.value(CONC_ASSOCIEE_FORMULE).toString()+";");
                    trace2.append(record.value(CONC_ASSOCIEE_CONCENTRATION).toString()+";");
                }
            }
            trace.append("\n");
            trace2.append("\n");
            paramsTest->m_fichierCSV->write(trace.toAscii());
            paramsTest->m_fichierCSV->write(trace2.toAscii());
        }
        trace = "Date et heure;Cycle de phase N°;Phase N°;Position durant phase;";
        paramsTest->m_fichierCSV->write(trace.toAscii());
        paramsTest->m_fichierCSV->flush();
    }
}

void et_InfosTestEnCours::enregistrerParamsTest(QPointer<et_ParamsTest> paramsTest)
{
    QPointer<QSqlTableModel> model = m_bdHandler->getTestMetroModel();

    QSqlRecord enregistrement = model->record();

    afficherTraceTest("Enregistrement des informations de test",2);
    afficherTraceTest("ID Test_XML "+QString::number(paramsTest->m_id_TestXML),2);
    afficherTraceTest("Type de Test "+typeTestToString( paramsTest->m_test->getTypeTest()),2);
    enregistrement.setValue(TEST_METRO__TYPE_TEST,QVariant::fromValue(typeTestToString( paramsTest->m_test->getTypeTest())));
    afficherTraceTest("ID Opérateur "+QString::number(paramsTest->m_idOperateur),2);
    enregistrement.setValue(TEST_METRO_ID_OPERATEUR,QVariant::fromValue(paramsTest->m_idOperateur));
    afficherTraceTest("ID Système Etalon "+QString::number(paramsTest->m_test->getIdSystemeEtalon()),2);
    enregistrement.setValue(TEST_METRO_ID_SYSTEME_ETALON,QVariant::fromValue(paramsTest->m_test->getIdSystemeEtalon()));
    afficherTraceTest("ID Lieu "+QString::number(paramsTest->m_idLieu),2);
    enregistrement.setValue(TEST_METRO_ID_LIEU,QVariant::fromValue(paramsTest->m_idLieu));
    afficherTraceTest("Pression "+QString::number(paramsTest->m_pression),2);
    enregistrement.setValue(TEST_METRO_PRESSION,QVariant::fromValue(paramsTest->m_pression));
    afficherTraceTest("Température "+QString::number(paramsTest->m_temperature),2);
    enregistrement.setValue(TEST_METRO_TEMPERATURE,QVariant::fromValue(paramsTest->m_temperature));
    afficherTraceTest("Date début "+paramsTest->m_dateHeureDebut.toString("yyyy-MM-dd hh:mm:ss"),2);
    enregistrement.setValue(TEST_METRO_DATE_DEBUT,QVariant::fromValue(paramsTest->m_dateHeureDebut.toString("yyyy-MM-dd hh:mm:ss")));
    afficherTraceTest("Temps d'acquisistion "+paramsTest->m_test->getTempsAcquisition(),2);
    enregistrement.setValue(TEST_METRO_TPS_ACQUISITION,QVariant::fromValue(paramsTest->m_test->getTempsAcquisition()));
    model->insertRecord(-1,enregistrement);

    model->submitAll();

    enregistrement = model->record(model->rowCount()-1);

    delete model;

    paramsTest->m_id_TestMetro = enregistrement.value(TEST_METRO_ID).toUInt();

    enregistrerAnalyseurTest(paramsTest);

    enregistrerConcTestMetro(paramsTest);

    afficherParamsTest(paramsTest);
}


void et_InfosTestEnCours::enregistrerAnalyseurTest(QPointer<et_ParamsTest> paramsTest)
{
    QMapIterator<ushort,QString> it_id_Analyseur (paramsTest->m_listeNumSerieAnalyseurs);

    while(it_id_Analyseur.hasNext()){
        it_id_Analyseur.next();
        ushort idAnalyseur = it_id_Analyseur.key();
        QPointer<QSqlTableModel> model = m_bdHandler->getListeAnalyseurTestModel();
        QSqlRecord enregistrement_ana = model->record();
        enregistrement_ana.setValue(LISTE_ANA_TEST_ID_TEST,QVariant::fromValue(paramsTest->m_id_TestMetro));
        enregistrement_ana.setValue(LISTE_ANA_ID_EQUIPEMENT,QVariant::fromValue(idAnalyseur));
        model->insertRecord(-1,enregistrement_ana);
        model->submitAll();
        delete model;
        }
}

void et_InfosTestEnCours::enregistrerConcTestMetro(QPointer<et_ParamsTest> paramsTest)
{

    QPointer<QSqlTableModel> model = m_bdHandler->getConcTestMetro();
    QSqlRecord enregistrement_conc = model->record();
    enregistrement_conc.setValue(CONC_TEST_METRO_ID_TEST,QVariant::fromValue(paramsTest->m_id_TestMetro));

    for(int i=1;i<=paramsTest->m_test->getNbPhases();i++) {
        QSqlRecord* record = m_bdHandler->getConcentrationRow(paramsTest->m_test->getPhase(i).getIdConcentration());
        QString concentration = record->value(CONCENTRATION_REELLE).toString();
        QString id_molecule = record->value(CONCENTRATION_ID_MOLECULE).toString();
        int nbreAcquisition = paramsTest->m_test->getPhase(i).getCritereArret_NbCyclesMesures();

        enregistrement_conc.setValue(CONC_TEST_METRO_NO_PHASE,QVariant::fromValue(i));
        enregistrement_conc.setValue(CONC_TEST_METRO_ID_MOLECULE,QVariant::fromValue(id_molecule));
        enregistrement_conc.setValue(CONC_TEST_METRO_CONCENTRATION,QVariant::fromValue(concentration));
        enregistrement_conc.setValue(CONC_TEST_METRO_NBRE_ACQUISITION,QVariant::fromValue(nbreAcquisition));

        model->insertRecord(-1,enregistrement_conc);
        model->submitAll();
        delete record;

        QMap<ushort, QPointer<QSqlTableModel> > mapConcentrationsAssociee;
        ushort nbMaxConcentrationAssociees=0;

        QPointer<QSqlTableModel> model1 = m_bdHandler->getConcentrationAssocieeModelRow(paramsTest->m_test->getPhase(i).getIdConcentration());
        if(model1->rowCount()>nbMaxConcentrationAssociees)
            nbMaxConcentrationAssociees = model1->rowCount();
        mapConcentrationsAssociee.insert(i,model1);

        for(int j=0;j<nbMaxConcentrationAssociees;j++) {
            QPointer<QSqlTableModel> model1 = mapConcentrationsAssociee.value(i);

            QSqlRecord record = model1->record(j);
            if(!record.isEmpty()) {
                enregistrement_conc.setValue(CONC_TEST_METRO_ID_TEST,QVariant::fromValue(paramsTest->m_id_TestMetro));
                enregistrement_conc.setValue(CONC_TEST_METRO_NO_PHASE,QVariant::fromValue(i));
                enregistrement_conc.setValue(CONC_TEST_METRO_CONCENTRATION,QVariant::fromValue(record.value(CONC_ASSOCIEE_CONCENTRATION).toString()));
                enregistrement_conc.setValue(CONC_TEST_METRO_ID_MOLECULE,QVariant::fromValue(record.value(CONC_ASSOCIEE_FORMULE).toString()));
                enregistrement_conc.setValue(CONC_TEST_METRO_NBRE_ACQUISITION,QVariant::fromValue(nbreAcquisition));

                qDebug()<<enregistrement_conc;
                model->insertRecord(-1,enregistrement_conc);
                model->submitAll();


            }      
        }

    }
    delete model;
  }


void et_InfosTestEnCours::comboBoxNiveauInfosIndexChanged(const int index)
{
    m_niveauInfos = index;
}
