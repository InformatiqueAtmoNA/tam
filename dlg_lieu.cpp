/*////////////////////////////////////////////////////////////
// \file dlg_Lieu.cpp
// \brief Classe d'interface graphique gerant la table Lieu
// \author FOUQUART Christophe
// \version 1.0
// \date 31/08/2011
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

#include "dlg_lieu.h"
#include "ui_dlg_lieu.h"

Dlg_Lieu::Dlg_Lieu(QWidget *parent,const QPointer<BdHandler> bdHandler,
                   const bool returnSelection,const int indexSelection) :
    QDialog(parent),
    ui(new Ui::Dlg_Lieu)
{
    ui->setupUi(this);
    this->setWindowTitle("Lieu");
    this->m_bdHandler = bdHandler;
    this->m_returnSelection = returnSelection;

    this->afficherTable();
    this->initialiserChamps();

    connect(this->ui->tableView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(changementSelection(const QModelIndex &)));
    connect(this->ui->button_Supprimer,SIGNAL(clicked()),
            this,SLOT(buttonSupprimerClicked()));
    connect(this->ui->button_Ajouter,SIGNAL(clicked()),
            this,SLOT(buttonAjouterClicked()));
    connect(this->ui->button_Annuler,SIGNAL(clicked()),
            this,SLOT(initialiserChamps()));
    connect(this->ui->button_Valider,SIGNAL(clicked()),
            this,SLOT(buttonValiderClicked()));
    connect(this->ui->button_Fermer,SIGNAL(clicked()),
            this,SLOT(buttonFermerClicked()));
    connect(this->ui->button_Selectionner,SIGNAL(clicked()),
            this,SLOT(buttonSelectionnerClicked()));

    if(indexSelection > -1) {
        this->ui->tableView->selectRow(indexSelection);
    }
}

Dlg_Lieu::~Dlg_Lieu()
{
    delete ui;
}

void Dlg_Lieu::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Dlg_Lieu::afficherTable() {
    if(!this->m_returnSelection)
       m_bdHandler->connexionBD();
    m_model = m_bdHandler->getLieuModel();
    this->ui->tableView->setModel(m_model);
    this->ui->tableView->setColumnHidden(MARQUE_ID, true);
    this->ui->tableView->resizeColumnsToContents();
}

void Dlg_Lieu::initialiserChamps() {
    this->ui->gb_edit_champs->setVisible(false);

    this->ui->lineEdit_Designation->clear();
    this->ui->button_Ajouter->setEnabled(true);
    this->ui->button_Supprimer->setEnabled(true);
    this->ui->button_Fermer->setEnabled(true);
    this->ui->button_Selectionner->setEnabled(true);

    if(!this->m_returnSelection || this->m_model->rowCount()==0) {
        this->ui->button_Selectionner->setVisible(false);
    }
}

void Dlg_Lieu::changementSelection(const QModelIndex & idxSelection) {
    this->m_indexSelection = idxSelection;
}

void Dlg_Lieu::buttonSupprimerClicked() {
    if(!m_indexSelection.isValid())
        return;
    int reponse = QMessageBox::question(this, "Supprimer un element", "�tes-vous s�r de vouloir effacer cet enregistrement?",
                  QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::No)
        return;

    if(!m_model->removeRow(m_indexSelection.row()))
        QMessageBox::critical(this,"Impossible de supprimer","Erreur de la suppression de l'enregistrement demande",QMessageBox::Ok);
    else {
        m_model->submitAll();
        this->initialiserChamps();
    }
}

void Dlg_Lieu::buttonAjouterClicked() {
    this->ui->gb_edit_champs->setVisible(true);
    this->ui->button_Ajouter->setEnabled(false);
    this->ui->button_Supprimer->setEnabled(false);
    this->ui->button_Fermer->setEnabled(false);
    this->ui->button_Selectionner->setEnabled(false);

    this->ui->lineEdit_Designation->setFocus();
    this->ui->button_Valider->setDefault(true);
}

void Dlg_Lieu::buttonValiderClicked() {
    QSqlRecord enregistrement = m_model->record();
    enregistrement.setValue(MARQUE_DESIGNATION,this->ui->lineEdit_Designation->text());

    m_model->insertRecord(-1,enregistrement);

    this->initialiserChamps();
}

void Dlg_Lieu::buttonFermerClicked() {
    this->reject();
}

void Dlg_Lieu::buttonSelectionnerClicked() {
    this->accept();
}

int Dlg_Lieu::getIdSelection() {
    return this->m_model->record(m_indexSelection.row()).value(MARQUE_ID).toInt();
}
