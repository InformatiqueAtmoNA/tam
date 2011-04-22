/*////////////////////////////////////////////////////////////
// \file dlg_concentration.cpp
// \brief Classe d'interface graphique  gérant la table Cocentration
// \author FOUQUART Christophe
// \version 1.0
// \date 20/04/2011
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

#include "dlg_concentration.h"
#include "ui_dlg_concentration.h"

Dlg_Concentration::Dlg_Concentration(QWidget *parent,const QPointer<BdHandler> bdHandler,
                                     const int idSystemeEtalon, const bool returnSelection,
                                     const int indexSelection) :
      QDialog(parent),
      ui(new Ui::Dlg_Concentration)
{
    ui->setupUi(this);
    this->setWindowTitle("Concentration");

    this->m_bdHandler = bdHandler;
    this->m_returnSelection = returnSelection;
    this->m_idSystemeEtalon = idSystemeEtalon;

    this->peuplerTable();
    this->initialiserChamps();

    connect(this->ui->tableView->selectionModel(),
          SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
          this,SLOT(changementSelection(const QModelIndex &)));
    connect(this->ui->button_Supprimer,SIGNAL(clicked()),
          this,SLOT(buttonSupprimerClicked()));
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

    if(indexSelection > -1) {
      this->ui->tableView->selectRow(indexSelection);
    }
}

Dlg_Concentration::~Dlg_Concentration()
{
    delete ui;

    if(!this->m_returnSelection)
      m_bdHandler->deconnexionBD();
}

void Dlg_Concentration::peuplerTable() {
    if(!this->m_bdHandler->isOpen())
     m_bdHandler->connexionBD();

    m_model = m_bdHandler->getConcentrationModel(m_idSystemeEtalon);
    m_model->setParent(this);
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    this->ui->tableView->setModel(m_model);
    this->ui->tableView->setColumnHidden(CONCENTRATION_ID, true);
    this->ui->tableView->setColumnHidden(CONCENTRATION_SYS_ETALON, true);
    this->ui->tableView->resizeColumnsToContents();
    this->ui->tableView->setItemDelegate(new QSqlRelationalDelegate(this));
}

void Dlg_Concentration::afficherFormulaire() {
    this->ui->gb_edit_champs->setVisible(true);
    this->ui->button_Ajouter->setEnabled(false);
    this->ui->button_Supprimer->setEnabled(false);
    this->ui->button_Fermer->setEnabled(false);

    this->ui->lineEdit_Point->setFocus();

    this->ui->button_Valider->setDefault(true);
    this->ui->button_Selectionner->setEnabled(false);
}

void Dlg_Concentration::initialiserChamps() {
    this->ui->gb_edit_champs->setVisible(false);
    this->ui->tableView->setEnabled(true);
    this->ui->button_Ajouter->setEnabled(true);
    this->ui->button_Supprimer->setEnabled(true);
    this->ui->button_Fermer->setEnabled(true);
    this->ui->button_Selectionner->setEnabled(true);
    if(!this->m_returnSelection || this->m_model->rowCount()==0)
      this->ui->button_Selectionner->setVisible(false);

    this->ui->lineEdit_Point->clear();
    this->ui->lineEdit_ConcReelle->clear();
    this->ui->lineEdit_ConcO3->clear();
}

void Dlg_Concentration::changementSelection(const QModelIndex & idxSelection) {
    this->m_indexSelection = idxSelection;
    if(this->m_indexSelection.isValid()) {
      if(this->m_returnSelection)
          this->ui->button_Selectionner->setEnabled(true);
    }
}

void Dlg_Concentration::buttonSupprimerClicked() {
    if(!m_indexSelection.isValid())
      return;
    int reponse = QMessageBox::question(this, "Supprimer un élément", "êtes-vous sûr de vouloir effacer cet enregistrement?",
                QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::No)
      return;

    if(!m_model->removeRow(m_indexSelection.row()))
      QMessageBox::critical(this,"Impossible de supprimer","Erreur de la suppression de l'enregistrement demandé",QMessageBox::Ok);
    else {
      m_model->submitAll();
      this->initialiserChamps();
    }
}

void Dlg_Concentration::buttonAjouterClicked() {
    this->afficherFormulaire();
}

void Dlg_Concentration::buttonValiderClicked() {
    int row = m_model->rowCount();
    if(row>0)
      row-=1;
    m_model->insertRow(row);

    m_model->setData(m_model->index(row,CONCENTRATION_SYS_ETALON),QVariant::fromValue(this->m_idSystemeEtalon));
    m_model->setData(m_model->index(row,CONCENTRATION_POINT),QVariant::fromValue(this->ui->lineEdit_Point->text()));
    m_model->setData(m_model->index(row,CONCENTRATION_REELLE),QVariant::fromValue(this->ui->lineEdit_ConcReelle->text()));
    m_model->setData(m_model->index(row,CONCENTRATION_OZONE),QVariant::fromValue(this->ui->lineEdit_ConcO3->text()));

    m_model->submitAll();

    this->initialiserChamps();
}

void Dlg_Concentration::buttonFermerClicked() {
    this->m_model->submitAll();
    this->reject();
}

void Dlg_Concentration::buttonSelectionnerClicked() {
    this->m_model->submitAll();
    this->accept();
}

int Dlg_Concentration::getIdSelection() {
    return this->m_model->record(m_indexSelection.row()).value(CONCENTRATION_ID).toInt();
}
