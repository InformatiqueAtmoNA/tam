/*////////////////////////////////////////////////////////////
// \file dlg_modele.cpp
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


#include "dlg_modele.h"
#include "ui_dlg_modele.h"

Dlg_Modele::Dlg_Modele(QWidget *parent,const QPointer<BdHandler> bdHandler,
                       const bool returnSelection,const int indexSelection,const QString & filtreType) :
    QDialog(parent),
    ui(new Ui::Dlg_Modele)
{
    ui->setupUi(this);
    this->setWindowTitle("Modèles");

    this->m_bdHandler = bdHandler;
    this->m_returnSelection = returnSelection;
    this->m_filtreType = filtreType;

    this->afficherTable();
    this->initialiserChamps();

    connect(this->ui->tableView->selectionModel(),
            SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this,SLOT(changementSelection(const QModelIndex &)));
    connect(this->ui->cb_Marque, SIGNAL(currentIndexChanged(int)),
            this,SLOT(cb_MarqueChanged(int)));
    connect(this->ui->cb_Protocole, SIGNAL(currentIndexChanged(int)),
            this,SLOT(cb_ProtocoleChanged(int)));
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
    connect(this->ui->button_EditProtocole,SIGNAL(clicked()),
            this,SLOT(buttonEditProtocoleClicked()));
    connect(this->ui->button_EditMarque,SIGNAL(clicked()),
            this,SLOT(buttonEditMarqueClicked()));

    if(indexSelection > -1) {
        this->ui->tableView->selectRow(indexSelection);
    }

    this->cb_MarqueChanged(this->ui->cb_Marque->currentIndex());
    this->cb_ProtocoleChanged(this->ui->cb_Protocole->currentIndex());
    if(!this->m_filtreType.isEmpty()) {
        this->ui->cb_Type->clear();
        this->ui->cb_Type->addItem(this->m_filtreType);
    }
}

Dlg_Modele::~Dlg_Modele()
{
    delete ui;

    if(!this->m_returnSelection)
        m_bdHandler->deconnexionBD();
}

void Dlg_Modele::afficherTable() {
    if(!this->m_bdHandler->isOpen())
       m_bdHandler->connexionBD();

    m_model = m_bdHandler->getModelesModel();
    m_model->setParent(this);
    this->ui->tableView->setModel(m_model);
    this->ui->tableView->setColumnHidden(MODELE_ID, true);
    this->ui->tableView->resizeColumnsToContents();
    this->ui->tableView->setItemDelegate(new QSqlRelationalDelegate(this));

    m_model_marque = m_bdHandler->getMarquesModel();
    m_model_marque->setParent(this);
    this->ui->cb_Marque->setModel(m_model_marque);
    this->ui->cb_Marque->setModelColumn(MARQUE_DESIGNATION);

    m_model_protocole = m_bdHandler->getProtocolesModel();
    m_model_protocole->setParent(this);
    this->ui->cb_Protocole->setModel(m_model_protocole);
    this->ui->cb_Protocole->setModelColumn(PROTOCOLE_DESIGNATION);
}

void Dlg_Modele::initialiserChamps() {
    this->ui->gb_edit_champs->setVisible(false);

    this->ui->lineEdit_Designation->clear();
    this->ui->button_Ajouter->setEnabled(true);
    this->ui->button_Supprimer->setEnabled(true);
    this->ui->button_Fermer->setEnabled(true);
    this->ui->button_Selectionner->setVisible(true);
    this->ui->button_Selectionner->setEnabled(true);

    if(!this->m_returnSelection || this->m_model->rowCount()==0) {
        this->ui->button_Selectionner->setVisible(false);
    }
}

void Dlg_Modele::changementSelection(const QModelIndex & idxSelection) {
    this->m_indexSelection = idxSelection;
}

void Dlg_Modele::buttonSupprimerClicked() {
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

void Dlg_Modele::buttonAjouterClicked() {
    this->ui->gb_edit_champs->setVisible(true);
    this->ui->button_Ajouter->setEnabled(false);
    this->ui->button_Supprimer->setEnabled(false);
    this->ui->button_Fermer->setEnabled(false);

    this->ui->cb_Type->setFocus();

    this->ui->button_Valider->setDefault(true);
    this->ui->button_Selectionner->setEnabled(false);
}

void Dlg_Modele::buttonValiderClicked() {
    int row = m_model->rowCount();

    if(row>0)
        row-=1;
    m_model->insertRow(row);

    m_model->setData(m_model->index(row,MODELE_ID_MARQUE),QVariant::fromValue(this->m_idMarque));
    m_model->setData(m_model->index(row,MODELE_ID_PROTOCOLE),QVariant::fromValue(this->m_idProtocole));
    m_model->setData(m_model->index(row,MODELE_DESIGNATION),QVariant::fromValue(this->ui->lineEdit_Designation->text()));
    m_model->setData(m_model->index(row,MODELE_TYPE),QVariant::fromValue(this->ui->cb_Type->currentText()));

    m_model->submitAll();

    this->initialiserChamps();
}

void Dlg_Modele::cb_MarqueChanged(const int index) {
    this->m_idMarque= this->m_model_marque->record(index).value(MARQUE_ID).toInt();
}

void Dlg_Modele::buttonEditMarqueClicked() {
    int oldIndexCbMarque = this->ui->cb_Marque->currentIndex();
    Dlg_Marque dlgMarque(this,this->m_bdHandler,true,oldIndexCbMarque);

    int result = dlgMarque.exec();

    delete m_model;
    delete m_model_marque;

    this->afficherTable();

    if(result) {
        int idSelection = dlgMarque.getIdSelection();
        QModelIndexList listIndexCbSelection = this->m_model_marque->match(this->m_model_marque->index(0,MARQUE_ID),Qt::DisplayRole,QVariant::fromValue(idSelection));
        this->ui->cb_Marque->setCurrentIndex(listIndexCbSelection.at(0).row());
    }
    else {
        this->ui->cb_Marque->setCurrentIndex(oldIndexCbMarque);
    }
}

void Dlg_Modele::cb_ProtocoleChanged(const int index) {
    this->m_idProtocole= this->m_model_protocole->record(index).value(PROTOCOLE_ID).toInt();
}

void Dlg_Modele::buttonEditProtocoleClicked() {
    int oldIndexCbProtocole = this->ui->cb_Protocole->currentIndex();
    Dlg_Protocole dlgProtocole(this,this->m_bdHandler,true,oldIndexCbProtocole);

    int result = dlgProtocole.exec();

    delete m_model;
    delete m_model_protocole;

    this->afficherTable();

    if(result) {
        int idSelection = dlgProtocole.getIdSelection();
        if(idSelection>0) {
            QModelIndexList listIndexCbSelection = this->m_model_protocole->match(this->m_model_protocole->index(0,PROTOCOLE_ID),Qt::DisplayRole,QVariant::fromValue(idSelection));
            this->ui->cb_Protocole->setCurrentIndex(listIndexCbSelection.at(0).row());
            return;
        }
    }
    this->ui->cb_Protocole->setCurrentIndex(oldIndexCbProtocole);
}

void Dlg_Modele::buttonFermerClicked() {
    this->reject();
}

void Dlg_Modele::buttonSelectionnerClicked() {
    this->accept();
}

int Dlg_Modele::getIdSelection() {
    return this->m_model->record(m_indexSelection.row()).value(MODELE_ID).toInt();
}
