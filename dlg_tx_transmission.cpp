/*////////////////////////////////////////////////////////////
// \file dlg_tx_transmission.cpp
// \brief Classe d'interface graphique de la fenetre principale
// \author FOUQUART Christophe
// \version 1.0
// \date 25/03/2011
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
#include "dlg_tx_transmission.h"
#include "ui_dlg_tx_transmission.h"

Dlg_Tx_Transmission::Dlg_Tx_Transmission(QWidget *parent,const QPointer<BdHandler> m_bdHandler,const bool returnSelection,const int indexSelection) :
        QDialog(parent),
        ui(new Ui::Dlg_Tx_Transmission)
{
    ui->setupUi(this);
    this->setWindowTitle("Taux de transmission");

    this->m_bdHandler = m_bdHandler;
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

Dlg_Tx_Transmission::~Dlg_Tx_Transmission()
{
    delete ui;
}

void Dlg_Tx_Transmission::afficherTable() {
    if(!this->m_returnSelection)
       m_bdHandler->connexionBD();
    m_model = m_bdHandler->getTxTransmissionModel();
    this->ui->tableView->setModel(m_model);
    this->ui->tableView->setColumnHidden(TX_TRANSMISSION_ID, true);
    this->ui->tableView->resizeColumnsToContents();
}

void Dlg_Tx_Transmission::initialiserChamps() {
    this->ui->gb_edit_champs->setVisible(false);

    this->ui->lineEdit_Designation->clear();
    this->ui->button_Ajouter->setEnabled(true);
    this->ui->button_Supprimer->setEnabled(true);
    this->ui->button_Fermer->setEnabled(true);
    this->ui->button_Selectionner->setEnabled(true);

    int height = this->ui->tableView->geometry().height()+(this->ui->gb_edit_champs->geometry().height());
    this->ui->tableView->setGeometry(this->ui->tableView->geometry().x(),this->ui->tableView->geometry().y(),
                      this->ui->tableView->geometry().width(),height);

    if(!this->m_returnSelection || this->m_model->rowCount()==0) {
        this->ui->button_Selectionner->setVisible(false);
    }
}

void Dlg_Tx_Transmission::changementSelection(const QModelIndex & idxSelection) {
    this->m_indexSelection = idxSelection;
}

void Dlg_Tx_Transmission::buttonSupprimerClicked() {
    if(!m_indexSelection.isValid())
        return;
    int reponse = QMessageBox::question(this, "Supprimer un élément", "êtes-vous sûr de vouloir effacer cet enregistrement?",
                  QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::No)
        return;

    if(!m_model.data()->removeRow(m_indexSelection.row()))
        QMessageBox::critical(this,"Impossible de supprimer","Erreur de la suppression de l'enregistrement demandé",QMessageBox::Ok);
}

void Dlg_Tx_Transmission::buttonAjouterClicked() {

    int height = this->ui->tableView->geometry().height()-(this->ui->gb_edit_champs->geometry().height());
    this->ui->tableView->setGeometry(this->ui->tableView->geometry().x(),this->ui->tableView->geometry().y(),
                      this->ui->tableView->geometry().width(),height);

    this->ui->gb_edit_champs->setVisible(true);
    this->ui->button_Ajouter->setEnabled(false);
    this->ui->button_Supprimer->setEnabled(false);
    this->ui->button_Fermer->setEnabled(false);
    this->ui->button_Selectionner->setEnabled(false);

    this->ui->button_Valider->setDefault(true);

    this->ui->lineEdit_Designation->setFocus();
    this->ui->button_Valider->setDefault(true);
}

void Dlg_Tx_Transmission::buttonValiderClicked() {
    QSqlRecord enregistrement = m_model.data()->record();
    enregistrement.setValue(TX_TRANSMISSION_DESIGNATION,this->ui->lineEdit_Designation->text());

    m_model->insertRecord(-1,enregistrement);

    m_model->submitAll();

    this->initialiserChamps();
}

void Dlg_Tx_Transmission::buttonFermerClicked() {
    this->reject();
}

void Dlg_Tx_Transmission::buttonSelectionnerClicked() {
    this->accept();
}

int Dlg_Tx_Transmission::getIdSelection() {
    uint idTxTransmission = this->m_model->record(m_indexSelection.row()).value(TX_TRANSMISSION_ID).toInt();
    return idTxTransmission;
}
