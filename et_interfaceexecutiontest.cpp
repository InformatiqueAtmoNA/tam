/*////////////////////////////////////////////////////////////
// \file et_interfaceexecutiontest.cpp
// \brief Classe d'interface gerant l'execution des tests
// \author FOUQUART Christophe
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

#include "et_interfaceexecutiontest.h"
#include "ui_et_interfaceexecutiontest.h"


QString decoupeNomFichier(QString nomFichier){

    if(nomFichier.indexOf(".xml")>0)
        nomFichier.remove(".xml");
    if(nomFichier.indexOf(".\\")>0)
        nomFichier.remove(".\\");
    if(nomFichier.contains("./"))
        nomFichier.remove("./");
    return nomFichier;
}

et_InterfaceExecutionTest::et_InterfaceExecutionTest(QPointer<BdHandler> bdHandler,QAuthenticator aUser,QList<ushort> idTestXML, QList<QString> fichierDescription,bool miseEnAttente,ushort idOperateur, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::et_InterfaceExecutionTest)
{
    ui->setupUi(this);

    m_user = aUser;
    m_idSonde =0;
    m_bdHandler = bdHandler;
    m_etape = 1;
    m_idOperateur = idOperateur;
    m_miseEnAttente = miseEnAttente;
    m_nomCheminCSV = getParam("Path_CSV").toString();
    QButtonGroup *buttonGroup = new QButtonGroup;
    for(int i=0 ; i<idTestXML.count() ; i++ ){
        m_idTestXML.append(idTestXML[i]);
        m_test.append(Test::importFromXml(fichierDescription[i]));
        m_fichierDescription.append(fichierDescription[i]);

        if(idTestXML.count()>1){

            QString nomFichier = decoupeNomFichier(m_fichierDescription[i].mid(m_fichierDescription[i].lastIndexOf('/')+1));

            QVBoxLayout *descritpionTest = new QVBoxLayout;
            QHBoxLayout *descritpionTestLine1 = new QHBoxLayout;
            QHBoxLayout *descritpionTestLine2 = new QHBoxLayout;
            QHBoxLayout *descritpionTestLine3 = new QHBoxLayout;

            QLabel *numTest = new QLabel(QString("Test n°" + QString::number(i+1) + " :"));
            QLabel *label1 = new QLabel("Fichier de description : ");
            label1->setMaximumSize(150,20);
            QLabel *label2 = new QLabel("Type de Test : ");
            label2->setMaximumSize(150,20);
            QLabel *label3 = new QLabel("Fichier CSV : ");
            label3->setMaximumSize(150,20);

            QLabel *FichierDescription =  new QLabel(fichierDescription[i]);
            QLabel *TypeTest =  new QLabel(typeTestToString(m_test[i]->getTypeTest()));
            QLineEdit *fichierCSV =  new QLineEdit((m_nomCheminCSV+"/DATE_HEURE_"+nomFichier+".csv"));
            fichierCSV->setMaximumSize(550,20);
            listeLineEditCSV.append(fichierCSV);

            QToolButton *boutonCSV = new QToolButton;
            boutonCSV->setText("...");
            buttonGroup->addButton(boutonCSV,i);


            descritpionTestLine1->addWidget(label1);
            descritpionTestLine1->addWidget(FichierDescription);

            descritpionTestLine2->addWidget(label2);
            descritpionTestLine2->addWidget(TypeTest);

            descritpionTestLine3->insertWidget(0,label3);
            descritpionTestLine3->insertWidget(1,fichierCSV);
            descritpionTestLine3->insertWidget(2,boutonCSV);

            descritpionTest->addWidget(numTest);
            descritpionTest->addLayout(descritpionTestLine1);
            descritpionTest->addLayout(descritpionTestLine2);
            descritpionTest->addLayout(descritpionTestLine3);

            this->ui->affichageInfoTests->addLayout(descritpionTest);
        }
    }

    if(this->m_idTestXML.count()>1){
        this->ui->lineEdit_FichierDescription->hide();
        this->ui->label->hide();
        this->ui->lineEdit_FichierCSV->hide();
        this->ui->label_CSV->hide();
        this->ui->button_fichierCSV->hide();
        this->ui->lineEdit_TypeTest->hide();
        this->ui->label_2->hide();
    }

    connect(this->ui->tableWidget_Analyseurs,SIGNAL(clicked(QModelIndex)),this,SLOT(tableWidgetAnalyseursClicked(QModelIndex)));
    connect(this->ui->ajouterSonde,SIGNAL(clicked()),this,SLOT(buttonAjouterSondeClicked()));
    connect(this->ui->supprimerSonde,SIGNAL(clicked()),this,SLOT(buttonSupprimerSondeClicked()));
    connect(this->ui->tableWidget_Communication,SIGNAL(clicked(QModelIndex)),this,SLOT(tableWidgetCommunicationClicked(QModelIndex)));
    connect(this->ui->button_Annuler,SIGNAL(clicked()),this,SLOT(buttonAnnulerClicked()));
    connect(this->ui->button_Ajouter,SIGNAL(clicked()),this,SLOT(buttonAjouterClicked()));
    connect(this->ui->button_Suivant,SIGNAL(clicked()),this,SLOT(buttonSuivantClicked()));
    connect(this->ui->button_Precedent,SIGNAL(clicked()),this,SLOT(buttonPrecedentClicked()));
    connect(this->ui->button_Supprimer,SIGNAL(clicked()),this,SLOT(buttonSupprimerClicked()));
    connect(this->ui->button_TestAnalyseur,SIGNAL(clicked()),this,SLOT(buttonTestAnalyseurClicked()));
    connect(this->ui->button_TestSonde,SIGNAL(clicked()),this,SLOT(buttonTestSondeClicked()));
    connect(this->ui->button_TestCalibrateur,SIGNAL(clicked()),this,SLOT(buttonTestCalibrateurClicked()));
    connect(this->ui->lineEdit_InterfaceAnalyseur,SIGNAL(textChanged(QString)),this,SLOT(lineEditInterfaceAnalyseurTextChanged(QString)));
    connect(this->ui->lineEdit_InterfaceSonde,SIGNAL(textChanged(QString)),this,SLOT(lineEditInterfaceSondeTextChanged(QString)));
    connect(this->ui->lineEdit_InterfaceAnalyseur,SIGNAL(returnPressed()),this,SLOT(lineEditInterfaceAnalyseurEnterPressed()));
    connect(this->ui->lineEdit_InterfaceSonde,SIGNAL(returnPressed()),this,SLOT(lineEditInterfaceSondeEnterPressed()));
    connect(this->ui->lineEdit_CanalCalibrateur,SIGNAL(textChanged(QString)),this,SLOT(lineEditCanalCalibrateurTextChanged(QString)));
    connect(this->ui->lineEdit_CanalCalibrateur,SIGNAL(returnPressed()),this,SLOT(lineEditParamsCalibrateurEnterPressed()));
    connect(this->ui->lineEdit_InterfaceCalibrateur,SIGNAL(textChanged(QString)),this,SLOT(lineEditInterfaceCalibrateurTextChanged(QString)));
    connect(this->ui->lineEdit_InterfaceCalibrateur,SIGNAL(returnPressed()),this,SLOT(lineEditParamsCalibrateurEnterPressed()));
    connect(this->ui->tabWidget_ExecutionTest,SIGNAL(currentChanged(int)),this,SLOT(tabWidgetExecutionTestIndexChanged(int)));
    connect(this->ui->button_Executer,SIGNAL(clicked()),this,SLOT(buttonExecuterClicked()));
    connect(this->ui->button_MettreEnAttente,SIGNAL(clicked()),this,SLOT(buttonMettreEnAttenteClicked()));
    connect(this->ui->bg_ChoixDebutTest,SIGNAL(idClicked(int)),this,SLOT(bgChoixDebutTestValueChanged(int)));
    connect(this->ui->button_fichierCSV,SIGNAL(clicked()),this,SLOT(button_choixEnregistrementCSV()));
    connect(buttonGroup,SIGNAL(idClicked(int)),this,SLOT(button_choixMultipleEnregistrementCSV(int)));

    for(const QPointer<Test> &test : m_test){
        if(test.isNull()) {
            QMessageBox msgBox;
            msgBox.setText(QLatin1String("Un problème est survenu lors du chargement du fichier de configuration du test" ));
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            msgBox.exec();
            return;
        }
    }

    this->ui->button_Precedent->setEnabled(false);

     this->ui->groupBox_5->hide();
    QString nomFichier = decoupeNomFichier(m_fichierDescription[0].mid(m_fichierDescription[0].lastIndexOf('/')+1));

    this->ui->lineEdit_FichierCSV->setText(m_nomCheminCSV+"/DATE_HEURE_"+nomFichier+".csv");
    this->ui->lineEdit_FichierDescription->setText(nomFichier);
    this->ui->lineEdit_TypeTest->setText(typeTestToString(m_test[0]->getTypeTest()));

    if(!m_miseEnAttente) {
        ui->button_MettreEnAttente->hide();
        m_infosTestEnCours = new et_InfosTestEnCours(m_bdHandler,this);
        this->ui->h_Layout_InfosTest->addWidget(m_infosTestEnCours.data());
    }

    m_modelOperateur = m_bdHandler->getOperateurModel();
    ui->comboBox_Operateur->setModel(m_modelOperateur);
    ui->comboBox_Operateur->setModelColumn(OPERATEUR_USERNAME);
    ui->comboBox_Operateur->setCurrentIndex(ui->comboBox_Operateur->findText(m_user.user()));

    m_debutImmediat=false;
    m_sondePresente = false;

    m_modelLieu = m_bdHandler->getLieuModel();
    ui->comboBox_Lieu->setModel(m_modelLieu);
    ui->comboBox_Lieu->setModelColumn(LIEU_DESIGNATION);



    ui->tabWidget_ExecutionTest->setCurrentIndex(0);
    ui->button_Executer->setVisible(false);
    ui->button_MettreEnAttente->setVisible(false);

    ui->bg_ChoixDebutTest->setId(ui->radioButton_DebutImmediat,0);
    ui->bg_ChoixDebutTest->setId(ui->radioButton_DebutDateHeure,1);

    ui->dateTime_DebutTest->setDateTime(QDateTime::currentDateTime());

    QSqlRecord canalDefCalibrateur = *(m_bdHandler->getSystemeEtalonRow(m_test[0]->getIdSystemeEtalon()));
    ui->lineEdit_CanalCalibrateur->setText(canalDefCalibrateur.value("canal_defaut").toString());

    ushort idCalibrateur = m_bdHandler->getIdCalibrateur(m_test[0]->getIdSystemeEtalon());
    QSqlRecord record = *(m_bdHandler->getDesignationPortSerie(idCalibrateur));
    ui->lineEdit_InterfaceCalibrateur->setText(record.value("designation").toString());

    QSqlRecord* recordEquipement = m_bdHandler->getEquipementRow(idCalibrateur);

    if(recordEquipement->value(EQUIPEMENT_TYPE_CONNEXION).toString() == "IP"){
        ui->lineEdit_InterfaceCalibrateur->setText(recordEquipement->value(EQUIPEMENT_ADRESSE_IP).toString());
    }
    else if(recordEquipement->value(EQUIPEMENT_TYPE_CONNEXION).toString() == "RS232"){
        QSqlRecord record = *(m_bdHandler->getDesignationPortSerie(idCalibrateur));
        ui->lineEdit_InterfaceCalibrateur->setText(record.value("designation").toString());
    }
}



et_InterfaceExecutionTest::~et_InterfaceExecutionTest()
{
    delete ui;

    if(!m_appareilEnTest.isNull())
        delete m_appareilEnTest;
    if(!m_modelTestXml.isNull())
        delete m_modelTestXml;
    if(!m_modelOperateur.isNull())
        delete m_modelOperateur;
    if(!m_modelLieu.isNull())
        delete m_modelLieu;
    if(!m_test.isEmpty())
        m_test.clear();
    if(!m_testAExecuter.isNull()) {
        m_testAExecuter->standByCalibrateur(false);
        delete m_testAExecuter;
    }
}


bool et_InterfaceExecutionTest::controleEtape1()
{
    bool passerEtapeSuivante = true;

    if(this->ui->comboBox_Operateur->currentIndex()<0) {
        passerEtapeSuivante = false;
        this->ui->label_Operateur->setText("<font color=#FF0000>"+this->ui->label_Operateur->text()+"</font>" );
    }
    else
        this->ui->label_Operateur->setText("<font color=#000000>"+this->ui->label_Operateur->text()+"</font>" );

    if(this->ui->comboBox_Lieu->currentIndex()<0) {
        passerEtapeSuivante = false;
        this->ui->label_Lieu->setText("<font color=#FF0000>"+this->ui->label_Lieu->text()+"</font>" );
    }
    else
        this->ui->label_Lieu->setText("<font color=#000000>"+this->ui->label_Lieu->text()+"</font>" );

    if(this->ui->doubleSpinBox_Pression->value()==0) {
        passerEtapeSuivante = false;
        this->ui->label_Pression->setText("<font color=#FF0000>"+this->ui->label_Pression->text()+"</font>" );
    }
    else
        this->ui->label_Pression->setText("<font color=#000000>"+this->ui->label_Pression->text()+"</font>" );


    if(this->ui->doubleSpinBox_Temperature->value()==0 and this->ui->label_Sonde->text().isEmpty()) {
        passerEtapeSuivante = false;
        this->ui->label_Temperature->setText("<font color=#FF0000>"+this->ui->label_Temperature->text()+"</font>" );
    }
    else
        this->ui->label_Temperature->setText("<font color=#000000>"+this->ui->label_Temperature->text()+"</font>" );

    if(this->ui->lineEdit_FichierCSV->text().isEmpty()) {
        passerEtapeSuivante = false;
        this->ui->label_CSV->setText("<font color=#FF0000>"+this->ui->label_CSV->text()+"</font>" );
    }
    else {
        if(QFile::exists(this->ui->lineEdit_FichierCSV->text())) {
            QMessageBox msgBox;
            msgBox.setText(QLatin1String("Le fichier d'export des resultats existe dejà.\nVoulez-vous l'ecraser ?"));
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Discard);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.setIcon(QMessageBox::Critical);
            int retour = msgBox.exec();

            if(retour == QMessageBox::Discard) {
                passerEtapeSuivante = false;
                this->ui->label_CSV->setText("<font color=#FF0000>"+this->ui->label_CSV->text()+"</font>" );
            }
            else
                this->ui->label_CSV->setText("<font color=#000000>"+this->ui->label_CSV->text()+"</font>" );
        }
        else
            this->ui->label_CSV->setText("<font color=#000000>"+this->ui->label_CSV->text()+"</font>" );

    }
    return passerEtapeSuivante;
}

bool et_InterfaceExecutionTest::controleEtape2()
{
    QMapIterator<ushort,bool> iterator(m_listeEtatComAnalyseurs);
    if (!iterator.hasNext())
        return false;

    while (iterator.hasNext()) {
        iterator.next();
        if(iterator.value() == false)
            return false;
    }

    if(!this->ui->groupBox_5->isHidden() && m_etatComSonde == false){
        return false;
    }

    if(m_etatComCalibrateur == false)
        return false;

    return true;
}

void et_InterfaceExecutionTest::buttonAjouterClicked()
{
    Dlg_Equipement dlgEquipement(this,this->m_bdHandler,true,0,"ANALYSEUR");

    if(!dlgEquipement.exec()) return;

    int idEquipement = dlgEquipement.getIdSelection();

    QSqlRecord recordEquipement = *(m_bdHandler->getEquipementModeledRow(idEquipement));
    QSqlRecord* record = m_bdHandler->getEquipementRow(idEquipement);

    QTableWidgetItem* item_idEquipement = new QTableWidgetItem(QString::number(idEquipement));
    QTableWidgetItem* item_noSerie = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_NUM_SERIE).toString());
    QTableWidgetItem* item_modele = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_MODELE).toString());
    QTableWidgetItem* item_marque = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_MARQUE).toString());
    QTableWidgetItem* item_minGamme = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_MIN_GAMME).toString());
    QTableWidgetItem* item_maxGamme = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_MAX_GAMME).toString());
    QTableWidgetItem* item_offset = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_OFFSET).toString());
    QTableWidgetItem* item_adresse = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_ADRESSE).toString());
    QTableWidgetItem* item_portSerie = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_PORTSERIE).toString());
    QTableWidgetItem* item_ip = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_IP).toString());

    uint idxNewRecord=0;


    idxNewRecord = this->ui->tableWidget_Analyseurs->rowCount();
    this->ui->tableWidget_Analyseurs->insertRow(idxNewRecord);
    this->ui->tableWidget_Analyseurs->setItem(idxNewRecord,ET_TABLEW_ANALYSEURS_ID_EQUIPEMENT,item_idEquipement);
    this->ui->tableWidget_Analyseurs->setItem(idxNewRecord,ET_TABLEW_ANALYSEURS_NUM_SERIE,item_noSerie);
    this->ui->tableWidget_Analyseurs->setItem(idxNewRecord,ET_TABLEW_ANALYSEURS_MODELE,item_modele);
    this->ui->tableWidget_Analyseurs->setItem(idxNewRecord,ET_TABLEW_ANALYSEURS_MARQUE,item_marque);
    this->ui->tableWidget_Analyseurs->setItem(idxNewRecord,ET_TABLEW_ANALYSEURS_MIN_GAMME,item_minGamme);
    this->ui->tableWidget_Analyseurs->setItem(idxNewRecord,ET_TABLEW_ANALYSEURS_MAX_GAMME,item_maxGamme);
    this->ui->tableWidget_Analyseurs->setItem(idxNewRecord,ET_TABLEW_ANALYSEURS_OFFSET,item_offset);
    this->ui->tableWidget_Analyseurs->setItem(idxNewRecord,ET_TABLEW_ANALYSEURS_ADRESSE,item_adresse);
    this->ui->tableWidget_Analyseurs->setItem(idxNewRecord,ET_TABLEW_ANALYSEURS_PORTSERIE,item_portSerie);
    this->ui->tableWidget_Analyseurs->setItem(idxNewRecord,ET_TABLEW_ANALYSEURS_IP,item_ip);



    this->ui->tableWidget_Communication->insertRow(idxNewRecord);
    QTableWidgetItem* itemCom_idEquipement = new QTableWidgetItem(QString::number(idEquipement));
    QTableWidgetItem* itemCom_noSerie = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_NUM_SERIE).toString());
    QTableWidgetItem* itemCom_portSerie = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_PORTSERIE).toString());
    QTableWidgetItem* itemCom_etat = new QTableWidgetItem("En attente de test...");
    QTableWidgetItem* itemCom_ip = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_IP).toString());

    this->ui->tableWidget_Communication->setItem(idxNewRecord,ET_TABLEW_COMMUNICATION_ID_EQUIPEMENT,itemCom_idEquipement);
    this->ui->tableWidget_Communication->setItem(idxNewRecord,ET_TABLEW_COMMUNICATION_NUM_SERIE,itemCom_noSerie);

    if(record->value(EQUIPEMENT_TYPE_CONNEXION).toString()=="IP"){
        this->ui->tableWidget_Communication->setItem(idxNewRecord,ET_TABLEW_COMMUNICATION_INTERFACE,itemCom_ip);
    }
    else if(record->value(EQUIPEMENT_TYPE_CONNEXION).toString()=="RS232"){
        this->ui->tableWidget_Communication->setItem(idxNewRecord,ET_TABLEW_COMMUNICATION_INTERFACE,itemCom_portSerie);
    }

    this->ui->tableWidget_Communication->setItem(idxNewRecord,ET_TABLEW_COMMUNICATION_ETAT_COM,itemCom_etat);
}

void et_InterfaceExecutionTest::buttonAjouterSondeClicked()
{
    Dlg_Equipement dlgEquipement(this,this->m_bdHandler,true,0,"SONDE");

    if(!dlgEquipement.exec()) return;

    this->m_idSonde = dlgEquipement.getIdSelection();

    QSqlRecord* record = m_bdHandler->getEquipementRow(m_idSonde);

    if(record->value(EQUIPEMENT_TYPE_CONNEXION).toString() == "IP"){
        ui->lineEdit_InterfaceSonde->setText(record->value(EQUIPEMENT_ADRESSE_IP).toString());
    }
    else if(record->value(EQUIPEMENT_TYPE_CONNEXION).toString() == "RS232"){
        QSqlRecord recordSonde = *(m_bdHandler->getDesignationPortSerie(m_idSonde));
        ui->lineEdit_InterfaceSonde->setText(recordSonde.value("designation").toString());
    }

    QSqlRecord recordEquipement = *(m_bdHandler->getEquipementModeledRow(m_idSonde));

    this->ui->label_Sonde->setText(recordEquipement.value(REL_EQUIPEMENT_NUM_SERIE).toString());
    this->ui->groupBox_5->show();

    if(this->ui->label_Sonde->text()==""){
        this->ui->ajouterSonde->setEnabled(true);
        this->ui->doubleSpinBox_Temperature->show();
        m_sondePresente = false;
    }
    else{
        this->ui->ajouterSonde->setEnabled(false);
        this->ui->doubleSpinBox_Temperature->hide();
        m_sondePresente = true;
    }
}

void et_InterfaceExecutionTest::buttonSupprimerSondeClicked()
{
    this->ui->label_Sonde->setText("");
    for(int i=0 ; i<this->ui->tableWidget_Communication->rowCount() ; i++){
        if(this->ui->tableWidget_Communication->item(i,ET_TABLEW_COMMUNICATION_ID_EQUIPEMENT)->text().toInt() == this->m_idSonde){
            this->ui->tableWidget_Communication->removeRow(i);
        }
    }

    this->ui->ajouterSonde->setEnabled(true);
    this->ui->doubleSpinBox_Temperature->show();
    ui->lineEdit_InterfaceSonde->setText("");
    this->ui->groupBox_5->hide();
    m_sondePresente = false;
}

void et_InterfaceExecutionTest::buttonSupprimerClicked()
{
    if(m_idxSelectionAnalyseurs.isValid()) {
        if(m_idxCommunicationAnalyseurs.row() == m_idxSelectionAnalyseurs.row())
            this->ui->lineEdit_InterfaceAnalyseur->setEnabled(false);
        this->ui->tableWidget_Communication->removeRow(m_idxSelectionAnalyseurs.row());
        this->ui->tableWidget_Analyseurs->removeRow(m_idxSelectionAnalyseurs.row());
        this->ui->lineEdit_InterfaceAnalyseur->clear();
        this->ui->button_TestAnalyseur->setEnabled(false);
    }
}

void et_InterfaceExecutionTest::buttonAnnulerClicked()
{
    QMessageBox msgBox;
    msgBox.setText("Annuler?");
    msgBox.setInformativeText("Voulez-vous annuler et revenir à l'accueil?");
    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);

    if(msgBox.exec()==QMessageBox::Ok)
        emit(this->fermeture());
}

void et_InterfaceExecutionTest::buttonPrecedentClicked()
{
    m_etape--;
    this->ui->tabWidget_ExecutionTest->setCurrentIndex(m_etape-1);
    this->ui->button_Suivant->setEnabled(true);
    ui->button_Executer->setVisible(false);
    ui->button_MettreEnAttente->setVisible(false);
}

void et_InterfaceExecutionTest::buttonSuivantClicked()
{
    switch(m_etape) {
    case 1:
        if(controleEtape1()) {
            m_etape = 2;
            this->ui->button_Precedent->setEnabled(true);
            this->ui->button_Suivant->setEnabled(true);
        }
        else return;
        break;
    case 2:
        QCoreApplication::sendPostedEvents(); // modifie
        if(controleEtape2()) {
            m_etape = 3;
            this->ui->button_Precedent->setEnabled(true);
            this->ui->button_Suivant->setEnabled(false);
            if(!m_miseEnAttente)
                ui->button_Executer->setVisible(true);
            ui->button_MettreEnAttente->setVisible(true);
        }
        else return;
        break;
    default :
        break;
    }
    this->ui->tabWidget_ExecutionTest->setCurrentIndex(m_etape-1);
}

void et_InterfaceExecutionTest::buttonTestAnalyseurClicked()
{

    this->ui->button_TestAnalyseur->setEnabled(false);
    this->ui->button_TestCalibrateur->setEnabled(false);
    this->ui->button_TestSonde->setEnabled(false);

    if(!m_appareilEnTest.isNull())
        delete m_appareilEnTest;

    if(!m_idxCommunicationAnalyseurs.isValid())
        return;
    ushort idAnalyseur = ui->tableWidget_Analyseurs->item(m_idxCommunicationAnalyseurs.row(),ET_TABLEW_ANALYSEURS_ID_EQUIPEMENT)->text().toUInt();
    DesignationProtocole protocole = m_bdHandler->getDesignationProtocole(idAnalyseur);

    QString interface = ui->lineEdit_InterfaceAnalyseur->text();
    QString adresse = ui->tableWidget_Analyseurs->item(m_idxCommunicationAnalyseurs.row(),ET_TABLEW_ANALYSEURS_ADRESSE)->text();
    QPointer<Protocole> analyseur = Protocole::getProtocoleObject(protocole,adresse);

    if(analyseur.isNull()) {
        QMessageBox msgBox;
        msgBox.setText(QLatin1String("Configuration de l'appareil impossible"));
        msgBox.setInformativeText(QLatin1String("Une erreur s'est produite lors de la configuration de l'appareil.\n\nVeuillez vefifier les paramètres de l'equipement avant de recommencer le test"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
    QSqlRecord* equipementRecord= m_bdHandler->getEquipementRow(idAnalyseur);
    QString typeConnexion = equipementRecord->value(EQUIPEMENT_TYPE_CONNEXION).toString();
    QPointer<ThreadComHandler> threadCommunication = new ThreadComHandler(typeConnexion);

    connect(threadCommunication,SIGNAL(ouverturePort(bool)),this,SLOT(ouverturePortComAnalyseur(bool)));
    connect(analyseur,SIGNAL(erreurTransmission()),this,SLOT(erreurCommunicationAnalyseur()));

    m_typeConnexion = equipementRecord->value(EQUIPEMENT_TYPE_CONNEXION).toString();
    m_IP =  equipementRecord->value(EQUIPEMENT_ADRESSE_IP).toString();
    m_numPort =  equipementRecord->value(EQUIPMENT_PORT_IP).toInt();
    m_typeSocket =  equipementRecord->value(EQUIPEMENT_TYPE_SOCKET).toString();

    if(m_typeConnexion=="IP"){
        threadCommunication->configureIP(m_IP, m_numPort, m_typeSocket);
    }
    else if(m_typeConnexion=="RS232"){
        threadCommunication->configureRS232(interface);
    }



    m_appareilEnTest = analyseur;

    analyseur->setThreadComHandler(threadCommunication);
    analyseur->setTimeOut(500);

    analyseur->init();

    m_listeInterfaceAnalyseurs[idAnalyseur] = interface;
    m_listeEtatComAnalyseurs[idAnalyseur] = true;

    if(analyseur->demandeAlarme() > 0) {
        QMessageBox msgBox;
        msgBox.setText(QLatin1String("Alarme presente"));
        msgBox.setInformativeText(QLatin1String("Une alarme a ete detectee sur l'appareil.\nVeuillez la corriger ou ignorer"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Ignore);
        msgBox.setDefaultButton(QMessageBox::Ok);
        if(msgBox.exec()==QMessageBox::Ok) {
            m_listeEtatComAnalyseurs[idAnalyseur] = false;

            QTableWidgetItem* itemCom_etat = new QTableWidgetItem(QLatin1String("Appareil en alarme"));
            itemCom_etat->setForeground(QColor(255,0,0));
            this->ui->tableWidget_Communication->setItem(m_idxCommunicationAnalyseurs.row(),ET_TABLEW_COMMUNICATION_ETAT_COM,itemCom_etat);
        }
    }
    QTableWidgetItem* itemCom_interface = new QTableWidgetItem(interface);
    this->ui->tableWidget_Communication->setItem(m_idxCommunicationAnalyseurs.row(),ET_TABLEW_COMMUNICATION_INTERFACE,itemCom_interface);
    analyseur->quitter();
    analyseur->deleteLater();
    QCoreApplication::processEvents();
    this->ui->button_TestAnalyseur->setEnabled(true);
    this->ui->button_TestCalibrateur->setEnabled(true);
    this->ui->button_TestSonde->setEnabled(true);
    //m_listeEtatComAnalyseurs[idAnalyseur] = true;
}

void et_InterfaceExecutionTest::buttonTestCalibrateurClicked()
{
    this->ui->button_TestAnalyseur->setEnabled(false);
    this->ui->button_TestCalibrateur->setEnabled(false);
    this->ui->button_TestSonde->setEnabled(false);

    if(!m_appareilEnTest.isNull())
        delete m_appareilEnTest;

    m_etatComCalibrateur = true;
    ushort idCalibrateur = m_bdHandler->getIdCalibrateur(m_test[0]->getIdSystemeEtalon());
    QSqlRecord* record = m_bdHandler->getEquipementRow(idCalibrateur);

    DesignationProtocole protocole = m_bdHandler->getDesignationProtocole(idCalibrateur);

    QPointer<Protocole> calibrateur = Protocole::getProtocoleObject(protocole,record->value(EQUIPEMENT_ADRESSE).toString());

    delete record;

    if(calibrateur.isNull()) {
        QMessageBox msgBox;
        msgBox.setText(QLatin1String("Configuration de l'appareil impossible"));
        msgBox.setInformativeText(QLatin1String("Une erreur s'est produite lors de la configuration de l'appareil.\n\nVeuillez vefifier les paramètres de l'equipement avant de recommencer le test"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }
    QSqlRecord* equipementRecord= m_bdHandler->getEquipementRow(idCalibrateur);
    QString interface = ui->lineEdit_InterfaceCalibrateur->text();
    m_typeConnexion = equipementRecord->value(EQUIPEMENT_TYPE_CONNEXION).toString();
    m_IP =  equipementRecord->value(EQUIPEMENT_ADRESSE_IP).toString();
    m_numPort =  equipementRecord->value(EQUIPMENT_PORT_IP).toInt();
    m_typeSocket =  equipementRecord->value(EQUIPEMENT_TYPE_SOCKET).toString();
    m_appareilEnTest = calibrateur;
    //QString typeConnexion = record->value(EQUIPEMENT_TYPE_CONNEXION).toString();
    QPointer<ThreadComHandler> threadCommunication = new ThreadComHandler(m_typeConnexion);

    connect(threadCommunication,SIGNAL(ouverturePort(bool)),this,SLOT(ouverturePortComCalibrateur(bool)));
    connect(calibrateur,SIGNAL(erreurTransmission()),this,SLOT(erreurCommunicationCalibrateur()));


    if(m_typeConnexion=="IP"){
        threadCommunication->configureIP(m_IP, m_numPort, m_typeSocket);
    }
    else if(m_typeConnexion=="RS232"){
        threadCommunication->configureRS232(interface);
    }
    calibrateur->setThreadComHandler(threadCommunication);
    calibrateur->setTimeOut(750);

    if(calibrateur->demandeAlarme() > 0) {
        QMessageBox msgBox;
        msgBox.setText(QLatin1String("Alarme presente"));
        msgBox.setInformativeText(QLatin1String("Une alarme a ete detectee sur l'appareil.\nVeuillez la corriger ou ignorer"));
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Ignore);
        msgBox.setDefaultButton(QMessageBox::Ok);

        if(msgBox.exec()==QMessageBox::Ok){
            ui->label_EtatCalibrateur->setText(QLatin1String("<font color=\"#FF6464\">Alarme</font>"));
            m_etatComCalibrateur = false;
        }
    }
    calibrateur->quitter();
    calibrateur->deleteLater();
    QCoreApplication::processEvents();
    //m_etatComCalibrateur = true;
    this->ui->button_TestAnalyseur->setEnabled(true);
    this->ui->button_TestCalibrateur->setEnabled(true);
    this->ui->button_TestSonde->setEnabled(true);
}

void et_InterfaceExecutionTest::buttonTestSondeClicked()
{
    this->ui->button_TestAnalyseur->setEnabled(false);
    this->ui->button_TestCalibrateur->setEnabled(false);
    this->ui->button_TestSonde->setEnabled(false);

    if(!m_appareilEnTest.isNull())
        delete m_appareilEnTest;

    m_etatComSonde = true;

    QSqlRecord* record = m_bdHandler->getEquipementRow(m_idSonde);

    DesignationProtocole protocole = m_bdHandler->getDesignationProtocole(m_idSonde);

    QPointer<Protocole> sonde = Protocole::getProtocoleObject(protocole,record->value(EQUIPEMENT_ADRESSE).toString());



    if(sonde.isNull()) {
        QMessageBox msgBox;
        msgBox.setText(QLatin1String("Configuration de l'appareil impossible"));
        msgBox.setInformativeText(QLatin1String("Une erreur s'est produite lors de la configuration de l'appareil.\n\nVeuillez vefifier les paramètres de l'equipement avant de recommencer le test"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    m_appareilEnTest = sonde;
    QString interface = ui->lineEdit_InterfaceSonde->text();
    m_typeConnexion = record->value(EQUIPEMENT_TYPE_CONNEXION).toString();
    m_IP =  record->value(EQUIPEMENT_ADRESSE_IP).toString();
    m_numPort =  record->value(EQUIPMENT_PORT_IP).toInt();
    m_typeSocket =  record->value(EQUIPEMENT_TYPE_SOCKET).toString();
    delete record;
    QPointer<ThreadComHandler> threadCommunication = new ThreadComHandler(m_typeConnexion);

    connect(threadCommunication,SIGNAL(ouverturePort(bool)),this,SLOT(ouverturePortComSonde(bool)));
    connect(sonde,SIGNAL(erreurTransmission()),this,SLOT(erreurCommunicationSonde()));

    if(m_typeConnexion=="IP"){
        threadCommunication->configureIP(m_IP, m_numPort, m_typeSocket);
    }
    else if(m_typeConnexion=="RS232"){
        threadCommunication->configureRS232(interface);
    }

    sonde->setThreadComHandler(threadCommunication);
    sonde->setTimeOut(750);

    sonde->demandeAlarme();

    sonde->quitter();
    sonde->deleteLater();
    QCoreApplication::processEvents();
    //m_etatComCalibrateur = true;
    this->ui->button_TestAnalyseur->setEnabled(true);
    this->ui->button_TestCalibrateur->setEnabled(true);
    this->ui->button_TestSonde->setEnabled(true);
}

void et_InterfaceExecutionTest::tableWidgetCommunicationClicked(const QModelIndex index)
{
    m_idxCommunicationAnalyseurs = index;

    this->ui->lineEdit_InterfaceAnalyseur->setEnabled(m_idxCommunicationAnalyseurs.isValid());
    this->ui->lineEdit_InterfaceAnalyseur->setFocus();
    int ligneSelect = this->ui->tableWidget_Communication->currentRow();
    this->ui->lineEdit_InterfaceAnalyseur->setText(this->ui->tableWidget_Communication->item(ligneSelect,2)->text());
}

void et_InterfaceExecutionTest::lineEditCanalCalibrateurTextChanged(const QString text)
{
    if(text.count()>0 && this->ui->lineEdit_InterfaceCalibrateur->text().count()>0)
        this->ui->button_TestCalibrateur->setEnabled(true);
    else
        this->ui->button_TestCalibrateur->setEnabled(false);
}

void et_InterfaceExecutionTest::lineEditInterfaceCalibrateurTextChanged(const QString text)
{
    if(text.count()<=0 && this->ui->lineEdit_CanalCalibrateur->text().count()<=0)
        this->ui->button_TestCalibrateur->setEnabled(false);
    else
        this->ui->button_TestCalibrateur->setEnabled(true);

}

void et_InterfaceExecutionTest::lineEditInterfaceAnalyseurTextChanged(const QString text)
{
    if(text.count()>0)
        this->ui->button_TestAnalyseur->setEnabled(true);
    else
        this->ui->button_TestAnalyseur->setEnabled(false);
}

void et_InterfaceExecutionTest::lineEditInterfaceSondeTextChanged(const QString text)
{
    if(text.count()>0)
        this->ui->button_TestSonde->setEnabled(true);
    else
        this->ui->button_TestSonde->setEnabled(false);
}

void et_InterfaceExecutionTest::lineEditParamsCalibrateurEnterPressed()
{
    if(ui->lineEdit_CanalCalibrateur->text().count()>0 && ui->lineEdit_InterfaceCalibrateur->text().count()>0)
        buttonTestCalibrateurClicked();
}

void et_InterfaceExecutionTest::lineEditInterfaceAnalyseurEnterPressed()
{
    if(ui->lineEdit_InterfaceAnalyseur->text().count()>0)
        buttonTestAnalyseurClicked();
}

void et_InterfaceExecutionTest::lineEditInterfaceSondeEnterPressed()
{
    if(ui->lineEdit_InterfaceSonde->text().count()>0)
        buttonTestSondeClicked();
}

void et_InterfaceExecutionTest::erreurCommunicationAnalyseur()
{
    ushort idAnalyseur = ui->tableWidget_Analyseurs->itemAt(m_idxCommunicationAnalyseurs.row(),ET_TABLEW_ANALYSEURS_ID_EQUIPEMENT)->text().toUInt();

    m_listeEtatComAnalyseurs[idAnalyseur] = false;

    QTableWidgetItem* itemCom_etat = new QTableWidgetItem("Erreur de communication");

    itemCom_etat->setForeground(QColor(255,0,0)); // modifie
    this->ui->tableWidget_Communication->setItem(m_idxCommunicationAnalyseurs.row(),ET_TABLEW_COMMUNICATION_ETAT_COM,itemCom_etat);
}

void et_InterfaceExecutionTest::erreurCommunicationCalibrateur()
{
    m_etatComCalibrateur = false;
    this->ui->label_EtatCalibrateur->setText(QLatin1String("<font color=\"#FF0000\">Erreur de communication!</font>"));
}

void et_InterfaceExecutionTest::erreurCommunicationSonde()
{
    m_etatComSonde = false;
    this->ui->label_EtatSonde->setText(QLatin1String("<font color=\"#FF0000\">Erreur de communication!</font>"));
}

void et_InterfaceExecutionTest::ouverturePortComAnalyseur(bool ouverturePort)
{
    if(!ouverturePort) {
        ushort idAnalyseur = ui->tableWidget_Analyseurs->itemAt(m_idxCommunicationAnalyseurs.row(),ET_TABLEW_ANALYSEURS_ID_EQUIPEMENT)->text().toUInt();

        m_listeEtatComAnalyseurs[idAnalyseur] = false;

        QTableWidgetItem* itemCom_etat = new QTableWidgetItem("Erreur de l'ouverture du port");
        itemCom_etat->setForeground(QColor(255,0,0)); // modifie
        this->ui->tableWidget_Communication->setItem(m_idxCommunicationAnalyseurs.row(),ET_TABLEW_COMMUNICATION_ETAT_COM,itemCom_etat);
    }
    else {
        QTableWidgetItem* itemCom_etat = new QTableWidgetItem("Communication Ok");
        itemCom_etat->setForeground(QColor(0,255,0)); // modifie
        this->ui->tableWidget_Communication->setItem(m_idxCommunicationAnalyseurs.row(),ET_TABLEW_COMMUNICATION_ETAT_COM,itemCom_etat);
    }
}

void et_InterfaceExecutionTest::ouverturePortComCalibrateur(const bool ouverturePort)
{
    if(!ouverturePort) {
        m_etatComCalibrateur = false;
        this->ui->label_EtatCalibrateur->setText(QLatin1String("<font color=\"#FF0000\">Erreur de l'ouverture du port</font>"));
    }
    else
        ui->label_EtatCalibrateur->setText(QLatin1String("<font color=\"#64FF64\">Communication Ok</font>"));
}

void et_InterfaceExecutionTest::ouverturePortComSonde(const bool ouverturePort)
{
    if(!ouverturePort) {
        m_etatComSonde = false;
        this->ui->label_EtatSonde->setText(QLatin1String("<font color=\"#FF0000\">Erreur de l'ouverture du port</font>"));
    }
    else
        ui->label_EtatSonde->setText(QLatin1String("<font color=\"#64FF64\">Communication Ok</font>"));
}

void et_InterfaceExecutionTest::tabWidgetExecutionTestIndexChanged(const int index)
{
    ui->tabWidget_ExecutionTest->setCurrentIndex(m_etape-1);
}

void et_InterfaceExecutionTest::buttonExecuterClicked()
{
    if(!m_appareilEnTest.isNull()) {
        m_appareilEnTest->deleteLater();
        QCoreApplication::sendPostedEvents();// modifie
    }
    ui->button_Precedent->setEnabled(false);
    ui->button_Executer->setEnabled(false);
    ui->button_MettreEnAttente->setEnabled(false);

    QPointer<et_ParamsTest> paramsTest = this->preparerInfosTest()[0];

    while(QDateTime::currentDateTime() < paramsTest->m_dateHeureDebutPrevu){

    }
    m_infosTestEnCours->enregistrerParamsTest(paramsTest);
    m_testAExecuter = new ExecutionTest(paramsTest,m_bdHandler);

    connect(m_testAExecuter,SIGNAL(traceTest(QString,ushort)),m_infosTestEnCours,SLOT(afficherTraceTest(QString,ushort)));

    connect(m_testAExecuter,SIGNAL(exitTest()),this,SLOT(finTest()));


    m_testAExecuter->run();

    ui->button_Precedent->setEnabled(true);
    ui->button_Executer->setEnabled(true);
    ui->button_MettreEnAttente->setEnabled(true);
}

void et_InterfaceExecutionTest::buttonMettreEnAttenteClicked() {

    QList<QPointer<et_ParamsTest>> paramsTest =this->preparerInfosTest();
    emit(miseEnAttente(paramsTest));
}

QList<QPointer<et_ParamsTest>> et_InterfaceExecutionTest::preparerInfosTest()
{
    QList<QPointer<et_ParamsTest>> ListeparamsTest;
    for(int i=0 ; i<m_idTestXML.count() ; i++){

        QPointer<et_ParamsTest> paramsTest = new et_ParamsTest();
        QString nomFichier = decoupeNomFichier(m_fichierDescription[i].mid(m_fichierDescription[i].lastIndexOf('/')+1));

        paramsTest->m_id_TestXML = m_idTestXML[i];
        paramsTest->m_test = new Test(*(m_test[i].data()));
        paramsTest->m_listeInterfaceAnalyseurs = m_listeInterfaceAnalyseurs;
        paramsTest->m_interfaceCalibrateur = ui->lineEdit_InterfaceCalibrateur->text();
        paramsTest->m_canalCalibrateur = ui->lineEdit_CanalCalibrateur->text();
        paramsTest->m_interfaceSonde = ui->lineEdit_InterfaceSonde->text();
        paramsTest->m_idOperateur = m_modelOperateur->record(ui->comboBox_Operateur->currentIndex()).value(OPERATEUR_ID).toUInt();
        paramsTest->m_idLieu = m_modelLieu->record(ui->comboBox_Lieu->currentIndex()).value(LIEU_ID).toUInt();
        paramsTest->m_nomTmpFichierCSV = m_nomCheminCSV+"/DATE_HEURE_"+nomFichier+".csv"; //
        paramsTest->m_designationLieu = m_modelLieu->record(ui->comboBox_Lieu->currentIndex()).value(LIEU_DESIGNATION).toString();
        paramsTest->m_nomTest = nomFichier;//
        paramsTest->m_pression = ui->doubleSpinBox_Pression->value();
        paramsTest->sondePresente=m_sondePresente;
        paramsTest->m_idSonde = m_idSonde;
        if(m_sondePresente == false){
            paramsTest->m_temperature = ui->doubleSpinBox_Temperature->value();
        }
        else{
            paramsTest->m_temperature = 0;
        }
        paramsTest->m_debutImmediat = m_debutImmediat;
        if(!m_debutImmediat){
            paramsTest->m_dateHeureDebutPrevu = this->ui->dateTime_DebutTest->dateTime();
        }
        else{
            paramsTest->m_dateHeureDebutPrevu = QDateTime::currentDateTime();
        }

        QString operateur = m_modelOperateur->record(ui->comboBox_Operateur->currentIndex()).value(OPERATEUR_NOM).toString();
        operateur.append(" ");
        operateur.append(m_modelOperateur->record(ui->comboBox_Operateur->currentIndex()).value(OPERATEUR_PRENOM).toString());
        paramsTest->m_nomOperateur = operateur;

        for(int i =0;i<ui->tableWidget_Communication->rowCount();i++) {
            ushort idAnalyseur = ui->tableWidget_Communication->item(i,ET_TABLEW_COMMUNICATION_ID_EQUIPEMENT)->text().toUInt();
            QString numSerieAna = ui->tableWidget_Communication->item(i,ET_TABLEW_COMMUNICATION_NUM_SERIE)->text();
            paramsTest->m_listeNumSerieAnalyseurs.insert(idAnalyseur,numSerieAna);
        }
        ListeparamsTest.append(paramsTest);
    }

    return ListeparamsTest;

}

void et_InterfaceExecutionTest::bgChoixDebutTestValueChanged(int idButton)
{
    m_debutImmediat = (idButton == 0);
}

void et_InterfaceExecutionTest::dateTimeDebutTestValueChanged(QDateTime dateTimeDebutTest)
{
    m_dateHeureDebutTest = dateTimeDebutTest;
}

void et_InterfaceExecutionTest::button_choixEnregistrementCSV()
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("*.csv");
    dialog.setDirectory(getParam("Path_CSV").toString());
    dialog.selectFile(this->ui->lineEdit_FichierCSV->text());
    if(dialog.exec()==QFileDialog::Accepted){
        QStringList chemin =  dialog.selectedFiles();
        this->ui->lineEdit_FichierCSV->setText(chemin.value(0));
    }
    else {
        return;
    }

}

void et_InterfaceExecutionTest::button_choixMultipleEnregistrementCSV(int idButon)
{
    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter("*.csv");
    dialog.setDirectory(getParam("Path_CSV").toString());
    dialog.selectFile(listeLineEditCSV[idButon]->text());
    if(dialog.exec()==QFileDialog::Accepted){
        QStringList chemin =  dialog.selectedFiles();
        listeLineEditCSV[idButon]->setText(chemin.value(0));
    }
    else {
        return;
    }
}
