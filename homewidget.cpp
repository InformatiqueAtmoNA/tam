/*////////////////////////////////////////////////////////////
// \file homewidget.cpp
// \brief Widget d'interface graphique de la fenetre principale
// \author FOUQUART Christophe
// \version 1.0
// \date 26/06/2011
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
#include "homewidget.h"
#include "ui_homewidget.h"


HomeWidget::HomeWidget(QPointer<BdHandler> bdHandler,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWidget)
{
    ui->setupUi(this);

    this->m_bdHandler = bdHandler;
    this->getListeTests();
    this->getListeRapports();

    //QHeaderView *headers = this->ui->tableWidget_TestXml->horizontalHeader();
    //headers->setResizeMode(QHeaderView::Stretch);

    connect(this->ui->button_Afficher,SIGNAL(clicked()),this,SLOT(buttonAfficherClicked()));
    connect(this->ui->button_Executer,SIGNAL(clicked()),this,SLOT(buttonExecuterClicked()));
    connect(this->ui->button_Modifier,SIGNAL(clicked()),this,SLOT(buttonModifierClicked()));
    connect(this->ui->button_Nouveau,SIGNAL(clicked()),this,SLOT(buttonNouveauClicked()));
    connect(this->ui->button_Supprimer,SIGNAL(clicked()),this,SLOT(buttonSupprimerClicked()));
    connect(this->ui->button_ProgrammerSerieTests,SIGNAL(clicked()),this,SIGNAL(programmerSerieTest()));
    connect(this->ui->tableWidget_TestXml,SIGNAL(clicked(QModelIndex)),this,SLOT(tableWidgetTestXmlIndexChanged(QModelIndex)));
    connect(this->ui->tableView_TestRapport,SIGNAL(clicked(QModelIndex)),this,SLOT(tableViewTestRapportIndexChanged(QModelIndex)));
    connect(this->ui->treeView,SIGNAL(clicked(QModelIndex)),this,SLOT(treeViewTestRapportIndexChanged(QModelIndex)));
    //connect(this->ui->button_supprimer_test_resultat,SIGNAL(clicked()),this,SLOT(buttonSupprimerTestResultatClicked()));

    this->ui->button_Executer->setEnabled(false);
    this->ui->button_Modifier->setEnabled(false);
    this->ui->button_Supprimer->setEnabled(false);
    this->ui->button_Afficher->setEnabled(false);
    this->ui->button_supprimer_test_resultat->setEnabled(false);
    this->ui->tabWidget_TestRapport->setCurrentIndex(0);
    this->ui->listView_Rapports->hide();
    this->ui->label_3->hide();

}

HomeWidget::~HomeWidget()
{
    delete m_modelTestXml;
    delete ui;
}

void HomeWidget::getListeTests()
{
    this->m_modelTestXml = this->m_bdHandler->getTestXmlModel();

    if(this->m_modelTestXml->rowCount()>0) {
        for(int i=0;i<this->m_modelTestXml->rowCount();i++) {
            QSqlRecord recordTestXml = this->m_modelTestXml->record(i);
            uint idSystemeEtalon = recordTestXml.value(TEST_XML_ID_SYSTEME_ETALON).toUInt();
            QSqlRecord recordSystemeEtalon = this->m_bdHandler->getSystemeEtalonModel(idSystemeEtalon)->record(0);

            QTableWidgetItem* item_idTest = new QTableWidgetItem(recordTestXml.value(TEST_XML_ID).toString());
            QTableWidgetItem* item_fichier = new QTableWidgetItem(recordTestXml.value(TEST_XML_NOM_FICHIER).toString());
            QTableWidgetItem* item_type = new QTableWidgetItem(recordTestXml.value(TEST_XML_TYPE_TEST).toString());
            QTableWidgetItem* item_numSerieDiluteur = new QTableWidgetItem(recordSystemeEtalon.value(SYS_ETALON_DILUTEUR).toString());
            QTableWidgetItem* item_numSerieBouteille = new QTableWidgetItem(recordSystemeEtalon.value(SYS_ETALON_BOUTEILLE).toString());
            QTableWidgetItem* item_numSerieGoz = new QTableWidgetItem(recordSystemeEtalon.value(SYS_ETALON_GZERO).toString());

            uint idxNewRecord = this->ui->tableWidget_TestXml->rowCount();
            this->ui->tableWidget_TestXml->insertRow(idxNewRecord);
            this->ui->tableWidget_TestXml->setItem(idxNewRecord,HOMEW_TABLEW_TEST_ID_TEST,item_idTest);
            this->ui->tableWidget_TestXml->setItem(idxNewRecord,HOMEW_TABLEW_TEST_FICHIER,item_fichier);
            this->ui->tableWidget_TestXml->setItem(idxNewRecord,HOMEW_TABLEW_TEST_TYPE,item_type);
            this->ui->tableWidget_TestXml->setItem(idxNewRecord,HOMEW_TABLEW_TEST_DILUTEUR,item_numSerieDiluteur);
            this->ui->tableWidget_TestXml->setItem(idxNewRecord,HOMEW_TABLEW_TEST_BOUTEILLE,item_numSerieBouteille);
            this->ui->tableWidget_TestXml->setItem(idxNewRecord,HOMEW_TABLEW_TEST_GZERO,item_numSerieGoz);
            this->ui->tableWidget_TestXml->setColumnHidden(HOMEW_TABLEW_TEST_ID_TEST,true);

            this->ui->tableWidget_TestXml->resizeColumnsToContents();
        }
    }
}

void HomeWidget::getListeRapports()
{
            //if(!this->m_bdHandler->isOpen())
            //   m_bdHandler->connexionBD();

            m_modelRapport = this->m_bdHandler->getTestRapportModel();

            m_modelRapport->setParent(this);
            this->ui->tableView_TestRapport->setModel(m_modelRapport);
            this->ui->tableView_TestRapport->resizeColumnsToContents();
            this->ui->tableView_TestRapport->setColumnHidden(HOMEW_TABVIEW_TEST_ID_TEST,true);
            this->ui->tableView_TestRapport->setColumnHidden(HOMEW_TABVIEW_TEST_ID_EQUIP,true);


            //m_Sysmodel.setRootPath(QDir::rootPath());
            //modele = model;

            m_itemModele = m_bdHandler->getItemModelListeRapport();
            this->ui->treeView->setModel(m_itemModele);


           //this->ui->treeView->setColumnHidden(HOMEW_TABVIEW_TEST_ID_TEST,true);
           //this->ui->treeView->setColumnHidden(HOMEW_TABVIEW_TEST_ID_EQUIP,true);

}




void HomeWidget::tableWidgetTestXmlIndexChanged(const QModelIndex & index)
{
    this->m_idxSelectionTest = index;
    this->ui->button_Executer->setEnabled(m_idxSelectionTest.isValid());
    this->ui->button_Modifier->setEnabled(m_idxSelectionTest.isValid());
    this->ui->button_Supprimer->setEnabled(m_idxSelectionTest.isValid());
}

void HomeWidget::tableViewTestRapportIndexChanged(const QModelIndex & index)
{
    this->m_idxSelectionRapport = index;
    this->ui->treeView->reset();
    this->ui->button_Afficher->setEnabled(m_idxSelectionRapport.isValid());
}

void HomeWidget::treeViewTestRapportIndexChanged(const QModelIndex & index)
{
    this->m_idxSelectionItemRapport = index;

        if (!m_itemModele->hasChildren(m_idxSelectionItemRapport.model()->index(0,0,m_idxSelectionItemRapport))){
            this->ui->button_Afficher->setEnabled(m_idxSelectionItemRapport.isValid());
        }
        else {
            this->ui->button_Afficher->setDisabled(m_idxSelectionItemRapport.isValid());
        }

}

void HomeWidget::buttonNouveauClicked()
{
    emit(this->creerTest());
}

void HomeWidget::buttonModifierClicked()
{
    if(ui->tableWidget_TestXml->currentIndex().isValid()) {
        QString fichierDescription = this->ui->tableWidget_TestXml->item(ui->tableWidget_TestXml->currentIndex().row(),HOMEW_TABLEW_TEST_FICHIER)->text();
        emit(this->modifierTest(fichierDescription));
    }
}

void HomeWidget::buttonExecuterClicked()
{
    if(ui->tableWidget_TestXml->currentIndex().isValid()) {
        ushort idTestXml = this->ui->tableWidget_TestXml->item(ui->tableWidget_TestXml->currentIndex().row(),HOMEW_TABLEW_TEST_ID_TEST)->text().toUInt();
        QString fichierDescription = this->ui->tableWidget_TestXml->item(ui->tableWidget_TestXml->currentIndex().row(),HOMEW_TABLEW_TEST_FICHIER)->text();
        emit(this->executerTest(idTestXml,fichierDescription));
    }
}

void HomeWidget::buttonSupprimerClicked() {
    if(!this->m_idxSelectionTest.isValid())
        return;
    int reponse = QMessageBox::question(this,"Supprimer un element?","Voulez vous vraiment supprimer le test selectionne?", QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::No)
        return;
    QString fichierDescription = this->ui->tableWidget_TestXml->item(m_idxSelectionTest.row(),HOMEW_TABLEW_TEST_FICHIER)->text();
    QFile fichierASupprimer(fichierDescription);
    fichierASupprimer.remove();
    this->m_modelTestXml->removeRow(this->m_idxSelectionTest.row());
    if(!this->m_modelTestXml->submitAll())
        qDebug()<<this->m_modelTestXml->lastError();
    this->ui->tableWidget_TestXml->removeRow(this->m_idxSelectionTest.row());
    this->ui->button_Executer->setEnabled(false);
    this->ui->button_Modifier->setEnabled(false);
    this->ui->button_Supprimer->setEnabled(false);
}

void HomeWidget::buttonAfficherClicked()
{
    if(ui->tableView_TestRapport->currentIndex().isValid()) {
         ushort idTest = this->m_modelRapport->record(m_idxSelectionRapport.row()).value(HOMEW_TABVIEW_TEST_ID_TEST).toUInt();
         ushort idAnalyseur = this->m_modelRapport->record(m_idxSelectionRapport.row()).value(HOMEW_TABVIEW_TEST_ID_EQUIP).toUInt();
         ushort typeTest = stringToTypeTest(this->m_modelRapport->record(m_idxSelectionRapport.row()).value(HOMEW_TABVIEW_TEST_TYPE_TEST).toString());
         emit(this->afficherRapport(idTest,idAnalyseur,typeTest));
    }
    else if (ui->treeView->currentIndex().isValid()){

        ushort typeTest = stringToTypeTest(m_idxSelectionItemRapport.parent().data().toString());
        ushort idTest = m_idxSelectionItemRapport.model()->index(0,0,m_idxSelectionItemRapport).data().toInt(); // modifie
        ushort idAnalyseur = m_idxSelectionItemRapport.model()->index(1,0,m_idxSelectionItemRapport).data().toInt(); // modifie

        emit(this->afficherRapport(idTest,idAnalyseur,typeTest));
     }
}

void HomeWidget::buttonSupprimerTestResultatClicked() {
    if(!m_idxSelectionRapport.isValid())
        return;
    int reponse = QMessageBox::question(this, "Supprimer un element", "êtes-vous sûr de vouloir effacer cet enregistrement?",
                  QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::No)
        return;

    if(!m_modelRapport->removeRow(m_idxSelectionRapport.row()))
        QMessageBox::critical(this,"Impossible de supprimer","Erreur de la suppression de l'enregistrement demande",QMessageBox::Ok);
    else {
        //m_modelRapport->();
        this->getListeRapports();
    }
}
