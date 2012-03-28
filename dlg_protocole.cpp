/*////////////////////////////////////////////////////////////
// \file dlg_protocole.cpp
// \brief Classe d'interface graphique de la fenetre principale
// \author FOUQUART Christophe
// \version 1.0
// \date 25/03/2011
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
#include "dlg_protocole.h"
#include "ui_dlg_protocole.h"

Dlg_Protocole::Dlg_Protocole(QWidget *parent,const QPointer<BdHandler> bdHandler,
                             const bool returnSelection,const int indexSelection) :

    QDialog(parent),
    ui(new Ui::Dlg_Protocole)
{
    ui->setupUi(this);
    this->setWindowTitle("Protocoles");

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
    connect(this->ui->button_Selectionner,SIGNAL(clicked()),
            this,SLOT(buttonSelectionnerClicked()));
    connect(this->ui->button_Fermer,SIGNAL(clicked()),
            this,SLOT(buttonFermerClicked()));

    if(indexSelection > -1) {
        this->ui->tableView->selectRow(indexSelection);
    }
}

Dlg_Protocole::~Dlg_Protocole()
{
    delete ui;
}

void Dlg_Protocole::afficherTable() {
    if(!this->m_returnSelection)
       m_bdHandler->connexionBD();
    m_model = m_bdHandler->getProtocolesModel();
    this->ui->tableView->setModel(m_model);
    this->ui->tableView->setColumnHidden(PROTOCOLE_ID, true);
    this->ui->tableView->resizeColumnsToContents();
}

void Dlg_Protocole::initialiserChamps() {
    this->ui->gb_edit_champs->setVisible(false);

    this->ui->lineEdit_Designation->clear();
    this->ui->button_Ajouter->setEnabled(true);
    this->ui->button_Supprimer->setEnabled(true);
    this->ui->button_Fermer->setEnabled(true);
    this->ui->button_Selectionner->setVisible(true);
    this->ui->button_Selectionner->setEnabled(true);

    int height = this->ui->tableView->geometry().height()+(this->ui->gb_edit_champs->geometry().height());
    this->ui->tableView->setGeometry(this->ui->tableView->geometry().x(),this->ui->tableView->geometry().y(),
                      this->ui->tableView->geometry().width(),height);

    if(!this->m_returnSelection || this->m_model->rowCount()==0) {
        this->ui->button_Selectionner->setVisible(false);
    }
}

void Dlg_Protocole::changementSelection(const QModelIndex & idxSelection) {
    this->m_indexSelection = idxSelection;
}

void Dlg_Protocole::buttonSupprimerClicked() {
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

void Dlg_Protocole::buttonAjouterClicked() {

    int height = this->ui->tableView->geometry().height()-(this->ui->gb_edit_champs->geometry().height());
    this->ui->tableView->setGeometry(this->ui->tableView->geometry().x(),this->ui->tableView->geometry().y(),
                      this->ui->tableView->geometry().width(),height);

    this->ui->gb_edit_champs->setVisible(true);
    this->ui->button_Ajouter->setEnabled(false);
    this->ui->button_Supprimer->setEnabled(false);
    this->ui->button_Fermer->setEnabled(false);
    this->ui->button_Selectionner->setEnabled(false);

    this->ui->lineEdit_Designation->setFocus();
    this->ui->button_Valider->setDefault(true);
}

void Dlg_Protocole::buttonValiderClicked() {
    QSqlRecord enregistrement = m_model->record();
    enregistrement.setValue(PROTOCOLE_DESIGNATION,this->ui->lineEdit_Designation->text());

    m_model->insertRecord(-1,enregistrement);
    m_model->submitAll();

    this->initialiserChamps();
}

void Dlg_Protocole::buttonFermerClicked() {
    this->reject();
}

void Dlg_Protocole::buttonSelectionnerClicked() {
    this->accept();
}

int Dlg_Protocole::getIdSelection() {
    return this->m_model->record(m_indexSelection.row()).value(PROTOCOLE_ID).toInt();
}

