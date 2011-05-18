/*////////////////////////////////////////////////////
// \file ct_phasewidget.cpp
// \brief Classe de widget gérant la création des phases de test
// \author FOUQUART Christophe
// \version 1.0
// \date 22/04/2011
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

#include "ct_phasewidget.h"
#include "ui_ct_phasewidget.h"

ct_PhaseWidget::ct_PhaseWidget(const int idSystemeEtalon,const TypeTest typeTest, const QPointer<BdHandler> bdHandler,
                               QWidget *parent, const int noPhase,const Phase & phase) :
    QWidget(parent),
    ui(new Ui::ct_PhaseWidget)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose,true);

    this->m_phase = phase;
    this->m_phaseOriginale = phase;
    this->m_idSystemeEtalon = idSystemeEtalon;
    this->m_bdHandler = bdHandler;
    this->m_typeTest = typeTest;

    this->ui->tableWidget_Polluants->setColumnHidden(PHASEW_TABLEW_POLLUANTS_IDMOLECULE,true);
    this->ui->tableWidget_Polluants->resizeColumnsToContents();
    if(phase.isEmpty())
        this->m_phase.setNoPhase(noPhase);

    connect(this->ui->tableWidget_Polluants,SIGNAL(clicked(QModelIndex)),this,SLOT(tableViewPolluantsIndexChanged(QModelIndex)));
    connect(this->ui->button_AjouterPolluant,SIGNAL(clicked()),this,SLOT(buttonAjouterPolluantClicked()));
    connect(this->ui->button_RetirerPolluant,SIGNAL(clicked()),this,SLOT(buttonSupprimerPolluantClicked()));
    connect(this->ui->button_Annuler,SIGNAL(clicked()),this,SLOT(buttonAnnulerClicked()));
    connect(this->ui->button_Valider,SIGNAL(clicked()),this,SLOT(buttonValiderClicked()));
    connect(this->ui->ckb_CritereArret,SIGNAL(stateChanged(int)),this,SLOT(ckb_CritereArretStateChanged(int)));
    connect(this->ui->spinBox_NbCycleMesureArret,SIGNAL(valueChanged(int)),this,SLOT(spinBox_NbCycleMesureArretValueChanged(int)));
    connect(this->ui->spinBox_nbCyclesMesures,SIGNAL(valueChanged(int)),this,SLOT(spinBoxNbCyclesMesuresValueChanged(int)));
    connect(this->ui->spinBox_PourcentageArret,SIGNAL(valueChanged(int)),this,SLOT(spinBoxPourcentageArretValueChanged(int)));
    connect(this->ui->cb_CmdDebutPhase,SIGNAL(currentIndexChanged(int)),this,SLOT(cb_CmdDebutPhaseIndexChanged(int)));
    connect(this->ui->timeEdit_TempsMaxPhase,SIGNAL(timeChanged(QTime)),this,SLOT(timeEditTempsMaxPhaseValueChanged(QTime)));
    connect(this->ui->timeEdit_TempsStabilisation,SIGNAL(timeChanged(QTime)),this,SLOT(timeEditTempsStabilisationValueChanged(QTime)));
    connect(this->ui->timeEdit_TempsMoyennageMesures,SIGNAL(timeChanged(QTime)),this,SLOT(timeEditTempsMoyennageMesuresValueChanged(QTime)));
    connect(this->ui->timeEdit_AttenteEntreMesures,SIGNAL(timeChanged(QTime)),this,SLOT(timeEditAttenteEntreMesuresValueChanged(QTime)));

    afficherPhase();
}

ct_PhaseWidget::~ct_PhaseWidget()
{
    delete ui;
}

void ct_PhaseWidget::afficherPhase()
{
    this->ui->lineEdit_NoPhase->setText(QString::number(m_phase.getNoPhase()));
    this->ui->spinBox_nbCyclesMesures->setValue(m_phase.getNbCyclesMesures());
    this->ui->timeEdit_AttenteEntreMesures->setTime(m_phase.getTpsAttenteEntreMesure());
    this->ui->timeEdit_TempsMaxPhase->setTime(m_phase.getTpsMaxPhase());
    this->ui->timeEdit_TempsMoyennageMesures->setTime(m_phase.getTpsMoyennageMesure());
    this->ui->timeEdit_TempsStabilisation->setTime(m_phase.getTpsStabilisation());

    if(m_typeTest==TEMP_REPONSE || m_typeTest==PERSO) {
        this->ui->gb_CritereArret->setVisible(true);
        this->ui->gb_CmdDebut_Phase->setVisible(true);
        if(m_phase.critereArretExiste()) {
            this->ui->ckb_CritereArret->setChecked(true);
            this->ui->spinBox_NbCycleMesureArret->setValue(m_phase.getCritereArret_NbCyclesMesures());
            this->ui->spinBox_PourcentageArret->setValue(m_phase.getCritereArret_PourcentageStabilisation());
        }
    }
    else {
        this->ui->gb_CritereArret->setVisible(false);
        this->ui->gb_CmdDebut_Phase->setVisible(false);
    }
    if(this->m_phase.getListePolluants().count()>0) {
        QMapIterator<ushort,ushort> iterator(this->m_phase.getListePolluants());
        while(iterator.hasNext()) {
            iterator.next();
            ushort idMolecule = iterator.key();
            ushort pointGaz = iterator.value();
            QSqlRecord* concentrationRecord = this->m_bdHandler->getConcentrationRow(this->m_idSystemeEtalon,idMolecule,pointGaz);
            Q_ASSERT_X(concentrationRecord,"afficherPhase()","concentration record==NULL");
            QSqlRecord* moleculeRecord = this->m_bdHandler->getMoleculeRow(idMolecule);
            Q_ASSERT_X(moleculeRecord,"afficherPhase()","molecule record==NULL");
            this->afficherPolluants(concentrationRecord,moleculeRecord);
            delete concentrationRecord;
            delete moleculeRecord;
        }
    }

    switch(m_phase.getCmdDebutPhase()) {
    case NO_CMD:
        this->ui->cb_CmdDebutPhase->setCurrentIndex(CMD_DEBUT_PHASE_NOCMD);
        break;
    case MODE_MESURE:
        this->ui->cb_CmdDebutPhase->setCurrentIndex(CMD_DEBUT_PHASE_MODE_MESURE);
        break;
    case MODE_ETALON:
        this->ui->cb_CmdDebutPhase->setCurrentIndex(CMD_DEBUT_PHASE_MODE_ETALON);
        break;
    default:
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Erreur!");
        msgBox.setText("La commande de début de phase n'est pas programmée");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        break;

    }
}

void ct_PhaseWidget::afficherPolluants(QSqlRecord* concentrationRecord,QSqlRecord* moleculeRecord) {
    QTableWidgetItem* item_IdMolecule = new QTableWidgetItem(concentrationRecord->value(CONCENTRATION_ID_MOLECULE).toString());
    QTableWidgetItem* item_MoleculeFormule = new QTableWidgetItem(moleculeRecord->value(MOLECULE_FORMULE).toString());
    QTableWidgetItem* item_CodeMolecule = new QTableWidgetItem(moleculeRecord->value(MOLECULE_CODE).toString());
    QTableWidgetItem* item_ConcentrationPoint = new QTableWidgetItem(concentrationRecord->value(CONCENTRATION_POINT).toString());
    QTableWidgetItem* item_ConcentrationReelle = new QTableWidgetItem(concentrationRecord->value(CONCENTRATION_REELLE).toString());
    QTableWidgetItem* item_ConcentrationOzone = new QTableWidgetItem(concentrationRecord->value(CONCENTRATION_OZONE).toString());

    uint idxNewRecord = this->ui->tableWidget_Polluants->rowCount();
    this->ui->tableWidget_Polluants->insertRow(idxNewRecord);
    this->ui->tableWidget_Polluants->setItem(idxNewRecord,PHASEW_TABLEW_POLLUANTS_IDMOLECULE,item_IdMolecule);
    this->ui->tableWidget_Polluants->setItem(idxNewRecord,PHASEW_TABLEW_POLLUANTS_CODE,item_CodeMolecule);
    this->ui->tableWidget_Polluants->setItem(idxNewRecord,PHASEW_TABLEW_POLLUANTS_FORMULE,item_MoleculeFormule);
    this->ui->tableWidget_Polluants->setItem(idxNewRecord,PHASEW_TABLEW_POLLUANTS_POINT,item_ConcentrationPoint);
    this->ui->tableWidget_Polluants->setItem(idxNewRecord,PHASEW_TABLEW_POLLUANTS_REELLE,item_ConcentrationReelle);
    this->ui->tableWidget_Polluants->setItem(idxNewRecord,PHASEW_TABLEW_POLLUANTS_OZONE,item_ConcentrationOzone);
}

void ct_PhaseWidget::cb_CmdDebutPhaseIndexChanged(const int index) {
    switch(index) {
    case CMD_DEBUT_PHASE_NOCMD:
        this->m_phase.setCmdFinTs(NO_CMD);
        break;
    case CMD_DEBUT_PHASE_MODE_MESURE:
        this->m_phase.setCmdFinTs(MODE_MESURE);
        break;
    case CMD_DEBUT_PHASE_MODE_ETALON:
        this->m_phase.setCmdFinTs(MODE_ETALON);
        break;
    default:
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setWindowTitle("Erreur!");
        msgBox.setText("La commande sélectionnée n'est pas programmée");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        break;
    }
}

void ct_PhaseWidget::buttonAjouterPolluantClicked()
{
    if(!m_bdHandler->isOpen())
        m_bdHandler->connexionBD();
    bool filtrerRdf = (m_typeTest==RENDEMENT_FOUR);
    Dlg_Concentration dlgConcentration(this,m_bdHandler,m_idSystemeEtalon,filtrerRdf,true);
    int result = dlgConcentration.exec();

    if(result) {
        QSqlRecord* concentrationRecord = m_bdHandler->getConcentrationRow(dlgConcentration.getIdSelection());
        Q_ASSERT_X(concentrationRecord,"buttonAjouterPolluantClicked()","concentration==NULL");
        uint idMolecule = concentrationRecord->value(CONCENTRATION_ID_MOLECULE).toUInt();
        // Si le polluant est déja présent dans la liste
        if(!m_phase.getListePolluants().isEmpty() && m_phase.polluantPresent(idMolecule)) {
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setWindowTitle("Attention");
            msgBox.setText("Une concentration est déja programmée pour ce polluant");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
            return;
        }
        QSqlRecord* moleculeRecord = m_bdHandler->getMoleculeRow(idMolecule);

        this->afficherPolluants(concentrationRecord,moleculeRecord);

        uint pointGaz = concentrationRecord->value(CONCENTRATION_POINT).toUInt();
        this->m_phase.ajouterPolluant(idMolecule,pointGaz);
        delete concentrationRecord;
        delete moleculeRecord;
    }
}

void ct_PhaseWidget::buttonSupprimerPolluantClicked() {
    if(m_indexListePolluant.isValid()) {
        int row = m_indexListePolluant.row();
        uint idMolecule = this->ui->tableWidget_Polluants->item(row,PHASEW_TABLEW_POLLUANTS_IDMOLECULE)->text().toUInt();
        this->ui->tableWidget_Polluants->removeRow(row);
        this->m_phase.supprimerPolluant(idMolecule);
    }
}

void ct_PhaseWidget::tableViewPolluantsIndexChanged(const QModelIndex index) {
    this->ui->button_RetirerPolluant->setEnabled(index.isValid());
    this->m_indexListePolluant = index;
}


void ct_PhaseWidget::ckb_CritereArretStateChanged(const int state) {
    bool enableWidgets = (state == Qt::Checked);
    this->ui->spinBox_NbCycleMesureArret->setEnabled(enableWidgets);
    this->ui->spinBox_PourcentageArret->setEnabled(enableWidgets);
    this->ui->label_CritereArret1->setEnabled(enableWidgets);
    this->ui->label_CritereArret2->setEnabled(enableWidgets);
    this->ui->label_CritereArret3->setEnabled(enableWidgets);
    this->ui->label_CritereArret4->setEnabled(enableWidgets);
    this->m_phase.setCritereArretPrevu(enableWidgets);
}

void ct_PhaseWidget::setReadOnly(bool readOnly) {
    this->m_isReadOnly = readOnly;
    this->ui->button_AjouterPolluant->setDisabled(readOnly);
    this->ui->button_Annuler->setDisabled(readOnly);
    this->ui->button_RetirerPolluant->setDisabled(readOnly);
    this->ui->button_Valider->setDisabled(readOnly);
    this->ui->cb_CmdDebutPhase->setDisabled(readOnly);
    this->ui->ckb_CritereArret->setDisabled(readOnly);
    this->ui->spinBox_NbCycleMesureArret->setReadOnly(readOnly);
    this->ui->spinBox_nbCyclesMesures->setReadOnly(readOnly);
    this->ui->spinBox_PourcentageArret->setReadOnly(readOnly);
    this->ui->timeEdit_AttenteEntreMesures->setReadOnly(readOnly);
    this->ui->timeEdit_TempsMaxPhase->setReadOnly(readOnly);
    this->ui->timeEdit_TempsMoyennageMesures->setReadOnly(readOnly);
    this->ui->timeEdit_TempsStabilisation->setReadOnly(readOnly);
}
