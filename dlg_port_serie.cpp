/*////////////////////////////////////////////////////////////
// \file dlg_port_serie.cpp
// \brief Classe d'interface graphique de la fenetre port serie
// \author PIET Regis
// \version 1.0
// \date 11/09/2011
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

#include "dlg_port_serie.h"
#include "ui_dlg_port_serie.h"

Dlg_Port_Serie::Dlg_Port_Serie(QWidget *parent,const QPointer<BdHandler> bdHandler,
                             const bool returnSelection,const int indexSelection) :
    QDialog(parent),
    ui(new Ui::Dlg_Port_Serie)
{
    ui->setupUi(this);
    this->setWindowTitle("Port Serie");

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
    connect(this->ui->button_Selectionner,SIGNAL(clicked()),
            this,SLOT(buttonSelectionnerClicked()));
    connect(this->ui->button_Fermer,SIGNAL(clicked()),
            this,SLOT(buttonFermerClicked()));
    connect(this->ui->button_Annuler,SIGNAL(clicked()),
            this,SLOT(initialiserChamps()));
    connect(this->ui->button_Valider,SIGNAL(clicked()),
            this,SLOT(buttonValiderClicked()));

    if(indexSelection > -1) {
        this->ui->tableView->selectRow(indexSelection);
    }
}

Dlg_Port_Serie::~Dlg_Port_Serie()
{
    delete ui;
}

void Dlg_Port_Serie::changeEvent(QEvent *e)
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

void Dlg_Port_Serie::afficherTable() {
    if(!this->m_bdHandler->isOpen())
       m_bdHandler->connexionBD();

    m_model = m_bdHandler->getNoPortSerie();
    m_model->setParent(this);

    this->ui->tableView->setModel(m_model);
    this->ui->tableView->resizeColumnsToContents();
    this->ui->tableView->setItemDelegate(new QSqlRelationalDelegate(this));
    this->ui->tableView->sortByColumn(0,Qt::AscendingOrder);
}

void Dlg_Port_Serie::initialiserChamps() {
    this->ui->gb_edit_champs->setVisible(false);
    this->ui->lineEdit_NoPortSerie->clear();
    this->ui->lineEdit_Designation->clear();
    this->ui->button_Ajouter->setEnabled(true);
    this->ui->button_Supprimer->setEnabled(true);
    this->ui->button_Fermer->setEnabled(true);
    this->ui->button_Selectionner->setVisible(true);

    if(!this->m_returnSelection || this->m_model->rowCount()==0) {
        this->ui->button_Selectionner->setVisible(false);
    }
}

void Dlg_Port_Serie::changementSelection(const QModelIndex & idxSelection) {
    this->m_indexSelection = idxSelection;
}

void Dlg_Port_Serie::buttonSupprimerClicked() {
    if(!m_indexSelection.isValid())
        return;
    int reponse = QMessageBox::question(this, "Supprimer un élément", "êtes-vous sûr de vouloir effacer cet enregistrement?",
                  QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::No)
        return;

    if(!m_model.data()->removeRow(m_indexSelection.row()))
        QMessageBox::critical(this,"Impossible de supprimer","Erreur de la suppression de l'enregistrement demandé",QMessageBox::Ok);
    else {
        m_model->submitAll();
        this->initialiserChamps();
    }
}

void Dlg_Port_Serie::buttonAjouterClicked() {
    this->ui->gb_edit_champs->setVisible(true);
    this->ui->button_Ajouter->setEnabled(false);
    this->ui->button_Supprimer->setEnabled(false);
    this->ui->button_Fermer->setEnabled(false);

    this->ui->lineEdit_NoPortSerie->setFocus();

    this->ui->button_Valider->setDefault(true);
    this->ui->button_Selectionner->setEnabled(false);
}

void Dlg_Port_Serie::buttonValiderClicked() {
    int row = m_model->rowCount();

    if(row>0)
        row-=1;
    m_model->insertRow(row);

    m_model->setData(m_model->index(row,PORT_SERIE_NO),QVariant::fromValue(this->ui->lineEdit_NoPortSerie->text()));
    m_model->setData(m_model->index(row,PORT_SERIE_DESIGNATION),QVariant::fromValue(this->ui->lineEdit_Designation->text()));

    m_model->submitAll();

    this->initialiserChamps();
}

void Dlg_Port_Serie::buttonFermerClicked() {
    this->reject();
}

void Dlg_Port_Serie::buttonSelectionnerClicked() {
    this->accept();
}

int Dlg_Port_Serie::getIdSelection() {
    return this->m_model->record(m_indexSelection.row()).value(PORT_SERIE_NO).toInt();
}
