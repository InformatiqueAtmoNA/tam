/*////////////////////////////////////////////////////////////
// \file et_generateurrapporttest.cpp
// \brief Classe d'interface gerant la generation des rapports des tests
// \author PIET Regis
// \version 1.0
// \date 10/08/2011
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

#include "et_generateurrapporttest.h"
#include "ui_et_generateurrapporttest.h"
#include <math.h>
#include <stdlib.h>

et_GenerateurRapportTest::et_GenerateurRapportTest(QPointer<BdHandler> bdHandler,
                                             const ushort idTest,
                                             const ushort idAnalyseur,
                                             const ushort typeTest,
                                             QWidget *parent):
    QWidget(parent),
    ui(new Ui::et_GenerateurRapportTest)
{
    ui->setupUi(this);

    m_idTest = idTest;
    m_idAnalyseur = idAnalyseur;
    m_typeTest = typeTest;


    /*if(bdHandler.isNull()) {
        QString driver = getParam("BD_Driver").toString();
        QString host = getParam("Host").toString();
        QString userName = getParam("UserName").toString();
        QString password = getParam("Password").toString();
        QString dbName = getParam("DB_Name").toString();

        m_bdHandler = new BdHandler(driver,host,userName,password,dbName);
    }
    else*/
        m_bdHandler = bdHandler;

    QSqlRecord* informationTest =  m_bdHandler->getInformationsTest(m_idTest);

    m_NomTest = informationTest->value("test_metro_type_test").toString();
    ui->label_TypeTest->setText(m_NomTest);
    m_Operateur = informationTest->value("Nom").toString() + " " + informationTest->value("Prenom").toString();
    ui->labelOperateur->setText(m_Operateur);
    m_DateDeb = informationTest->value("date_debut").toDateTime();
    ui->dateTimeEdit_datedebut->setDateTime(m_DateDeb);
    m_DateFin = informationTest->value("date_fin").toDateTime();
    ui->dateTimeEdit_datefin->setDateTime(m_DateFin);
    m_Lieu = informationTest->value("designation").toString();
    ui->labelLieu->setText(m_Lieu);
    m_Temperature = informationTest->value("temperature").toString();
    ui->labelTemp->setText(m_Temperature);
    m_Pression = informationTest->value("pression").toString();
    ui->labelPression->setText(m_Pression);
    m_tabCritere.append (informationTest->value("critere1").toDouble());
    m_tabCritere.append(informationTest->value("critere2").toDouble());

    affichageEquipement(m_idAnalyseur,"ANALYSEUR");

    QSqlRecord* systemEtalon = m_bdHandler->getSystemeEtalonRow(informationTest->value("id_systeme_etalon").toInt());

    affichageEquipement(systemEtalon->value("id_etalon").toInt(),"ETALON");
    affichageEquipement(systemEtalon->value("id_bouteille").toInt(),"BOUTEILLE");
    affichageEquipement(systemEtalon->value("id_gzero").toInt(),"GENERATEUR AIR ZERO");

    this->ui->tableWidget_Equip->setVerticalHeaderLabels(m_listeEnteteLigne);
    this->ui->tableWidget_Equip->resizeColumnsToContents();
    this->ui->tableWidget_Equip->resizeRowsToContents();

    m_tpsAcquisition = informationTest->value("tps_acquisition").toInt();

    genererRapport();

    connect(this->ui->button_Fermer,SIGNAL(clicked()),this,SLOT(buttonFermerClicked()));

    this->ui->Button_Exporter->setEnabled(false);
    this->ui->Button_Valider->setEnabled(false);

}

et_GenerateurRapportTest::~et_GenerateurRapportTest()
{
    delete ui;
}

void et_GenerateurRapportTest::buttonFermerClicked()
{
    QMessageBox msgBox;
    msgBox.setText(QLatin1String("Fermer ?"));
    msgBox.setInformativeText(QLatin1String("Voulez-vous fermer et revenir à l'accueil ?"));
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    if(msgBox.exec()==QMessageBox::Ok)
        emit(this->fermeture());
}

//Mise en forme du tableau de mesure separe par phase identique par polluant
void et_GenerateurRapportTest::tableauMesure2(int idMolecule, int codeMolecule)
{

    m_ConcTestAnalyseur = m_bdHandler->getTestPhaseConcentration(m_idTest,idMolecule);
    m_CyclePhaseTest = m_bdHandler->getCyclePhaseTest(m_idTest);
    m_tabMesures.clear();
    m_tabConcentration.clear();
    m_tabMoyenne.clear();
    m_tabEcartType.clear();


    for (int k = 0; k < m_CyclePhaseTest->rowCount();k++){
        for (int i = 0; i < m_ConcTestAnalyseur->rowCount();i++){
            qDebug() << m_ConcTestAnalyseur->data(m_ConcTestAnalyseur->index(i,2)).toInt();
            m_tabNbreAcquisition.append(m_ConcTestAnalyseur->data(m_ConcTestAnalyseur->index(i,2)).toInt());
            m_MesureTestAnalyseur = m_bdHandler->getMesureTestAnalyseur(
                        m_idTest,m_idAnalyseur,codeMolecule,m_ConcTestAnalyseur->data(m_ConcTestAnalyseur->index(i,0)).toInt(),k+1);
            if (!m_MesureTestAnalyseur->rowCount()==0){
                m_tabMesures.append(QVector<float>());
                int l =  m_tabMesures.count();
                for (int j = 0; j < m_MesureTestAnalyseur->rowCount();j++ ) {
                    float mesure = m_MesureTestAnalyseur->data(m_MesureTestAnalyseur->index(j,3)).toFloat();
                    m_tabMesures[l-1].append(mesure);
                    }
                float moyenneTab = moyenne(m_tabMesures[l-1],m_tabMesures[l-1].count());
                float ecarttypeTab = ecarttype(m_tabMesures[l-1],m_tabMesures[l-1].count());
                m_tabMoyenne.append(moyenneTab);
                m_tabEcartType.append(ecarttypeTab);
                m_tabConcentration.append(m_ConcTestAnalyseur->data(m_ConcTestAnalyseur->index(i,1)).toFloat());
            }
         }
    }
}

//Mise en forme du tableau de mesure groupe par phase identique par polluant
void et_GenerateurRapportTest::tableauMesure(int idMolecule, int codeMolecule)
{

    m_ConcTestAnalyseur = m_bdHandler->getTestPhaseConcentration(m_idTest,idMolecule);
    //m_CyclePhaseTest = m_bdHandler->getCyclePhaseTest(m_idTest);
    m_tabMesures.clear();
    m_tabConcentration.clear();
    m_tabMoyenne.clear();
    m_tabEcartType.clear();



    for (int i = 0; i < m_ConcTestAnalyseur->rowCount();i++){
        m_MesureTestAnalyseur = m_bdHandler->getMesureTestAnalyseur(
                    m_idTest,m_idAnalyseur,codeMolecule,m_ConcTestAnalyseur->data(m_ConcTestAnalyseur->index(i,0)).toInt(),0);
        if (!m_MesureTestAnalyseur->rowCount()==0){
            m_tabMesures.append(QVector<float>());
            int l =  m_tabMesures.count();
            for (int j = 0; j < m_MesureTestAnalyseur->rowCount();j++ ) {
                float mesure = m_MesureTestAnalyseur->data(m_MesureTestAnalyseur->index(j,3)).toFloat();
                m_tabMesures[l-1].append(mesure);
                }
            float moyenneTab = moyenne(m_tabMesures[l-1],m_tabMesures[l-1].count());
            float ecarttypeTab = ecarttype(m_tabMesures[l-1],m_tabMesures[l-1].count());
            m_tabMoyenne.append(moyenneTab);
            m_tabEcartType.append(ecarttypeTab);
            m_tabConcentration.append(m_ConcTestAnalyseur->data(m_ConcTestAnalyseur->index(i,1)).toFloat());
        }
     }

}

//Mise en forme du tableau de mesure par phase pour TPG
void et_GenerateurRapportTest::tableauMesure(int no_Phase)
{

    m_tabMesures.clear();
    m_tabMoyenne.clear();
    m_tabCodeMolecule.clear();

    m_tabCodeMolecule.append(2);
    m_tabCodeMolecule.append(12);
    m_tabCodeMolecule.append(3);

    for (int i = 0; i < 3;i++){
        m_tabMesures.append(QVector<float>());
        m_MesureTestAnalyseur = m_bdHandler->getMesureTestAnalyseur(
                    m_idTest,m_idAnalyseur,m_tabCodeMolecule[i],no_Phase,1);
        for (int j = 0; j < m_MesureTestAnalyseur->rowCount();j++ ) {
            float mesure = m_MesureTestAnalyseur->data(m_MesureTestAnalyseur->index(j,3)).toFloat();
            m_tabMesures[i].append(mesure);
            }
        float moyenneTab = moyenne(m_tabMesures[i],m_tabMesures[i].count());
        m_tabMoyenne.append(moyenneTab);
    }

}

//Mise en forme des Tableaux de residus pour la linearite
void et_GenerateurRapportTest::affichageTableauResidu ()
{
    m_tabResidu.clear();
    m_tabResiduRel.clear();
    m_tabResiduInc.clear();
    m_tabValeurPourCritere.clear();
    m_pente = pente(m_tabConcentration,m_tabMoyenne,m_tabConcentration.count());
    m_ordonnee = ordonnee(m_tabConcentration,m_tabMoyenne,m_tabConcentration.count());
    for (int i = 0 ; i < m_tabConcentration.count() ;i ++){
        if (!m_tabConcentration[i] == 0){
            m_tabResidu.append(m_tabMoyenne[i]-(m_ordonnee + m_pente * m_tabConcentration[i]));
            m_tabResiduRel.append(m_tabResidu[i]/m_tabConcentration[i]*100);
            m_tabResiduInc.append((m_tabMoyenne[i]-m_tabConcentration[i])/m_tabConcentration[i]*100);
        }
        else {
            m_ResiduZero = m_tabMoyenne[i]-(m_ordonnee + m_pente * m_tabConcentration[i]);
            m_tabResidu.append(m_ResiduZero);
            m_tabResiduRel.append(0);
            m_tabResiduInc.append(0);
        }
    }
    m_tabValeurPourCritere.append(m_ResiduZero);
    m_tabValeurPourCritere.append(rmax(m_tabConcentration,m_tabResiduRel));
    m_tabValeurPourCritere.append(m_pente);
    m_tabValeurPourCritere.append(m_ordonnee);
    m_tabValeurPourCritere.append(rmax(m_tabResiduRel,m_tabResiduInc));
}

//Mise en forme du Tableau des temps de reponse
void et_GenerateurRapportTest::affichageTableauTpsReponse ()
{
    //int tpsAcquisition = 5;

    m_tabTpsReponse.clear();
    m_tabMoyenneTpsReponse.clear();
    m_tabValeurPourCritere.clear();

    m_tabTpsReponse.append(QVector<float>());
    m_tabTpsReponse.append(QVector<float>());
    for (int i=0;i<m_tabMesures.size()-1;i++){
        if (i%2 == 0){
            int nbreAcquisition = m_tabNbreAcquisition[i];
            QVector <float > tabMesureStable = m_tabMesures[i];
            tabMesureStable.remove(0,tabMesureStable.size()-nbreAcquisition);
            float moyenneMesureStable = moyenne(tabMesureStable,tabMesureStable.size());
            m_tabTpsReponse[0].append(calculTpsReponseMontee(m_tabMesures[i],0.9*moyenneMesureStable,m_tpsAcquisition));
            m_tabTpsReponse[1].append(calculTpsReponseDescente(m_tabMesures[i+1],0.1*moyenneMesureStable,m_tpsAcquisition));
         }
    }
    m_tabMoyenneTpsReponse.append(moyenne(m_tabTpsReponse[0],m_tabTpsReponse[0].size()));
    m_tabMoyenneTpsReponse.append(moyenne(m_tabTpsReponse[1],m_tabTpsReponse[1].size()));
    m_tabValeurPourCritere.append(rmax(m_tabMoyenneTpsReponse,m_tabMoyenneTpsReponse));
    m_tabValeurPourCritere.append(val_abs(m_tabMoyenneTpsReponse[0]-m_tabMoyenneTpsReponse[1]));

    qDebug()<<m_tabMoyenneTpsReponse;
}

//Mise en forme du Tableau des Equipements
void et_GenerateurRapportTest::affichageEquipement(ushort idEquipement,QString nomLigne)
{
    QSqlRecord recordEquipement = *(m_bdHandler->getEquipementModeledRow(idEquipement));

    QTableWidgetItem* item_noSerie = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_NUM_SERIE).toString());
    QTableWidgetItem* item_modele = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_MODELE).toString());
    QTableWidgetItem* item_marque = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_MARQUE).toString());


    uint idxNewRecord = this->ui->tableWidget_Equip->rowCount();
    this->ui->tableWidget_Equip->insertRow(idxNewRecord);
    this->ui->tableWidget_Equip->setItem(idxNewRecord,0,item_noSerie);
    this->ui->tableWidget_Equip->setItem(idxNewRecord,1,item_modele);
    this->ui->tableWidget_Equip->setItem(idxNewRecord,2,item_marque);

    m_listeEnteteLigne.append(QString("%1").arg(nomLigne));

}

bool et_GenerateurRapportTest::genererRapport()
{
    switch(this->   m_typeTest) {
    case REPETABILITE:
        return genererRapportRepetabilite();
        break;
    /*case REPETABILITE_2:
        return genererRapportRepetabilite_2();
        break;*/
    case LINEARITE:
        return genererRapportLinearite();
        break;
    case TEMPS_REPONSE:
        return genererRapportTempsReponse();
        break;
    case RENDEMENT_FOUR:
        return genererRapportRendementFour();
        break;
    default:
        return true;
    }
}

bool et_GenerateurRapportTest::genererRapportRepetabilite()
{
    m_PolluantTest = m_bdHandler->getPolluantTestConcentration(m_idTest);
    for (int i=0;i<m_PolluantTest->rowCount();i++){
        tableauMesure(m_PolluantTest->data(m_PolluantTest->index(i,0)).toInt(),
                      m_PolluantTest->data(m_PolluantTest->index(i,1)).toInt());
        QWidget* resultatPolluant = new et_Resultatpolluant(m_tabConcentration,m_tabMesures,
                                                            m_tabMoyenne,m_tabEcartType,m_tabCritere,this);
        ui->tabWidget->addTab(resultatPolluant,m_PolluantTest->data(m_PolluantTest->index(i,2)).toString());
    }
    this->ui->tableWidget_Rdf->hide();
    this->ui->label->hide();
    return true;
}


bool et_GenerateurRapportTest::genererRapportLinearite()
{

    m_PolluantTest = m_bdHandler->getPolluantTestConcentration(m_idTest);
    for (int i=0;i<m_PolluantTest->rowCount();i++){
        tableauMesure(m_PolluantTest->data(m_PolluantTest->index(i,0)).toInt(),
                      m_PolluantTest->data(m_PolluantTest->index(i,1)).toInt());
        affichageTableauResidu();

        QWidget* resultatPolluant = new et_Resultatpolluant(m_tabConcentration,m_tabMesures,m_tabMoyenne,
                                                            m_tabEcartType,m_tabValeurPourCritere,
                                                            m_tabResidu,m_tabResiduRel,m_tabResiduInc,m_tabCritere,this);
        ui->tabWidget->addTab(resultatPolluant,m_PolluantTest->data(m_PolluantTest->index(i,2)).toString());
    }
    this->ui->tableWidget_Rdf->hide();
    this->ui->label->hide();
    return true;
}

bool et_GenerateurRapportTest::genererRapportTempsReponse()
{
    m_PolluantTest = m_bdHandler->getPolluantTestConcentration(m_idTest);
    for (int i=0;i<m_PolluantTest->rowCount();i++){
        tableauMesure2(m_PolluantTest->data(m_PolluantTest->index(i,0)).toInt(),
                      m_PolluantTest->data(m_PolluantTest->index(i,1)).toInt());
        affichageTableauTpsReponse();
        QList<QString> listEnteteColonne;
        listEnteteColonne << "TpsMontee" << "TpsDescente";
        QWidget* resultatPolluant = new et_Resultatpolluant(listEnteteColonne,m_tabTpsReponse,
                                                            m_tabMoyenneTpsReponse,m_tabValeurPourCritere,m_tabCritere,this);
        ui->tabWidget->addTab(resultatPolluant,m_PolluantTest->data(m_PolluantTest->index(i,2)).toString());
    }

    this->ui->tableWidget_Rdf->hide();
    this->ui->label->hide();
    return true;
}

bool et_GenerateurRapportTest::genererRapportRendementFour()
{
    m_CyclePhaseTest = m_bdHandler->getCyclePhaseTest(m_idTest);
    m_TestPhase = m_bdHandler->getTestPhase(m_idTest);
    for (int j=0;j<m_CyclePhaseTest->rowCount();j++){
        for (int i=0;i<m_TestPhase->rowCount();i++){
            tableauMesure(m_TestPhase->data(m_TestPhase->index(i,0)).toInt());
            QList<QString> listNomMolecule;
            listNomMolecule << "NO" << "NOX" << "NO2";
            m_tabMoyenneCalculTPG.append(m_tabMoyenne);
            QWidget* resultatPolluant = new et_Resultatpolluant(listNomMolecule,m_tabMesures,m_tabMoyenne,this);
            ui->tabWidget->addTab(resultatPolluant,"Phase N°"+ QString::number(i+1));
        }
     }
    // Calcul des rendements de four et de la diff
    for (int i=0;i<m_tabMoyenneCalculTPG.count();i++){
        if (i%2 != 0){
            float rdf = (1 - ((m_tabMoyenneCalculTPG[i-1].value(1)-m_tabMoyenneCalculTPG[i].value(1))/(m_tabMoyenneCalculTPG[i-1].value(0)-m_tabMoyenneCalculTPG[i].value(0))))*100;
            m_tabResultatTPG.append(rdf);
            QString enteteLigneTPG = "RdF (%) entre Phase " + QString::number(i) + " et " + QString::number(i+1);
            m_listeEnteteLigneTPG.append(enteteLigneTPG);
        }
    }

    m_tabResultatTPG.append(m_tabCritere.value(0));
    m_listeEnteteLigneTPG.append("Critere RdF (%) >");

    if ((m_tabMoyenneCalculTPG.count()>1) && (m_tabMoyenneCalculTPG.count()%2 == 1)){
        float diffNO = ((m_tabMoyenneCalculTPG[m_tabMoyenneCalculTPG.count()-1].value(0)-m_tabMoyenneCalculTPG[0].value(0))/m_tabMoyenneCalculTPG[0].value(0))*100;
        m_tabResultatTPG.append(diffNO);
        m_listeEnteteLigneTPG.append("Difference NO (%) :");
        float diffNOX = ((m_tabMoyenneCalculTPG[m_tabMoyenneCalculTPG.count()-1].value(1)-m_tabMoyenneCalculTPG[0].value(1))/m_tabMoyenneCalculTPG[0].value(1))*100;
        m_tabResultatTPG.append(diffNOX);
        m_listeEnteteLigneTPG.append("Difference NOX (%) :");

        m_tabResultatTPG.append(m_tabCritere.value(1));
        m_listeEnteteLigneTPG.append("Critere Diff NO/NOX (%) <");
    }

    //Remplissage tableWidgetTPG
    for (int j=0;j<m_tabResultatTPG.count();j++){

        this->ui->tableWidget_Rdf->insertRow(j);
        QTableWidgetItem* item = new QTableWidgetItem(QString::number(m_tabResultatTPG.value(j),'f',2));

        if(j < 2){
            if(m_tabResultatTPG.value(j)>m_tabCritere.value(0))
            {
                item->setForeground(QBrush(QColor("green")));
            }
            else
            {
                item->setForeground(QBrush(QColor("red")));
            }
        }
        else if(j>2 and j<5)
        {
            if(m_tabResultatTPG.value(j)<m_tabCritere.value(1))
            {
                item->setForeground(QBrush(QColor("green")));
            }
            else
            {
                item->setForeground(QBrush(QColor("red")));
            }
        }
        this->ui->tableWidget_Rdf->setItem(j,0,item);
    }
    this->ui->tableWidget_Rdf->setVerticalHeaderLabels(m_listeEnteteLigneTPG);
    return true;
}



int et_GenerateurRapportTest::test_zero(QVector<float> &tab)
{
    for(int i=0;i<tab.count();i++)
    {
            if(tab[i]==0)
            { return i;}
    }

    return -1;
}

int et_GenerateurRapportTest::test_negatif(QVector<float> &tab)
{
    for(int i=0;i<tab.count();i++)
    {
            if(tab[i]<0)
            { return i;}
    }

    return -1;
}

float et_GenerateurRapportTest::val_abs(float x)
{
    if(x<0) {return -x;}
    else {return x;}
}

float et_GenerateurRapportTest::rmax(QVector<float> &Xi,QVector<float> &Yi)
{
    float temp=0;

    for(int i=0;i<Yi.count();i++)
    {
        if((val_abs(Yi[i])>val_abs(temp))&&(!Xi[i]==0))
            {
                 temp=Yi[i];
            }
    }

    return temp;
}

int et_GenerateurRapportTest::somme(QVector<int> &tab)
{
    int somme=0;

    for (int i=0;i<tab.count();i++)
    {
     somme=((tab[i])+(somme));
    }

    return(somme);
}

float et_GenerateurRapportTest::somme(QVector<float> &tab)
{
    float somme=0;

    for (int i=0;i<tab.count();i++)
    {
     somme=((tab[i])+(somme));
    }

    return(somme);
}

float et_GenerateurRapportTest::moyenne(QVector<int> &tab,int n)
{
    float moyenne = float(somme(tab))/n;

    return (moyenne);
}

float et_GenerateurRapportTest::moyenne(QVector<float> &tab,int n)
{
    float moyenne = somme(tab)/n;

    return (moyenne);
}

float et_GenerateurRapportTest::moyenne2(float somme,int n)
{
    float moyenne = somme/n;

    return (moyenne);
}

void et_GenerateurRapportTest::produittab(QVector<float> &tab1,QVector<float> &tab2,int n)
{
    m_tabTemp.resize(n);

    for (int i=0;i<n;i++)
    {
        m_tabTemp[i]=(tab1[i]*tab2[i]);
    }
}

void et_GenerateurRapportTest::lntab(QVector<float> &tab,QVector<float> &tabTemp,int n)
{
    tabTemp.resize(n);

    for (int i=0;i<n;i++)
    {
            tabTemp[i]=(log(tab[i]));
    }
}

void et_GenerateurRapportTest::logtab(QVector<float> &tab,QVector<float> &tabTemp,int n)
{
    tabTemp.resize(n);

    for (int i=0;i<n;i++)
    {
            tabTemp[i]=(log10(tab[i]));
    }
}

void et_GenerateurRapportTest::invtab(QVector<float> &tab,QVector<float> &tabTemp,int n)
{
    tabTemp.resize(n);

    for (int i=0;i<n;i++)
    {
            tabTemp[i]=(1/tab[i]);
    }
}

void et_GenerateurRapportTest::ecart_a_moyenne(QVector<float> &tab,float Moyenne,int n)
{
    m_tabTemp.resize(n);

    for (int i=0;i<n;i++)
    {
            m_tabTemp[i]=(tab[i] - Moyenne);
    }
}

float et_GenerateurRapportTest::covariance(QVector<float> &Xi,QVector<float> &Yi,int n)
{
    float cov;

    produittab(Xi,Yi,n);
    //cov = moyenne(m_tabTemp,n) - ( moyenne(Xi,n) * moyenne(Yi,n) );
    cov = moyenne(m_tabTemp,n) - ( moyenne(Xi,n) * moyenne(Yi,n) );
    return (cov);
}

float et_GenerateurRapportTest::variance(QVector<float> &val,int n)
{
    float sce;

    ecart_a_moyenne(val,moyenne(val,n),n);
    produittab(m_tabTemp,m_tabTemp,n);
    sce = somme(m_tabTemp)/(n-1);

    return (sce);
}

float et_GenerateurRapportTest::ecarttype(QVector<float> &val,int n)
{
    float ect= sqrt(variance(val,n));

    return (ect);
}

float et_GenerateurRapportTest::pente(QVector<float> &Xi,QVector<float> &Yi,int n)
{
    float Xz = moyenne(Xi,n);
    ecart_a_moyenne(Xi,Xz,n);
    produittab(Yi,m_tabTemp,n);
    float a1 = somme (m_tabTemp);
    ecart_a_moyenne(Xi,Xz,n);
    produittab(m_tabTemp,m_tabTemp,n);
    float a2 = somme (m_tabTemp);
    float a = a1/a2;

    return (a);
}

float et_GenerateurRapportTest::ordonnee(QVector<float> &Xi,QVector<float> &Yi,int n)
{
    float b = moyenne(Yi,n) - ( pente(Xi,Yi,n) * moyenne(Xi,n) );

    return (b);
}

float et_GenerateurRapportTest::corr(QVector<float> &Xi,QVector<float> &Yi,int n)
{
    float r = covariance(Xi,Yi,n)/(ecarttype(Xi,n)*ecarttype(Yi,n));
    //float r=pente(Xi,Yi,n)*pente(Xi,Yi,n)*(variance(Xi,n)/variance(Yi,n));
    return (r);
}

float et_GenerateurRapportTest::calculTpsReponseMontee(QVector<float> &Xi,float moyenne,int tpsAcq){

    for (int i=0;i<Xi.size()-1;i++){
        if (Xi.value(i)<=moyenne && Xi.value(i+1)>=moyenne){
            float tpsMontee = tpsAcq*(((moyenne - Xi.value(i))/(Xi.value(i+1) - Xi.value(i)))+(i+1));
            return tpsMontee;
        }
    }
    return -99;

}

float et_GenerateurRapportTest::calculTpsReponseDescente(QVector<float> &Xi,float moyenne,int tpsAcq){

    for (int i=0;i<Xi.size()-1;i++){
        if (Xi.value(i)>=moyenne && Xi.value(i+1)<=moyenne){
            float tpsMontee = tpsAcq*(((moyenne - Xi.value(i))/(Xi.value(i+1) - Xi.value(i)))+(i+1));
            return tpsMontee;
        }
    }
    return -99;

}
