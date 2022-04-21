/*////////////////////////////////////////////////////////////
// \file dlg_modele.cpp
// \brief Classe d'interface graphique de la fenetre principale
// \author FOUQUART Christophe
// \version 1.0
// \date 25/03/2011
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

#include "dlg_operateur.h"
#include "ui_dlg_operateur.h"

Dlg_Operateur::Dlg_Operateur(QWidget *parent,const QPointer<BdHandler> bdHandler,
                             const bool returnSelection,const int indexSelection) :
    QDialog(parent),
    ui(new Ui::Dlg_Operateur)
{
    ui->setupUi(this);
    this->setWindowTitle("Operateurs");

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

Dlg_Operateur::~Dlg_Operateur()
{
    delete ui;
}

void Dlg_Operateur::changeEvent(QEvent *e)
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

void Dlg_Operateur::afficherTable() {
    if(!this->m_bdHandler->isOpen())
       m_bdHandler->connexionBD();

    m_model = m_bdHandler->getOperateurModel();
    m_model->setParent(this);

    this->ui->tableView->setModel(m_model);
    this->ui->tableView->setColumnHidden(OPERATEUR_ID, true);
    this->ui->tableView->setColumnHidden(OPERATEUR_MDP, true);
    this->ui->tableView->setColumnHidden(OPERATEUR_ADMIN, true);
    this->ui->tableView->resizeColumnsToContents();
    this->ui->tableView->setItemDelegate(new QSqlRelationalDelegate(this));
}

void Dlg_Operateur::initialiserChamps() {
    this->ui->gb_edit_champs->setVisible(false);
    this->ui->lineEdit_Nom->clear();
    this->ui->lineEdit_Prenom->clear();
    this->ui->button_Ajouter->setEnabled(true);
    this->ui->button_Supprimer->setEnabled(true);
    this->ui->button_Fermer->setEnabled(true);
    this->ui->button_Selectionner->setVisible(true);

    if(!this->m_returnSelection || this->m_model->rowCount()==0) {
        this->ui->button_Selectionner->setVisible(false);
    }
}

void Dlg_Operateur::changementSelection(const QModelIndex & idxSelection) {
    this->m_indexSelection = idxSelection;
}

void Dlg_Operateur::buttonSupprimerClicked() {
    if(!m_indexSelection.isValid())
        return;
    int reponse = QMessageBox::question(this, "Supprimer un element", "êtes-vous sûr de vouloir effacer cet enregistrement?",
                  QMessageBox::Yes | QMessageBox::No);

    if (reponse == QMessageBox::No)
        return;

    if(!m_model.data()->removeRow(m_indexSelection.row()))
        QMessageBox::critical(this,"Impossible de supprimer","Erreur de la suppression de l'enregistrement demande",QMessageBox::Ok);
    else {
        m_model->submitAll();
        this->initialiserChamps();
    }
}

void Dlg_Operateur::buttonAjouterClicked() {
    this->ui->gb_edit_champs->setVisible(true);
    this->ui->button_Ajouter->setEnabled(false);
    this->ui->button_Supprimer->setEnabled(false);
    this->ui->button_Fermer->setEnabled(false);

    this->ui->lineEdit_Nom->setFocus();

    this->ui->button_Valider->setDefault(true);
    this->ui->button_Selectionner->setEnabled(false);
}

void Dlg_Operateur::buttonValiderClicked() {
    int row = m_model->rowCount();

    if(row>0)
        row-=1;
    m_model->insertRow(row);

    m_model->setData(m_model->index(row,OPERATEUR_NOM),QVariant::fromValue(this->ui->lineEdit_Nom->text()));
    m_model->setData(m_model->index(row,OPERATEUR_PRENOM),QVariant::fromValue(this->ui->lineEdit_Prenom->text()));

    m_model->submitAll();

    this->initialiserChamps();
}

void Dlg_Operateur::buttonFermerClicked() {
    this->reject();
}

void Dlg_Operateur::buttonSelectionnerClicked() {
    this->accept();
}

int Dlg_Operateur::getIdSelection() {
    return this->m_model->record(m_indexSelection.row()).value(MODELE_ID).toInt();
}
