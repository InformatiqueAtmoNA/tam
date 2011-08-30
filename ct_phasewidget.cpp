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

    if(phase.isEmpty())
        this->m_phase.setNoPhase(noPhase);

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

    if(m_typeTest==TEMPS_REPONSE || m_typeTest==PERSO) {
        this->ui->gb_CritereArret->setVisible(true);
        this->ui->gb_CmdDebut_Phase->setVisible(true);
        if(m_phase.critereArretExiste()) {
            this->ui->ckb_CritereArret->setChecked(true);
            this->ui->timeEdit_TempsMaxPhase->setEnabled(true);
            this->ui->spinBox_NbCycleMesureArret->setValue(m_phase.getCritereArret_NbCyclesMesures());
            this->ui->spinBox_PourcentageArret->setValue(m_phase.getCritereArret_PourcentageStabilisation());
        }
        else
            this->ui->timeEdit_TempsMaxPhase->setEnabled(false);
    }
    else {
        this->ui->gb_CritereArret->setVisible(false);
        this->ui->gb_CmdDebut_Phase->setVisible(false);
    }
    ushort idMolecule = this->m_phase.getIdMolecule();
    if(idMolecule>0) {
        ushort idMolecule = m_phase.getIdMolecule();
        uint idConcentration = m_phase.getIdConcentration();

        QSqlRecord* concentrationRecord = this->m_bdHandler->getConcentrationRow(idConcentration);
        if(concentrationRecord == NULL) {
            QMessageBox msgBox;
            msgBox.setText("Problème lors du chargement de la concentration");
            msgBox.setInformativeText("La concentration de polluant enregistrée n'est pas pas valide. Veuillez en sélectionner une autre.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.setDefaultButton(QMessageBox::Ok);
            msgBox.exec();

            m_phase.supprimerPolluant();
        }
        else {
            QSqlRecord* moleculeRecord = this->m_bdHandler->getMoleculeRow(idMolecule);
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
    m_idConcentration = concentrationRecord->value(CONCENTRATION_ID_MOLECULE).toUInt();

    this->ui->lineEdit_Formule->setText(moleculeRecord->value(MOLECULE_FORMULE).toString());
    this->ui->lineEdit_Point->setText(concentrationRecord->value(CONCENTRATION_POINT).toString());
    this->ui->lineEdit_ConcReelle->setText(concentrationRecord->value(CONCENTRATION_REELLE).toString());
    this->ui->lineEdit_ConcOzone->setText(concentrationRecord->value(CONCENTRATION_OZONE).toString());
}

void ct_PhaseWidget::cb_CmdDebutPhaseIndexChanged(const int index) {
    switch(index) {
    case CMD_DEBUT_PHASE_NOCMD:
        this->m_phase.setCmdDebutPhase(NO_CMD);
        break;
    case CMD_DEBUT_PHASE_MODE_MESURE:
        this->m_phase.setCmdDebutPhase(MODE_MESURE);
        break;
    case CMD_DEBUT_PHASE_MODE_ETALON:
        this->m_phase.setCmdDebutPhase(MODE_ETALON);
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
        int idSelection = dlgConcentration.getIdSelection();
        QSqlRecord* concentrationRecord = m_bdHandler->getConcentrationRow(idSelection);
        Q_ASSERT_X(concentrationRecord,"buttonAjouterPolluantClicked()","concentration==NULL");
        uint idMolecule = concentrationRecord->value(CONCENTRATION_ID_MOLECULE).toUInt();
        QSqlRecord* moleculeRecord = m_bdHandler->getMoleculeRow(idMolecule);

        this->afficherPolluants(concentrationRecord,moleculeRecord);

        this->m_phase.ajouterPolluant(idMolecule,concentrationRecord->value(CONCENTRATION_ID).toUInt());
        delete concentrationRecord;
        delete moleculeRecord;
    }
}

void ct_PhaseWidget::buttonSupprimerPolluantClicked() {
    this->ui->lineEdit_Formule->clear();
    this->ui->lineEdit_Point->clear();
    this->ui->lineEdit_ConcReelle->clear();
    this->ui->lineEdit_ConcOzone->clear();
    this->m_phase.supprimerPolluant();
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
    this->ui->timeEdit_TempsMaxPhase->setEnabled(enableWidgets);
    this->m_phase.setCritereArretPrevu(enableWidgets);
    if(!enableWidgets) {
        this->ui->timeEdit_TempsMaxPhase->setTime(QTime(0,0));
    }
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
