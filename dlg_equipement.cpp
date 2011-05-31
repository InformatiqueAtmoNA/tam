/*////////////////////////////////////////////////////////////
// \file dlg_equipement.cpp
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

#include "dlg_equipement.h"
#include "ui_dlg_equipement.h"

Dlg_Equipement::Dlg_Equipement(QWidget *parent,const QPointer<BdHandler> bdHandler,
                               const bool returnSelection, const int indexSelection,
                               const QString & filtreType) :
    QDialog(parent),
    ui(new Ui::Dlg_Equipement)
{
    ui->setupUi(this);
    this->setWindowTitle("Equipement");

    this->m_bdHandler = bdHandler;
    this->m_returnSelection = returnSelection;
    this->m_filtreType = filtreType;

    this->peuplerTable();

    connect(this->ui->tableView->selectionModel(),
            SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            this,SLOT(changementSelection(QModelIndex)));
    connect(this->ui->cb_Modele, SIGNAL(currentIndexChanged(int)),
            this,SLOT(cb_ModeleChanged(int)));
    connect(this->ui->cb_Tx_Transmission, SIGNAL(currentIndexChanged(int)),
            this,SLOT(cb_tauxTransmissionChanged(int)));
    connect(this->ui->button_Supprimer,SIGNAL(clicked()),
            this,SLOT(buttonSupprimerClicked()));
    connect(this->ui->button_Ajouter,SIGNAL(clicked()),
            this,SLOT(buttonAjouterClicked()));
    connect(this->ui->button_Fermer,SIGNAL(clicked()),
            this,SLOT(buttonFermerClicked()));
    connect(this->ui->button_Selectionner,SIGNAL(clicked()),
            this,SLOT(buttonSelectionnerClicked()));
    connect(this->ui->button_Modifier,SIGNAL(clicked()),
            this,SLOT(buttonModifierClicked()));

    connect(this->ui->button_Annuler,SIGNAL(clicked()),
            this,SLOT(initialiserChamps()));
    connect(this->ui->button_Valider,SIGNAL(clicked()),
            this,SLOT(buttonValiderClicked()));
    connect(this->ui->button_EditModele,SIGNAL(clicked()),
            this,SLOT(buttonEditModeleClicked()));
    connect(this->ui->button_EditTxTransmission,SIGNAL(clicked()),
            this,SLOT(buttonEditTxTransmissionClicked()));

    connect(this->ui->button_AddPolluant,SIGNAL(clicked()),
            this,SLOT(buttonAddPolluantClicked()));
    connect(this->ui->button_RemovePolluant,SIGNAL(clicked()),
            this,SLOT(buttonRemovePolluantClicked()));

    this->initialiserChamps();

    if(indexSelection > -1) {
        this->ui->tableView->selectRow(indexSelection);
    }
    this->cb_ModeleChanged(this->ui->cb_Modele->currentIndex());
}

Dlg_Equipement::~Dlg_Equipement()
{
    delete ui;

    if(!this->m_returnSelection)
        m_bdHandler->deconnexionBD();
}

void Dlg_Equipement::peuplerTable() {
    if(!this->m_bdHandler->isOpen())
       m_bdHandler->connexionBD();

    m_model = m_bdHandler->getEquipementModel();
    m_model->setParent(this);
    m_model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    m_model_Modele = m_bdHandler->getModelesModel();
    m_model_Modele->setParent(this);

    if(!this->m_filtreType.isEmpty())
    {
        QString filtre;
        if(this->m_filtreType=="ETALON")
            filtre = "type LIKE 'DILUTEUR' OR type LIKE 'GO3'";
        else
            filtre = QString("type LIKE '%1'").arg(this->m_filtreType);
        m_model_Modele->setFilter(filtre);

        QPointer<QSqlQueryModel> modelEquipementFiltreParType = m_bdHandler->getEquipementFiltreParModele(filtre);

        if(modelEquipementFiltreParType->rowCount()==0 && !this->ui->gb_edit_champs->isVisible()) {
            QMessageBox::information(this, "Aucun équipement du type demandé", QString("Aucun équipement de type %1").arg(m_filtreType),
                          QMessageBox::Ok);

        }
        filtre = QString("id_equipement=%1").arg(modelEquipementFiltreParType->record(0).value("id_equipement").toString());
        for(int i=1;i<modelEquipementFiltreParType->rowCount();i++) {
            uint id_equipement = modelEquipementFiltreParType->record(i).value("id_equipement").toUInt();
            filtre.append(QString(" OR id_equipement=%1").arg(QString::number(id_equipement)));
        }
        m_model->setFilter(filtre);
    }

    m_model_tx_transmission = m_bdHandler->getTxTransmissionModel();
    m_model_tx_transmission->setParent(this);

    this->ui->tableView->setModel(m_model);
    this->ui->tableView->setColumnHidden(EQUIPEMENT_ID, true);
    this->ui->tableView->resizeColumnsToContents();
    this->ui->tableView->setItemDelegate(new QSqlRelationalDelegate(this));

    this->ui->cb_Modele->setModel(m_model_Modele);
    this->ui->cb_Modele->setModelColumn(MODELE_DESIGNATION);
    this->ui->cb_Tx_Transmission->setModel(m_model_tx_transmission);
    this->ui->cb_Tx_Transmission->setModelColumn(TX_TRANSMISSION_DESIGNATION);
}

void Dlg_Equipement::afficherFormulaire() {
    this->ui->gb_edit_champs->setVisible(true);
    this->ui->button_Ajouter->setEnabled(false);
    this->ui->button_Supprimer->setEnabled(false);
    this->ui->button_Fermer->setEnabled(false);
    this->ui->button_Modifier->setEnabled(false);

    this->ui->cb_Modele->setFocus();

    this->ui->button_Valider->setDefault(true);
    this->ui->button_Selectionner->setEnabled(false);
    this->ui->button_AddPolluant->setEnabled(false);
    this->ui->button_RemovePolluant->setEnabled(false);

    if(!this->m_nouvelEnregistrement) {
        this->ui->tabWidget->setTabEnabled(2,true);
        m_model_molecule = m_bdHandler->getMoleculesModel();
        m_model_molecule->setParent(this);
        this->ui->tableView_Molecule->setModel(m_model_molecule);
        this->ui->tableView_Molecule->setColumnHidden(MOLECULE_ID,true);
        this->ui->tableView_Molecule->setColumnHidden(MOLECULE_NOM,true);
        this->ui->tableView_Molecule->resizeColumnsToContents();

        m_model_polluant_associe = m_bdHandler->getPolluantAssocieModel(m_model->record(m_indexSelection.row()).value(EQUIPEMENT_ID).toInt());
        m_model_polluant_associe->setParent(this);
        this->ui->tableView_PolluantsAssocies->setModel(m_model_polluant_associe);
        this->ui->tableView_PolluantsAssocies->setColumnHidden(POLLUANT_ASSOCIE_ID_EQUIPEMENT,true);
        this->ui->tableView_PolluantsAssocies->setColumnHidden(POLLUANT_ASSOCIE_ID,true);
        this->ui->tableView_PolluantsAssocies->setItemDelegate(new QSqlRelationalDelegate(this));

        connect(this->ui->tableView_Molecule->selectionModel(),
                SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                this,SLOT(tableViewMoleculeIndexChanged(QModelIndex)));
        connect(this->ui->tableView_PolluantsAssocies->selectionModel(),
                SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                this,SLOT(tableViewPolluantIndexChanged(QModelIndex)));

        for(int i=0;i<m_model_polluant_associe->rowCount();i++) {
            QModelIndexList listMolecule = m_model_molecule->match(m_model_molecule->index(0,MOLECULE_ID),Qt::DisplayRole,
                                           m_model_polluant_associe->record(i).value(POLLUANT_ASSOCIE_ID_MOLECULE));
            for(int j=0;j<listMolecule.count();j++) {
                this->ui->tableView_Molecule->setRowHidden(listMolecule.at(j).row(),true);
            }
        }
    }
    else
        this->ui->tabWidget->setTabEnabled(2,false);
}

void Dlg_Equipement::initialiserChamps() {
    this->m_idModifie = -1;
    this->m_modifEnCours = false;
    this->m_nouvelEnregistrement = false;

    this->ui->gb_edit_champs->setVisible(false);
    this->ui->tableView->setEnabled(true);
    this->ui->button_Ajouter->setEnabled(true);
    this->ui->button_Supprimer->setEnabled(true);
    this->ui->button_Fermer->setEnabled(true);
    if(this->m_indexSelection.isValid()) {
        this->ui->button_Selectionner->setEnabled(true);
        this->ui->button_Modifier->setEnabled(true);
    }
    else {
        this->ui->button_Selectionner->setEnabled(false);
        this->ui->button_Modifier->setEnabled(false);
    }

    if(!this->m_returnSelection || this->m_model->rowCount()==0)
        this->ui->button_Selectionner->setVisible(false);

    this->ui->lineEdit_NoSerie->clear();
    this->ui->lineEdit_MaxGamme->clear();
    this->ui->lineEdit_MinGamme->clear();
    this->ui->lineEdit_Offset->clear();
    this->ui->lineEdit_Adresse->clear();
    this->ui->cb_Nb_Bits_Stop->setCurrentIndex(0);
    this->ui->cb_Nb_Bits_Transmission->setCurrentIndex(3);
    this->m_idTxTransmission = this->ui->cb_Tx_Transmission->findText("9600");
    this->ui->cb_Tx_Transmission->setCurrentIndex(this->m_idTxTransmission);
    this->ui->cb_Type_Controle_Flux->setCurrentIndex(0);
    this->ui->cb_Type_Parite->setCurrentIndex(0);
}

void Dlg_Equipement::changementSelection(const QModelIndex & idxSelection) {
    this->m_indexSelection = idxSelection;
    if(this->m_indexSelection.isValid()) {
        this->ui->button_Modifier->setEnabled(true);
        if(this->m_returnSelection)
            this->ui->button_Selectionner->setEnabled(true);
    }
}

void Dlg_Equipement::buttonSupprimerClicked() {
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

void Dlg_Equipement::buttonAjouterClicked() {
    m_nouvelEnregistrement = true;
    this->afficherFormulaire();
}

void Dlg_Equipement::buttonValiderClicked()
{
    int row;
    if(m_modifEnCours)
        row=m_idModifie;
    else {
        row = m_model->rowCount();
        if(row>0)
            row-=1;
        m_model->insertRow(row);
    }
    m_model->setData(m_model->index(row,EQUIPEMENT_ID_MODELE),QVariant::fromValue(this->m_idModele));
    m_model->setData(m_model->index(row,EQUIPEMENT_ID_TX_TRANSMISSION),QVariant::fromValue(this->m_idTxTransmission));
    m_model->setData(m_model->index(row,EQUIPEMENT_NO_SERIE),QVariant::fromValue(this->ui->lineEdit_NoSerie->text()));
    m_model->setData(m_model->index(row,EQUIPEMENT_EN_SERVICE),QVariant::fromValue(this->ui->ckB_EnService->isChecked()));
    m_model->setData(m_model->index(row,EQUIPEMENT_MIN_GAMME),QVariant::fromValue(this->ui->lineEdit_MinGamme->text()));
    m_model->setData(m_model->index(row,EQUIPEMENT_MAX_GAMME),QVariant::fromValue(this->ui->lineEdit_MaxGamme->text()));
    m_model->setData(m_model->index(row,EQUIPEMENT_OFFSET),QVariant::fromValue(this->ui->lineEdit_Offset->text()));
    m_model->setData(m_model->index(row,EQUIPEMENT_ADRESSE),QVariant::fromValue(this->ui->lineEdit_Adresse->text()));
    m_model->setData(m_model->index(row,EQUIPEMENT_NB_BITS_TRANSMISSION),QVariant::fromValue(this->ui->cb_Nb_Bits_Transmission->currentText().toInt()));
    m_model->setData(m_model->index(row,EQUIPEMENT_NB_BITS_STOP),QVariant::fromValue(this->ui->cb_Nb_Bits_Stop->currentText().toInt()));
    m_model->setData(m_model->index(row,EQUIPEMENT_CONTROLE_FLUX),QVariant::fromValue(this->ui->cb_Type_Controle_Flux->currentText()));
    m_model->setData(m_model->index(row,EQUIPEMENT_PARITE),QVariant::fromValue(this->ui->cb_Type_Parite->currentText()));

    m_model->submitAll();

    bool afficherForm = this->m_nouvelEnregistrement;
    this->initialiserChamps();
    this->peuplerTable();
    this->changementSelection(this->m_model->index(row,EQUIPEMENT_ID));
    if(afficherForm) {
        this->buttonModifierClicked();
        this->ui->tabWidget->setCurrentIndex(2);
        this->ui->tabWidget->setFocus();
    }
    else {
        if(this->m_returnSelection) {
        this->ui->button_Selectionner->setVisible(true);
        if(this->m_indexSelection.isValid())
            this->ui->button_Selectionner->setEnabled(true);
        }
    }
}

void Dlg_Equipement::cb_ModeleChanged(const int index)
{
    this->m_idModele= this->m_model_Modele->record(index).value(MODELE_ID).toInt();
}

void Dlg_Equipement::buttonEditModeleClicked()
{
    int oldIndexCbModele = this->ui->cb_Modele->currentIndex();
    Dlg_Modele dlgModele(this,this->m_bdHandler,true,oldIndexCbModele,this->m_filtreType);

    int result = dlgModele.exec();

    delete m_model;
    delete m_model_Modele;

    this->peuplerTable();

    if(result)
    {
        int idSelection = dlgModele.getIdSelection();
        if(idSelection>0)
        {
            QModelIndexList listIndexCbSelection = this->m_model_Modele->match(this->m_model_Modele->index(0,MODELE_ID),Qt::DisplayRole,QVariant::fromValue(idSelection));
            this->ui->cb_Modele->setCurrentIndex(listIndexCbSelection.at(0).row());
            return;
        }
    }
    this->ui->cb_Modele->setCurrentIndex(oldIndexCbModele);
}

void Dlg_Equipement::cb_tauxTransmissionChanged(const int index)
{
    qDebug()<<"-----------------------------------------";
    qDebug()<<"call Dlg_Equipement::cb_tauxTransmissionChanged("<<QString::number(index)<<")";
    this->m_idTxTransmission = this->m_model_tx_transmission->record(index).value(TX_TRANSMISSION_ID).toInt();
}

void Dlg_Equipement::buttonEditTxTransmissionClicked()
{
    Dlg_Tx_Transmission dlgTxTransmission(this,this->m_bdHandler,true,this->ui->cb_Tx_Transmission->currentIndex());
    int oldIndexCbTxTransmission = this->ui->cb_Tx_Transmission->currentIndex();

    int result = dlgTxTransmission.exec();

    delete m_model;
    delete m_model_tx_transmission;

    this->peuplerTable();

    if(result)
    {
        int idSelection = dlgTxTransmission.getIdSelection();
        QModelIndexList listIndexCbSelection = this->m_model_tx_transmission->match(this->m_model_tx_transmission->index(0,TX_TRANSMISSION_ID),Qt::DisplayRole,QVariant::fromValue(idSelection));
        this->ui->cb_Tx_Transmission->setCurrentIndex(listIndexCbSelection.at(0).row());
    }
    else
    {
        this->ui->cb_Tx_Transmission->setCurrentIndex(oldIndexCbTxTransmission);
    }
}

void Dlg_Equipement::buttonFermerClicked()
{
    this->reject();
}

void Dlg_Equipement::buttonSelectionnerClicked()
{
    this->accept();
}

int Dlg_Equipement::getIdSelection()
{
    return this->m_model->record(m_indexSelection.row()).value(EQUIPEMENT_ID).toInt();
}

QString Dlg_Equipement::getNoSerieSelection()
{
    return this->m_model->record(m_indexSelection.row()).value(EQUIPEMENT_NO_SERIE).toString();
}

void Dlg_Equipement::buttonModifierClicked()
{
    if(!m_indexSelection.isValid())
        return;
    this->m_modifEnCours = true;
    this->m_idModifie = m_indexSelection.row();
    this->ui->tableView->setEnabled(false);

    QSqlRecord selection = this->m_model->record(m_idModifie);

    QModelIndexList listIndexCbSelection = this->m_model_Modele->match(this->m_model_Modele->index(0,MODELE_DESIGNATION),
                                                                       Qt::DisplayRole,selection.value(EQUIPEMENT_ID_MODELE),
                                                                       1,Qt::MatchExactly|Qt::MatchWrap);
    this->ui->cb_Modele->setCurrentIndex(listIndexCbSelection.at(0).row());

    listIndexCbSelection = this->m_model_tx_transmission->match(this->m_model_tx_transmission->index(0,TX_TRANSMISSION_DESIGNATION),
                                                                Qt::DisplayRole,selection.value(EQUIPEMENT_ID_TX_TRANSMISSION),
                                                                1,Qt::MatchExactly|Qt::MatchWrap);
    this->ui->cb_Tx_Transmission->setCurrentIndex(listIndexCbSelection.at(0).row());

    this->ui->lineEdit_NoSerie->setText(selection.value(EQUIPEMENT_NO_SERIE).toString());
    this->ui->ckB_EnService->setChecked(selection.value(EQUIPEMENT_EN_SERVICE).toBool());
    this->ui->lineEdit_MinGamme->setText(selection.value(EQUIPEMENT_MIN_GAMME).toString());
    this->ui->lineEdit_MaxGamme->setText(selection.value(EQUIPEMENT_MAX_GAMME).toString());
    this->ui->lineEdit_Offset->setText(selection.value(EQUIPEMENT_OFFSET).toString());
    this->ui->lineEdit_Adresse->setText(selection.value(EQUIPEMENT_ADRESSE).toString());

    this->ui->cb_Nb_Bits_Transmission->setCurrentIndex(this->ui->cb_Nb_Bits_Transmission->findText(selection.value(EQUIPEMENT_NB_BITS_TRANSMISSION).toString(),Qt::MatchExactly));
    this->ui->cb_Nb_Bits_Stop->setCurrentIndex(this->ui->cb_Nb_Bits_Stop->findText(selection.value(EQUIPEMENT_NB_BITS_STOP).toString(),Qt::MatchExactly));
    this->ui->cb_Type_Controle_Flux->setCurrentIndex(this->ui->cb_Type_Controle_Flux->findText(selection.value(EQUIPEMENT_CONTROLE_FLUX).toString(),Qt::MatchExactly));
    this->ui->cb_Type_Parite->setCurrentIndex(this->ui->cb_Type_Parite->findText(selection.value(EQUIPEMENT_PARITE).toString(),Qt::MatchExactly));

    this->afficherFormulaire();
}

void Dlg_Equipement::tableViewMoleculeIndexChanged(const QModelIndex & idxSelection)
{
    if(!idxSelection.isValid())
        this->ui->button_AddPolluant->setEnabled(false);
    else {
        this->m_indexSelectionMolecule = idxSelection;
        this->ui->button_AddPolluant->setEnabled(true);
    }
}

void Dlg_Equipement::tableViewPolluantIndexChanged(const QModelIndex & idxSelection)
{
    if(!idxSelection.isValid())
        this->ui->button_RemovePolluant->setEnabled(false);
    else {
        this->m_indexSelectionPolluant= idxSelection;
        this->ui->button_RemovePolluant->setEnabled(true);
    }
}

void Dlg_Equipement::buttonAddPolluantClicked()
{
    if(!m_indexSelectionMolecule.isValid()) return;
    QVariant idEquipement = m_model->record(m_idModifie).value(EQUIPEMENT_ID);
    QVariant idMolecule = m_model_molecule->record(m_indexSelectionMolecule.row()).value(MOLECULE_ID);
    if(m_model_polluant_associe->match(m_model_polluant_associe->index(0,POLLUANT_ASSOCIE_ID_MOLECULE),Qt::DisplayRole,
                                       idMolecule).count()>0)
        return;
    int row = m_model_polluant_associe->rowCount();
    m_model_polluant_associe->insertRow(row);
    m_model_polluant_associe->setData(m_model_polluant_associe->index(row,POLLUANT_ASSOCIE_ID_EQUIPEMENT),idEquipement);
    m_model_polluant_associe->setData(m_model_polluant_associe->index(row,POLLUANT_ASSOCIE_ID_MOLECULE),idMolecule);
    m_model_polluant_associe->submitAll();
    this->ui->tableView_Molecule->setRowHidden(m_indexSelectionMolecule.row(),true);
}

void Dlg_Equipement::buttonRemovePolluantClicked()
{
    if(!m_indexSelectionPolluant.isValid()) return;
    QVariant idMolecule = m_model_polluant_associe->record(m_indexSelectionPolluant.row()).value(POLLUANT_ASSOCIE_ID_MOLECULE);

    qDebug()<<m_model_polluant_associe->removeRow(m_indexSelectionPolluant.row());
    m_model_polluant_associe->submitAll();

    for(int i=0;i<m_model_polluant_associe->rowCount();i++) {
        QModelIndexList listMolecule = m_model_molecule->match(m_model_molecule->index(0,MOLECULE_FORMULE),Qt::DisplayRole,
                                                               idMolecule,1,Qt::MatchExactly | Qt::MatchWrap);
        for(int j=0;j<listMolecule.count();j++) {
            this->ui->tableView_Molecule->setRowHidden(listMolecule.at(j).row(),false);
        }
    }
}
