/*////////////////////////////////////////////////////////////
// \file dlg_concentration.cpp
// \brief Classe d'interface graphique  gérant la table Cocentration
// \author FOUQUART Christophe
// \version 1.0
// \date 20/04/2011
//
// TAM - Tests Automatiques Métrologiques
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

#include "dlg_concentration.h"
#include "ui_dlg_concentration.h"

Dlg_Concentration::Dlg_Concentration(QWidget *parent,const QPointer<BdHandler> bdHandler,
                                     const int idSystemeEtalon, const bool filtrerRdf, const bool returnSelection,
                                     const int indexSelection) :
      QDialog(parent),
      ui(new Ui::Dlg_Concentration)
{
    ui->setupUi(this);
    this->setWindowTitle("Concentration");

    this->m_bdHandler = bdHandler;
    this->m_returnSelection = returnSelection;
    this->m_idSystemeEtalon = idSystemeEtalon;
    this->m_filtrerRdf = filtrerRdf;

    this->ui->tabWidget->setCurrentIndex(0);
    m_pageConcentrationAssociee = this->ui->tabWidget->widget(1);

    this->peuplerCbPolluant();
    this->peuplerTable();
    this->initialiserChamps();

    connect(this->ui->button_Supprimer,SIGNAL(clicked()),
          this,SLOT(buttonSupprimerClicked()));
    connect(this->ui->button_Modifier,SIGNAL(clicked()),
          this,SLOT(buttonModifierClicked()));
    connect(this->ui->button_Ajouter,SIGNAL(clicked()),
          this,SLOT(buttonAjouterClicked()));
    connect(this->ui->button_Fermer,SIGNAL(clicked()),
          this,SLOT(buttonFermerClicked()));
    connect(this->ui->button_Selectionner,SIGNAL(clicked()),
          this,SLOT(buttonSelectionnerClicked()));
    connect(this->ui->button_Annuler,SIGNAL(clicked()),
          this,SLOT(initialiserChamps()));
    connect(this->ui->button_Valider,SIGNAL(clicked()),
          this,SLOT(buttonValiderClicked()));
    connect(this->ui->comboBox_SelectPolluant,SIGNAL(currentIndexChanged(int)),
          this,SLOT(cbSelectPolluantIndexChanged(int)));

    connect(this->ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(tabWidgetIndexChanged(int)));
    connect(this->ui->button_Ajouter_ConcAssociee,SIGNAL(clicked()),this,SLOT(buttonAjouterConcAssocieeClicked()));
    connect(this->ui->button_Annuler_ConcAssociee,SIGNAL(clicked()),this,SLOT(initialiserChampsConcAssociee()));
    connect(this->ui->button_Modifier_ConcAssociee,SIGNAL(clicked()),this,SLOT(buttonModifierConcAssocieeClicked()));
    connect(this->ui->button_Supprimer_ConcAssociee,SIGNAL(clicked()),this,SLOT(buttonSupprimerConcAssocieeClicked()));
    connect(this->ui->button_Valider_ConcAssociee,SIGNAL(clicked()),this,SLOT(buttonValiderConcAssocieeClicked()));
    connect(this->ui->comboBox_ConcAssociee_Polluant,SIGNAL(currentIndexChanged(int)),
          this,SLOT(cbConcAssocieeSelectPolluantIndexChanged(int)));

    if(indexSelection > -1) {
        this->ui->tableView->selectRow(indexSelection);
    }
}

Dlg_Concentration::~Dlg_Concentration()
{
    delete ui;
}

void Dlg_Concentration::peuplerCbPolluant() {
    if(!this->m_bdHandler->isOpen())
        m_bdHandler->connexionBD();

    m_modelPolluants = m_bdHandler->getPolluantsParSystemeEtalon(m_idSystemeEtalon,m_filtrerRdf);

    if(m_modelPolluants->rowCount()==0) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        if(m_filtrerRdf)
            msgBox.setText(tr("Aucun polluant de type NO/NO2/NOX n'est associé aux équipement du système d'étalonnage"));
        else
            msgBox.setText(tr("Aucun polluant n'est associé aux équipement du système d'étalonnage"));
        msgBox.setInformativeText(tr("Vérifiez la configuration des équipements"));
        msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Yes);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Cancel:
            this->reject();
            break;
        case QMessageBox::Yes:
            Dlg_Equipement dlgEquipement(this,this->m_bdHandler);
            dlgEquipement.exec();
            delete m_modelPolluants;
            peuplerCbPolluant();
            return;
        }
    }

    this->ui->comboBox_SelectPolluant->setModel(m_modelPolluants);
    this->ui->comboBox_SelectPolluant->setModelColumn(POLLUANT_BY_SYS_ETALON_FORMULE);
    this->ui->comboBox_SelectPolluant->setCurrentIndex(0);
    this->m_idPolluant = m_modelPolluants->record(0).value(POLLUANT_BY_SYS_ETALON_ID).toUInt();
}

void Dlg_Concentration::peuplerTable()
{
    if(!this->m_bdHandler->isOpen())
        m_bdHandler->connexionBD();

    m_model = m_bdHandler->getConcentrationModel(m_idSystemeEtalon,m_idPolluant);
    m_model->setParent(this);
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    this->ui->tableView->setModel(m_model);
    this->ui->tableView->setColumnHidden(CONCENTRATION_ID, true);
    this->ui->tableView->setColumnHidden(CONCENTRATION_SYS_ETALON, true);
    this->ui->tableView->setColumnHidden(CONCENTRATION_ID_MOLECULE, true);
    this->ui->tableView->resizeColumnsToContents();
    this->ui->tableView->setItemDelegate(new QSqlRelationalDelegate(this));

    connect(this->ui->tableView,
          SIGNAL(clicked(QModelIndex)),
          this,SLOT(tableViewSelectedItemChanged(const QModelIndex &)));
    connect(this->ui->tableView,SIGNAL(doubleClicked(QModelIndex)),
          this,SLOT(tableViewItemDoubleClicked(QModelIndex)));
}

void Dlg_Concentration::afficherFormulaire()
{
    this->ui->gb_edit_champs->setVisible(true);
    this->ui->button_Ajouter->setEnabled(false);
    this->ui->button_Supprimer->setEnabled(false);
    this->ui->button_Fermer->setEnabled(false);
    this->ui->button_Valider->setDefault(true);
    this->ui->button_Selectionner->setEnabled(false);
    this->ui->button_Modifier->setEnabled(false);

    this->ui->spinBox_PointGaz->setFocus();

}

void Dlg_Concentration::afficherFormulaireConcAssociee()
{
    this->ui->gb_edit_champs_ConcAssociee->setVisible(true);
    this->ui->button_Ajouter_ConcAssociee->setEnabled(false);
    this->ui->button_Supprimer_ConcAssociee->setEnabled(false);
    this->ui->button_Valider_ConcAssociee->setDefault(true);
    this->ui->button_Modifier_ConcAssociee->setEnabled(false);
}

void Dlg_Concentration::initialiserChamps()
{
    this->m_modifEnCours=false;
    this->ui->comboBox_SelectPolluant->setEnabled(true);
    this->ui->gb_edit_champs->setVisible(false);
    this->ui->tableView->setEnabled(true);
    this->ui->tabWidget->removeTab(1);

    this->ui->button_Modifier->setEnabled(false);
    this->ui->button_Supprimer->setEnabled(false);
    this->ui->button_Ajouter->setEnabled(true);
    this->ui->button_Fermer->setEnabled(true);
    if(!this->m_returnSelection)
      this->ui->button_Selectionner->setVisible(false);
    else
        this->ui->button_Selectionner->setVisible(true);

    this->ui->spinBox_PointGaz->clear();
    this->ui->doubleSpinBox_ConcReelle->clear();
    this->ui->spinBox_ConcO3->clear();
}

void Dlg_Concentration::initialiserChampsConcAssociee()
{
    this->m_modifConcAssocieeEnCours=false;
    this->ui->gb_edit_champs_ConcAssociee->setVisible(false);
    this->ui->tableView_ConcAssociee->setEnabled(true);

    this->ui->button_Modifier_ConcAssociee->setEnabled(false);
    this->ui->button_Supprimer_ConcAssociee->setEnabled(false);
    this->ui->button_Ajouter_ConcAssociee->setEnabled(true);

    this->ui->doubleSpinBox_ConcAssociee_Concentration->clear();
}

void Dlg_Concentration::setGbEditChampsReadOnly(const bool readOnly)
{
    this->ui->spinBox_PointGaz->setReadOnly(readOnly);
    this->ui->doubleSpinBox_ConcReelle->setReadOnly(readOnly);
    this->ui->spinBox_ConcO3->setReadOnly(readOnly);
    this->ui->button_Annuler->setEnabled(!readOnly);
    this->ui->button_Valider->setEnabled(!readOnly);
}

void Dlg_Concentration::setGbEditChampsConcAssocieeReadOnly(const bool readOnly)
{
    this->ui->comboBox_ConcAssociee_Polluant->setEnabled(!readOnly);
    this->ui->doubleSpinBox_ConcAssociee_Concentration->setReadOnly(readOnly);
    this->ui->button_Annuler_ConcAssociee->setEnabled(!readOnly);
    this->ui->button_Valider_ConcAssociee->setEnabled(!readOnly);
}

void Dlg_Concentration::editCurrentRow(const QModelIndex& idxSelection)
{
    if(!idxSelection.isValid())
        return;

    this->m_modifEnCours= true;
    this->ui->tableView->setEnabled(false);
    this->ui->v_Layout_Buttons->setEnabled(false);
    this->ui->comboBox_SelectPolluant->setEnabled(false);
    this->ui->gb_edit_champs->setVisible(true);
    this->setGbEditChampsReadOnly(false);

    QSqlRecord record = m_model->record(idxSelection.row());
    this->ui->spinBox_PointGaz->setValue(this->m_model->record(idxSelection.row()).value(CONCENTRATION_POINT).toUInt());
    this->ui->doubleSpinBox_ConcReelle->setValue(this->m_model->record(idxSelection.row()).value(CONCENTRATION_REELLE).toDouble());
    this->ui->spinBox_ConcO3->setValue(this->m_model->record(idxSelection.row()).value(CONCENTRATION_OZONE).toUInt());
}

void Dlg_Concentration::editConcAssocieeCurrentRow(const QModelIndex & idxSelection)
{
    if(!idxSelection.isValid())
        return;

    this->m_modifConcAssocieeEnCours = true;
    this->ui->gb_edit_champs_ConcAssociee->setVisible(true);
    this->ui->tableView_ConcAssociee->setEnabled(false);
    this->ui->button_Ajouter_ConcAssociee->setEnabled(false);
    this->setGbEditChampsConcAssocieeReadOnly(false);

    QSqlRecord record = m_modelConcentrationAssociee->record(idxSelection.row());
    QString formuleMolecule = record.value(CONC_ASSOCIEE_FORMULE).toString();
    this->ui->comboBox_ConcAssociee_Polluant->setCurrentIndex(this->ui->comboBox_ConcAssociee_Polluant->findText(formuleMolecule));
    this->ui->doubleSpinBox_ConcAssociee_Concentration->setValue(record.value(CONC_ASSOCIEE_CONCENTRATION).toDouble());
}

void Dlg_Concentration::tableViewSelectedItemChanged(const QModelIndex & idxSelection)
{
    qDebug()<<"---------------------------------------------";
    qDebug()<<"call Dlg_Concentration::tableViewSelectedItemChanged()";
    qDebug()<<"selected : "<<idxSelection.row();
    this->m_indexSelection = idxSelection;
    if(this->m_indexSelection.isValid()) {
        if(this->m_returnSelection)
            this->ui->button_Selectionner->setEnabled(true);
        this->ui->tabWidget->addTab(m_pageConcentrationAssociee,"Concentrations associées");
        this->ui->gb_edit_champs->setVisible(true);
        this->setGbEditChampsReadOnly(true);
        this->ui->button_Modifier->setEnabled(true);
        this->ui->button_Supprimer->setEnabled(true);

        this->ui->spinBox_PointGaz->setValue(this->m_model->record(idxSelection.row()).value(CONCENTRATION_POINT).toUInt());
        this->ui->doubleSpinBox_ConcReelle->setValue(this->m_model->record(idxSelection.row()).value(CONCENTRATION_REELLE).toDouble());
        this->ui->spinBox_ConcO3->setValue(this->m_model->record(idxSelection.row()).value(CONCENTRATION_OZONE).toUInt());
        connect(this->ui->tableView_ConcAssociee,SIGNAL(clicked(QModelIndex)),this,SLOT(tableViewConcAssocieeItemChanged(QModelIndex)));
    }
    else {
        this->ui->button_Modifier->setEnabled(false);
        this->ui->button_Supprimer->setEnabled(false);
        if(this->ui->gb_edit_champs->isVisible()) {
            this->ui->gb_edit_champs->setVisible(false);
            this->setGbEditChampsReadOnly(false);
        }
        this->ui->tabWidget->removeTab(1);
    }
}

void Dlg_Concentration::tableViewConcAssocieeItemChanged(const QModelIndex & idxSelection)
{
    qDebug()<<"---------------------------------------------";
    qDebug()<<"call Dlg_Concentration::tableViewConcAssocieeIntemChanged()";
    qDebug()<<"selected : "<<idxSelection.row();
    this->m_indexConcAssocieSelection = idxSelection;

    if(this->m_indexConcAssocieSelection.isValid()) {
        this->ui->gb_edit_champs_ConcAssociee->setVisible(true);
        this->ui->button_Modifier_ConcAssociee->setEnabled(true);
        this->ui->button_Supprimer_ConcAssociee->setEnabled(true);

        QSqlRecord record = m_modelConcentrationAssociee->record(m_indexConcAssocieSelection.row());
        QString formuleMolecule = record.value(CONC_ASSOCIEE_FORMULE).toString();

        qDebug()<<formuleMolecule<<"  "<<this->ui->comboBox_ConcAssociee_Polluant->findText(formuleMolecule);
        this->ui->comboBox_ConcAssociee_Polluant->setCurrentIndex(this->ui->comboBox_ConcAssociee_Polluant->findText(formuleMolecule));
        this->ui->doubleSpinBox_ConcAssociee_Concentration->setValue(record.value(CONC_ASSOCIEE_CONCENTRATION).toDouble());
        this->setGbEditChampsConcAssocieeReadOnly(true);
    }
    else {
        if(this->ui->gb_edit_champs_ConcAssociee->isVisible()) {
            this->ui->gb_edit_champs_ConcAssociee->setVisible(false);
            this->setGbEditChampsConcAssocieeReadOnly(false);
        }
        this->ui->button_Modifier_ConcAssociee->setEnabled(false);
        this->ui->button_Supprimer_ConcAssociee->setEnabled(false);
    }
}

void Dlg_Concentration::buttonAjouterClicked() {
    this->initialiserChamps();
    this->setGbEditChampsReadOnly(false);
    this->afficherFormulaire();
}


void Dlg_Concentration::buttonSupprimerClicked()
{
    if(!m_indexSelection.isValid())
       return;
    int reponse = QMessageBox::question(this, "Supprimer un élément", "êtes-vous sûr de vouloir effacer cet enregistrement?",
                QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::No)
       return;

    if(!m_model->removeRow(m_indexSelection.row()))
       QMessageBox::critical(this,"Impossible de supprimer","Erreur de la suppression de l'enregistrement demandé.",QMessageBox::Ok);
    else {
       m_model->submitAll();
       this->initialiserChamps();
    }
}

/*void Dlg_Concentration::buttonValiderClicked()
{
    int row = m_model->rowCount();
    if(row>0) row-=1;

    ushort idConcentration = m_model->record(m_indexSelection.row()).value(CONCENTRATION_ID).toUInt();

    m_model->insertRow(row);
    m_model->setData(m_model->index(row,CONCENTRATION_SYS_ETALON),QVariant::fromValue(this->m_idSystemeEtalon));
    m_model->setData(m_model->index(row,CONCENTRATION_ID_MOLECULE),QVariant::fromValue(this->m_idPolluant));
    m_model->setData(m_model->index(row,CONCENTRATION_POINT),QVariant::fromValue(this->ui->spinBox_PointGaz->value()));
    m_model->setData(m_model->index(row,CONCENTRATION_REELLE),QVariant::fromValue(this->ui->doubleSpinBox_ConcReelle->value()));
    m_model->setData(m_model->index(row,CONCENTRATION_OZONE),QVariant::fromValue(this->ui->spinBox_ConcO3->value()));

    m_model->submitAll();

    if(m_modifEnCours) {
        QSharedPointer<QSqlRelationalTableModel> modelConcAssociee = m_bdHandler->getConcentrationAssocieeModel(idConcentration).data();
    }

    this->initialiserChamps();
}*/

void Dlg_Concentration::buttonValiderClicked()
{
    int row;
    if(m_modifEnCours) {
        row = this->ui->tableView->currentIndex().row();
    }
    else {
        row = m_model->rowCount();
        if(row>0) row-=1;
        m_model->insertRow(row);
    }
    m_model->setData(m_model->index(row,CONCENTRATION_SYS_ETALON),QVariant::fromValue(this->m_idSystemeEtalon));
    m_model->setData(m_model->index(row,CONCENTRATION_ID_MOLECULE),QVariant::fromValue(this->m_idPolluant));
    m_model->setData(m_model->index(row,CONCENTRATION_POINT),QVariant::fromValue(this->ui->spinBox_PointGaz->value()));
    m_model->setData(m_model->index(row,CONCENTRATION_REELLE),QVariant::fromValue(this->ui->doubleSpinBox_ConcReelle->value()));
    m_model->setData(m_model->index(row,CONCENTRATION_OZONE),QVariant::fromValue(this->ui->spinBox_ConcO3->value()));

    m_model->submitAll();

    this->initialiserChamps();
}


void Dlg_Concentration::buttonModifierClicked()
{
    this->editCurrentRow(this->ui->tableView->currentIndex());
}

void Dlg_Concentration::cbSelectPolluantIndexChanged(const int index)
{
    this->m_idPolluant = m_modelPolluants->record(index).value(POLLUANT_BY_SYS_ETALON_ID).toUInt();
    delete m_model;
    this->ui->button_Selectionner->setEnabled(false);
    this->ui->button_Modifier->setEnabled(false);
    this->ui->button_Supprimer->setEnabled(false);
    this->ui->gb_edit_champs->setVisible(false);
    this->peuplerTable();
    this->ui->tabWidget->removeTab(1);
}

void Dlg_Concentration::cbConcAssocieeSelectPolluantIndexChanged(const int index) {
    this->m_idMoleculeConcentrationAssocie = m_modelMolecule->record(index).value(POLLUANT_BY_SYS_ETALON_ID).toUInt();
}

void Dlg_Concentration::tabWidgetIndexChanged(const int index)
{
    switch(index) {
    case 0:
        break;
    case 1:
        if(this->ui->gb_edit_champs->isVisible() || !this->ui->tableView->currentIndex().isValid())
            this->ui->tabWidget->setCurrentIndex(0);
        else {
            this->ui->gb_edit_champs_ConcAssociee->setVisible(false);
            uint idConcentration = m_model->record(this->ui->tableView->currentIndex().row()).value(CONCENTRATION_ID).toUInt();
            m_modelConcentrationAssociee = this->m_bdHandler->getConcentrationAssocieeModel(idConcentration);
            this->ui->tableView_ConcAssociee->setModel(m_modelConcentrationAssociee);
            this->ui->tableView_ConcAssociee->hideColumn(CONC_ASSOCIEE_ID);
            this->ui->tableView_ConcAssociee->hideColumn(CONC_ASSOCIEE_ID_CONCENTRATION);
            this->ui->button_Supprimer_ConcAssociee->setEnabled(false);
            this->ui->button_Modifier_ConcAssociee->setEnabled(false);
            if(m_modelMolecule.isNull()) {
                m_modelMolecule = this->m_bdHandler->getMoleculesModel();
            }
            this->ui->comboBox_ConcAssociee_Polluant->setModel(m_modelMolecule);
            this->ui->comboBox_ConcAssociee_Polluant->setModelColumn(MOLECULE_FORMULE);

            this->initialiserChampsConcAssociee();
        }
        break;
    }
}

void Dlg_Concentration::buttonSupprimerConcAssocieeClicked()
{
    if(!m_indexConcAssocieSelection.isValid())
       return;
    int reponse = QMessageBox::question(this, "Supprimer un élément", "êtes-vous sûr de vouloir effacer cet enregistrement?",
                QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::No)
       return;

    if(!m_modelConcentrationAssociee->removeRow(m_indexConcAssocieSelection.row()))
       QMessageBox::critical(this,"Impossible de supprimer","Erreur de la suppression de l'enregistrement demandé",QMessageBox::Ok);
    else {
       m_modelConcentrationAssociee->submitAll();
       this->initialiserChampsConcAssociee();
    }

}

void Dlg_Concentration::buttonModifierConcAssocieeClicked()
{
    this->editConcAssocieeCurrentRow(this->ui->tableView_ConcAssociee->currentIndex());
}

void Dlg_Concentration::buttonAjouterConcAssocieeClicked()
{
    this->initialiserChampsConcAssociee();
    this->setGbEditChampsConcAssocieeReadOnly(false);
    this->afficherFormulaireConcAssociee();
    this->ui->comboBox_ConcAssociee_Polluant->setFocus();
    this->ui->tableView_ConcAssociee->setEnabled(false);
}

void Dlg_Concentration::buttonValiderConcAssocieeClicked()
{
    int row;
    if(m_modifConcAssocieeEnCours) {
        row = this->ui->tableView_ConcAssociee->currentIndex().row();
    }
    else {
        row = m_modelConcentrationAssociee->rowCount();
        if(row>0) row-=1;
        m_modelConcentrationAssociee->insertRow(row);
    }
    QVariant idConcentration = this->m_model->record(m_indexSelection.row()).value(CONCENTRATION_ID);

    m_modelConcentrationAssociee->setData(m_modelConcentrationAssociee->index(row,CONC_ASSOCIEE_ID_CONCENTRATION),idConcentration);
    m_modelConcentrationAssociee->setData(m_modelConcentrationAssociee->index(row,CONC_ASSOCIEE_FORMULE),QVariant::fromValue(this->m_idMoleculeConcentrationAssocie));
    m_modelConcentrationAssociee->setData(m_modelConcentrationAssociee->index(row,CONC_ASSOCIEE_CONCENTRATION),QVariant::fromValue(this->ui->doubleSpinBox_ConcAssociee_Concentration->value()));

    m_modelConcentrationAssociee->submitAll();

    this->initialiserChampsConcAssociee();
}

