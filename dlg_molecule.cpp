/*///////////////////////////////////////////////////////////////////////////////////
// \file dlg_molecule.cpp
// \brief Classe d'interface gérant la table Molecule
// \author FOUQUART Christophe
// \version 1.0
// \date 07/04/2011
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
///////////////////////////////////////////////////////////////////////////////////*/

#include "dlg_molecule.h"
#include "ui_dlg_molecule.h"

Dlg_Molecule::Dlg_Molecule(QWidget *parent,const QPointer<BdHandler> bdHandler) :
    QDialog(parent),
    ui(new Ui::Dlg_Molecule)
{
    ui->setupUi(this);
    this->setWindowTitle("Molécules");
    this->bdHandler = bdHandler;
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
}

Dlg_Molecule::~Dlg_Molecule()
{
    delete ui;
}

void Dlg_Molecule::afficherTable() {
    bdHandler->connexionBD();
    model = bdHandler->getMoleculesModel();
    this->ui->tableView->setModel(model);
    this->ui->tableView->setColumnHidden(MOLECULE_ID, true);
    this->ui->tableView->resizeColumnToContents(MOLECULE_CODE);
}

void Dlg_Molecule::initialiserChamps() {
    this->ui->gb_edit_champs->setVisible(false);

    this->ui->lineEdit_Code->clear();
    this->ui->lineEdit_Formule->clear();
    this->ui->lineEdit_Nom->clear();
    this->ui->button_Ajouter->setEnabled(true);
    this->ui->button_Supprimer->setEnabled(true);
    this->ui->button_Fermer->setEnabled(true);
}

void Dlg_Molecule::changementSelection(const QModelIndex & idxSelection) {
    this->indexSelection = idxSelection;
}

void Dlg_Molecule::buttonSupprimerClicked() {
    if(!indexSelection.isValid())
        return;
    int reponse = QMessageBox::question(this, "Supprimer un élément", "êtes-vous sûr de vouloir effacer cet enregistrement?",
                  QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::No)
        return;

    if(!model->removeRow(indexSelection.row()))
        QMessageBox::critical(this,"Impossible de supprimer","Erreur de la suppression de l'enregistrement demandé",QMessageBox::Ok);
}

void Dlg_Molecule::buttonAjouterClicked() {
    this->ui->gb_edit_champs->setVisible(true);
    this->ui->button_Ajouter->setEnabled(false);
    this->ui->button_Supprimer->setEnabled(false);
    this->ui->button_Fermer->setEnabled(false);

    this->ui->lineEdit_Code->setFocus();
    this->ui->button_Valider->setDefault(true);
}

void Dlg_Molecule::buttonValiderClicked() {
    QSqlRecord enregistrement = model->record();
    enregistrement.setValue(MOLECULE_CODE,this->ui->lineEdit_Code->text());
    enregistrement.setValue(MOLECULE_NOM,this->ui->lineEdit_Nom->text());
    enregistrement.setValue(MOLECULE_FORMULE,this->ui->lineEdit_Formule->text());

    model->insertRecord(-1,enregistrement);

    this->initialiserChamps();
}


