/*////////////////////////////////////////////////////////////
// \file et_interfaceexecutiontest.cpp
// \brief Classe d'interface gérant l'exécution des tests
// \author FOUQUART Christophe
// \version 1.0
// \date 10/08/2011
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

#include "et_interfaceexecutiontest.h"
#include "ui_et_interfaceexecutiontest.h"

et_InterfaceExecutionTest::et_InterfaceExecutionTest(QPointer<BdHandler> bdHandler,ushort idTestXML, QString fichierDescription,bool miseEnAttente,ushort idOperateur, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::et_InterfaceExecutionTest)
{
    ui->setupUi(this);

    m_bdHandler = bdHandler;
    m_fichierDescription = fichierDescription;
    m_etape = 1;
    m_idOperateur = idOperateur;
    m_miseEnAttente = miseEnAttente;
    m_idTestXML = idTestXML;
    m_test = Test::importFromXml(fichierDescription);

    if(m_test.isNull()) {
        QMessageBox msgBox;
        msgBox.setText("Un problème est survenu lors du chargement du fichier de configuration du test");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.exec();

        emit(this->fermeture());
        return;
    }

    this->ui->button_Precedent->setEnabled(false);
//    this->ui->tableWidget_Analyseurs->setColumnHidden(ET_TABLEW_ANALYSEURS_ID_EQUIPEMENT,true);
//    this->ui->tableWidget_Communication->setColumnHidden(ET_TABLEW_COMMUNICATION_ID_EQUIPEMENT,true);

    QString nomFichier = m_fichierDescription;
    if(nomFichier.indexOf(".xml")>0)
        nomFichier.remove(".xml");
    if(nomFichier.indexOf(".\\")>0)
        nomFichier.remove(".\\");
    if(nomFichier.contains("./"))
        nomFichier.remove("./");

    this->ui->lineEdit_FichierCSV->setText("DATE_HEURE_"+nomFichier+".csv");
    this->ui->lineEdit_FichierDescription->setText(nomFichier);
    this->ui->lineEdit_TypeTest->setText(typeTestToString(m_test->getTypeTest()));

    if(!m_miseEnAttente) {
        m_infosTestEnCours = new et_InfosTestEnCours(m_bdHandler,this);
        this->ui->h_Layout_InfosTest->addWidget(m_infosTestEnCours.data());
    }
    m_modelOperateur = m_bdHandler->getOperateurModel();
    ui->comboBox_Operateur->setModel(m_modelOperateur);
    ui->comboBox_Operateur->setModelColumn(OPERATEUR_NOM);

    m_modelLieu = m_bdHandler->getLieuModel();
    ui->comboBox_Lieu->setModel(m_modelLieu);
    ui->comboBox_Lieu->setModelColumn(LIEU_DESIGNATION);

    ui->tabWidget_ExecutionTest->setCurrentIndex(0);
    ui->button_Executer->setVisible(false);
    ui->button_MettreEnAttente->setVisible(false);

    ui->bg_ChoixDebutTest->setId(ui->radioButton_DebutImmediat,0);
    ui->bg_ChoixDebutTest->setId(ui->radioButton_DebutDateHeure,1);

    ui->dateTime_DebutTest->setDateTime(QDateTime::currentDateTime());

    ushort idCalibrateur = m_bdHandler->getIdCalibrateur(m_test->getIdSystemeEtalon());
    QSqlRecord record = *(m_bdHandler->getDesignationPortSerie(idCalibrateur));
    ui->lineEdit_InterfaceCalibrateur->setText(record.value("designation").toString());



    connect(this->ui->tableWidget_Analyseurs,SIGNAL(clicked(QModelIndex)),this,SLOT(tableWidgetAnalyseursClicked(QModelIndex)));
    connect(this->ui->tableWidget_Communication,SIGNAL(clicked(QModelIndex)),this,SLOT(tableWidgetCommunicationClicked(QModelIndex)));
    connect(this->ui->button_Annuler,SIGNAL(clicked()),this,SLOT(buttonAnnulerClicked()));
    connect(this->ui->button_Ajouter,SIGNAL(clicked()),this,SLOT(buttonAjouterClicked()));
    connect(this->ui->button_Suivant,SIGNAL(clicked()),this,SLOT(buttonSuivantClicked()));
    connect(this->ui->button_Precedent,SIGNAL(clicked()),this,SLOT(buttonPrecedentClicked()));
    connect(this->ui->button_Supprimer,SIGNAL(clicked()),this,SLOT(buttonSupprimerClicked()));
    connect(this->ui->button_TestAnalyseur,SIGNAL(clicked()),this,SLOT(buttonTestAnalyseurClicked()));
    connect(this->ui->button_TestCalibrateur,SIGNAL(clicked()),this,SLOT(buttonTestCalibrateurClicked()));
    connect(this->ui->lineEdit_InterfaceAnalyseur,SIGNAL(textChanged(QString)),this,SLOT(lineEditInterfaceAnalyseurTextChanged(QString)));
    connect(this->ui->lineEdit_InterfaceAnalyseur,SIGNAL(returnPressed()),this,SLOT(lineEditInterfaceAnalyseurEnterPressed()));
    connect(this->ui->lineEdit_CanalCalibrateur,SIGNAL(textChanged(QString)),this,SLOT(lineEditCanalCalibrateurTextChanged(QString)));
    connect(this->ui->lineEdit_CanalCalibrateur,SIGNAL(returnPressed()),this,SLOT(lineEditParamsCalibrateurEnterPressed()));
    connect(this->ui->lineEdit_InterfaceCalibrateur,SIGNAL(textChanged(QString)),this,SLOT(lineEditInterfaceCalibrateurTextChanged(QString)));
    connect(this->ui->lineEdit_InterfaceCalibrateur,SIGNAL(returnPressed()),this,SLOT(lineEditParamsCalibrateurEnterPressed()));
    connect(this->ui->tabWidget_ExecutionTest,SIGNAL(currentChanged(int)),this,SLOT(tabWidgetExecutionTestIndexChanged(int)));
    connect(this->ui->button_Executer,SIGNAL(clicked()),this,SLOT(buttonExecuterClicked()));
    connect(this->ui->button_MettreEnAttente,SIGNAL(clicked()),this,SLOT(buttonMettreEnAttenteClicked()));
    connect(this->ui->bg_ChoixDebutTest,SIGNAL(buttonClicked(int)),this,SLOT(bgChoixDebutTestValueChanged(int)));
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
    if(!m_test.isNull())
        delete m_test;
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


    if(this->ui->doubleSpinBox_Temperature->value()==0) {
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
            msgBox.setText("Le fichier d'export des résultats existe déjà.\nVoulez-vous l'écraser?");
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
    while (iterator.hasNext()) {
        iterator.next();
        if(iterator.value() == false)
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

    QTableWidgetItem* item_idEquipement = new QTableWidgetItem(QString::number(idEquipement));
    QTableWidgetItem* item_noSerie = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_NUM_SERIE).toString());
    QTableWidgetItem* item_modele = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_MODELE).toString());
    QTableWidgetItem* item_marque = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_MARQUE).toString());
    QTableWidgetItem* item_minGamme = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_MIN_GAMME).toString());
    QTableWidgetItem* item_maxGamme = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_MAX_GAMME).toString());
    QTableWidgetItem* item_offset = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_OFFSET).toString());
    QTableWidgetItem* item_adresse = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_ADRESSE).toString());
    QTableWidgetItem* item_portSerie = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_PORTSERIE).toString());

    uint idxNewRecord = this->ui->tableWidget_Analyseurs->rowCount();
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

    this->ui->tableWidget_Communication->insertRow(idxNewRecord);
    QTableWidgetItem* itemCom_idEquipement = new QTableWidgetItem(QString::number(idEquipement));
    QTableWidgetItem* itemCom_noSerie = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_NUM_SERIE).toString());
    QTableWidgetItem* itemCom_portSerie = new QTableWidgetItem(recordEquipement.value(REL_EQUIPEMENT_PORTSERIE).toString());
    QTableWidgetItem* itemCom_etat = new QTableWidgetItem("En attente de test...");

    this->ui->tableWidget_Communication->setItem(idxNewRecord,ET_TABLEW_COMMUNICATION_ID_EQUIPEMENT,itemCom_idEquipement);
    this->ui->tableWidget_Communication->setItem(idxNewRecord,ET_TABLEW_COMMUNICATION_NUM_SERIE,itemCom_noSerie);
    this->ui->tableWidget_Communication->setItem(idxNewRecord,ET_TABLEW_COMMUNICATION_INTERFACE,itemCom_portSerie);
    this->ui->tableWidget_Communication->setItem(idxNewRecord,ET_TABLEW_COMMUNICATION_ETAT_COM,itemCom_etat);
}

void et_InterfaceExecutionTest::buttonSupprimerClicked()
{
    if(m_idxSelectionAnalyseurs.isValid()) {
        if(m_idxCommunicationAnalyseurs.row() == m_idxSelectionAnalyseurs.row())
            this->ui->lineEdit_InterfaceAnalyseur->setEnabled(false);
        this->ui->tableWidget_Communication->removeRow(m_idxSelectionAnalyseurs.row());
        this->ui->tableWidget_Analyseurs->removeRow(m_idxSelectionAnalyseurs.row());
    }
}

void et_InterfaceExecutionTest::buttonAnnulerClicked()
{
    QMessageBox msgBox;
    msgBox.setText("Annuler?");
    msgBox.setInformativeText("Voulez-vous annuler et revenir à l'acceuil?");
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
        QCoreApplication::flush();
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
        msgBox.setText("Configuration de l'appareil impossible");
        msgBox.setInformativeText("Une erreur s'est produite lors de la configuration de l'appareil.\n\nVeuillez véfifier les paramètres de l'équipement avant de recommencer le test");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    QPointer<ThreadComHandler> threadCommunication = new ThreadComHandler();

    connect(threadCommunication,SIGNAL(ouverturePort(bool)),this,SLOT(ouverturePortComAnalyseur(bool)));
    connect(analyseur,SIGNAL(erreurTransmission()),this,SLOT(erreurCommunicationAnalyseur()));

    threadCommunication->configureRS232(interface);

    m_appareilEnTest = analyseur;

    analyseur->setThreadComHandler(threadCommunication);
    analyseur->setTimeOut(500);

    analyseur->init();

    m_listeInterfaceAnalyseurs[idAnalyseur] = interface;
    m_listeEtatComAnalyseurs[idAnalyseur] = true;

    if(analyseur->demandeAlarme() > 0) {
        QMessageBox msgBox;
        msgBox.setText("Alarme présente");
        msgBox.setInformativeText("Une alarme a été détectée sur l'appareil.\nVeuillez la corriger ou ignorer");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Ignore);
        msgBox.setDefaultButton(QMessageBox::Ok);
        if(msgBox.exec()==QMessageBox::Ok) {
            m_listeEtatComAnalyseurs[idAnalyseur] = false;

            QTableWidgetItem* itemCom_etat = new QTableWidgetItem("Appareil en alarme");
            itemCom_etat->setTextColor(QColor(255,0,0));
            this->ui->tableWidget_Communication->setItem(m_idxCommunicationAnalyseurs.row(),ET_TABLEW_COMMUNICATION_ETAT_COM,itemCom_etat);
        }
    }
    QTableWidgetItem* itemCom_interface = new QTableWidgetItem(interface);
    itemCom_interface->setTextColor(QColor(255,0,0));
    this->ui->tableWidget_Communication->setItem(m_idxCommunicationAnalyseurs.row(),ET_TABLEW_COMMUNICATION_INTERFACE,itemCom_interface);
    analyseur->quitter();
    analyseur->deleteLater();
    QCoreApplication::processEvents();
}

void et_InterfaceExecutionTest::buttonTestCalibrateurClicked()
{
    if(!m_appareilEnTest.isNull())
        delete m_appareilEnTest;

    m_etatComCalibrateur = true;
    ushort idCalibrateur = m_bdHandler->getIdCalibrateur(m_test->getIdSystemeEtalon());
    QSqlRecord* record = m_bdHandler->getEquipementRow(idCalibrateur);

    DesignationProtocole protocole = m_bdHandler->getDesignationProtocole(idCalibrateur);

    QPointer<Protocole> calibrateur = Protocole::getProtocoleObject(protocole,record->value(EQUIPEMENT_ADRESSE).toString());

    delete record;

    if(calibrateur.isNull()) {
        QMessageBox msgBox;
        msgBox.setText("Configuration de l'appareil impossible");
        msgBox.setInformativeText("Une erreur s'est produite lors de la configuration de l'appareil.\n\nVeuillez véfifier les paramètres de l'équipement avant de recommencer le test");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return;
    }

    m_appareilEnTest = calibrateur;

    QPointer<ThreadComHandler> threadCommunication = new ThreadComHandler();

    connect(threadCommunication,SIGNAL(ouverturePort(bool)),this,SLOT(ouverturePortComCalibrateur(bool)));
    connect(calibrateur,SIGNAL(erreurTransmission()),this,SLOT(erreurCommunicationCalibrateur()));

    threadCommunication->configureRS232(ui->lineEdit_InterfaceCalibrateur->text());

    calibrateur->setThreadComHandler(threadCommunication);
    calibrateur->setTimeOut(500);

    calibrateur->init();

    if(calibrateur->demandeAlarme() > 0) {
        QMessageBox msgBox;
        msgBox.setText("Alarme présente");
        msgBox.setInformativeText("Une alarme a été détectée sur l'appareil.\nVeuillez la corriger ou ignorer");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Ignore);
        msgBox.setDefaultButton(QMessageBox::Ok);

        if(msgBox.exec()==QMessageBox::Ok){
            ui->label_EtatCalibrateur->setText("<font color=\"#FF6464\">Alarme</font>");
            m_etatComCalibrateur = false;
        }
    }
    calibrateur->quitter();
    calibrateur->deleteLater();
    QCoreApplication::processEvents();
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
    if(text.count()>0 && this->ui->lineEdit_CanalCalibrateur->text().count()>0)
        this->ui->button_TestCalibrateur->setEnabled(true);
    else
        this->ui->button_TestCalibrateur->setEnabled(false);
}

void et_InterfaceExecutionTest::lineEditInterfaceAnalyseurTextChanged(const QString text)
{
    this->ui->button_TestAnalyseur->setEnabled((text.count()>0));
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

void et_InterfaceExecutionTest::erreurCommunicationAnalyseur()
{
    ushort idAnalyseur = ui->tableWidget_Analyseurs->itemAt(m_idxCommunicationAnalyseurs.row(),ET_TABLEW_ANALYSEURS_ID_EQUIPEMENT)->text().toUInt();

    m_listeEtatComAnalyseurs[idAnalyseur] = false;

    QTableWidgetItem* itemCom_etat = new QTableWidgetItem("Erreur de communication");
    itemCom_etat->setTextColor(QColor(255,0,0));
    this->ui->tableWidget_Communication->setItem(m_idxCommunicationAnalyseurs.row(),ET_TABLEW_COMMUNICATION_ETAT_COM,itemCom_etat);
}

void et_InterfaceExecutionTest::erreurCommunicationCalibrateur()
{
    m_etatComCalibrateur = false;
    this->ui->label_EtatCalibrateur->setText("<font color=\"#FF0000\">Erreur de communication!</font>");
}

void et_InterfaceExecutionTest::ouverturePortComAnalyseur(bool ouverturePort)
{
    if(!ouverturePort) {
        ushort idAnalyseur = ui->tableWidget_Analyseurs->itemAt(m_idxCommunicationAnalyseurs.row(),ET_TABLEW_ANALYSEURS_ID_EQUIPEMENT)->text().toUInt();

        m_listeEtatComAnalyseurs[idAnalyseur] = false;

        QTableWidgetItem* itemCom_etat = new QTableWidgetItem("Erreur de l'ouverture du port");
        itemCom_etat->setTextColor(QColor(255,0,0));
        this->ui->tableWidget_Communication->setItem(m_idxCommunicationAnalyseurs.row(),ET_TABLEW_COMMUNICATION_ETAT_COM,itemCom_etat);
    }
    else {
        QTableWidgetItem* itemCom_etat = new QTableWidgetItem("Communication Ok");
        itemCom_etat->setTextColor(QColor(255,0,0));
        this->ui->tableWidget_Communication->setItem(m_idxCommunicationAnalyseurs.row(),ET_TABLEW_COMMUNICATION_ETAT_COM,itemCom_etat);
    }
}

void et_InterfaceExecutionTest::ouverturePortComCalibrateur(const bool ouverturePort)
{
    if(!ouverturePort) {
        m_etatComCalibrateur = false;
        this->ui->label_EtatCalibrateur->setText("<font color=\"#FF0000\">Erreur de l'ouverture du port</font>");
    }
    else
        ui->label_EtatCalibrateur->setText("<font color=\"#64FF64\">Communication Ok</font>");
}

void et_InterfaceExecutionTest::tabWidgetExecutionTestIndexChanged(const int index)
{
    ui->tabWidget_ExecutionTest->setCurrentIndex(m_etape-1);
}

void et_InterfaceExecutionTest::buttonExecuterClicked()
{
    if(!m_appareilEnTest.isNull()) {
        m_appareilEnTest->deleteLater();
        QCoreApplication::flush();
    }
    ui->button_Precedent->setEnabled(false);
    ui->button_Executer->setEnabled(false);
    ui->button_MettreEnAttente->setEnabled(false);

    QPointer<et_ParamsTest> paramsTest = this->preparerInfosTest();

    m_infosTestEnCours->enregistrerParamsTest(paramsTest);
    m_testAExecuter = new ExecutionTest(paramsTest,m_bdHandler);

    connect(m_testAExecuter,SIGNAL(traceTest(QString,ushort)),m_infosTestEnCours,SLOT(afficherTraceTest(QString,ushort)));

// m_testAExecuter->moveToThread(&m_threadExecutionTest);

//    connect(&m_threadExecutionTest,SIGNAL(started()),m_testAExecuter,SLOT(run()));
//    connect(m_testAExecuter,SIGNAL(exitTest()),&m_threadExecutionTest,SLOT(quit()));
//    connect(&m_threadExecutionTest,SIGNAL(finished()),this,SLOT(finTest()));

//    m_threadExecutionTest.start();
    connect(m_testAExecuter,SIGNAL(exitTest()),this,SLOT(finTest()));

    m_testAExecuter->run();

    ui->button_Precedent->setEnabled(true);
    ui->button_Executer->setEnabled(true);
    ui->button_MettreEnAttente->setEnabled(true);
}

void et_InterfaceExecutionTest::buttonMettreEnAttenteClicked() {
    QPointer<et_ParamsTest> paramsTest = this->preparerInfosTest();
    emit(miseEnAttente(paramsTest));
}

QPointer<et_ParamsTest> et_InterfaceExecutionTest::preparerInfosTest()
{
    QString nomFichierCSV = ui->lineEdit_FichierCSV->text();

    if(nomFichierCSV.contains("DATE_HEURE")) {
        QDateTime currentDateTime = QDateTime::currentDateTime();
        QString strCurrentDateTime = currentDateTime.toString("ddMMyyyy_hhmm");
        nomFichierCSV.replace("DATE_HEURE",strCurrentDateTime);
    }

    QPointer<QFile> fichierCSV = new QFile(nomFichierCSV);

    if(fichierCSV.isNull()) {
        QMessageBox msgBox;
        msgBox.setText("Un problème a été rencontré lors de la création du fichier CSV");
        msgBox.setInformativeText("Impossible de créer le fichier CSV");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return 0;
    }

    bool ouvertureFichierCSV = fichierCSV->open(QFile::WriteOnly | QFile::Append);

    if(!ouvertureFichierCSV) {
        QMessageBox msgBox;
        msgBox.setText("Un problème a été rencontré lors de la création du fichier CSV");
        msgBox.setInformativeText("Impossible de créer le fichier CSV");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
        return 0;
    }

    QPointer<et_ParamsTest> paramsTest = new et_ParamsTest();

    paramsTest->m_id_TestXML = m_idTestXML;
    paramsTest->m_test = new Test(*(m_test.data()));
    paramsTest->m_listeInterfaceAnalyseurs = m_listeInterfaceAnalyseurs;
    paramsTest->m_interfaceCalibrateur = ui->lineEdit_InterfaceCalibrateur->text();
    paramsTest->m_canalCalibrateur = ui->lineEdit_CanalCalibrateur->text();
    paramsTest->m_idOperateur = m_modelOperateur->record(ui->comboBox_Operateur->currentIndex()).value(OPERATEUR_ID).toUInt();
    paramsTest->m_idLieu = m_modelLieu->record(ui->comboBox_Lieu->currentIndex()).value(LIEU_ID).toUInt();
    paramsTest->m_fichierCSV = fichierCSV;
    paramsTest->m_designationLieu = m_modelLieu->record(ui->comboBox_Lieu->currentIndex()).value(LIEU_DESIGNATION).toString();
    paramsTest->m_nomTest = this->ui->lineEdit_FichierDescription->text();
    paramsTest->m_pression = ui->doubleSpinBox_Pression->value();
    paramsTest->m_temperature = ui->doubleSpinBox_Temperature->value();
    paramsTest->m_debutImmediat = m_debutImmediat;
    paramsTest->m_dateHeureDebut = QDateTime::currentDateTime();

    QString operateur = m_modelOperateur->record(ui->comboBox_Operateur->currentIndex()).value(OPERATEUR_NOM).toString();
    operateur.append(" ");
    operateur.append(m_modelOperateur->record(ui->comboBox_Operateur->currentIndex()).value(OPERATEUR_PRENOM).toString());
    paramsTest->m_nomOperateur = operateur;

    for(int i =0;i<ui->tableWidget_Communication->rowCount();i++) {
        ushort idAnalyseur = ui->tableWidget_Communication->item(i,ET_TABLEW_COMMUNICATION_NUM_SERIE)->text().toUInt();
        QString numSerieAna = ui->tableWidget_Communication->item(i,ET_TABLEW_COMMUNICATION_NUM_SERIE)->text();
        paramsTest->m_listeNumSerieAnalyseurs.insertMulti(idAnalyseur,numSerieAna);
    }

    return paramsTest;
}

void et_InterfaceExecutionTest::bgChoixDebutTestValueChanged(int idButton)
{
    m_debutImmediat = (idButton == 0);
}

void et_InterfaceExecutionTest::dateTimeDebutTestValueChanged(QDateTime dateTimeDebutTest)
{
    m_dateHeureDebutTest = dateTimeDebutTest;
}
