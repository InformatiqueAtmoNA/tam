/*////////////////////////////////////////////////////
// \file creationtest.cpp
// \brief Classe d'interface graphique gérant la création des tests
// \author FOUQUART Christophe
// \version 1.0
// \date 01/04/2011
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
////////////////////////////////////////////////////*/

#include "creationtest.h"
#include "ui_creationtest.h"

CreationTest::CreationTest(QPointer<BdHandler> bdHandler,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CreationTest)
{
    ui->setupUi(this);

    this->m_bdHandler = bdHandler;
    m_nbPhases=0;
    m_etape=0;
    this->m_autoriserCreationPhase=true;

    connect(this->ui->button_AjouterPhase,SIGNAL(clicked()),this,SLOT(button_AjouterPhaseClicked()));
    connect(this->ui->button_Suivant,SIGNAL(clicked()),this,SLOT(button_SuivantClicked()));
    connect(this->ui->button_Precedent,SIGNAL(clicked()),this,SLOT(button_PrecedentClicked()));
    connect(this->ui->button_ChoixSystemeEtalon,SIGNAL(clicked()),this,SLOT(button_ChoixSystemeEtalonClicked()));
    connect(this->ui->button_InsererPhase,SIGNAL(clicked()),this,SLOT(button_InsererPhaseClicked()));
    connect(this->ui->button_SupprimerPhase,SIGNAL(clicked()),this,SLOT(button_SupprimerPhaseClicked()));
    connect(this->ui->button_Annuler,SIGNAL(clicked()),this,SLOT(button_AnnulerClicked()));
    connect(this->ui->button_Sauvegarder,SIGNAL(clicked()),this,SLOT(button_SauvegarderClicked()));
    connect(this->ui->cb_ChoixTypeTest,SIGNAL(currentIndexChanged(int)),this,SLOT(cb_ChoixTypeTestIndexChanged(int)));
    connect(this->ui->spinBox_TempsAcquisition,SIGNAL(valueChanged(int)),this,SLOT(spinBox_TempsAcquisitionValueChanged(int)));
    connect(this->ui->spinBox_nbCyclesMesures,SIGNAL(valueChanged(int)),this,SLOT(spinBox_nbCyclesMesuresValueChanged(int)));
    connect(this->ui->spinBox_NbCyclesPhases,SIGNAL(valueChanged(int)),this,SLOT(spinBox_nbCyclesPhasesValueChanged(int)));
    connect(this->ui->timeEdit_TempsAttenteEntreMesure,SIGNAL(timeChanged(QTime)),this,SLOT(timeEdit_TempsAttenteEntreMesureValueChanged(QTime)));
    connect(this->ui->timeEdit_TempsMaxTest,SIGNAL(timeChanged(QTime)),this,SLOT(timeEdit_TempsMaxTestValueChanged(QTime)));
    connect(this->ui->timeEdit_TempsStabilisation,SIGNAL(timeChanged(QTime)),this,SLOT(timeEdit_TempsStabilisation(QTime)));
    connect(this->ui->timeEdit_TempsMoyennageMesures,SIGNAL(timeChanged(QTime)),this,SLOT(timeEdit_TempsMoyennageMesuresValueChanged(QTime)));
    connect(this->ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabWidgetIndexChanged(int)));
    connect(this->ui->listWidget_Phases,SIGNAL(currentTextChanged(QString)),this,SLOT(listWidgetCurrentTextChanged(QString)));
    connect(this->ui->listWidget_Phases,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(listWidgetItemDoubleClicked(QListWidgetItem*)));
    connect(this->ui->listWidget_Phases,SIGNAL(currentRowChanged(int)),this,SLOT(listWidgetCurrentRowChanged(int)));
    connect(this->ui->lineEdit_NomTest,SIGNAL(textChanged(QString)),this,SLOT(lineEditNomTestTextChanged(QString)));

    this->ui->tabWidget->setCurrentIndex(0);
    this->ui->tab_Deroulement->setEnabled(false);
    this->ui->tab_NomTest->setEnabled(false);
    this->ui->button_Suivant->setEnabled(false);
    this->ui->button_InsererPhase->setEnabled(false);
    this->ui->button_SupprimerPhase->setEnabled(false);
    this->m_indexTypeTest = this->ui->cb_ChoixTypeTest->currentIndex();
}

CreationTest::~CreationTest()
{
    delete ui;
}

void CreationTest::afficherPhaseWidget(const ushort noPhase, bool readOnly) {
    qDebug()<<"----------------------------------------";
    qDebug()<<"call : CreationTest::afficherPhaseWidget("<<QString::number(noPhase)<<","<<readOnly<<")";
    if(!m_phaseWidget.isNull()) {
        qDebug()<<"Phase déjà affichée :";
        qDebug()<<m_phaseWidget->getPhase();
        if(m_phaseWidget->getPhase().getNoPhase() != noPhase) {
            m_phaseWidget->close();
            delete m_phaseWidget;
        }
    }
    if(m_phaseWidget.isNull()) {
        if(this->m_test.phaseExiste(noPhase)) {
            qDebug()<<"Phase existante à afficher :";
            qDebug()<<m_test.getPhase(noPhase);
            this->m_phaseWidget = new ct_PhaseWidget(this->m_test.getIdSystemeEtalon(),this->m_typeTest,this->m_bdHandler,this,noPhase,this->m_test.getPhase(noPhase));
        }
        else {
            PhaseConfig phaseConf;
            phaseConf.nbCyclesMesures = this->m_test.getNbCyclesMesureParPhase();
            phaseConf.tempsAttenteEntreMesure = this->m_test.getTempsAttenteEntreMesure();
            phaseConf.tempsMoyennageMesure= this->m_test.getTempsMoyennageMesure();
            phaseConf.tempsStabilisation = this->m_test.getTempsStabilisation();

            this->m_phaseWidget = new ct_PhaseWidget(this->m_test.getIdSystemeEtalon(),this->m_typeTest,this->m_bdHandler,this,noPhase,*(Phase::getPhaseFromConf(phaseConf).data()));
        }
        this->ui->h_Layout_EditPhase->insertWidget(1,this->m_phaseWidget);
        this->m_phaseWidget->show();
    }

    if(readOnly)
        this->m_phaseWidget->setReadOnly(true);
    else {
        this->m_phaseWidget->setReadOnly(false);
        connect(this->m_phaseWidget,SIGNAL(annulation()),this,SLOT(editionPhaseAnnulee()));
        connect(this->m_phaseWidget,SIGNAL(phaseValidee(Phase)),this,SLOT(editionPhaseValidee(Phase)));
    }

    this->ui->v_Layout_ButtonPhase->setEnabled(readOnly);
}

void CreationTest::incrementerNbPhases() {
    this->m_nbPhases++;
    if((this->m_typeTest==REPETABILITE_1 || this->m_typeTest==REPETABILITE_2) && this->m_nbPhases==2) {
        this->m_autoriserCreationPhase=false;
        this->ui->button_AjouterPhase->setEnabled(false);
        this->ui->button_Suivant->setEnabled(true);
        return;
    }
    if(this->m_nbPhases>0)
        this->ui->button_Suivant->setEnabled(true);
}

void CreationTest::decrementerNbPhases() {
    this->m_nbPhases--;
    if((this->m_typeTest==REPETABILITE_1 || this->m_typeTest==REPETABILITE_2) && this->m_nbPhases<2) {
        this->ui->button_AjouterPhase->setEnabled(true);
        this->m_autoriserCreationPhase=true;
        this->ui->button_Suivant->setEnabled(false);
        return;
    }
    if(this->m_nbPhases==0)
        this->ui->button_Suivant->setEnabled(true);
}

void CreationTest::button_InsererPhaseClicked() {
    int rowToInsert = this->ui->listWidget_Phases->currentRow();
    ushort noPhaseToInsert = this->ui->listWidget_Phases->item(rowToInsert)->text().toUShort();

    for(int i = rowToInsert;i<this->ui->listWidget_Phases->count();i++) {
        ushort noPhase = this->ui->listWidget_Phases->item(i)->text().toUShort();
        this->ui->listWidget_Phases->item(i)->setText(QString::number(++noPhase));
    }

    this->m_test.phaseInseree(noPhaseToInsert);
    this->m_phaseWidget->close();
    delete this->m_phaseWidget;
    this->afficherPhaseWidget(noPhaseToInsert);
}

void CreationTest::button_SupprimerPhaseClicked() {
    int row = this->ui->listWidget_Phases->currentRow();
    this->m_test.supprimerPhase(this->ui->listWidget_Phases->currentItem()->text().toUShort());
    QListWidgetItem* item = this->ui->listWidget_Phases->takeItem(row);
    this->ui->listWidget_Phases->removeItemWidget(item);
    delete item;
    for(int i =row;i<this->ui->listWidget_Phases->count();i++) {
        ushort noPhase = this->ui->listWidget_Phases->item(i)->text().toUShort();
        this->ui->listWidget_Phases->item(i)->setText(QString::number(noPhase-1));
    }
    this->decrementerNbPhases();
}

void CreationTest::button_ChoixSystemeEtalonClicked() {
    if(this->m_test.getIdSystemeEtalon()>0 && !this->m_test.isListePhasesEmpty()) {
        QMessageBox msgBox;
        msgBox.setText("Changement de système étalon");
        msgBox.setInformativeText("Changer de système d'étalonnage va réinitialiser le déroulement du test. Voulez-vous continuer?");
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();

        switch(ret) {
        case QMessageBox::Cancel:
            return;
        case QMessageBox::Ok:
            this->m_test.viderListePhase();
            this->ui->listWidget_Phases->clear();
            this->m_nbPhases=0;
            break;
        }
    }
    Dlg_Systeme_Etalon dlgSystemeEtalon(this,m_bdHandler,true);
    if(dlgSystemeEtalon.exec()) {
        this->m_test.setSystemeEtalon(dlgSystemeEtalon.getIdSelection());
        this->ui->lineEdit_Diluteur->setText(dlgSystemeEtalon.getNoSerieEtalon());
        this->ui->lineEdit_Bouteille->setText(dlgSystemeEtalon.getNoSerieBouteille());
        this->ui->lineEdit_GZero->setText(dlgSystemeEtalon.getNoSerieGZero());
        this->ui->button_Suivant->setEnabled(true);
    }
}

void CreationTest::button_AnnulerClicked() {
    this->close();
}

void CreationTest::button_SuivantClicked() {
    switch(this->ui->tabWidget->currentIndex()) {
    case 0 :
        this->ui->tab_Deroulement->setEnabled(true);
        this->ui->tab_Parametres->setEnabled(false);

        this->m_etape=1;
        this->ui->tabWidget->setCurrentIndex(1);
        break;
    case 1:
        this->ui->tab_NomTest->setEnabled(true);
        this->ui->tab_Deroulement->setEnabled(false);
        this->ui->tab_Parametres->setEnabled(false);

        this->m_etape=2;
        this->ui->tabWidget->setCurrentIndex(2);
    }
}

void CreationTest::button_PrecedentClicked() {
    switch(this->ui->tabWidget->currentIndex()) {
    case 1 :
        if(!this->m_phaseWidget.isNull()) {
            this->m_phaseWidget->close();
            delete this->m_phaseWidget;
        }
        this->ui->tab_Deroulement->setEnabled(false);
        this->ui->tab_Parametres->setEnabled(true);

        this->m_etape=0;
        this->ui->tabWidget->setCurrentIndex(0);
        break;
    case 2 :
        this->ui->tab_Deroulement->setEnabled(true);
        this->ui->tab_NomTest->setEnabled(false);

        this->m_etape=1;
        this->ui->tabWidget->setCurrentIndex(1);
        break;
    }
}

void CreationTest::button_SauvegarderClicked () {
    this->m_test.exportToXml(this->m_nomFichier);
}

void CreationTest::tabWidgetIndexChanged(const int index) {
    switch(index) {
    case 0:
        if(this->m_etape==1 || this->m_etape==2) {
            this->ui->tabWidget->setCurrentIndex(m_etape);
            return;
        }
        break;
    case 1:
        if(this->m_etape==0 || this->m_etape==2) {
            this->ui->tabWidget->setCurrentIndex(m_etape);
            return;
        }
//        this->ui->button_Suivant->setEnabled(true);
        this->ui->button_Sauvegarder->setEnabled(false);
        this->ui->button_Precedent->setEnabled(true);
        break;
    case 2:
        if(this->m_etape==0 || this->m_etape==1) {
            this->ui->tabWidget->setCurrentIndex(m_etape);
            return;
        }
        this->ui->button_Suivant->setEnabled(false);
        break;
    }
}

void CreationTest::editionPhaseAnnulee() {
    if(this->ui->listWidget_Phases->currentRow()>-1) {
        if(this->m_autoriserCreationPhase) this->ui->button_InsererPhase->setEnabled(true);
        this->ui->button_SupprimerPhase->setEnabled(true);
    }
    this->m_phaseWidget->close();
}

void CreationTest::editionPhaseValidee(const Phase & phase) {
    if(!this->m_test.phaseExiste(phase.getNoPhase())) {
        this->incrementerNbPhases();
        QListWidgetItem* item = new QListWidgetItem(QString::number(phase.getNoPhase()));
        this->ui->listWidget_Phases->addItem(item);
        this->ui->listWidget_Phases->sortItems();
        this->ui->v_Layout_ButtonPhase->setEnabled(true);
    }
    this->m_test.ajouterPhase(phase);
    this->m_phaseWidget->close();
}

void CreationTest::cb_ChoixTypeTestIndexChanged(const int index) {
    if(!this->m_test.isListePhasesEmpty() && m_indexTypeTest != index) {
        QMessageBox msgBox;
        msgBox.setText("Changement de type de test");
        msgBox.setInformativeText("Changer de type de test va réinitialiser le déroulement du test. Voulez-vous continuer?");
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        int ret = msgBox.exec();

        switch(ret) {
        case QMessageBox::Cancel:
            this->ui->cb_ChoixTypeTest->setCurrentIndex(this->m_indexTypeTest);
            return;
        case QMessageBox::Ok:
            this->m_test.viderListePhase();
            this->ui->listWidget_Phases->clear();
            this->m_nbPhases=0;
            break;
        }
    }

    switch(index) {
    case 0:
        this->m_typeTest=REPETABILITE_1;
        break;
    case 1:
        this->m_typeTest=REPETABILITE_2;
        this->ui->spinBox_NbCyclesPhases->setValue(1);
        break;
    case 2:
        this->m_typeTest=LINEARITE;
        this->ui->spinBox_nbCyclesMesures->setMinimum(5);
        break;
    case 3:
        this->m_typeTest=TEMP_REPONSE;
        break;
    case 4:
        this->m_typeTest=RENDEMENT_FOUR;
        this->ui->spinBox_nbCyclesMesures->setMinimum(0);
        this->ui->spinBox_nbCyclesMesures->setValue(4);
        break;
    case 5:
        this->m_typeTest=PERSO;
        break;
    }
    this->m_indexTypeTest = index;
}

void CreationTest::listWidgetCurrentTextChanged(const QString & text) {
    ushort noPhase = text.toUShort();

    this->afficherPhaseWidget(noPhase,true);
    qDebug()<<this->m_test.getPhase(noPhase);
}

void CreationTest::listWidgetItemDoubleClicked(QListWidgetItem* item) {
    this->ui->button_InsererPhase->setEnabled(false);
    this->ui->button_SupprimerPhase->setEnabled(false);
    ushort noPhase = item->text().toUShort();
    this->afficherPhaseWidget(noPhase);
}

void CreationTest::listWidgetCurrentRowChanged(const int row) {
    if(row==-1) {
        this->ui->button_InsererPhase->setEnabled(false);
        this->ui->button_SupprimerPhase->setEnabled(false);
    }
    else {
        if(this->m_autoriserCreationPhase) this->ui->button_InsererPhase->setEnabled(true);
        this->ui->button_SupprimerPhase->setEnabled(true);
    }
}

void CreationTest::lineEditNomTestTextChanged(const QString nomTest) {
    this->m_nomFichier = "./"+nomTest+".xml";
    if((this->m_typeTest==REPETABILITE_1 || this->m_typeTest==REPETABILITE_2) && this->m_nbPhases<2)
        return;
    this->ui->button_Sauvegarder->setEnabled(!nomTest.isEmpty());
}
